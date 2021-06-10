@echo off
echo ---------------------------------------------------------------
echo =
echo = Custom Build Script for Meijer Remote Console Manager (Debug)
echo =
echo ---------------------------------------------------------------
SET MSD="d:\Program Files\Microsoft Visual Studio .NET 2003\Common7\IDE\devenv.exe"
SET MSD6="d:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin\MSDev.exe"
SET CURPROJBASE=%1
SET CURPROJAPP=%CURPROJBASE%\Source\AfxTransBroker\ACSHook
SET CURRCMAPP=%CURPROJBASE%\Platform-Core-Patch\Remote Console Manager
SET LOGFILE=%CURPROJBASE%\RemoteConsoleBuild_Debug.log
SET COPYCMD=/Y
SET VC7DIR=d:\"Program Files"\"Microsoft Visual Studio .NET 2003"


rem Add other includes
SET SAVELIB=%LIB%
SET SAVEINCLUDE=%INCLUDE%
SET SAVEPATH=%PATH%

call %VC7DIR%\common7\tools\vsvars32.bat
SET INCLUDE=%INCLUDE%;c:\scot\adk\include;
SET LIB=%LIB%;c:\scot\adk\lib;c:\scot\adk\lib\debug7


echo Deleting %LOGFILE%
del %LOGFILE% /F /Q


echo Cleaning and rebuilding Remote Console Manager - Unicode Debug'
%MSD% "%CURRCMAPP%\RemoteConsoleMgr.sln" /REBUILD "Unicode Debug" /UseEnv /OUT %LOGFILE%

rem echo Copying Debug symbols to install folder
md %CURPROJBASE%\DebugSymbols\
md %CURPROJBASE%\DebugSymbols\Dll\
md %CURPROJBASE%\DebugSymbols\Ocx\
xcopy "%CURRCMAPP%\ocx - RemoteConsoleMgr\Debug_Unicode\RemoteConsoleMgr.pdb" %CURPROJBASE%\DebugSymbols\ocx\ /R /V /Y
xcopy "%CURRCMAPP%\dll - RCMgr\Debug_Unicode\RCMgr.pdb" %CURPROJBASE%\DebugSymbols\Dll\ /R /V /Y
xcopy "%CURRCMAPP%\dll - StateMachine\Debug_Unicode\statem.pdb" %CURPROJBASE%\DebugSymbols\Dll\ /R /V /Y
xcopy "%CURRCMAPP%\dll - Utils\Debug_Unicode\Utils.pdb" %CURPROJBASE%\DebugSymbols\Dll\ /R /V /Y
xcopy "%CURRCMAPP%\exe - TestRCM\Debug_Unicode\TestRCM.pdb" %CURPROJBASE%\DebugSymbols\exe\ /R /V /Y

echo Copy RemoteConsoleMgr debug binary files to output folder
xcopy "%CURRCMAPP%\ocx - RemoteConsoleMgr\Debug_Unicode\RemoteConsoleMgrUD.ocx" %CURPROJBASE%\Scot\Controls\ /R /V /Y
xcopy "%CURRCMAPP%\dll - RCMgr\Debug_Unicode\RCMgrUD.dll" %CURPROJBASE%\Scot\dll\ /R /V /Y
xcopy "%CURRCMAPP%\dll - StateMachine\Debug_Unicode\statemUD.dll" %CURPROJBASE%\Scot\dll\ /R /V /Y
xcopy "%CURRCMAPP%\dll - Utils\Debug_Unicode\UtilsUD.dll" %CURPROJBASE%\Scot\dll\ /R /V /Y
xcopy "%CURRCMAPP%\exe - TestRCM\Debug_Unicode\TestRCMUD.exe" %CURPROJBASE%\Scot\bin\ /R /V /Y


SET BROOT=""
SET MSD=""
SET LOGFILE=""
SET CURPROJBASE=""
SET CURPROJSCOT=""
SET LIB=%SAVELIB%
SET INCLUDE=%SAVEINCLUDE%
SET PATH=%SAVEPATH%
