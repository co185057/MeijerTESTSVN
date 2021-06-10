@echo off
SETLOCAL ENABLEEXTENSIONS
SETLOCAL ENABLEDELAYEDEXPANSION
::====================================================================
::                     stopAllFastLaneApps.bat
::
:: 1st parm = timer to indicate how long, 0 or timer amount.  0=forever
::====================================================================

:: first parameter is the timer value
set TIMER=%1

:: If timer is 0, loop forever waiting for scotapp to terminate
if %TIMER%.==0. (
  set INFINITE=1
  set TIMER=600
)

set EXITCODE=0
set IsLane=TRUE
set SLEEP_TIMER=2

if not defined APP_DRIVE set APP_DRIVE=c:
call :batchEnvironSetup
if not %EXITCODE%.==0. goto :END

call :displayandlog ******************** stopAllFastLaneApps - Begin ***********************
call :displayandlog %~dp0%0 %TIMER%

call :GetProcessNames
rem if not %SCOTAPP%.==. goto :StopLane
rem if not %RAPAPP%.==.  goto :StopRAP
if exist c:\scot\bin\scotappu.exe goto :StopLane
if exist c:\scot\bin\scotapp.exe goto :StopLane
if exist c:\scot\bin\rapnet.exe goto :StopRap
goto :StopServices

:StopLane
::call :displayandlog Begin Lane Status query
::call :QuerySCOT

call :displayandlog Stopping %SCOTAPP% now
call :StopSCOT

call :displayandlog Checking Lane Status again.
call :IsProcessSCOTAlive
if defined SCOTAPP call :killApp %SCOTAPP%

call :displayandlog Shutting down ACS
cmd /c c:\scot\config\postscotstop.bat

call :killApp traceexe.exe
goto :StopServices

:StopRAP
REM call :displayandlog Begin RAP Status query
REM call :QueryRAP

:: skip StopRAP if it is not running
rem don't worry about whether the rap was up or not
rem if not defined ISACTIVE goto :StopServices  

call :displayandlog Stopping %RAPAPP% now
call :StopRAPApp

call :displayandlog Checking RAP Status again.
call :IsProcessRAPAlive
if defined RAPAPP call :killApp %RAPAPP%

::=========================================
:StopServices
::=========================================
::call :displayandlog Now stopping other processes
::if defined WEDGEAPP call :killApp %WEDGEAPP%

::call :displayandlog Now stopping RPSW Services
::if defined NCRLOADER    call :stopservice NCR Loader Service
::if defined NCRSMCLIENT  call :stopservice NCR Store Minder Client

call :displayandlog Last check of running processes before exiting
call :GetProcessNames
goto :END
:: ==============================================


::=========================================
:QuerySCOT
::=========================================
set tmpFile=%temp%\querySCOT.log
set IsNotRunning=

for /L %%A in (1,%SLEEP_TIMER%,%TIMER%) do (
  %SENDSCOT% -Query -D>"%tmpFile%" 2>&1
  for /f "tokens=1,2,3,4*" %%a IN (%tmpFile%) DO if /i %%c.==not. set IsNotRunning=TRUE
  
  rem First check if ScotApp is still in a transaction or not
  if defined IsNotRunning goto :FinishQuery

  rem output progress indicator
  >>%TRACELOG% Set /p count=.<%TRACELOG%
  
  rem if neither are true, sleep for %SLEEP_TIMER% second and try again.
  %SCOTBIN%\sleep.exe %SLEEP_TIMER%
)
if defined INFINITE call :QueryScot

:FinishQuery
>>%TRACELOG% echo.

goto :eof
:: ==============================================


::=========================================
:QueryRAP 
:: - check is RAP is running or not ... skip stopRAP if not running
::=========================================
set tmpFile=%temp%\queryRAP.log
set ISACTIVE=
%SENDSCOT% -RAPRunning -D >"%tmpFile%" 2>&1
for /f "tokens=1,2,3,4*" %%a IN (%tmpFile%) DO if /i %%c.==active. set ISACTIVE=1
goto :eof
:: ==============================================


