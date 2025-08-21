#pragma once
#include "CookieReader.h"
#include "BrowserDetector.h"
#include <QString>

class ChromiumCookieReader : public CookieReader {
public:
    explicit ChromiumCookieReader(const BrowserInfo& info);
    QVector<CookieItem> readAll() override;
    bool deleteCookies(const QVector<CookieItem>& items, QString* error) override;
private:
    BrowserInfo m_info;
    QString copyDbToTemp() const;
    QString tryDecrypt(const QByteArray& encrypted) const; // MVP: v10 → "[ENCRYPTED]"
};