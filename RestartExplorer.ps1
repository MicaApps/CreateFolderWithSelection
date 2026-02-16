Write-Host "Restarting Windows Explorer..."
Stop-Process -Name explorer -Force
Start-Sleep -Seconds 2
Start-Process explorer
Write-Host "Done. Please try running the app again from Visual Studio."