::=========================================
:StopSCOT
::=========================================
set tmpFile=%temp%\StopSCOT.log

%SENDSCOT% -Stop -D >>%TRACELOG% 2>&1
>>%TRACELOG% echo.
goto :eof
:: ==============================================


::=========================================
:StopRAPApp
::=========================================
set tmpFile=%temp%\StopRAP.log

rem %SENDSCOT% -StopRAP -D >>%TRACELOG% 2>&1
%SENDSCOT% -ForceStopRAP -D >>%TRACELOG% 2>&1
%SENDSCOT% -Stop -D >>%TRACELOG% 2>&1
call :killApp NCRDeviceServer.exe
>>%TRACELOG% echo.
goto :eof
:: ==============================================


::=========================================
:IsProcessSCOTAlive
::=========================================
set tmpFile=%PROCESSLIST_LOG%
%SENDMSG% /p > %tmpFile%

set SCOTAPP=
FOR /F "tokens=1,2" %%a IN (%tmpFile%) DO (
  set Processname=%%b
  set ProcessShort=!Processname:~0,7!
  if /i !ProcessShort!.==SCOTAPP. set SCOTAPP=!Processname!
)
goto :eof
:: ==============================================


::=========================================
:IsProcessRAPAlive
::=========================================
set tmpFile=%PROCESSLIST_LOG%
%SENDMSG% /p > "%tmpFile%"

set RAPAPP=
FOR /F "tokens=1,2" %%a IN (%tmpFile%) DO (
  set Processname=%%b
  set ProcessShort=!Processname:~0,3!
  if /i !ProcessShort!.==RAP. set RAPAPP=!Processname!
)
goto :eof
:: ==============================================

::=========================================
:killApp
::=========================================
set tmpFile=%temp%\Kill %*.log
call :displayandlog Attempting to bring down %*

%SCOTBIN%\kill.exe /f "%*" >"%tmpFile%" 2>&1
set ERRNO=%ERRORLEVEL%
  if %ERRNO%.==603. goto :eof
  if %ERRNO%.==0.   goto :eof
goto :stopError %*
:: ==============================================


::=========================================
:stopService
::=========================================
set tmpFile=%temp%\Stop %*.log
call :displayandlog Attempting to bring down %*
net start | find /I "%*" >NUL && net stop "%*" /Y>"%tmpFile%" 2>&1
set ERRNO=%ERRORLEVEL%
if not %ERRNO%.==0. goto :stopError %*
goto :eof
:: ==============================================


::=========================================
:stopError
::=========================================
call :displayandlog Failed to stop %* with error (%ERRNO%)
set EXITCODE=%ERRNO%
call :displayandlog Start printing Process List at time of error.
type %PROCESSLIST_LOG% >> "%TRACELOG%" 2>&1
call :displayandlog End printing Process List at time of error.
goto :END
:: ==============================================


::=========================================
:GetProcessNames
::=========================================
set tmpFile=%PROCESSLIST_LOG%
%SENDMSG% /p > "%tmpFile%"

set SCOTAPP=
FOR /F "tokens=1,2" %%a IN (%tmpFile%) DO (
  set Processname=%%b
  set ProcessShort=!Processname:~0,7!
  if /i !ProcessShort!.==SCOTAPP. set SCOTAPP=!Processname!
)
if defined SCOTAPP call :displayandlog FastLane Application=%SCOTAPP%

set SWITCHAPP=
FOR /F "tokens=1,2" %%a IN (%tmpFile%) DO (
  set Processname=%%b
  set ProcessShort=!Processname:~0,6!
  if /i !ProcessShort!.==SWITCH. set SWITCHAPP=!Processname!
)
if defined SWITCHAPP call :displayandlog Switch Authentication App=%SWITCHAPP%

