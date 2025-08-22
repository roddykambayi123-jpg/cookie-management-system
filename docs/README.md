
# Cookie Management System

A cross‑platform Qt app to **inspect, filter, and (where supported) delete** cookies from installed browsers.  
Supports **Chrome/Chromium/Edge/Brave**, **Firefox**, and **Safari** (read‑only for BinaryCookies).

## Features

- 🔎 Read cookies directly from browser stores (SQLite / BinaryCookies) — *no CSV needed*
- 🧰 Auto‑detect installed browsers; user selects which to import
- 🧮 Smart categorization (**Functional, Analytics, Marketing, Unknown**)
- 🧹 Delete selected/all cookies (Firefox/Chromium SQLite stores)
- 🕵️ Anonymisation toggle (mask sensitive names/domains)
- 📊 Summary with risk meter + quick privacy tips
- 🧪 Diagnostics dialog (path, readable, size, format guess, rows)
- 💾 Export table to CSV
- 🖼️ App/Window icon via Qt resources

> ❗ **Safari**: BinaryCookies are **read‑only**; deletion is not supported. For macOS Big Sur+ you may also see SQLite stores under `NetworkPersistentStorage`.

---

## Supported Platforms

- **macOS** 12+ (Apple Silicon/Intel) — Qt 6 (Homebrew)
- **Ubuntu** 22.04/24.04 — Qt 5 or Qt 6
- **Windows** 10/11 — Qt 6 (MSVC) *(not covered in detail below, but builds with `windeployqt`)*

---

## Quick Start (macOS)

1) Install Qt:
```bash
brew install qt
# (Optional) expose qmake on PATH
export PATH="/opt/homebrew/opt/qt/bin:$PATH"
```

2) Build:
```bash
git clone https://github.com/roddykambayi123-jpg/cookie-management-system-main.git
cd cookie-management-system-main/project
qmake
make -j"$(sysctl -n hw.ncpu)"
```

3) Run:
```bash
open build/bin/CookieManagementSystem.app
# or
./build/bin/CookieManagementSystem.app/Contents/MacOS/CookieManagementSystem
```

> If the app complains about missing Qt frameworks when launched from Finder, bundle them:
```bash
/opt/homebrew/opt/qt/bin/macdeployqt build/bin/CookieManagementSystem.app
```

---

## Quick Start (Ubuntu)

1) Install Qt:
```bash
sudo apt update
sudo apt install -y qtbase5-dev qt5-qmake qtbase5-dev-tools libqt5sql5-sqlite build-essential
# or, for Qt 6:
# sudo apt install -y qt6-base-dev qt6-base-dev-tools build-essential
```

2) Build:
```bash
git clone https://github.com/roddykambayi123-jpg/cookie-management-system-main.git
cd cookie-management-system-main/project
qmake
make -j"$(nproc)"
```

3) Run:
```bash
./build/bin/CookieManagementSystem
```

> If you see “could not load the Qt platform plugin ‘xcb’”, install:
```bash
sudo apt install -y libxcb1 libx11-xcb1 libxcb-render0 libxcb-image0 libxcb-keysyms1 libxcb-icccm4 libxcb-shm0 libxcb-xfixes0 libxcb-shape0 libxcb-randr0 libxcb-util1
```

---

## Using the App

1. Click **Read Cookies** → pick a detected browser.  
2. If no rows appear, the **Diagnostics** dialog will show the exact file path and access status.  
3. Use the **category dropdown** (includes **Unknown**) and **search** to filter.  
4. Select rows (checkbox column) → **Delete Selected** (Firefox/Chromium only).  
5. **Export** visible rows to CSV if needed.

---

## ⌨️ Keyboard Shortcuts
- Read Cookies: (use the button)
- Search: Ctrl+F
- Delete Selected: Ctrl+D
- Diagnostics: Ctrl+I

---

## 📸 Screenshots

<img width="1188" height="850" alt="Screenshot 2025-08-21 at 21 37 17" src="https://github.com/user-attachments/assets/1374dc7b-491f-4069-8dfb-8fa378b7ae09" />

Main Window after reading from a  browser



<img width="702" height="531" alt="Screenshot 2025-08-21 at 21 37 40" src="https://github.com/user-attachments/assets/d207f954-7450-4891-b62f-909356133b4c" />

The Diagnostic Window containing extra information (Ctrl + I)


---

## Permissions (macOS Safari)

