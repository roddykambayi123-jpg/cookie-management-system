#include "CookieReader.h"
#include "ChromiumCookieReader.h"
#include "FirefoxCookieReader.h"
#include "SafariCookieReader.h"
#include <memory>

std::unique_ptr<CookieReader> makeReader(const BrowserInfo& info) {
    switch (info.engine) {
        case BrowserEngine::Chromium: return std::make_unique<ChromiumCookieReader>(info);
        case BrowserEngine::Firefox:  return std::make_unique<FirefoxCookieReader>(info);
        case BrowserEngine::Safari:   return std::make_unique<SafariCookieReader>(info);
    }
    return nullptr;
}