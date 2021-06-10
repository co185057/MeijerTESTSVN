set SCOTBIN=%APP_DRIVE%\scot\bin

if exist %TEMP%\.SyncTimeServer del %TEMP%\.SyncTimeServer
%SCOTBIN%\ini2reg.exe /rr "SOFTWARE\NCR\SCOT\CurrentVersion\SCOTAPP\SyncTime" SyncTimeServer /f %APP_DRIVE%\scot\bin\config.ini > %TEMP%\.SyncTimeServer
FOR /F "tokens=1,2" %%a IN (%TEMP%\.SyncTimeServer) DO call :CheckServerName %%a
goto :EOF

:CheckServerName
if "%1"=="" goto :EOF
net time \\%1 /set /yes
goto :EOF
