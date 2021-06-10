rem prescotstart.bat

echo begin prescotstart.bat >> c:\scot\bin\switch.log
REM Added date and time
echo %DATE% >> c:\scot\bin\switch.log
echo %TIME% >> c:\scot\bin\switch.log
 
call c:\scot\config\WriteDateTimeToSwitchLog.bat

REM Serial port switching changes. Set the baud, etc for scotapp (e.g. display header, etc.) and set serial port to local IP/port.
mode COM13:9600,N,8
start /WAIT /MIN C:\scot\Eltima\Lane.bat

start /min "Pipeserver" C:\scot\bin\pipeserver.exe

rem TAR387943
regsvr32 /s c:\Scot\controls\ACSHookManager.dll

regsvr32 /s c:\acs\bin\acsiochannelps.dll

rem kill any acsiochannel processes running before registering
kill acsiochannel.exe
kill acsioc~1
start /min c:\acs\bin\acsiochannel.exe /regserver
Sleep.exe 1
kill acsiochannel.exe
kill acsioc~1

echo Starting TMS >> c:\scot\bin\switch.log
start /min c:\acs\bin\tms32.exe

echo Waiting for TMS >> c:\scot\bin\switch.log
c:\acs\bin\polltms.exe

echo TMS Started >> c:\scot\bin\switch.log

REM start /min c:\scot\bin\BioDevMgr.exe

echo end   prescotstart.bat >> c:\scot\bin\switch.log
