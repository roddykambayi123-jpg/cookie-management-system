#include "BrowserDetector.h"
#include <QDir>
#include <QFileInfo>

static QString home() { return QDir::homePath(); }

QString BrowserDetector::findFirstExisting(const QStringList& candidates) {
    for (const auto& path : candidates) {
        QFileInfo fi(path);
        if (fi.exists()) return fi.absoluteFilePath();
    }
    return {};
}

QVector<BrowserInfo> BrowserDetector::detectAll() {
    QVector<BrowserInfo> out;
    detectChromium(out);
    detectFirefox(out);
    detectSafari(out);
    return out;
}

void BrowserDetector::detectChromium(QVector<BrowserInfo>& out) {
#if defined(Q_OS_WIN)
    const QString base = qEnvironmentVariable("LOCALAPPDATA");
    struct Entry { const char* name; const char* rel; } list[] = {
        {"Google Chrome", "Google/Chrome/User Data/Default/Cookies"},
        {"Microsoft Edge", "Microsoft/Edge/User Data/Default/Cookies"},
        {"Brave", "BraveSoftware/Brave-Browser/User Data/Default/Cookies"},
        {"Opera", "Opera Software/Opera Stable/Cookies"},
    };
    for (const auto& e : list) {
        const QString p = base + "/" + e.rel;
        if (QFileInfo::exists(p)) {
            BrowserInfo bi; bi.name = e.name; bi.engine = BrowserEngine::Chromium;
            bi.cookieDbPath = p; bi.profilePath = QFileInfo(p).dir().absolutePath();
            out.push_back(bi);
        }
    }
#elif defined(Q_OS_MAC)
    const QString h = home();
    struct Entry { const char* name; const char* rel; } list[] = {
        {"Google Chrome", "Library/Application Support/Google/Chrome/Default/Cookies"},
        {"Microsoft Edge", "Library/Application Support/Microsoft Edge/Default/Cookies"},
        {"Brave", "Library/Application Support/BraveSoftware/Brave-Browser/Default/Cookies"},
        {"Opera", "Library/Application Support/com.operasoftware.Opera/Cookies"},
    };
    for (const auto& e : list) {
        const QString p = h + "/" + e.rel;
        if (QFileInfo::exists(p)) {
            BrowserInfo bi; bi.name = e.name; bi.engine = BrowserEngine::Chromium;
            bi.cookieDbPath = p; bi.profilePath = QFileInfo(p).dir().absolutePath();
            out.push_back(bi);
        }
    }
#else
    const QString h = home();
    struct Entry { const char* name; const char* rel; } list[] = {
        {"Google Chrome", ".config/google-chrome/Default/Cookies"},
        {"Chromium",      ".config/chromium/Default/Cookies"},
        {"Brave",         ".config/BraveSoftware/Brave-Browser/Default/Cookies"},
        {"Opera",         ".config/opera/Cookies"},
    };
    for (const auto& e : list) {
        const QString p = h + "/" + e.rel;
        if (QFileInfo::exists(p)) {
            BrowserInfo bi; bi.name = e.name; bi.engine = BrowserEngine::Chromium;
            bi.cookieDbPath = p; bi.profilePath = QFileInfo(p).dir().absolutePath();
            out.push_back(bi);
        }
    }
#endif
}

void BrowserDetector::detectFirefox(QVector<BrowserInfo>& out) {
#if defined(Q_OS_WIN)
    const QString base = qEnvironmentVariable("APPDATA") + "/Mozilla/Firefox/Profiles";
#elif defined(Q_OS_MAC)
    const QString base = home() + "/Library/Application Support/Firefox/Profiles";
#else
    const QString base = home() + "/.mozilla/firefox";
#endif
    QDir d(base);
    if (!d.exists()) return;
    for (const auto& dir : d.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        if (!dir.contains("default")) continue;
        const QString path = base + "/" + dir + "/cookies.sqlite";
        if (QFileInfo::exists(path)) {
            BrowserInfo bi;
            bi.name = QString("Firefox (%1)").arg(dir);
            bi.engine = BrowserEngine::Firefox;
            bi.cookieDbPath = path;
            bi.profilePath = base + "/" + dir;
            out.push_back(bi);
        }
    }
}

void BrowserDetector::detectSafari(QVector<BrowserInfo>& out) {
#if defined(Q_OS_MAC)
    const QString h = QDir::homePath();
    QStringList candidates;
    // Legacy + sandboxed binarycookies
    candidates << h + "/Library/Containers/com.apple.Safari/Data/Library/Cookies/Cookies.binarycookies";
    candidates << h + "/Library/Cookies/Cookies.binarycookies";
    // Newer WebKit: SQLite cookie store
    candidates << h + "/Library/Containers/com.apple.Safari/Data/Library/WebKit/NetworkPersistentStorage/Cookies.sqlite";
    // Safari Technology Preview (both formats)
    candidates << h + "/Library/Containers/com.apple.SafariTechnologyPreview/Data/Library/Cookies/Cookies.binarycookies";
    candidates << h + "/Library/Containers/com.apple.SafariTechnologyPreview/Data/Library/WebKit/NetworkPersistentStorage/Cookies.sqlite";

    QString picked;
    for (const QString& c : candidates) {
        QFileInfo fi(c);
        if (fi.exists() && fi.isFile() && fi.size() > 0) { picked = fi.absoluteFilePath(); break; }
    }

    // Last resort: any *.sqlite under NetworkPersistentStorage
    if (picked.isEmpty()) {
        const QString dirPath = h + "/Library/Containers/com.apple.Safari/Data/Library/WebKit/NetworkPersistentStorage";
        QDir d(dirPath);
        if (d.exists()) {
            const auto list = d.entryInfoList(QStringList() << "*.sqlite", QDir::Files | QDir::Readable);
            if (!list.isEmpty()) picked = list.first().absoluteFilePath();
        }
    }

    if (!picked.isEmpty()) {
        BrowserInfo bi;
        bi.name = picked.contains("TechnologyPreview") ? "Safari Technology Preview" : "Safari";
        bi.engine = BrowserEngine::Safari;
        bi.cookieDbPath = picked;
        bi.profilePath = QFileInfo(picked).dir().absolutePath();
        out.push_back(bi);
    }
#else
    Q_UNUSED(out);
#endif
}