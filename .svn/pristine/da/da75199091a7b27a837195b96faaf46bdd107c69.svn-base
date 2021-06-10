@echo off
echo ---------------------------------------------------------------
echo =
echo = Custom Build Script for NextGenUIViews C# library
echo =
echo ---------------------------------------------------------------
SET MSBUILD="C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
SET CURPROJBASE=%1
SET CURPROJAPP=%CURPROJBASE%\NextGenUI\NextGenUIViews
SET LOGFILE=%CURPROJBASE%\CustomScotAll-NextGenUIViews.log
SET COPYCMD=/Y

echo Deleting %LOGFILE%
IF EXIST %LOGFILE% del %LOGFILE% /F /Q

echo %CURPROJAPP%\NextGenUIViews.sln

echo Cleaning and rebuilding NextGenUIViews Solution - NextGenUIViews Project
%MSBUILD% %CURPROJAPP%\NextGenUIViews.sln /t:rebuild /p:Configuration="Release" /p:Platform="Any CPU" /fl /flp:logfile=%LOGFILE%;verbosity=diagnostic

echo Copying NextGenUIViews Debug symbols to install folder
md %CURPROJBASE%\Symbols\
md %CURPROJBASE%\Symbols\bin\
xcopy "%CURPROJAPP%\SSCOUIViews\bin\Release\SSCOUIViews.pdb" %CURPROJBASE%\Symbols\bin\ /R /V /Y
xcopy "%CURPROJAPP%\SSCOUIViews\bin\Release\SSCOUISkin.pdb" %CURPROJBASE%\Symbols\bin\ /R /V /Y

echo Copy NextGenUIViews binary output files to installation recipe folder
xcopy "%CURPROJAPP%\SSCOUIViews\bin\Release\SSCOUIViews.dll" %CURPROJBASE%\SCOT\bin\ /R /V /Y
xcopy "%CURPROJAPP%\SSCOUIViews\bin\Release\SSCOUISkin.dll" %CURPROJBASE%\SCOT\bin\ /R /V /Y

rem Restore environment variables
SET LOGFILE=""
