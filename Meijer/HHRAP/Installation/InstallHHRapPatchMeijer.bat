@echo off
rem @echo on
if exist c:\scot\install\HHRap\SetPartitionDrivers.bat call c:\scot\install\HHRap\SetPartitionDrivers.bat
if exist %APP_DRIVE%\scot\install\HHRap\SetPartitionDrivers.bat call %APP_DRIVE%\scot\install\HHRap\SetPartitionDrivers.bat

set INSTALLDIR=%APP_DRIVE%\scot\install\HHRap
set SCOTBINDIR=%APP_DRIVE%\scot\bin
if not exist %INSTALLDIR% md %INSTALLDIR%
if not exist %SCOTBINDIR% md %SCOTBINDIR%

set ERRORFILEHHRAP=%APP_DRIVE%\scot\install\InstallHistory.log
set TRACEFILEHHRAP=%APP_DRIVE%\scot\install\InstallTrace.log

set HISTORYLOG=%APP_DRIVE%\scot\install\InstallHistory.log
set TRACELOG=%APP_DRIVE%\scot\install\InstallTrace.log
set COPYCMD=/Y

cd /d %INSTALLDIR%

call :getdt
echo ********************************************************************************>> %HISTORYLOG%
echo ********************************************************************************>> %TRACEFILEHHRAP%
echo Begin Date/Time = %DT% >> %HISTORYLOG%
echo Begin Date/Time = %DT% >> %TRACEFILEHHRAP%

echo Installing 4.2.1 HHRap >> %HISTORYLOG%
call :displayandlog NCR HandHeld Rap
rem echo on


set tempFile=%TEMP%\.tmpFile
set CONFIGINI="%INSTALLDIR%\config.ini"
set INI2REG="%INSTALLDIR%\ini2reg.exe"
if not exist %CONFIGINI%    echo. >%CONFIGINI%

if exist %SCOTBINDIR%\activesync.lnk goto :LOADMC50andMC70
copy /Y "%APP_DRIVE%\Documents and Settings\All Users\Start Menu\Programs\Microsoft ActiveSync.lnk" %SCOTBINDIR%\activesync.lnk

:: =================================================
:LOADMC50andMC70
:: =================================================
:: For the MC50 we delete from the registry the name the programs
:: that we are about install to prevent popup message on the handheld
:: closeapp and hhrap. Check if .NET is installed. If it is not need to re install. 
:: If not, when installing on the mc70 pause for the used to press continue. 
:: No need for this on the MC50. We can read from the registry because we know that the
:: handhele is in the cradle otherwise SingleHHUpdate would not return a good status
::-----------------------------------------------------------------------------------
start  " " /MIN %SCOTBINDIR%\activesync.lnk
echo   please wait...
sleep 5
set NET20INSTALLED=""
%INSTALLDIR%\SignalHHUpdate.exe

echo step 1

set savederrorlevel=%ERRORLEVEL%
if  "%savederrorlevel%"=="0" goto :checktypeandcont
if not "%savederrorlevel%"=="9" goto :install_err
set NET20INSTALLED=YES
:checktypeandcont
call :checkhhtype

:install
if /i "%HHRAPTYPE%"=="SYMBOL MC50" goto :installcabs
call :displayandlog Uninstalling the previous version of HHRAP 
call :displayandlog Please wait this may take several minutes......

echo step 2

start /MIN /WAIT %INSTALLDIR%\RapiConfig.exe %INSTALLDIR%\uninstallMeijerPatch.xml

:: call :displayandlog Uninstalling the previous version of.NET Please wait this may take several minutes......
:: start /WAIT %INSTALLDIR%\RapiConfig.exe %INSTALLDIR%\uninstallNET.xml

:: call :displayandlog Uninstalling the previous version of closeapp Please wait this may take several minutes......
:: start /WAIT %INSTALLDIR%\RapiConfig.exe %INSTALLDIR%\uninstallcloseapp.xml

