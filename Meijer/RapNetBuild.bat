@echo off
echo ---------------------------------------------------------------
echo =
echo = Custom Build Script for RAPNET
echo =
echo ---------------------------------------------------------------
SET MSBUILD="C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
SET CURPROJBASE=%1
SET CURPROJAPP=%CURPROJBASE%"\Platform-Core-Patch\Remote Approval\RAPNET\source"
SET LOGFILE=%CURPROJBASE%\CustomScotAll-RapNet.log
SET COPYCMD=/Y

SET INCLUDE=%INCLUDE%;c:\scot\adk\include;
SET LIB=%LIB%;c:\scot\adk\lib;c:\scot\adk\lib\release7

echo Deleting %LOGFILE%
IF EXIST %LOGFILE% del %LOGFILE% /F /Q

echo %CURPROJAPP%\Solutions\VS2013\RapBase.sln
echo Cleaning and rebuilding RapBase Solution - RapNetBase13 Project
%MSBUILD% %CURPROJAPP%\Solutions\VS2013\RapBase.sln /t:rebuild /p:Configuration="Release" /p:Platform="Any CPU" /fl /flp:logfile=%LOGFILE%;verbosity=diagnostic

echo %CURPROJAPP%\Solutions\VS2013\RapNet.sln
echo Cleaning and rebuilding RapBase Solution - RapNet13 Project
%MSBUILD% %CURPROJAPP%\Solutions\VS2013\RapNet.sln /t:rebuild /p:Configuration="Release" /p:Platform="Any CPU" /fl /flp:logfile=%LOGFILE%;verbosity=diagnostic

echo Copying RapNet Debug symbols to install folder
echo %CURPROJBASE%
echo %CURPROJAPP%
md %CURPROJBASE%\Symbols\
md %CURPROJBASE%\Symbols\bin\

xcopy "%CURPROJBASE%\Platform-Core-Patch\Remote Approval\RAPNET\source\bin\Release13\RapNet.pdb" %CURPROJBASE%\Symbols\bin\ /R /V /Y
xcopy "%CURPROJBASE%\Platform-Core-Patch\Remote Approval\RAPNET\source\bin\Release13\RapNetBase.pdb" %CURPROJBASE%\Symbols\bin\ /R /V /Y

echo Copy RapNet binary output files to installation recipe folder
xcopy "%CURPROJBASE%\Platform-Core-Patch\Remote Approval\RAPNET\source\bin\Release13\RapNet.exe" %CURPROJBASE%\RAP\bin\ /R /V /Y
xcopy "%CURPROJBASE%\Platform-Core-Patch\Remote Approval\RAPNET\source\bin\Release13\RapNet.exe.config" %CURPROJBASE%\RAP\bin\ /R /V /Y
xcopy "%CURPROJBASE%\Platform-Core-Patch\Remote Approval\RAPNET\source\bin\Release13\RapNetBase.dll" %CURPROJBASE%\RAP\bin\ /R /V /Y

rem Restore environment variables
SET LOGFILE=""
