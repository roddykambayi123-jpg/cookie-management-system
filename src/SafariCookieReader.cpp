#include "SafariCookieReader.h"
#include <QFile>
#include <QByteArray>
#include <QDateTime>
#include <QtEndian>
#include <QFileInfo>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVariant>
#include <QTimeZone>
#include <QDir>
#include <QDebug>

static inline quint32 BE32(const uchar* p) { return qFromBigEndian<quint32>(p); }
static inline quint32 LE32(const uchar* p) { return qFromLittleEndian<quint32>(p); }
static inline double  LE64F(const uchar* p) { quint64 v = qFromLittleEndian<quint64>(p); double d; memcpy(&d,&v,8); return d; }
// Mac epoch (2001-01-01) ➜ UTC
static inline QDateTime macEpochToUtc(double sec) {
    qint64 unixSecs = static_cast<qint64>(sec + 978307200.0);
    return QDateTime::fromSecsSinceEpoch(unixSecs, QTimeZone::UTC);
}

QVector<CookieItem> SafariCookieReader::readAll() {
    const QString p = m_info.cookieDbPath;
    if (p.endsWith(".binarycookies", Qt::CaseInsensitive)) return readBinaryCookies(p);
    if (p.endsWith(".sqlite", Qt::CaseInsensitive))        return readSQLiteCookies(p);

    QFileInfo fi(p);
    if (fi.isDir()) {
        // Try any sqlite file inside the directory
        QDir d(fi.absoluteFilePath());
        const auto list = d.entryInfoList(QStringList() << "*.sqlite", QDir::Files | QDir::Readable);
        if (!list.isEmpty()) return readSQLiteCookies(list.first().absoluteFilePath());
    }
    return {};
}

QVector<CookieItem> SafariCookieReader::readBinaryCookies(const QString& path) {
    QVector<CookieItem> out;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) { qWarning() << "Safari: cannot open" << path; return out; }
    QByteArray data = f.readAll(); f.close();
    if (data.size() < 16) return out;

    const uchar* b = reinterpret_cast<const uchar*>(data.constData());
    if (!(data[0]=='c' && data[1]=='o' && data[2]=='o' && data[3]=='k')) return out;
    const quint32 numPages = BE32(b + 4);
    if (data.size() < 8 + 4*int(numPages)) return out;

    QVector<quint32> pageOffsets; pageOffsets.reserve(int(numPages));
    for (quint32 i=0;i<numPages;++i) pageOffsets.push_back(BE32(b + 8 + 4*i));

    for (quint32 po : pageOffsets) {
        if (po + 12 > (quint32)data.size()) continue;
        const uchar* p = b + po;
        if (BE32(p + 0) != 0x00000100) continue; // page sig
        const quint32 numCookies = LE32(p + 4);
        const uchar* offsBase = p + 8;

        for (quint32 i=0;i<numCookies;++i) {
            const quint32 rel = LE32(offsBase + 4*i);
            const uchar* c = p + rel;
            if (c + 56 > b + data.size()) continue;

            const quint32 cookieSize = LE32(c + 0);
            (void)LE32(c + 4);
            const quint32 flags   = LE32(c + 8);
            (void)LE32(c + 12);
            const quint32 oDomain = LE32(c + 16);
            const quint32 oName   = LE32(c + 20);
            const quint32 oPath   = LE32(c + 24);
            const quint32 oValue  = LE32(c + 28);
            (void)LE32(c + 32);
            (void)LE32(c + 36);
            const double  expiresMac = LE64F(c + 40);
            // (void)LE64F(c + 48); // created

            if (cookieSize < 56) continue;
            const uchar* s = c;
            auto str = [&](quint32 start, quint32 end)->QString{
                if (start >= cookieSize) return {};
                if (end <= start || end > cookieSize) end = cookieSize;
                const char* cs = reinterpret_cast<const char*>(s + start);
                int len = int(end - start), realLen = 0;
                while (realLen < len && cs[realLen] != '\0') ++realLen;
                return QString::fromUtf8(cs, realLen);
            };

            CookieItem ci;
            ci.sourceBrowser = "Safari";
            ci.domain   = str(oDomain, oName);
            ci.name     = str(oName,   oPath);
            ci.value    = str(oValue,  cookieSize);
            ci.httpOnly = (flags & 0x4) != 0;
            ci.secure   = (flags & 0x1) != 0;
            ci.expiresAt= macEpochToUtc(expiresMac);
            out.push_back(std::move(ci));
        }
    }
    return out;
}

static bool tableExists(QSqlDatabase& db, const QString& t) {
    QSqlQuery q(db);
    q.exec(QStringLiteral("SELECT name FROM sqlite_master WHERE type='table' AND name='%1'").arg(t));
    return q.next();
}

