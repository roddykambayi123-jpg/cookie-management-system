#pragma once
#include <QString>
#include <QVector>
#include <QIcon>

enum class BrowserEngine { Chromium, Firefox, Safari };

struct BrowserInfo {
    QString name;           // e.g., "Google Chrome"
    BrowserEngine engine;   // Chromium, Firefox, Safari
    QString profilePath;    // profile folder (or Cookies file folder)
    QString cookieDbPath;   // DB path (Chromium/Firefox) or Cookies.binarycookies (Safari)
    QIcon icon;             // optional
};

class BrowserDetector {
public:
    static QVector<BrowserInfo> detectAll();

private:
    static void detectChromium(QVector<BrowserInfo>& out);
    static void detectFirefox(QVector<BrowserInfo>& out);
    static void detectSafari(QVector<BrowserInfo>& out);
    static QString findFirstExisting(const QStringList& candidates);
};