::-------------------------------------------------------------------------------------
:installcabs
::-------------------------------------------------------------------------------------
:: The MC70 has a bug. This registry entry cannot be set in the CAB File. Since we took
:: the entry to set this value out of the cab file we need to add it for both the MC50 & MC70
::
start /MIN /WAIT %INSTALLDIR%\RapiConfig.exe /P /M %INSTALLDIR%\RemConnSocket.xml
start /MIN /WAIT %INSTALLDIR%\RapiConfig.exe /P /M %INSTALLDIR%\HandheldUPCEtoUPCA.xml
start /MIN /WAIT %INSTALLDIR%\RapiConfig.exe /P /M %INSTALLDIR%\HandheldPowerSettings.xml
call :displayandlog Updated the registry.

echo step 3


:: -----------------------------------------------------------------------------------
:: Only have to shut down the HHRAP and Launchpad if we are installing 2 cab files on the MC50.
:: Since we uninstall the HHRAP Application from the MC70 to get it to work we do not have to
:: shut down the application.
:: If the .NET20 is already installed we only need to load the HHRAP cab file. This will
:: shut down the application, apply the changes then reboot. 
::-----------------------------------------------------------------------------------
::if "%NET20INSTALLED%"=="YES" goto :loadhhrapcab


::if /i "%HHRAPTYPE%"=="SYMBOL MC70" goto :loadnet20mc70

echo step 4

start /MIN %INSTALLDIR%\AutoCeAppMgr.exe

::if not "%ERRORLEVEL%"=="9" goto :install_err

::%INSTALLDIR%\Setup.exe  %INSTALLDIR%\installcloseapp.ini
::if not "%ERRORLEVEL%"=="0" goto :install_err
::echo.
::echo.
::echo Please wait while the HandHeld RAP application is shutdown
::echo.
::%INSTALLDIR%\sleep.exe 10


:: -----------------------------------------------------------------------------------
:: Install .NET Compact Framework V2
:: This is a Microsoft CAB file. we do not build it
::-----------------------------------------------------------------------------------
:loadnet20
REM start /MIN %INSTALLDIR%\AutoCeAppMgr.exe
REM if not "%ERRORLEVEL%"=="0" goto :install_err
      
REM %INSTALLDIR%\Setup.exe  %INSTALLDIR%\installNETCFv2mc70.ini
REM if not "%ERRORLEVEL%"=="0" goto :install_err
::echo.
::echo Please be patient while .NET Compact Framework is installed
::echo  on the handheld device. This process will take 2 minutes.
::echo.
::echo.
::%INSTALLDIR%\sleep.exe 120
goto :loadhhrapcab

:loadnet20mc70
REM start /MIN %INSTALLDIR%\AutoCeAppMgr.exe
REM if not "%ERRORLEVEL%"=="0" goto :install_err
      
REM %INSTALLDIR%\Setup.exe  %INSTALLDIR%\installNETCFv2mc70.ini
REM if not "%ERRORLEVEL%"=="0" goto :install_err
:: if NOT exist "%APP_DRIVE%\scot\install\HHRap\NETCFv2.ppc.armv4.cab" goto :install_err
:: if NOT exist "%APP_DRIVE%\scot\install\HHRap\NETCFv2.wm.armv4i.cab" goto :install_err
REM cls
REM echo.
REM echo Please be patient while .NET Compact Framework is installed.
REM echo.
:: echo 1. When prompted on the handheld device: 
:: echo      "Choose a location to install Microsoft .NET CF 2.0"
:: echo    Verify that device is highlighted then touch INSTALL.
:: echo.
:: echo 2. When prompted on the handheld device:
:: echo      "You must restart the device to complete installation.
:: echo       Select OK to restart or Cancel to continue"
:: echo    Touch CANCEL to CONTINUE 
:: echo       (The device will restart on its own after HHRAP Application is installed)
:: echo.
:: echo When both of these steps have been completed.
:: pause
:: echo.



:: =================================================
:loadhhrapcab
:: =================================================
:: When running an updated if the name stored in the ini file and the cab file
:: is different uninstall will not be necessary.
::-------------------------------------------------------------------------------------
echo step 5

start /MIN %INSTALLDIR%\AutoCeAppMgr.exe

:: if not "%ERRORLEVEL%"=="0" goto :install_err
%INSTALLDIR%\Setup.exe  %INSTALLDIR%\installMeijerHHRapPatch.ini

echo step 6

