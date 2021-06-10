@echo off
SETLOCAL ENABLEEXTENSIONS

call :setup

call :displayandlog *****************************
call :displayandlog Starting update of Meijer Sprint 2017 Sprint 3
call :WriteDateTimeToHistoryLog


if exist %SCOTUTIL%\CreatePopup.exe (
  REM call :displayandlog Displaying NCR Phase 8 Splash screen for %SplashTimer% seconds
  call :GetTime
  start %SCOTUTIL%\createpopup.exe "%SCOTUTIL%\popup.exe /p %SplashTimer% /focus /MSG Update in progress - Installing Meijer Update Begin %time2% - Please wait..."
)

 :: Check and set HW type for install 
    ECHO Check SCO Release Type
    SET SCOType=Null
    FOR /F "tokens=2* " %%A IN ('REG.exe query "HKLM\software\NCR\SCOT - Platform\ObservedOptions" /v "Type"') DO SET SCOType=%%B
    IF "%SCOType%" NEQ "SCOT4" (
      SET HardwareType=SCOT5
    ) ELSE (
      SET HardwareType=SCOT4
    )
    reg query "HKLM\software\NCR" /v "HWType"
    if %errorlevel% EQU 0 (
      %APP_DRIVE%\scot\bin\ini2reg.exe /rr "SOFTWARE\NCR" HWType /f %APP_DRIVE%\scot\bin\config.ini  > %TEMP%\.SCOType
      FOR /F "tokens=1,2" %%a IN (%TEMP%\.SCOType) DO SET SCOType=%%a
      del /q %TEMP%\.SCOType 
    )
    IF "%SCOType%" EQU "SCOT6" (
      SET HardwareType=SCOT6
    )    
    
    REM ****************************************************************
    REM Note HWType doesn't always equal SCOT4 or SCOT5 in new OS images
    REM ****************************************************************
    REM SET HardwareType=    
    REM %APP_DRIVE%\scot\bin\ini2reg.exe /rr "SOFTWARE\NCR\SCOT - Platform\ObservedOptions" HWType /f %APP_DRIVE%\scot\bin\config.ini  > %TEMP%\.HardwareType
    REM FOR /F "tokens=1,2" %%a IN (%TEMP%\.HardwareType) DO SET HardwareType=%%a
    REM del /q %TEMP%\.HardwareType

 :: Check and skip installing on non-50 hardware
    SET TABUnit=
    %APP_DRIVE%\scot\bin\ini2reg.exe /rr "SOFTWARE\NCR\SCOT - Platform\ObservedOptions" TakeAwayBelts /f %APP_DRIVE%\scot\bin\config.ini  > %TEMP%\.TabType
    FOR /F "tokens=1,2" %%a IN (%TEMP%\.TabType) DO SET TABUnit=%%a
    del /q %TEMP%\.TabType

call dr /u

kill -f SSCOUI.exe
kill -f BioDevMgr.exe
kill -f NCR.APTRA.UAWSTSSrv.exe

REM POS107500 - force to stop traceexe.exe
REM kill -f traceexe.exe
REM POS123659 - This cmd needs to be removed on DG59!!!!!!!!
sleep 10
c:\scot\bin\cmdasuser.exe scot scot c:\scot\bin\kill.exe /f "traceexe.exe"


REM ********* Meijer Lane and RAP setup

:NormalInstall
REM Lane setup

if NOT exist "c:\scot\bin\scotappu.exe" goto :NoScotAppU

	start /wait .\NextGenUI_v1.0.4_b555_Hotfix_v24.exe /s
    sleep 10
	call ::DisplayPopup
	
	start /wait .\SSCO6.0.5_Build13_Hotfix_v19.exe /s
    sleep 10
	call ::DisplayPopup
	
	IF "%HardwareType%"=="SCOT5" (
	REM Restore the original file after SSCO6.0.5_Build13_Hotfix_v12 installation completes, coz it overrides the scot credentials with default		
		xcopy .\NextLogon.reg C:\scot\bin /I /R /V /Y >> %HISTORYTRACE% 2>&1
		sleep 5
		
		regedit.exe /S .\NextLogon.reg >> %HISTORYTRACE% 2>&1
		sleep 5

	call ::DisplayPopup
	echo Change scot password >> %HISTORYTRACE%
	net.exe user scot ncr1234 | find "successfully" >> %HISTORYTRACE%
	echo Return code is %errorlevel% >> %HISTORYTRACE%		
	)

