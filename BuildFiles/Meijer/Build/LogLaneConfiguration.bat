@echo off

rem Param1 = %AppDir% 
rem Param2 = %TempDir% 
rem Param3 = %ProductVer%

echo , >> \scot\Install\"InstallHistory.log"

set TAB=NO
set DATATEMP=C:\scot\logs\.Data
C:\scot\bin\ini2reg.exe /rr "SOFTWARE\NCR\SCOT - Platform\ObservedOptions" TakeAwayBelts /f c:\scot\bin\config.ini  > %DATATEMP%
FOR /F "tokens=1,2" %%a IN (%DATATEMP%) DO set TAB=%%a
if exist %DATATEMP% del /Q %DATATEMP%
if /i "%TAB%"=="YES" goto :LogTAB

echo Configured as Regular FastLane. >> \scot\Install\InstallHistory.log
goto :LogBuild

:LogTAB
echo Configured as TakeAwayBelt FastLane. >> \scot\Install\InstallHistory.log

:LogBuild
echo Successfully installed Meijer build %3 >> \scot\Install\InstallHistory.log

goto :eof