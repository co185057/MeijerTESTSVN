rem Copying Files necessary for Mini PayStation

xcopy /R /Y c:\scot\Config\Mini\*.* c:\scot\Config\

echo Configured for Pay Station Mini. >> \scot\Install\"InstallHistory.log"
date /T >> \scot\Install\"InstallHistory.log"
time /T >> \scot\Install\"InstallHistory.log"