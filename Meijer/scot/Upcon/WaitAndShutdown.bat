@echo off
setlocal ENABLEEXTENSIONS
set LOGFILE=c:\scot\install\WaitAndShutdown.log

echo ==================== > %LOGFILE%

set WAITTIME=%1
if "%WAITTIME%"=="" set WAITTIME=120

call :displayandlog sleeping for %WAITTIME% seconds before calling shutdown...
call Sleep %WAITTIME%
call :displayandlog calling shutdown.exe to reboot...
shutdown.exe -r -f -t 0

setlocal DISABLEEXTENSIONS

goto :end

:displayandlog
call :GetDateTime
echo %DT% - !%*! >> %LOGFILE%
echo !%*!
goto :eof


:: ======================================================================
 :GetDateTime
:: ======================================================================
for /f "tokens=*" %%i in ('date /t') do set t=%%i
set SEP=%t:~4,1%
echo %SEP%
set TID=2
if "%SEP%"=="/" set TID=1
for /f "tokens=*" %%i in ('date /t') do set date1=%%i
for /f "tokens=1" %%i in ('time /t') do set time1=%%i 
set DT=%date1% %time1%
goto :EOF

:end
