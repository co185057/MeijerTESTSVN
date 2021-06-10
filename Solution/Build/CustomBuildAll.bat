@echo off
echo ---------------------------------------------------------------
echo =
echo = Custom Build Script for Lowes
echo =
echo ---------------------------------------------------------------
SET MSD="C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\IDE\devenv.exe"
SET MSD6="C:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin\MSDev.exe"
SET CURPROJBASE=%1
SET CURPROJAPP=%CURPROJBASE%\Source\AfxTransBroker\ACSHook
SET LOGFILE=%CURPROJBASE%\CustomScotAll.log
SET COPYCMD=/Y


rem Add other includes
SET SAVELIB=%LIB%
SET SAVEINCLUDE=%INCLUDE%

SET INCLUDE=%INCLUDE%;%CURPROJBASE%\Platform\include;
SET LIB=%LIB%;%CURPROJBASE%\Platform\release\lib;
SET LIB=%LIB%;%CURPROJBASE%\Platform\SCOTEventLog\release;
SET LIB=%LIB%;%CURPROJBASE%\UI\lib;

echo Deleting %LOGFILE%
del %LOGFILE% /F /Q

echo Copy and register Control dependencies
xcopy %CURPROJBASE%\Source\AfxTransBroker\tbControls\Release_Unicode\tbControlsU.dll %CURPROJBASE%\Scot\Controls\ /R /V /Y
xcopy %CURPROJBASE%\Source\AfxTransBroker\tbControls\Release_Unicode\tbControlsU.dll C:\Scot\Controls\ /R /V /Y
regsvr32 /s C:\Scot\Controls\tbControlsU.dll

xcopy %CURPROJBASE%\Source\AfxTransBroker\OposServiceObject\NCRTBDevice\Release_UNICODE\NCRTBDeviceU.dll %CURPROJBASE%\Scot\Controls\ /R /V /Y

rem no need to register
rem xcopy %CURPROJBASE%\Source\AfxTransBroker\OposServiceObject\NCRTBDevice\Release_UNICODE\NCRTBDeviceU.dll C:\Scot\Controls\ /R /V /Y
rem regsvr32 /s C:\Scot\Controls\NCRTBDeviceU.dll


xcopy %CURPROJBASE%\Source\AfxTransBroker\PipeServer\Release\pipeServer.exe %CURPROJBASE%\Scot\bin\ /R /V /Y
xcopy %CURPROJBASE%\Source\AfxTransBroker\PipeServer\Release\pipeServer.exe C:\Scot\Bin\ /R /V /Y
C:\Scot\Bin\pipeServer.exe /RegServer

xcopy %CURPROJBASE%\Scot\bin\traceExe.exe C:\Scot\Bin\ /R /V /Y
C:\Scot\Bin\traceExe.exe /RegServer


echo Cleaning and rebuilding ACSHook - Release'
%MSD% "%CURPROJAPP%\ACSHook.sln" /REBUILD "Release" /OUT %LOGFILE%-ReleaseACSHookManager.log

echo Copying Debug symbols to install folder
md %CURPROJBASE%\Symbols\
md %CURPROJBASE%\Symbols\Dll\
xcopy %CURPROJAPP%\ACSHookManager\Release\ACSHookManager.pdb %CURPROJBASE%\Symbols\Dll\ /R /V /Y

echo Copy binary output files to installation recipe folder
xcopy %CURPROJAPP%\ACSHookManager\Release\ACSHookManager.dll %CURPROJBASE%\Scot\Controls\ /R /V /Y

rem echo Copy the ACSHookManager control in the scot folder and register it - required for building SCOTAPP
xcopy %CURPROJAPP%\ACSHookManager\Release\ACSHookManager.dll C:\Scot\Controls\ /R /V /Y
regsvr32 /s C:\Scot\Controls\ACSHookManager.dll

rem notepad %LOGFILE%

SET BROOT=""
SET MSD=""
SET LOGFILE=""
SET CURPROJBASE=""
SET CURPROJSCOT=""
SET LIB=%SAVELIB%
SET INCLUDE=%SAVEINCLUDE%

exit