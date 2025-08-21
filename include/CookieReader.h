#pragma once
#include <QString>
#include <QVector>
#include <QDateTime>
#include <QMetaType>

struct CookieItem {
    QString sourceBrowser; // Chrome, Edge, Firefox
    QString domain;
    QString name;
    QString value;         // decrypted or placeholder
    bool httpOnly = false;
    bool secure = false;
    QDateTime expiresAt;
};

class CookieReader {
public:
    virtual ~CookieReader() = default;
    virtual QVector<CookieItem> readAll() = 0;
    virtual bool deleteCookies(const QVector<CookieItem>& items, QString* error) {
    Q_UNUSED(items);
    if (error) *error = QStringLiteral("Delete not implemented for this reader");
    return false;
    }
};

// Factory
#include "BrowserDetector.h"
#include <memory>
std::unique_ptr<CookieReader> makeReader(const BrowserInfo& info);