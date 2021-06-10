@echo off
echo ---------------------------------------------------------------
echo =
echo = Custom Build Script for LAUNCHPAD
echo =
echo ---------------------------------------------------------------
SET MSBUILD="C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
SET CURPROJBASE=%1
SET CURPROJAPP=%CURPROJBASE%"\Platform-Core-Patch\LaunchPadNet\LPN"
SET LOGFILE=%CURPROJBASE%\CustomScotAll-LAUNCHPAD.log
SET COPYCMD=/Y

SET INCLUDE=%INCLUDE%;c:\scot\adk\include;
SET LIB=%LIB%;c:\scot\adk\lib;c:\scot\adk\lib\release7

echo Deleting %LOGFILE%
IF EXIST %LOGFILE% del %LOGFILE% /F /Q

echo %CURPROJAPP%\LaunchPadNet13.sln
echo Cleaning and rebuilding LaunchPad Solution - LaunchPadNet13 Project
%MSBUILD% %CURPROJAPP%\LaunchPadNet13.sln /t:rebuild /p:Configuration="Release" /p:Platform="Any CPU" /fl /flp:logfile=%LOGFILE%;verbosity=diagnostic

echo Copying LaunchPadNet Debug symbols to install folder
echo %CURPROJBASE%
echo %CURPROJAPP%
md %CURPROJBASE%\Symbols\
md %CURPROJBASE%\Symbols\bin\

echo Copying LaunchPadNet Debug symbols to install folder
md %CURPROJBASE%\Symbols\
md %CURPROJBASE%\Symbols\bin\
xcopy "%CURPROJAPP%\bin\Release13\LaunchPadNet.pdb" %CURPROJBASE%\Symbols\bin\ /R /V /Y

echo Copy LaunchPadNet binary output files to installation recipe folder
xcopy "%CURPROJAPP%\bin\Release13\LaunchPadNet.exe" %CURPROJBASE%\SCOT\bin\ /R /V /Y
xcopy "%CURPROJAPP%\bin\Release13\LaunchPadNet.exe" %CURPROJBASE%\RAP\bin\ /R /V /Y

echo Copy LaunchPadNet binary output files to SCOT folder
xcopy "%CURPROJAPP%\bin\Release13\LaunchPadNet.pdb" C:\SCOT\bin\ /R /V /Y
xcopy "%CURPROJAPP%\bin\Release13\LaunchPadNet.exe" C:\SCOT\bin\ /R /V /Y


rem Restore environment variables
SET VS7DIR=""
SET VS7=""
SET LOGFILE=""
SET LIB=%SAVELIB%
SET INCLUDE=%SAVEINCLUDE%
SET PATH=%SAVEPATH%
