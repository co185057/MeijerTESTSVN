
set UPCON_ERRORFILE=c:\scot\util\upconupdate.log
set UPCON_ERRORFILE_APPLY=c:\scot\install\upconupdate.log
set COPYCMD=/Y
set UTILDIR=C:\scot\util
set BINDIR=C:\scot\bin

echo. > UPCON_ERRORFILE_APPLY

rem set env variables for upcon server
call c:\scot\util\UpConServer.bat


:IsUpcon
c:\scot\util\sendmessage /cp upcon.exe /S
@if NOT %ERRORLEVEL%==9 goto :IsUpconEnd
call :displayandlog UpCon.exe is running on that machine
call :runonce Upcon cmd.exe /c start c:\\scot\\upcon\\upcon.exe %UserName% %Password%
:IsUpconEnd

call :ShowPopup

rem call :CheckLastReboot

call :displayandlog stop scotall
net use /Persistent:no

call :StopAll

call :ShowPopup

net start workstation
call :displayandlog net use v: /d
net use v: /d
rem if exist v:\. call :displayandlog *** net use v: /d failed with error=%ERRORLEVEL% ***
rem if exist v:\. goto :StopAllError

REM POS10232 Upcon encryption tool
call :displayandlog  execute EncryptedMapDrive
pushd c:\scot\util
EncryptedMapDrive -c \\%UpconIP%\c$ >> UPCON_ERRORFILE_APPLY 2>>&1
popd
call :displayandlog  executed EncryptedMapDrive -c \\%UpconIP%\c$

REM set NETUSE=net use v: \\%UpconIP%\c$ /user:%UserName% %Password%
REM call :displayandlog %NETUSE%
REM %NETUSE% >> UPCON_ERRORFILE_APPLY 2>>&1
REM @if NOT %ERRORLEVEL%==0 goto :StopAllError

call :displayandlog  execute update package
v:
cd \updates

call Update.bat

call :displayandlog now reboot the system for update to take effect
call :RebootSystem
goto :eof

:StopAll

call c:\scot\util\stopAllFastLaneApps.bat 300
if NOT %ERRORLEVEL%==0 goto :StopAllError
goto :StopAllEnd

:StopAllError

call :displayandlog Inside StopAllError...

:RebootOnError
call :displayandlog DisconnectAllAZ.
call :DisconnectAllAZ

rem call :displayandlog Set RunOnce to Restart upconupdatepatchballapply.bat
rem call :runonce UpconUpdate cmd.exe /c c:\\scot\\util\\upconupdatepatchballapply.bat

call :displayandlog Reboot on stopAll errors.
call :RebootSystem
rem start c:\scot\util\initsys.exe
exit

:StopAllEnd
goto :eof

:runonce
@echo off
call :displayandlog :runonce function
set F=%TEMP%\%1runonce.reg
echo REGEDIT4> %F%
echo [HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\RunOnce]>>%F%
echo "%1"="%2 %3 %4 %5 %6 %7">>%F%
regedit /s %F%
@if NOT %ERRORLEVEL%==0 call :displayandlog Failed to apply %F%
goto :EOF

:displayandlog 
  call :GetDateTime
  echo %*
  @echo %DT% !%*! >> %UPCON_ERRORFILE% 2>>&1
  @echo %DT% !%*! >> %UPCON_ERRORFILE_APPLY% 2>>&1
goto :EOF

:: ======================================================================
 :GetDateTime
:: ======================================================================
for /f "tokens=*" %%i in ('date /t') do set t=%%i
set SEP=%t:~4,1%
echo %SEP%
set TID=2
if "%SEP%"=="/" set TID=1
for /f "tokens=%TID%" %%i in ('date /t') do set date1=%%i
for /f "tokens=1" %%i in ('time /t') do set time1=%%i 
set DT=%date1% %time1%
goto :EOF


:DisconnectAllAZ
net use \\maria\document /d
net use d: /d
net use e: /d
net use f: /d
net use g: /d
net use h: /d
net use j: /d
net use k: /d
net use l: /d
net use m: /d
net use n: /d
net use o: /d
net use p: /d
net use q: /d
net use r: /d
net use s: /d
net use t: /d
net use u: /d
net use v: /d
net use w: /d
net use x: /d
net use y: /d
net use z: /d
goto :EOF

:ShowPopup
call :GetTime
call :displayandlog popup the msg screen for update
c:\scot\util\createpopup.exe "c:\scot\util\popup.exe /focus /MSG Update in progress - Start: %time2% - wait for reboot..."
goto :eof

:GetLastRebootTime
for /f "tokens=*" %%i in ('%UTILDIR%\TimeSinceReboot.exe') do set SINCE_LAST_REBOOT=%%i
goto :eof


:CheckLastReboot
call :GetLastRebootTime
call :displayandlog Time since last reboot on this machine= %SINCE_LAST_REBOOT% ms
call :displayandlog Making sure at least 10 minutes have elapsed since last reboot
call c:\scot\util\TimeSinceReboot.exe /w 600000
call :displayandlog Done waiting on startup...
goto :eof

:: ======================================================================
 :GetTime
:: ======================================================================
for /f "tokens=1-2" %%i in ('time /t') do set time2=%%i %%j
goto :EOF

:RebootSystem
call :GetTime
c:\scot\util\createpopup.exe "c:\scot\util\popup.exe /focus /MSG Install Complete %time2% - wait for reboot..."
start /min cmd /c c:\scot\util\WaitAndShutdown.bat 120
rem wait for command to start
call Sleep 15
rem now try to initsys...
call c:\scot\util\initsys.exe
exit