Safari cookies live in protected app containers. To read them:

1. **Quit Safari** (⌘Q) to release file locks.  
2. **System Settings → Privacy & Security → Full Disk Access**  
   - Add **Terminal** (if running from terminal), **VS Code** (if launching from VS Code), and/or your **CookieManagementSystem.app**.  
3. Try **Read Cookies → Safari** again.

> **BinaryCookies** are read‑only (no deletion). SQLite stores under `NetworkPersistentStorage` may be writable but are not created in all setups.

---

## Project Layout

```
cookie-management-system-main/
├─ project/                     # qmake project folder (build from here)
│  ├─ CookieManagementSystem.pro
│  └─ build/                    # out-of-source build output (created by make)
├─ src/                         # C++ sources
├─ include/                     # headers
├─ ui/                          # .ui forms
├─ resources/
│  ├─ CookieManagementSystem.qrc
│  ├─ cookie.png                # window icon (via .qrc)
│  └─ cookie.icns               # (optional) macOS bundle icon
└─ data/                        # sample csv, etc.
```

---

## App Icon

- **Window icon** is set in `main.cpp` via `QIcon(":/resources/cookie.jpg")` from `resources/CookieManagementSystem.qrc`.
- **macOS bundle icon (Dock/Finder)**: place `resources/cookie.icns` and ensure `.pro` includes:
  ```pro
  macx:exists($$PWD/../resources/cookie.icns) { ICON = $$PWD/../resources/cookie.icns }
  ```
- **Windows exe icon** (optional):
  ```pro
  win32:RC_ICONS = $$PWD/../resources/cookie.ico
  ```

---

## Packaging (Linux AppImage — optional)

```bash
# Build release
qmake CONFIG+=release
make -j"$(nproc)"

# Create AppImage
wget -O linuxdeployqt https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage
chmod +x linuxdeployqt
mkdir -p AppDir/usr/bin AppDir/usr/share/applications AppDir/usr/share/icons/hicolor/256x256/apps
cp build/bin/CookieManagementSystem AppDir/usr/bin/
printf "[Desktop Entry]\nType=Application\nName=Cookie Management System\nExec=CookieManagementSystem\nIcon=cms\nCategories=Utility;Security;Qt;\nTerminal=false\n" > AppDir/usr/share/applications/cms.desktop
cp resources/cookie.png AppDir/usr/share/icons/hicolor/256x256/apps/cms.png
./linuxdeployqt AppDir/usr/share/applications/cms.desktop -appimage -unsupported-allow-new-glibc
```

---

## Troubleshooting

- **No browsers found**  
  Ensure the browser is installed and has run at least once (so its profile/cookie DB exists).

- **Rows reappear after delete (Firefox/Chrome)**  
  Close the browser (SQLite lock) and retry. We delete by **name + host** (exact and dotted).

- **Safari shows 0 rows / Readable: no**  
  Grant **Full Disk Access** to the app/Terminal/VS Code (see *Permissions* above).

- **Crash/slow when deleting many cookies**  
  We batch deletes with transactions; if your profiles are huge, keep the app responsive by closing the browser first.

- **Linux: xcb plugin error**  
  Install the `libxcb*` packages listed in the Ubuntu section.

---

## Development Tips

- Clean rebuild:
  ```bash
  cd project
  rm -rf build .qmake.stash
  qmake
  make -j4
  ```

- VS Code (Run button):
  `.vscode/launch.json`
  ```json
  {
    "version": "0.2.0",
    "configurations": [
      {
        "name": "Run CookieManagementSystem",
        "type": "cppdbg",
        "request": "launch",
        "program": "${workspaceFolder}/project/build/bin/CookieManagementSystem",
        "cwd": "${workspaceFolder}",
        "MIMode": "gdb",
        "stopAtEntry": false
      }
    ]
  }
  ```

---

## Limitations & Notes

- Safari **BinaryCookies**: read‑only.  
- Schema differences exist across browser versions; the app autodetects table/column variants and uses fast batched SQL where possible.  
- Deletion for Firefox/Chromium matches **name + host** (exact and dotted). If you want strict per‑path deletion, extend `CookieItem` with `path`.

---

## License

MIT (see /docs/`LICENSE`).

---

## Contributing

Issues and PRs welcome:
- Repro steps and screenshots help.
- For new browser variants, include a sample cookies DB schema (`PRAGMA table_info(...)` output).
