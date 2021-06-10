@echo off
echo ---------------------------------------------------------------
echo =
echo = POST Custom Build Script for Meijer
echo =
echo ---------------------------------------------------------------

rem Building Remote Console Manager Debug 
call %CURPROJCUST%\RemoteConsoleBuildDebug.bat %1

rem build the Debug version of SCOTAPP and create the Meijer FastLane Development Kit Package.
call %CURPROJCUST%\MeijerDevKitBuild.bat %1

exit