set RAPAPP=
FOR /F "tokens=1,2" %%a IN (%tmpFile%) DO (
  set Processname=%%b
  set ProcessShort=!Processname:~0,3!
  if /i !ProcessShort!.==RAP. set RAPAPP=!Processname!
)
if defined RAPAPP call :displayandlog Rap Application=%RAPAPP%

set LPAPP=
FOR /F "tokens=1,2" %%a IN (%tmpFile%) DO (
  set Processname=%%b
  set ProcessShort=!Processname:~0,9!
  if /i !ProcessShort!.==LaunchPad. set LPAPP=!Processname!
)
if defined LPAPP call :displayandlog LaunchPadNet App=%LPAPP%

set LCAPP=
FOR /F "tokens=1,2" %%a IN (%tmpFile%) DO (
  set Processname=%%b
  set ProcessShort=!Processname:~0,11!
  if /i !ProcessShort!.==loadcontrol. set LCAPP=!Processname!
)
if defined LCAPP call :displayandlog Load Controller App=%LCAPP%

set WEDGEAPP=
FOR /F "tokens=1,2" %%a IN (%tmpFile%) DO (
  set Processname=%%b
  set ProcessShort=!Processname:~0,8!
  if /i !ProcessShort!.==NCRWEDGE. set WEDGEAPP=!Processname!
)
if defined WEDGEAPP call :displayandlog Wedge Application=%WEDGEAPP%

set ONTRAKAPP=
FOR /F "tokens=1,2" %%a IN (%tmpFile%) DO (
  set Processname=%%b
  set ProcessShort=!Processname:~0,7!
  if /i !ProcessShort!.==NCRONTR. set ONTRAKAPP=!Processname!
)
if defined ONTRAKAPP call :displayandlog OnTrack App=%ONTRAKAPP%

set NCRDLLLOADER=
FOR /F "tokens=1,2" %%a IN (%tmpFile%) DO (
  set Processname=%%b
  set ProcessShort=!Processname:~0,7!
  if /i !ProcessShort!.==NCRDLLL. set NCRDLLLOADER=!Processname!
)
if defined NCRDLLLOADER call :displayandlog NCR DLL Loader Service=%NCRDLLLOADER%

set NCRLOADER=
FOR /F "tokens=1,2" %%a IN (%tmpFile%) DO (
  set Processname=%%b
  set ProcessShort=!Processname:~0,7!
  if /i !ProcessShort!.==NCRLOAD. set NCRLOADER=!Processname!
)
if defined NCRLOADER call :displayandlog NCR Loader Service=%NCRLOADER%

set NCRSMCLIENT=
FOR /F "tokens=1,2" %%a IN (%tmpFile%) DO (
  set Processname=%%b
  set ProcessShort=!Processname:~0,7!
  if /i !ProcessShort!.==NCRSMCL. set NCRSMCLIENT=!Processname!
)
if defined NCRSMCLIENT call :displayandlog NCR Store Minder Client Service=%NCRSMCLIENT%

set FTS=
FOR /F "tokens=1,2" %%a IN (%tmpFile%) DO (
  set Processname=%%b
  set ProcessShort=!Processname:~0,20!
  if /i !ProcessShort!.==FileTransportService. set FTS=!Processname!
)
if defined FTS call :displayandlog File Transport Service=%FTS%

set RSHD=
FOR /F "tokens=1,2" %%a IN (%tmpFile%) DO (
  set Processname=%%b
  set ProcessShort=!Processname:~0,4!
  if /i !ProcessShort!.==rshd. set RSHD=!Processname!
)
if defined RSHD call :displayandlog Remote Shell Deamon=%RSHD%

set TMS32=
FOR /F "tokens=1,2" %%a IN (%tmpFile%) DO (
  set Processname=%%b
  set ProcessShort=!Processname:~0,5!
  if /i !ProcessShort!.==tms32. set TMS32=!Processname!
)
if defined TMS32 call :displayandlog TMS32=%TMS32%

