#!/usr/bin/env powershell
# Install-Qt6.ps1 - Automated Qt6 installation for Windows

param(
    [string]$QtVersion = "6.10.1",
    [string]$InstallPath = "C:\Qt",
    [string]$Compiler = "mingw_64"  # or 'msvc2022_64'
)

Write-Host "╔════════════════════════════════════════╗"
Write-Host "║   Qt6 Installer for WIZZ Mania        ║"
Write-Host "╚════════════════════════════════════════╝" -ForegroundColor Cyan

# Step 1: Check if Qt is already installed
$QtPath = "$InstallPath\$QtVersion\$Compiler"
if (Test-Path "$QtPath\lib\cmake\Qt6\Qt6Config.cmake") {
    Write-Host "✓ Qt6 already installed at: $QtPath" -ForegroundColor Green
    $env:Qt6_DIR = "$QtPath"
    exit 0
}

# Step 2: Check if installer exists
$InstallerPath = "C:\Temp\qt-unified-windows-x86-4.7.0-online.exe"
if (-not (Test-Path $InstallerPath)) {
    Write-Host ""
    Write-Host "╔════════════════════════════════════════╗"
    Write-Host "║  Qt6 Installer Not Found               ║"
    Write-Host "╚════════════════════════════════════════╝" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Please download Qt6 installer manually:"
    Write-Host ""
    Write-Host "  1. Visit: https://www.qt.io/download/qt-unified-installer"
    Write-Host "  2. Download Qt Online Installer for Windows"
    Write-Host "  3. Save to: $InstallerPath"
    Write-Host ""
    Write-Host "After downloading, run this script again."
    Write-Host ""
    exit 1
}

# Step 3: Download or prepare installer
Write-Host ""
Write-Host "Step 1: Downloading Qt6 (if needed)..." -ForegroundColor Cyan
Write-Host ""

# Try to run installer in non-interactive mode (note: Qt installer is tricky)
# For now, we'll show manual instructions

Write-Host "╔════════════════════════════════════════╗"
Write-Host "║  Manual Installation Steps             ║"
Write-Host "╚════════════════════════════════════════╝" -ForegroundColor Yellow
Write-Host ""

Write-Host "1. Run the Qt Installer:"
Write-Host "   $InstallerPath"
Write-Host ""

Write-Host "2. Select Installation Path:"
Write-Host "   $InstallPath"
Write-Host ""

Write-Host "3. Select Components:"
Write-Host "   ✓ Qt -> Qt $QtVersion"
Write-Host "   ✓ $Compiler (MinGW 11.2.0 or MSVC 2022)"
Write-Host "   ✓ Qt SQL"
Write-Host "   ✓ Qt Network"
Write-Host "   ✓ CMake (if not already installed)"
Write-Host ""

Write-Host "4. Installation will take ~10-15 minutes"
Write-Host ""

Write-Host "5. After installation complete, press Enter and this script will verify..."
Write-Host ""

Read-Host "Press Enter to continue" | Out-Null

# Step 4: Verify installation
if (Test-Path "$QtPath\lib\cmake\Qt6\Qt6Config.cmake") {
    Write-Host ""
    Write-Host "✓ Qt6 installation verified!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Qt6 Details:"
    Write-Host "  Path: $QtPath"
    Write-Host "  Version: $QtVersion"
    Write-Host "  Compiler: $Compiler"
    Write-Host ""
    
    # Set environment variable
    $env:Qt6_DIR = "$QtPath"
    [Environment]::SetEnvironmentVariable("Qt6_DIR", "$QtPath", "User")
    
    Write-Host "Environment variable Qt6_DIR set to:"
    Write-Host "  $env:Qt6_DIR"
    Write-Host ""
    
    Write-Host "Now you can build the project:"
    Write-Host "  cd c:\Users\User\Documents\WizzMania\build"
    Write-Host "  cmake .."
    Write-Host "  cmake --build . --config Release"
    Write-Host ""
    exit 0
} else {
    Write-Host ""
    Write-Host "✗ Qt6 installation verification failed!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Expected to find:"
    Write-Host "  $QtPath\lib\cmake\Qt6\Qt6Config.cmake"
    Write-Host ""
    Write-Host "Please check:"
    Write-Host "  1. Installation path is correct"
    Write-Host "  2. Components were selected properly"
    Write-Host "  3. Installation completed without errors"
    Write-Host ""
    exit 1
}
