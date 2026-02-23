@echo off
REM Install-Qt6.bat - Simple Qt6 installation helper for Windows

setlocal enabledelayedexpansion

echo.
echo ╔════════════════════════════════════════╗
echo ║   Qt6 Installation Helper               ║
echo ║   WIZZ Mania Build Setup                ║
echo ╚════════════════════════════════════════╝
echo.

REM Check if Qt6 is already installed
set "QT_PATH=C:\Qt\6.10.1\mingw_64"

if exist "%QT_PATH%\lib\cmake\Qt6\Qt6Config.cmake" (
    echo ✓ Qt6 found at: %QT_PATH%
    setx Qt6_DIR "%QT_PATH%"
    echo ✓ Environment variable Qt6_DIR set
    goto BUILD
)

echo.
echo Qt6 not found. Two options:
echo.
echo [1] Download and Install Qt6 Installer (Recommended)
echo     - Go to: https://www.qt.io/download
echo     - Download: Qt Online Installer for Windows
echo     - Run installer with these settings:
echo       * Install Path: C:\Qt
echo       * Version: 6.10.1 or newer
echo       * Compiler: MinGW 11.2.0 (or MSVC 2022)
echo       * Components: Core, Gui, Widgets, Network, Sql
echo.
echo [2] Provide custom Qt6 installation path
echo.
echo Choose [1] or [2] (or press Enter to skip for now):
set /p choice="Your choice: "

if "%choice%"=="1" (
    echo.
    echo Please install Qt6 using the online installer:
    echo https://www.qt.io/download
    echo.
    echo After installation, run this script again.
    echo.
    pause
    goto END
)

if "%choice%"=="2" (
    set /p custom_path="Enter Qt6 installation path (e.g., C:\Qt\6.10.1\mingw_64): "
    if exist "!custom_path!\lib\cmake\Qt6\Qt6Config.cmake" (
        setx Qt6_DIR "!custom_path!"
        echo ✓ Qt6_DIR set to: !custom_path!
        goto BUILD
    ) else (
        echo ✗ Qt6 not found at: !custom_path!
        goto END
    )
)

:BUILD
echo.
echo ═════════════════════════════════════════════════
echo Ready to build! Run these commands:
echo ═════════════════════════════════════════════════
echo.
echo   cd "c:\Users\User\Documents\WizzMania\build"
echo   cmake ..
echo   cmake --build . --config Release
echo.
echo ═════════════════════════════════════════════════
echo.
pause

:END
endlocal
