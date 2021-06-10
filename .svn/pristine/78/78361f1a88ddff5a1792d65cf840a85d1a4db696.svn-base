rem postscotstop.bat

echo begin postscotstop.bat  >> c:\scot\bin\switch.log

call c:\scot\config\WriteDateTimeToSwitchLog.bat

kill acsiochannel.exe
kill acsioc~1.exe

rem POS149398 run xmhookservice as console instead of services
c:\scot\bin\ini2reg.exe /os
set OSTYPE=%ERRORLEVEL%

IF %OSTYPE% == 10 echo The system running POSReady 7 >> c:\scot\bin\switch.log
IF %OSTYPE% == 10 kill XMHookService.exe

rem This kills everything having to do with ACS. If more ACS processes need
rem to be killed, add them to the apl file that this cmd file uses.
start /wait c:\windows\system32\FLRSMTerminateHostApplication.cmd


echo end   postscotstop.bat  >> c:\scot\bin\switch.log