REM Main Lane Install	

	call ::DisplayPopup
	call :displayandlog Installing Meijer Lane Setup
	

	call .\FLServices1.0.5_Build3_Hotfix_v5.exe
	sleep 10

	call .\MeijerLaneSetup.exe /s /w
	sleep 10	

	call :WriteDateTimeToHistoryLog 

	REM enable trace check
	reg add "HKEY_LOCAL_MACHINE\Software\NCR\TraceCheck" /v "Configure" /t REG_SZ /d "Yes" /f >> %HISTORYLOG%	

	call ::DisplayPopup
	echo Change support password >> %HISTORYTRACE%
	net.exe user support @DG@MjrSupport!@DG@ | find "successfully" >> %HISTORYTRACE%
	echo Return code is %errorlevel% >> %HISTORYTRACE%
	
:NoScotAppU 

REM RAP installation

if NOT exist "c:\scot\bin\rapnet.exe" goto :NoRap

	
REM RAP ADD and Hotfix install
	
	start /wait .\NextGenUI_v1.0.4_b555_Hotfix_v24.exe /s
    sleep 10
	call ::DisplayPopup

	start /wait .\SSCO6.0.5_Build13_Hotfix_v19.exe /s
    sleep 10
	call ::DisplayPopup

	:: Installations on Release-5 hardware
	
    	IF "%HardwareType%"=="SCOT5" (		
		
		REM Restore the original file after SSCO6.0.5_Build13_Hotfix_v12 installation completes, coz it overrides the scot credentials with default		
		xcopy .\NextLogon.reg C:\scot\bin /I /R /V /Y >> %HISTORYTRACE% 2>&1
		sleep 5
		
		regedit.exe /S .\NextLogon.reg >> %HISTORYTRACE% 2>&1
		sleep 5

		call ::DisplayPopup
		echo Change scot password >> %HISTORYTRACE%
		net.exe user scot ncr1234 | find "successfully" >> %HISTORYTRACE%
		echo Return code is %errorlevel% >> %HISTORYTRACE%
	)
	
REM Main RAP Install

    call ::DisplayPopup
	call :displayandlog Installing Meijer RAP Setup
	call .\MeijerRapSetup.exe /s /w /SKIPHHRAP	
    sleep 10

	
	call :WriteDateTimeToHistoryLog

	REM enable trace check
	reg add "HKEY_LOCAL_MACHINE\Software\NCR\TraceCheck" /v "Configure" /t REG_SZ /d "Yes" /f >> %HISTORYLOG%	

	REM POS152895 - Update Support every DG
	echo Change support password >> %HISTORYTRACE%
	net.exe user support @DG@MjrSupport!@DG@ | find "successfully" >> %HISTORYTRACE%
	echo Return code is %errorlevel% >> %HISTORYTRACE%


goto :END

	
:NoRap

REM ********* EMPLID BIOMET
REM call :displayandlog Configuring Biometerics and Employee ID
REM set CONFIG_PARAMS=EMPLID_ON BIOMET_OFF
REM @echo %date% %time% Configuring EMPLID and Meijer Biometrics with options: %CONFIG_PARAMS% >> %INSTALLHISTORY_LOG% 2>>&1 
REM @echo %date% %time% Configuring EMPLID and Meijer Biometrics with options: %CONFIG_PARAMS% >> %INSTALLTRACE_LOG% 2>>&1
REM C:\WINDOWS\system32\cscript.exe .\EmplidBiometConfig.vbs %CONFIG_PARAMS% >> %INSTALLTRACE_LOG% 2>>&1

sleep 1

call dr /i

    	REM :: Installations on Release-5 hardware
    	REM IF "%HardwareType%"=="SCOT5" (
    	REM )

    	REM :: Installations on non-Release-5 hardware
    	REM IF "%HardwareType%"=="SCOT4" (
	REM )
	

REM goto :END
goto :CHKDSK

