@echo off
echo ---------------------------------------------------------------
echo =
echo = Custom Build Script for LaunchPadNet C# library
echo =
echo ---------------------------------------------------------------
SET VS7DIR=C:\Program Files\Microsoft Visual Studio .NET 2003
SET VS7="%VS7DIR%\Common7\IDE\devenv.exe"
SET CURPROJBASE=%1
SET CURPROJAPP=%CURPROJBASE%\Platform-Core-Patch\LaunchPadNet
SET LOGFILE=%CURPROJBASE%\CustomScotAll-LaunchPadNet.log
SET COPYCMD=/Y

rem Add other includes
SET SAVELIB=%LIB%
SET SAVEINCLUDE=%INCLUDE%
SET SAVEPATH=%PATH%

call "%VS7DIR%\Common7\Tools\vsvars32.bat"
SET INCLUDE=%INCLUDE%;c:\scot\adk\include;
SET LIB=%LIB%;c:\scot\adk\lib;c:\scot\adk\lib\release7

echo Deleting %LOGFILE%
IF EXIST %LOGFILE% del %LOGFILE% /F /Q

echo Cleaning and rebuilding LaunchPadNet Solution - LaunchPadNet Project
%VS7% "%CURPROJAPP%\LaunchPadNet.sln" /REBUILD "Release" /PROJECT "LaunchPadNet" /UseEnv /OUT %LOGFILE%

echo Copying LaunchPadNet Debug symbols to install folder
md %CURPROJBASE%\Symbols\
md %CURPROJBASE%\Symbols\bin\
xcopy "%CURPROJAPP%\bin\Release\LaunchPadNet.pdb" %CURPROJBASE%\Symbols\bin\ /R /V /Y

echo Copy LaunchPadNet binary output files to installation recipe folder
xcopy "%CURPROJAPP%\bin\Release\LaunchPadNet.exe" %CURPROJBASE%\SCOT\bin\ /R /V /Y
xcopy "%CURPROJAPP%\bin\Release\LaunchPadNet.exe" %CURPROJBASE%\RAP\bin\ /R /V /Y

echo Copy LaunchPadNet binary output files to SCOT folder
xcopy "%CURPROJAPP%\bin\Release\LaunchPadNet.pdb" C:\SCOT\bin\ /R /V /Y
xcopy "%CURPROJAPP%\bin\Release\LaunchPadNet.exe" C:\SCOT\bin\ /R /V /Y


rem Restore environment variables
SET VS7DIR=""
SET VS7=""
SET LOGFILE=""
SET LIB=%SAVELIB%
SET INCLUDE=%SAVEINCLUDE%
SET PATH=%SAVEPATH%
