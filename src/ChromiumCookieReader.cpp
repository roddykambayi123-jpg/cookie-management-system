#include "ChromiumCookieReader.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QFile>
#include <QFileDevice>
#include <QTemporaryFile>
#include <QDir>
#include <QDateTime>
#include <QTimeZone>
#include <QDebug>


#if defined(Q_OS_WIN)
#include <windows.h>
#include <wincrypt.h>
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "bcrypt.lib")
#endif

ChromiumCookieReader::ChromiumCookieReader(const BrowserInfo& info) : m_info(info) {}

static QDateTime chromiumEpochToUtc(qint64 epochMicro) {
    // microseconds since 1601-01-01 UTC
    const qint64 WINDOWS_TO_UNIX_EPOCH_MICROS = 11644473600000000LL;
    qint64 unixMicros = epochMicro - WINDOWS_TO_UNIX_EPOCH_MICROS;
    return QDateTime::fromSecsSinceEpoch(unixMicros / 1000000, QTimeZone::UTC);
}

QString ChromiumCookieReader::copyDbToTemp() const {
    const QString tmpPath = QDir::temp().absoluteFilePath("chromium_cookies_copy.sqlite");
    QFile::remove(tmpPath);
    if (!QFile::copy(m_info.cookieDbPath, tmpPath)) {
        qWarning() << "[ChromiumCookieReader] Failed to copy" << m_info.cookieDbPath;
        return {};
    }
    QFile::setPermissions(tmpPath, QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    return tmpPath;
}

QString ChromiumCookieReader::tryDecrypt(const QByteArray& enc) const {
    if (enc.isEmpty()) return {};
    if (enc.startsWith("v10") || enc.startsWith("v11")) {
        return QStringLiteral("[ENCRYPTED]"); // Hook for full AES-GCM later
    }
#if defined(Q_OS_WIN)
    // Try legacy DPAPI blobs
    DATA_BLOB in { static_cast<DWORD>(enc.size()), (BYTE*)enc.data() };
    DATA_BLOB out { 0, nullptr };
    if (CryptUnprotectData(&in, nullptr, nullptr, nullptr, nullptr, 0, &out)) {
        QString s = QString::fromUtf8(reinterpret_cast<char*>(out.pbData), out.cbData);
        LocalFree(out.pbData);
        return s;
    }
#endif
    return QStringLiteral("[ENCRYPTED]");
}

QVector<CookieItem> ChromiumCookieReader::readAll() {
    QVector<CookieItem> items;
    if (m_info.cookieDbPath.isEmpty()) return items;

    const QString path = copyDbToTemp();
    if (path.isEmpty()) return items;

    const char* kConn = "chromium_cookies_conn";
    if (QSqlDatabase::contains(kConn)) QSqlDatabase::removeDatabase(kConn);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", kConn);
    db.setDatabaseName(path);
    if (!db.open()) {
        qWarning() << "[ChromiumCookieReader] SQLite open error:" << db.lastError();
        return items;
    }

    QSqlQuery q(db);
    if (!q.exec("SELECT host_key, name, encrypted_value, expires_utc, is_httponly, is_secure FROM cookies")) {
        qWarning() << "[ChromiumCookieReader] Query error:" << q.lastError();
        db.close();
        QSqlDatabase::removeDatabase(kConn);
        return items;
    }

    items.reserve(4096);
    while (q.next()) {
        CookieItem ci;
        ci.sourceBrowser = m_info.name;
        ci.domain    = q.value(0).toString();
        ci.name      = q.value(1).toString();
        ci.value     = tryDecrypt(q.value(2).toByteArray());
        ci.expiresAt = chromiumEpochToUtc(q.value(3).toLongLong());
        ci.httpOnly  = q.value(4).toInt() != 0;
        ci.secure    = q.value(5).toInt() != 0;
        items.push_back(std::move(ci));
    }

    db.close();
    QSqlDatabase::removeDatabase(kConn);
    return items;
}

bool ChromiumCookieReader::deleteCookies(const QVector<CookieItem>& items, QString* error)
{
    // Deduplicate (name, domain) and prepare exact + dotted hosts
    QSet<QPair<QString,QString>> uniq;
    QVariantList names, hostsExact, hostsDot;
    for (const auto &it : items) {
        auto key = qMakePair(it.name, it.domain);
        if (uniq.contains(key)) continue;
        uniq.insert(key);
        names << it.name;
        if (it.domain.startsWith('.')) {
            hostsExact << it.domain;
            hostsDot   << it.domain;          // already dotted
        } else {
            hostsExact << it.domain;
            hostsDot   << ('.' + it.domain);  // Chromium often stores with a leading dot
        }
    }
    if (names.isEmpty()) return true;

    const QString conn = QStringLiteral("chromium_del");
    if (QSqlDatabase::contains(conn)) QSqlDatabase::removeDatabase(conn);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", conn);
    db.setDatabaseName(m_info.cookieDbPath);
    if (!db.open()) { if (error) *error = db.lastError().text(); QSqlDatabase::removeDatabase(conn); return false; }

    // Detect table name and host column
    auto detect = [&](QString &table, QString &hostCol)->bool {
        table = "cookies"; hostCol = "host_key";
        QSqlQuery q(db);

        // Table: prefer 'cookies', else 'Cookies'
        if (!q.exec("SELECT name FROM sqlite_master WHERE type='table' AND (name='cookies' OR name='Cookies') LIMIT 1"))
            return false;
        if (q.next()) table = q.value(0).toString();

        // Column sniff
        QStringList cols;
        QSqlQuery qi(db);
        if (!qi.exec(QString("PRAGMA table_info(%1)").arg(table))) return false;
        while (qi.next()) cols << qi.value(1).toString();

        if (cols.contains("host_key"))      hostCol = "host_key";
        else if (cols.contains("host"))     hostCol = "host";
        else if (cols.contains("domain"))   hostCol = "domain";
        else return false;
        return true;
    };

    QString table, hostCol;
    if (!detect(table, hostCol)) {
        if (error) *error = QStringLiteral("Could not detect Chromium cookies schema");
        db.close(); QSqlDatabase::removeDatabase(conn);
        return false;
    }

    bool ok = db.transaction();
    if (!ok) { if (error) *error = db.lastError().text(); db.close(); QSqlDatabase::removeDatabase(conn); return false; }

    // Batch delete: name + (host == exact OR dotted)
    QSqlQuery del(db);
    del.prepare(QString("DELETE FROM %1 WHERE name = ? AND (%2 = ? OR %2 = ?)").arg(table, hostCol));
    del.addBindValue(names);
    del.addBindValue(hostsExact);
    del.addBindValue(hostsDot);
    ok = del.execBatch();

    if (ok) db.commit(); else db.rollback();
    if (!ok && error) *error = del.lastError().text();

    db.close(); QSqlDatabase::removeDatabase(conn);
    return ok;
}