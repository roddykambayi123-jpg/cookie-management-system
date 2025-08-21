# Cookie Management System

A cross-platform **Cookie Management System (CMS)** built with Qt and C++ that allows users to view, analyze, and manage browser cookies.  
Supports major browsers like **Safari, Firefox, Chrome, Edge and Brave**.

---

## ✨ Features

- Detects installed browsers automatically  
- Reads cookies from **Chromium, Firefox, Safari**  
- Displays cookies in a **table view** with categories  
- Cookie anonymisation for sensitive identifiers (e.g., `_ga`, `SID`, `sessionid`)  
- Delete cookies from the database (not just UI view)  
- Category filtering, including `Unknown`  
- Diagnostics dialog for debugging  
- Color-coded category badges (Marketing, Analytics, etc.)  
- Cross-platform support (macOS, Linux, Windows planned)  
- Application icon support (`cookie.icns` / `cookie.png`)  

---

## 🚀 How to Build

### macOS
```bash
brew install qt sqlite
qmake project/cookiemanagementsystem.pro
make -j4
./build/bin/CookieManagementSystem.app
```

### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools libsqlite3-dev build-essential
qmake project/cookiemanagementsystem.pro
make -j4
./build/bin/CookieManagementSystem
```

### Windows
1. Install [Qt](https://www.qt.io/download) and MSVC or MinGW.  
2. Open `Qt Creator`.  
3. Load `project/cookiemanagementsystem.pro`.  
4. Build & Run from within Qt Creator.  

---

## 🖥️ How to Run on a Fresh Machine

1. **Clone the repo**
   ```bash
   git clone https://github.com/roddykambayi123-jpg/cookie-management-system-main.git
   cd cookie-management-system-main/project
   ```

2. **Install dependencies**
   - macOS: `brew install qt sqlite`  
   - Linux: `sudo apt install qtbase5-dev qt5-qmake libsqlite3-dev build-essential`  
   - Windows: Install Qt + compiler  

3. **Build the project**
   ```bash
   cd project
   qmake cookiemanagementsystem.pro
   make -j4
   ```

4. **Run the app**
   ```bash
   ./build/bin/CookieManagementSystem
   ```

---

## 📂 Project Structure

```
cookie-management-system-main/
│── include/        # Header files
│── src/            # Source files
│── ui/             # UI forms (.ui)
│── resources/      # Icons, .qrc
│── project/        # .pro file
│── build/          # Build output
│── data/           # Sample cookies
│── README.md
```

---

## 📸 Screenshots

(Add screenshots of app running, cookie table view, and diagnostics dialog)
<img width="1188" height="850" alt="Screenshot 2025-08-21 at 21 37 17" src="https://github.com/user-attachments/assets/1374dc7b-491f-4069-8dfb-8fa378b7ae09" />
<img width="702" height="531" alt="Screenshot 2025-08-21 at 21 37 40" src="https://github.com/user-attachments/assets/d207f954-7450-4891-b62f-909356133b4c" />

---

## ⚠️ Notes

- **Safari cookies** may be protected by OS sandboxing. Full deletion might not be possible.  
- This is an **educational/demo project** – not production-grade cookie manipulation software.  
- Cross-platform testing is ongoing – report bugs via GitHub Issues.  

---

## 📜 License

MIT License © 2025 Roddy Kambayi
