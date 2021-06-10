for /F "usebackq" %%i in (`C:\cygwin\bin\date.exe +%%Y%%m%%d`) do set PACKAGE_DATE=%%i


REM create the aclient package
call :DeleteOldFiles
call :BuildPackage

move "d:\Meijer Package"\updates.exe FL%PACKAGE_DATE%MEIJER%1.exe


goto :eof

:DeleteOldFiles
del /q "d:\Meijer Package"\updates.zip
del /q "d:\Meijer Package"\updates.exe

goto :eof


:BuildPackage
pushd .
cd "d:\Meijer Package"
"C:\Program Files"\WINZIP\wzzip -rP updates.zip scot updates
"c:\Program Files\WinZip Self-Extractor\WZIPSE32.EXE" -auto updates.zip -overwrite -d c:\
popd
goto :eof
