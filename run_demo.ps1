param()

# Set up paths for Qt and MinGW
$env:Path = "C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.10.2\mingw_64\bin;" + $env:Path

# Kill any existing instances to start fresh
Stop-Process -Name WizzServer -Force -ErrorAction SilentlyContinue
Stop-Process -Name WizzClient -Force -ErrorAction SilentlyContinue

# Start the WizzServer
Write-Host "Starting WizzServer..."
Start-Process -FilePath "build\WizzServer.exe"
Start-Sleep -Seconds 2

# Load Windows Forms for SendKeys to automate typing
Add-Type -AssemblyName System.Windows.Forms

# Start Client 1
Write-Host "Starting Client 1..."
Start-Process -FilePath "build\WizzClient.exe"
Start-Sleep -Seconds 3

# Send "user1", tab to password, "pass1", tab to login button, press enter
[System.Windows.Forms.SendKeys]::SendWait("user1{TAB}pass1{TAB}{ENTER}")
Start-Sleep -Seconds 2

# Start Client 2
Write-Host "Starting Client 2..."
Start-Process -FilePath "build\WizzClient.exe"
Start-Sleep -Seconds 3

# Send "user2", tab to password, "pass2", tab to login button, press enter
[System.Windows.Forms.SendKeys]::SendWait("user2{TAB}pass2{TAB}{ENTER}")
Start-Sleep -Seconds 2

# Since Client 2 is still focused, let's type a message and hit enter
# Wait, mainwindow has a chat input field? Let's try sending a message
[System.Windows.Forms.SendKeys]::SendWait("Hello from user 2! This is an automated message.{ENTER}")
Start-Sleep -Seconds 2

Write-Host "Automation complete!"
