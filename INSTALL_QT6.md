# Installing Qt6 for WIZZ Mania Build

## ⚠️ Qt6 Not Found

The build process requires **Qt6 (Core, Gui, Widgets, Network, Sql components)**.

---

## Option 1: Install Qt6 via Official Installer (Recommended)

### 1.1 Download Qt6 Installer

- Go to: https://www.qt.io/download
- Select **Open Source** → **Go to downloads**
- Download **Qt Online Installer for Windows** (~60 MB)

### 1.2 Run Qt Installer

1. Execute the installer
2. Create a Qt account (or login if you have one)
3. Choose **Custom Installation**
4. Select **Qt 6.10.1** (or latest stable 6.x)
5. Select Components:
   - ✅ Desktop - **MinGW 11.2.0 64-bit** (for GCC)
   - ✅ Desktop - **MSVC 2022 64-bit** (if you prefer MSVC)
   - ✅ Qt SQL
   - ✅ Qt Network
   - ✅ CMake >= 3.24
6. Install to default location: `C:\Qt` (or remember your path)

### 1.3 Verify Installation

```bash
ls "C:\Qt\6.10.1\mingw_64\lib\cmake\Qt6"
# Should show: Qt6Config.cmake and other cmake files
```

### 1.4 Configure Build

**Option A: Auto-detection (Recommended)**
- CMakeLists.txt now auto-detects Qt at: `C:\Qt\6.10.1\mingw_64`
- Run build:
  ```bash
  cd build
  cmake ..
  cmake --build . --config Release
  ```

**Option B: Manual Path via CMake**
```bash
cd build
cmake -DQt6_DIR="C:\Qt\6.10.1\mingw_64\lib\cmake\Qt6" ..
cmake --build . --config Release
```

**Option C: Set Environment Variable**
```powershell
$env:Qt6_DIR = "C:\Qt\6.10.1\mingw_64"
cd build
cmake ..
cmake --build . --config Release
```

---

## Option 2: Install Qt6 via vcpkg

If you have **vcpkg** installed:

```bash
vcpkg install qt6:x64-windows
# Then set Qt6_DIR to vcpkg/installed/x64-windows/share/cmake/Qt6
```

---

## Option 3: Install Qt6 via Conan

If you have **Conan** installed:

```bash
conan install . --requires=qt/6.10.1 --build=missing
```

---

## Troubleshooting

### CMake still can't find Qt6

**Solution 1: Clear CMake cache**
```bash
cd build
rm -r CMakeCache.txt CMakeFiles
cmake -DQt6_DIR="<YOUR_QT_PATH>\lib\cmake\Qt6" ..
```

**Solution 2: Check Qt installation**
```bash
# Windows - check if Qt6Config.cmake exists
Test-Path "C:\Qt\6.10.1\mingw_64\lib\cmake\Qt6\Qt6Config.cmake"
# Should return: True
```

**Solution 3: Check CMAKE_PREFIX_PATH**
```bash
cd build
cmake -L | findstr Qt6_DIR
# Should show the Qt6 directory
```

### Build fails with compiler errors

**If using MinGW:**
- Ensure MinGW 11+ is in PATH: `g++ --version`
- Rebuild: `cmake --build . --config Release -- -j4`

**If using MSVC:**
- Ensure MSVC 2022+ is installed
- Change CMakeLists.txt to use MSVC instead of MinGW

---

## Expected Build Output

After successful Qt6 installation, you should see:

```
-- Selecting Windows SDK version 10.0.26100.0
-- Found Qt6 (version 6.10.1)
-- Found Qt6::Core
-- Found Qt6::Gui
-- Found Qt6::Widgets
-- Found Qt6::Network
-- Found Qt6::Sql
[100%] Built target Mess
```

---

## Quick Start After Qt6 Installation

```bash
cd c:\Users\User\Documents\WizzMania\build
cmake ..
cmake --build . --config Release
./Release/Mess.exe --help
```

---

**Installation Time:** ~15 minutes (with download)  
**Qt6 Version:** 6.10.1 LTS Recommended  
**Disk Space:** ~5-10 GB (with all components)

For more help: https://doc.qt.io/qt-6/gettingstarted.html
