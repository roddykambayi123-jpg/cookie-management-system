

#pragma once
/**
 * FirefoxCookieReader
 * --------------------
 * Reads cookies from a Firefox profile's cookies.sqlite and returns them as CookieItem rows.
 *
 * Expected schema (moz_cookies):
 *   host TEXT, name TEXT, value TEXT, expiry INTEGER (unix seconds),
 *   isHttpOnly INTEGER, isSecure INTEGER
 *
 * Notes:
 *  - We always copy the DB to a temp file before opening with QSQLITE to avoid lock issues.
 *  - Values are typically plaintext for cookies (unlike saved logins which use key4.db).
 *  - Timestamps are unix seconds (UTC).
 */

#include "CookieReader.h"
#include "BrowserDetector.h"
#include <QString>

class FirefoxCookieReader : public CookieReader {
public:
    explicit FirefoxCookieReader(const BrowserInfo& info) : m_info(info) {}
    ~FirefoxCookieReader() override = default;

    // Opens a temp copy of cookies.sqlite and loads all cookies.
    QVector<CookieItem> readAll() override;
    bool deleteCookies(const QVector<CookieItem>& items, QString* error) override;
    
private:
    BrowserInfo m_info;
};