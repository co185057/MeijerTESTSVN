rem The "Defrag" and "WorkingSpace" tasks were created in DG49 so they are commented out for DG51.
rem schtasks /create /ru scot /rp scot /tn "Defrag" /sc weekly /d WED /st 01:00:00 /tr c:\scot\bin\defrag.vbs
rem schtasks /create /ru scot /rp scot /tn "WorkingSpace" /sc daily /st 02:30:00 /tr c:\scot\bin\ProcessQuery.vbs

rem POS43830: For DG51, create the "ChkdskFix" task so that the harddrive can be setup weekly to undergo a fix upon reboot.

@echo off
cls
schtasks /query > doh
findstr /B /I "ChkdskFix" doh >nul
if %errorlevel%==0  goto :found
goto :create
 
:found
SCHTASKS /Change /ru scot /rp ncr1234 /tn "ChkdskFix" /F >nul
goto :end
 
:create
schtasks /create /ru scot /rp ncr1234 /tn "ChkdskFix" /sc weekly /d WED /st 01:00:00 /tr c:\scot\bin\ChkdskFix.vbs

:end