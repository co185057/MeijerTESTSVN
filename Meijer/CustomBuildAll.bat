@echo off
echo ---------------------------------------------------------------
echo =
echo = Custom Build Script for Meijer
echo =
echo ---------------------------------------------------------------
SET MSD="d:\Program Files\Microsoft Visual Studio .NET 2003\Common7\IDE\devenv.exe"
SET MSD6="d:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin\MSDev.exe"
SET CURPROJBASE=%1
SET CURPROJAPP=%CURPROJBASE%\Source\AfxTransBroker\ACSHook
SET CURRCMAPP=%CURPROJBASE%\Platform-Core-Patch\Remote Console Manager
SET LOGFILE=%CURPROJBASE%\CustomScotAll.log
SET COPYCMD=/Y
SET VC7DIR=d:\"Program Files"\"Microsoft Visual Studio .NET 2003"

echo ---------------------------------------------------------------
echo =
echo = Building Meijer Hand Held CAB 
echo =
echo ---------------------------------------------------------------
call %CURPROJBASE%\build\MakeMeijerCab.bat %CURPROJBASE%

rem Add other includes
SET SAVELIB=%LIB%
SET SAVEINCLUDE=%INCLUDE%
SET SAVEPATH=%PATH%

call %VC7DIR%\common7\tools\vsvars32.bat
SET INCLUDE=%INCLUDE%;c:\scot\adk\include
SET LIB=%LIB%;c:\scot\adk\lib;c:\scot\adk\lib\release7

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

echo Copy the ACSHookManager control in the scot folder and register it - required for building SCOTAPP
xcopy %CURPROJAPP%\ACSHookManager\Release\ACSHookManager.dll C:\Scot\Controls\ /R /V /Y
xcopy %CURPROJAPP%\ACSHookManager\Release\ACSHookManager.dll D:\Scot\Controls\ /R /V /Y
regsvr32 /s C:\Scot\Controls\ACSHookManager.dll

rem building LaunchPad, RapNet and Biometric Device Manager
call %CURPROJCUST%\LaunchPadNetBuild.bat %1
call %CURPROJCUST%\RapNetBuild.bat %1
call %CURPROJCUST%\BioDevMgrBuild.bat %1

rem notepad %LOGFILE%

call %CURPROJCUST%\NextGenUIViewsBuild.bat %1

SET BROOT=""
SET MSD=""
SET LOGFILE=""
SET CURPROJBASE=""
SET CURPROJSCOT=""
SET LIB=%SAVELIB%
SET INCLUDE=%SAVEINCLUDE%
SET PATH=%SAVEPATH%

exit