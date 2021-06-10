@echo off
echo , >> \scot\Install\"InstallHistory.log"
echo Starting ConvertToServiceCenter... >> \scot\Install\"InstallHistory.log"
rem install ADD Package with "MULTI" as an option
cd c:\temp\master\ADDPackage
call c:\temp\master\ADDPackage\InstallADDPackage MULTI

echo calling ConvertToPayStation... >> \scot\Install\"InstallHistory.log"
cd c:\updates
call ConvertToPayStation