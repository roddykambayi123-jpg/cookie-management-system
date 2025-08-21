# ---- Project ----
TARGET   = CookieManagementSystem
TEMPLATE = app

# ---- Qt modules ----
QT += core gui widgets sql network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# C++ standard
CONFIG += c++17 warn_on

# ---- Include paths ----
INCLUDEPATH += \
    $$PWD/../include \
    $$PWD/build/ui    # generated ui_*.h live here

# ---- Sources / Headers / Forms ----
SOURCES += \
    $$PWD/../src/main.cpp \
    $$PWD/../src/CookieManagementSystem.cpp \
    $$PWD/../src/BrowserDetector.cpp \
    $$PWD/../src/CookieReader.cpp \
    $$PWD/../src/ChromiumCookieReader.cpp \
    $$PWD/../src/FirefoxCookieReader.cpp \
    $$PWD/../src/ChooseBrowserDialog.cpp \
    $$PWD/../src/SafariCookieReader.cpp \

HEADERS += \
    $$PWD/../include/CookieManagementSystem.h \
    $$PWD/../include/BrowserDetector.h \
    $$PWD/../include/CookieReader.h \
    $$PWD/../include/ChromiumCookieReader.h \
    $$PWD/../include/FirefoxCookieReader.h \
    $$PWD/../include/ChooseBrowserDialog.h \
    $$PWD/../include/SafariCookieReader.h

FORMS += \
    $$PWD/../ui/CookieManagementSystem.ui \
    $$PWD/../ui/ChooseBrowserDialog.ui \
    $$PWD/../ui/ImportProgressDialog.ui \
    $$PWD/../ui/CookieDetailsDialog.ui \
    $$PWD/../ui/SettingsDialog.ui \
    $$PWD/../ui/RiskLegendPage.ui

# ---- Resources ----
# Update this to the resource file you actually have. If you don't have one yet, comment out the next two lines.
RESOURCES += \
    $$PWD/../resources/CookieManagementSystem.qrc

# ---- Platform libs (for legacy Chromium DPAPI on Windows) ----
win32: LIBS += -lcrypt32 -lbcrypt

# ---- Output dirs (optional, keeps build tidy) ----
DESTDIR     = $$PWD/build/bin
OBJECTS_DIR = $$PWD/build/obj
MOC_DIR     = $$PWD/build/moc
RCC_DIR     = $$PWD/build/rcc
UI_DIR      = $$PWD/build/ui

# ---- Deployment helpers (optional) ----
DISTFILES += \
    $$PWD/../README.md \
    $$PWD/../LICENSE \
    $$PWD/../data/cookies.csv \
    $$PWD/../data/anon.csv