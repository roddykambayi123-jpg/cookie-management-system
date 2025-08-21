#include "FirefoxCookieReader.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QFile>
#include <QFileDevice>
#include <QDir>
#include <QDateTime>
#include <QTimeZone>
#include <QDebug>

static QDateTime firefoxEpochToUtc(qint64 epochSeconds) {
    return QDateTime::fromSecsSinceEpoch(epochSeconds, QTimeZone::UTC);
}

QVector<CookieItem> FirefoxCookieReader::readAll() {
    QVector<CookieItem> items;

    if (m_info.cookieDbPath.isEmpty()) {
        qWarning() << "[FirefoxCookieReader] cookieDbPath is empty";
        return items;
    }

    // Copy DB to temp to avoid locks
    const QString tmpPath = QDir::temp().absoluteFilePath("ff_cookies_copy.sqlite");
    QFile::remove(tmpPath);
    if (!QFile::copy(m_info.cookieDbPath, tmpPath)) {
        qWarning() << "[FirefoxCookieReader] Failed to copy DB:" << m_info.cookieDbPath;
        return items;
    }
    QFile::setPermissions(tmpPath, QFileDevice::ReadOwner | QFileDevice::WriteOwner);

    // Open SQLite with a unique connection name
    const char* kConn = "ff_cookies_conn";
    if (QSqlDatabase::contains(kConn)) {
        QSqlDatabase::removeDatabase(kConn);
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", kConn);
    db.setDatabaseName(tmpPath);
    if (!db.open()) {
        qWarning() << "[FirefoxCookieReader] SQLite open error:" << db.lastError();
        return items;
    }

    QSqlQuery q(db);
    // host, name, value, expiry, isHttpOnly, isSecure  (moz_cookies)
    if (!q.exec("SELECT host, name, value, expiry, isHttpOnly, isSecure FROM moz_cookies")) {
        qWarning() << "[FirefoxCookieReader] Query error:" << q.lastError();
        db.close();
        QSqlDatabase::removeDatabase(kConn);
        return items;
    }

    items.reserve(4096);
    while (q.next()) {
        CookieItem ci;
        ci.sourceBrowser = m_info.name;
        ci.domain       = q.value(0).toString();
        ci.name         = q.value(1).toString();
        ci.value        = q.value(2).toString();
        ci.expiresAt    = firefoxEpochToUtc(q.value(3).toLongLong());
        ci.httpOnly     = q.value(4).toInt() != 0;
        ci.secure       = q.value(5).toInt() != 0;
        items.push_back(std::move(ci));
    }

    db.close();
    QSqlDatabase::removeDatabase(kConn);
    return items;
}

bool FirefoxCookieReader::deleteCookies(const QVector<CookieItem>& items, QString* error)
{
    // Deduplicate (name, domain) pairs
    QSet<QPair<QString,QString>> uniq;
    QVariantList names, hostsExact, hostsDot;
    names.reserve(items.size());
    hostsExact.reserve(items.size());
    hostsDot.reserve(items.size());

    for (const auto &it : items) {
        auto key = qMakePair(it.name, it.domain);
        if (uniq.contains(key)) continue;
        uniq.insert(key);

        names << it.name;
        if (it.domain.startsWith('.')) {
            hostsExact << it.domain;
            hostsDot   << it.domain; // already dotted
        } else {
            hostsExact << it.domain;
            hostsDot   << ('.' + it.domain);
        }
    }
    if (names.isEmpty()) return true;

    const QString connName = QStringLiteral("ff_del");
    if (QSqlDatabase::contains(connName)) QSqlDatabase::removeDatabase(connName);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
    db.setDatabaseName(m_info.cookieDbPath);
    if (!db.open()) {
        if (error) *error = db.lastError().text();
        QSqlDatabase::removeDatabase(connName);
        return false;
    }

    // Detect table name: 'moz_cookies' (standard) or 'cookies' (some forks)
    QString table = "moz_cookies";
    {
        QSqlQuery tq(db);
        if (!tq.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='moz_cookies'") || !tq.next()) {
            table = "cookies";
        }
    }

    bool ok = db.transaction();
    if (!ok) {
        if (error) *error = db.lastError().text();
        db.close();
        QSqlDatabase::removeDatabase(connName);
        return false;
    }

    // Batch delete: match by name and either exact or dotted host
    QSqlQuery q(db);
    q.prepare(QString("DELETE FROM %1 WHERE name = ? AND (host = ? OR host = ?)").arg(table));
    q.addBindValue(names);
    q.addBindValue(hostsExact);
    q.addBindValue(hostsDot);

    ok = q.execBatch();

    if (ok) db.commit(); else db.rollback();
    if (!ok && error) *error = q.lastError().text();

    db.close();
    QSqlDatabase::removeDatabase(connName);
    return ok;
}