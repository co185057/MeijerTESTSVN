rem prescotstart.bat

echo begin prescotstart.bat >> c:\scot\bin\switch.log
REM Added date and time
echo %DATE% >> c:\scot\bin\switch.log
echo %TIME% >> c:\scot\bin\switch.log
 
call c:\scot\config\WriteDateTimeToSwitchLog.bat

rem +POS321530
set EXE=NCR.APTRA.CollectorProxyWS.exe
tasklist /fi "imagename eq %EXE%" |find ":" > nul
if errorlevel 1 goto ProxyWSRunning
   echo Starting NCR.APTRA.CollectorProxyWS.exe... >> c:\scot\bin\switch.log
   start C:\Progra~1\NCRAPT~1\Unifie~1\WS\bin\NCR.APTRA.CollectorProxyWS.exe
goto ProxyWSStarted

:ProxyWSRunning
echo NCR.APTRA.CollectorProxyWS.exe is already running. >> c:\scot\bin\switch.log
:ProxyWSStarted
rem -POS321530

REM Start CRD 380563
set EXE=salesw2k.exe
tasklist /fi "imagename eq %EXE%" |find ":" > nul
if errorlevel 1 goto SalesRunning
   echo POS is not running. >> c:\scot\bin\switch.log
goto SalesKilled

:SalesRunning
    REM If pos is running, run postscotstop.bat
	echo Sales detected during boot, running postscotstop >> c:\scot\bin\switch.log
    start c:\scot\config\postscotstop.bat
    sleep 3
	
:SalesKilled
REM End CRD 380563

start /min "Pipeserver" C:\scot\bin\pipeserver.exe

rem POS149398 run xmhookservice as console instead of services
c:\scot\bin\ini2reg.exe /os
set OSTYPE=%ERRORLEVEL%

IF %OSTYPE% == 10 echo The system running POSReady 7 >> c:\scot\bin\switch.log
rem IF %OSTYPE% == 10 start /min "XMHookService" %APP_DRIVE%\scot\bin\XMHookService.exe -c

rem TAR387943
regsvr32 /s c:\Scot\controls\ACSHookManager.dll

echo Starting TMS >> c:\scot\bin\switch.log
start /min c:\acs\bin\tms32.exe

echo Waiting for TMS >> c:\scot\bin\switch.log
c:\acs\bin\polltms.exe

echo TMS Started >> c:\scot\bin\switch.log

REM start /min c:\scot\bin\BioDevMgr.exe

wscript.exe "c:\scot\bin\ssco_options.js"
echo executed ssco_option >> c:\scot\bin\switch.log

sleep 25

REM start POS133487
REM start change 110901 move here due to R5 issue

set SCOTCONFIG=%APP_DRIVE%\scot\config
perl %SCOTCONFIG%\MakeQRCode.pl
echo executed prescotstart.bat >> c:\scot\bin\switch.log
sleep 5

REM end change 110901
REM end POS133487

REM call c:\scot\config\ChangeSecurity.bat
echo executed changesecurity >> c:\scot\bin\switch.log

REM change 110901 MOVED MakeQRCode above due to R5 issue

echo end >> c:\scot\bin\switch.log

sleep 5
rem restart FastLaneSvc to pickup any changes pushed from POS
net stop "NCR FastLane Service" & net start "NCR FastLane Service"