if not "%ERRORLEVEL%"=="0" goto :install_err
if NOT exist "%APP_DRIVE%\scot\install\HHRap\NCR Meijer HandHeld Rap.ARM.CAB" goto :install_err
echo.
echo Please do not remove the handheld device from the cradle
echo   until installation is complete and the handheld device 
echo   has reboot! This process may take several minutes. 
echo   No operator intervention is needed on the handheld device. 
echo Please reboot the host PC after installtion is complete.
echo.
%INSTALLDIR%\sleep.exe 25
if /i "%HHRAPTYPE%"=="SYMBOL MC70" goto :stopactivesync
if exist "%APP_DRIVE%\Documents and Settings\All Users\Start Menu\Programs\Startup\stop.bat" del /Q "%APP_DRIVE%\Documents and Settings\All Users\Start Menu\Programs\Startup\stop.bat"
goto :finishup
:stopactivesync
copy /Y  %SCOTBINDIR%\stop.bat "%APP_DRIVE%\Documents and Settings\All Users\Start Menu\Programs\Startup\stop.bat"
regedit.exe /s %SCOTBINDIR%\runoncestopAS.reg
:finishup
call :displayandlog Done successfully!
echo Done successfully!>> %HISTORYLOG% 
echo *************************************************
echo *
echo * HHRAP version 4.32.4.1 installed successfully
echo *
echo *************************************************
goto :eof
  
:: ==============================================
:install_err
:: ==============================================
call :displayandlog Failed to install 4.2.1 HHRap 
echo Failed to install 4.2.1 HHRap %DT%! >> %HISTORYLOG%
exit /B
goto :EOF


:: ==============================================
:displayandlog  
:: ==============================================
  call :GetDateTime
  echo %*
  @echo %DT% %* >> "%TRACELOG%" 2>>&1
  rem @echo %DT% %* >> "%HISTORYLOG%" 2>>&1
goto :EOF


:: ==============================================
:GetDateTime
:: ==============================================
@echo off
for /f "tokens=*" %%i in ('date /t') do set date1=%%i 
set CD=%date1%
for /f "tokens=*" %%i in ('time /t') do set time1=%%i 
set CT=%time1%
set DT=%CD%%CT: =%
goto :EOF

:WriteDateTimeToHistoryLog
call :GetDateTime
set CDCT=Date/Time = %DT%
echo %CDCT% >> %HISTORYLOG%
echo %CDCT% >> %TRACELOG%
goto :EOF

:getdt
	for /f "tokens=*" %%i in ('date /t') do set DT1=%%i 
	for /f "tokens=*" %%i in ('time /t') do set time1=%%i 
	set DT2=%time1::=% 
	set DT=%DT1: =% %DT2: =%
goto :eof

:: ==============================================
:checkhhtype
:: ==============================================
rem Command line parameters can override what we read from the registry in HKCU\Software\microsoft\windows ce services\deviceinfo
rem if command line parameters are specified do not check the registry. This is an undocumented unused feature
rem that may be helpful in debugging

set HHRAPTYPE=""
if /i "%1"=="MC50" set HHRAPTYPE="SYMBOL MC50"
if /i "%1"=="MC70" set HHRAPTYPE="SYMBOL MC70"

if NOT %HHRAPTYPE%=="" goto :START
if exist %tempFile% del /Q %tempFile%
%INI2REG% /rrcu "SOFTWARE\Microsoft\Windows CE Services" DeviceOemInfo /f %CONFIGINI% > %tempFile%
FOR /F "tokens=1 delims=\" %%a IN (%tempFile%) DO set HHRAPTYPE=%%a
echo.
call :displayandlog HHRAPTYPE = %HHRAPTYPE%
echo.
if exist %tempFile% del /Q %tempFile%

rem Command line parameters can override what we read from the registry in HKCU\Software\microsoft\windows ce services\deviceinfo
if /i "%HHRAPTYPE%"=="SYMBOL MC50" goto :eof
if /i "%HHRAPTYPE%"=="SYMBOL MC70" goto :eof
echo.
call :displayandlog Cannot determine then handheld device type.
call :displayandlog Please verify the handheld is connected via activsync
call :displayandlog and rerun the installation program.
echo.
goto :install_err
