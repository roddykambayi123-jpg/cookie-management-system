#pragma once
#include "CookieReader.h"
#include "BrowserDetector.h"
#include <QString>
#include <QVector>

class SafariCookieReader : public CookieReader {
public:
    explicit SafariCookieReader(const BrowserInfo& info) : m_info(info) {}
    QVector<CookieItem> readAll() override;
    bool deleteCookies(const QVector<CookieItem>& items, QString* error) override;
private:
    QVector<CookieItem> readBinaryCookies(const QString& path);
    QVector<CookieItem> readSQLiteCookies(const QString& path);
    BrowserInfo m_info;
};