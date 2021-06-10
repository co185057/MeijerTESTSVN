del /q C:\SCOT\Install\SuccessLines 2>NUL
del /q C:\SCOT\Install\AllSuccessLines 2>NUL
@findstr /I /b "Success= z:\scot\install\InstallHistory.log," C:\SCOT\Upcon\upcon.ini | find /I "%1" > C:\SCOT\Install\SuccessLines
@for /f tokens^=2^ delims^=^" %%i in (C:\SCOT\Install\SuccessLines) do @echo %%i >> C:\SCOT\Install\AllSuccessLines 
