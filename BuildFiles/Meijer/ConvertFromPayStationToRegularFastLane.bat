rem Copying Files necessary for regular FastLane

xcopy /R /Y c:\scot\Config\Standard\*.* c:\scot\Config\

echo Configured back for regular FastLane. >> \scot\Install\"InstallHistory.log"
date /T >> \scot\Install\"InstallHistory.log"
time /T >> \scot\Install\"InstallHistory.log"