QVector<CookieItem> SafariCookieReader::readSQLiteCookies(const QString& path) {
    QVector<CookieItem> out;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "safari_db_conn");
    db.setDatabaseName(path);
    if (!db.open()) {
        qWarning() << "Safari: SQLite open failed" << path << db.lastError().text();
        return out;
    }

    // Try a few likely schemas. WebKit's 'cookies' table is most common.
    QString sql;
    if (tableExists(db, "cookies")) {
        sql = R"SQL(
            SELECT 
              coalesce(host, host_key, domain)   AS domain,
              name,
              value,
              coalesce(isHttpOnly, is_httponly, httponly, 0) AS httpOnly,
              coalesce(isSecure,   is_secure,   secure,   0) AS isSecure,
              coalesce(expiryTime, expires,      expires_utc, 0) AS expiry
            FROM cookies
        )SQL";
    } else if (tableExists(db, "moz_cookies")) {  // extremely rare on Safari, but harmless fallback
        sql = R"SQL(
            SELECT host AS domain, name, value,
                   isHttpOnly AS httpOnly, isSecure AS isSecure,
                   expiry AS expiry
            FROM moz_cookies
        )SQL";
    } else {
        // Best effort on unknown schema where the table is still named 'cookies'
        sql = R"SQL(
            SELECT 
              coalesce(host, host_key, domain)   AS domain,
              name,
              value,
              coalesce(isHttpOnly, is_httponly, httponly, 0) AS httpOnly,
              coalesce(isSecure,   is_secure,   secure,   0) AS isSecure,
              coalesce(expiryTime, expires,      expires_utc, 0) AS expiry
            FROM cookies
        )SQL";
    }

    QSqlQuery q(db);
    if (!q.exec(sql)) {
        qWarning() << "Safari: query failed" << q.lastError().text();
        db.close(); QSqlDatabase::removeDatabase("safari_db_conn");
        return out;
    }

    while (q.next()) {
        CookieItem ci;
        ci.sourceBrowser = "Safari";
        ci.domain   = q.value("domain").toString();
        ci.name     = q.value("name").toString();
        ci.value    = q.value("value").toString();
        ci.httpOnly = q.value("httpOnly").toInt() != 0;
        ci.secure   = q.value("isSecure").toInt() != 0;
        const QVariant expV = q.value("expiry");
        if (expV.isValid()) {
            qint64 v = expV.typeId() == QMetaType::Double ? (qint64)expV.toDouble()
                                                          : expV.toLongLong();
            ci.expiresAt = QDateTime::fromSecsSinceEpoch(v, QTimeZone::UTC);
        }
        out.push_back(std::move(ci));
    }

    db.close(); QSqlDatabase::removeDatabase("safari_db_conn");
    return out;
}

bool SafariCookieReader::deleteCookies(const QVector<CookieItem>& items, QString* error)
{
    // Don’t write BinaryCookies
    if (m_info.cookieDbPath.endsWith(".binarycookies", Qt::CaseInsensitive)) {
        if (error) *error = QStringLiteral("Deleting from BinaryCookies is not supported");
        return false;
    }

    // Deduplicate and build host variants
    QSet<QPair<QString,QString>> uniq;
    QVariantList names, hostsExact, hostsDot;
    for (const auto &it : items) {
        auto key = qMakePair(it.name, it.domain);
        if (uniq.contains(key)) continue;
        uniq.insert(key);
        names << it.name;
        if (it.domain.startsWith('.')) {
            hostsExact << it.domain;
            hostsDot   << it.domain;
        } else {
            hostsExact << it.domain;
            hostsDot   << ('.' + it.domain);
        }
    }
    if (names.isEmpty()) return true;

    const QString conn = QStringLiteral("safari_del");
    if (QSqlDatabase::contains(conn)) QSqlDatabase::removeDatabase(conn);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", conn);
    db.setDatabaseName(m_info.cookieDbPath);
    if (!db.open()) { if (error) *error = db.lastError().text(); QSqlDatabase::removeDatabase(conn); return false; }

    // Detect table and host column (Safari often uses 'Cookies' and 'host')
    auto detect = [&](QString &table, QString &hostCol)->bool {
        table = "Cookies"; hostCol = "host";
        QSqlQuery q(db);

        if (!q.exec("SELECT name FROM sqlite_master WHERE type='table' AND (name='Cookies' OR name='cookies') LIMIT 1"))
            return false;
        if (q.next()) table = q.value(0).toString();

        QStringList cols;
        QSqlQuery qi(db);
        if (!qi.exec(QString("PRAGMA table_info(%1)").arg(table))) return false;
        while (qi.next()) cols << qi.value(1).toString();

        if      (cols.contains("host"))     hostCol = "host";
        else if (cols.contains("host_key")) hostCol = "host_key";
        else if (cols.contains("domain"))   hostCol = "domain";
        else return false;
        return true;
    };

    QString table, hostCol;
    if (!detect(table, hostCol)) {
        if (error) *error = QStringLiteral("Could not detect Safari cookies schema");
        db.close(); QSqlDatabase::removeDatabase(conn);
        return false;
    }

    bool ok = db.transaction();
    if (!ok) { if (error) *error = db.lastError().text(); db.close(); QSqlDatabase::removeDatabase(conn); return false; }

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