:setup
  if not defined OS_DRIVE   set OS_DRIVE=C:
  if not defined APP_DRIVE  set APP_DRIVE=C:
  if not defined DATA_DRIVE set DATA_DRIVE=C:
  if not defined APP_DIR    set APP_DIR=scot
  
  set COPYCMD=\Y
  set SCOTBIN=%APP_DRIVE%\%APP_DIR%\bin
  set SCOTCONFIG=%APP_DRIVE%\%APP_DIR%\config
  set SCOTUTIL=%APP_DRIVE%\%APP_DIR%\util
  set LOGFILE=%APP_DRIVE%\%APP_DIR%\install\InstallTrace.log
  set XMLSRC=.\FSM_Definitions
  set XMLDST="c:\Program Files\NCR\RSM\FSM Definitions"
  set RSMLICENSE="c:\Program Files\NCR\RSM\Website\XML"
  set INSTALLREMAGTDIR=%APP_DRIVE%\Install\RemoteAgent

  set SplashTimer=180
  set temptime=%DATA_DRIVE%\%APP_DIR%\logs\.time
  set return=%APP_DRIVE%\%APP_DIR%\firmware\util\return
  set GATEWAY_PACKAGE=".\Meijer RSM Patch 1.2.exe"
  set RSMSMCLIENT="c:\program files\ncr\rsm\rsmsmclient.dll"
  set temp_time=%DATA_DRIVE%\%APP_DIR%\logs\.time
  set HISTORYLOG=%APP_DRIVE%\%APP_DIR%\install\InstallHistory.log
  set INSTALLHISTORY_LOG=%APP_DRIVE%\%APP_DIR%\install\InstallHistory.log
  set INSTALLTRACE_LOG=%APP_DRIVE%\%APP_DIR%\install\InstallTrace.log
  set HISTORYTRACE=%APP_DRIVE%\%APP_DIR%\install\InstallTrace.log
  REM set TEMP=%DATA_DRIVE%\temp - this caused some base hotfix to fail 
goto :eof

:DisplayPopup
  start %SCOTUTIL%\createpopup.exe "%SCOTUTIL%\popup.exe /p %SplashTimer% /focus /MSG Update in progress - Installing Meijer Update Begin %time2% - Please wait..."
goto :EOF

:GetTime
  for /f "tokens=1-2" %%i in ('time /t') do set time2=%%i %%j
goto :EOF

:getDT
  for /f "tokens=*" %%i in ('date /t') do set date1=%%i
  time < %return% > %temp_time%
  for /f "tokens=5" %%i in ('type %temp_time%') do set MYDTS=%date1:~4,-1% %%i
  del /q %temp_time%
goto :eof

:displayandlog
  call :getDT
  @echo %MYDTS%  %*
  @echo %MYDTS%  %* >> %LOGFILE% 2>>&1
goto :EOF

:: ======================================================================
:WriteDateTimeToHistoryLog
:: ======================================================================
  set CDCT=
  for /f "tokens=*" %%i in ('date /t') do set date1=%%i 
  set CD=%date1%
  for /f "tokens=*" %%i in ('time /t') do set time1=%%i 
  set CT=%time1% 
  set CDCT=Date/Time = %CD%%CT: =%

  echo %CDCT% >> %HISTORYLOG%
goto :EOF


:CHKDSK
rem **** Below snippet is to create CHKDSK task ****
rem The "Defrag" and "WorkingSpace" tasks were created in DG49 so they are commented out for DG51.
rem schtasks /create /ru scot /rp scot /tn "Defrag" /sc weekly /d WED /st 01:00:00 /tr c:\scot\bin\defrag.vbs
rem schtasks /create /ru scot /rp scot /tn "WorkingSpace" /sc daily /st 02:30:00 /tr c:\scot\bin\ProcessQuery.vbs

rem POS43830: For DG51, create the "ChkdskFix" task so that the harddrive can be setup weekly to undergo a fix upon reboot.
rem POS235402: For Sprint_1, "ChkdskFix" task to run as system user.	Name: Jangala Sunny		Date: June 27, 2016

@echo off
cls
schtasks /query > doh
findstr /B /I "ChkdskFix" doh >nul
if %errorlevel%==0  goto :found
goto :create
 
:found
SCHTASKS /Change /ru "SYSTEM" /tn "ChkdskFix" >nul
goto :end
 
:create
schtasks /create /ru "SYSTEM" /tn "ChkdskFix" /sc weekly /d WED /st 01:00:00 /tr c:\scot\bin\ChkdskFix.vbs

:END
ENDLOCAL


