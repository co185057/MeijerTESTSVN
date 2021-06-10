@echo off

rem Param1 = %ProductVer%

rem Copying Files necessary for PayStation

set COPYCMD=/Y
echo , >> \scot\Install\"InstallHistory.log"
echo Starting ConvertToPayStation... >> \scot\Install\InstallHistory.log
xcopy /R /Y c:\scot\Config\PayStation\*.* c:\scot\Config\

set HWType=
set DATATEMP=C:\scot\logs\.Data
C:\scot\bin\ini2reg.exe /rr "SOFTWARE\NCR\SCOT - Platform\ObservedOptions" HWSpec /f c:\scot\bin\config.ini  > %DATATEMP%
FOR /F "tokens=1,2" %%a IN (%DATATEMP%) DO set HWType=%%a
if exist %DATATEMP% del /Q %DATATEMP%
if "%HWType%"=="MULTI" goto :SetServiceCenterFiles 

echo Configured as PayStation. >> c:\scot\Install\InstallHistory.log
goto :LogBuild

:SetServiceCenterFiles

cd c:\updates

xcopy /R /Y c:\updates\ADD.ini c:\scot\config\

echo Configured as PayStation Service center. >> c:\scot\Install\InstallHistory.log

:LogBuild
echo Successfully installed Meijer build %1 >> \scot\Install\InstallHistory.log
date /T >> c:\scot\Install\InstallHistory.log
time /T >> c:\scot\Install\InstallHistory.log

goto :eof