goto :eof
:: ==============================================


:: ==============================================
:batchEnvironSetup
:: ==============================================
if not defined APP_DRIVE    set APP_DRIVE=c:
if not defined TEMP_DRIVE   set TEMP_DRIVE=%APP_DRIVE%

if not defined temp         set temp=%DATA_DRIVE%\temp
if not defined tmp          set tmp=%temp%
if not defined TEMP         set TEMP=%temp%
if not defined TMP          set TMP=%temp%

set EXITCODE=1
set SCOTDIR=%APP_DRIVE%\scot
set SCOTBIN=%SCOTDIR%\bin
set SCOTINSTALL=%SCOTDIR%\install
set SCOTUTIL=%SCOTDIR%\util
set SCOTFRMUTIL=%SCOTDIR%\Firmware\Util
if not defined HISTORYLOG set HISTORYLOG=%SCOTINSTALL%\InstallHistory.log
if not defined TRACELOG   set TRACELOG=%SCOTINSTALL%\InstallTrace.log

if exist "%SCOTBIN%\SendScot.exe" set SENDSCOT=%SCOTBIN%\SendScot.exe
if exist "%SCOTFRMUTIL%\SendScot.exe" set SENDSCOT=%SCOTFRMUTIL%\SendScot.exe
if exist "%SCOTUTIL%\SendScot.exe" set SENDSCOT=%SCOTUTIL%\SendScot.exe
if not defined SENDSCOT (
  call :displayandlog SENDSCOT cannot be found on filesystem
  goto :eof
)

if exist "%SCOTBIN%\sendmessage.exe" set SENDMSG=%SCOTBIN%\sendmessage.exe
if exist "%SCOTUTIL%\sendmessage.exe" set SENDMSG=%SCOTUTIL%\sendmessage.exe
if exist "%SCOTFRMUTIL%\sendmessage.exe" set SENDMSG=%SCOTFRMUTIL%\sendmessage.exe
if not defined SENDMSG (
  call :displayandlog SENDMSG cannot be found on filesystem
  goto :eof
)

set PROCESSLIST_LOG=%temp%\ProcessList.log

if defined DEBUG (
  call :displayandlog APP_DRIVE=%APP_DRIVE%
  call :displayandlog TEMP_DRIVE=%TEMP_DRIVE%
  call :displayandlog temp=%temp%
  call :displayandlog SCOTDIR=%SCOTDIR%
  call :displayandlog SCOTBIN=%SCOTBIN%
  call :displayandlog SCOTINSTALL=%SCOTINSTALL%
  call :displayandlog SCOTUTIL=%SCOTUTIL%
  call :displayandlog HISTORYLOG=%HISTORYLOG%
  call :displayandlog TRACELOG=%TRACELOG%
  call :displayandlog Using %SENDSCOT%
  call :displayandlog Using %SENDMSG%
)
set EXITCODE=0
goto :eof
:: ==============================================

:: ==============================================
:displayandlog
:: ==============================================
set TextOut=%*

call :GetDateTime
echo %TextOut%
@echo %DT% %TextOut% >> "%TRACELOG%" 2>>&1
goto :eof
:: ==============================================

:: ==============================================
:GetDateTime
:: ==============================================
for /f "tokens=*" %%i in ('date /t') do set date1=%%i 
set CD=%date1%
for /f "tokens=*" %%i in ('time /t') do set time1=%%i 
set CT=%time1%
set DT=%CD%%CT: =%
goto :eof
:: ==============================================

::=========================================
:END
::=========================================
%SENDSCOT% -killpopup

call :displayandlog EXITCODE %EXITCODE%
call :displayandlog ******************** stopAllFastLaneApps - End   ***********************

exit /B %EXITCODE%
