FOR /F "TOKENS=1* DELIMS= " %%A IN ('DATE/T') DO SET CDATE=%%B
FOR /F "TOKENS=1,2 eol=/ DELIMS=/ " %%A IN ('DATE/T') DO SET mm=%%B
FOR /F "TOKENS=1,2 DELIMS=/ eol=/" %%A IN ('echo %CDATE%') DO SET dd=%%B
FOR /F "TOKENS=2,3 DELIMS=/ " %%A IN ('echo %CDATE%') DO SET yyyy=%%B
SET PACKAGE_DATE=%yyyy%%mm%%dd%

REM for /F "usebackq" %%i in (`C:\cygwin\bin\date.exe +%%Y%%m%%d`) do set PACKAGE_DATE=%%i



REM create the EMPLID_ON BIOMET_OFF package
call :DeleteOldFiles

call :BuildPackage

REM move "d:\Meijer Package"\updates.exe FL%PACKAGE_DATE%%1_EMPLID_ON_BIOMET_OFF.exe 

move "d:\Meijer Package"\updates.exe FL%PACKAGE_DATE%%1.exe 




goto :eof

:DeleteOldFiles
del /q /f "d:\Meijer Package"\updates.zip
del /q /f "d:\Meijer Package"\updates.exe
goto :eof


:BuildPackage
pushd .
cd "d:\Meijer Package"
"c:\Program Files"\WINZIP\wzzip -rP updates.zip scot updates
"c:\Program Files\WinZip Self-Extractor\WZIPSE32.EXE" -auto updates.zip -overwrite -d c:\
popd
goto :eof
