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

    // Candidate BinaryCookies paths (standard + alt + STP + legacy)
    const QStringList binCandidates = {
        h + "/Library/Containers/com.apple.Safari/Data/Library/Cookies/Cookies.binarycookies",
        h + "/Library/Containers/com.apple.Safari/Data/Library/WebKit/Network/Cookies/Cookies.binarycookies",
        h + "/Library/Cookies/Cookies.binarycookies",
        h + "/Library/Containers/com.apple.SafariTechnologyPreview/Data/Library/Cookies/Cookies.binarycookies",
        h + "/Library/Containers/com.apple.SafariTechnologyPreview/Data/Library/WebKit/Network/Cookies/Cookies.binarycookies"
    };

    // Candidate SQLite paths (modern WebKit persistent store)
    const QStringList sqliteCandidates = {
        h + "/Library/Containers/com.apple.Safari/Data/Library/WebKit/NetworkPersistentStorage/Cookies.sqlite",
        h + "/Library/Containers/com.apple.SafariTechnologyPreview/Data/Library/WebKit/NetworkPersistentStorage/Cookies.sqlite"
    };

    auto pushIfValid = [&](const QString& path) {
        QFileInfo fi(path);
        if (!fi.exists() || !fi.isFile() || fi.size() <= 0) return;

        BrowserInfo bi;
        const bool isSTP = path.contains("SafariTechnologyPreview", Qt::CaseInsensitive);
        bi.name = isSTP ? "Safari Technology Preview" : "Safari";
        bi.engine = BrowserEngine::Safari;
        bi.cookieDbPath = fi.absoluteFilePath();
        bi.profilePath = fi.dir().absolutePath();
        out.push_back(bi);
    };

    // Push known candidates first (BinaryCookies & SQLite)
    for (const QString& p : binCandidates)    pushIfValid(p);
    for (const QString& p : sqliteCandidates) pushIfValid(p);

    // Also enumerate any extra *.sqlite files under NetworkPersistentStorage
    struct DirSpec { const char* rel; const char* label; } dirs[] = {
        { "Library/Containers/com.apple.Safari/Data/Library/WebKit/NetworkPersistentStorage", "Safari" },
        { "Library/Containers/com.apple.SafariTechnologyPreview/Data/Library/WebKit/NetworkPersistentStorage", "Safari Technology Preview" }
    };
    for (const auto& dspec : dirs) {
        const QString dirPath = h + "/" + dspec.rel;
        QDir d(dirPath);
        if (!d.exists()) continue;
        const auto list = d.entryInfoList(QStringList() << "*.sqlite", QDir::Files | QDir::Readable);
        for (const QFileInfo& fi : list) {
            // Avoid duplicating the specific Cookies.sqlite we might have already added
            if (!fi.exists() || fi.size() <= 0) continue;

            BrowserInfo bi;
            bi.name = dspec.label;
            bi.engine = BrowserEngine::Safari;
            bi.cookieDbPath = fi.absoluteFilePath();
            bi.profilePath = fi.dir().absolutePath();
            // Skip if this exact path is already in 'out'
            bool dup = false;
            for (const auto& existing : out) {
                if (existing.cookieDbPath == bi.cookieDbPath) { dup = true; break; }
            }
            if (!dup) out.push_back(bi);
        }
    }
#else
    Q_UNUSED(out);
#endif
}