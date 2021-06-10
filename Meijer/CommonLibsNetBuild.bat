@echo off
echo ---------------------------------------------------------------
echo =
echo = Custom Build Script for CommonLibsNet C# library
echo = used by RapNet And LaunchPadNet
echo =
echo ---------------------------------------------------------------
SET VS7DIR=d:\Program Files\Microsoft Visual Studio .NET 2003
SET VS7="%VS7DIR%\Common7\IDE\devenv.exe"
SET CURPROJBASE=%1
SET CURPROJAPP=%CURPROJBASE%\Platform-Core-Patch\Remote Approval\CommonLibsNet
SET LOGFILE=%CURPROJBASE%\CustomScotAll-CommonLibsNet.log
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

echo Cleaning and rebuilding CommonLibsNet Solution - CommonLibsNet Project
%VS7% "%CURPROJAPP%\CommonLibsNet.sln" /REBUILD "Release" /PROJECT "CommonLibsNet" /UseEnv /OUT %LOGFILE%

echo Copying CommonLibsNet Debug symbols to install folder
md %CURPROJBASE%\Symbols\
md %CURPROJBASE%\Symbols\bin\
xcopy "%CURPROJAPP%\bin\Release\CommonLibsNet.pdb" %CURPROJBASE%\Symbols\bin\ /R /V /Y

echo Copy CommonLibsNet binary output files to installation recipe folder
xcopy "%CURPROJAPP%\bin\Release\CommonLibsNet.dll" %CURPROJBASE%\SCOT\bin\ /R /V /Y
xcopy "%CURPROJAPP%\bin\Release\CommonLibsNet.dll" %CURPROJBASE%\RAP\bin\ /R /V /Y

echo Copy CommonLibsNet binary output files to SCOT folder required for RapNet and LaunchPadNet 
xcopy "%CURPROJAPP%\bin\Release\CommonLibsNet.pdb" C:\SCOT\bin\ /R /V /Y
xcopy "%CURPROJAPP%\bin\Release\CommonLibsNet.dll" C:\SCOT\bin\ /R /V /Y


rem Restore environment variables
SET VS7DIR=""
SET VS7=""
SET LOGFILE=""
SET LIB=%SAVELIB%
SET INCLUDE=%SAVEINCLUDE%
SET PATH=%SAVEPATH%
