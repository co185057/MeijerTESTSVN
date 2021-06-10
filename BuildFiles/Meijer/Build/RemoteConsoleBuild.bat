@echo off
echo ---------------------------------------------------------------
echo =
echo = Custom Build Script for Meijer Remote Console Manager (Release)
echo =
echo ---------------------------------------------------------------
SET MSD="C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\IDE\devenv.exe"
SET MSD6="C:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin\MSDev.exe"
SET CURPROJBASE=%1
SET CURPROJAPP=%CURPROJBASE%\Source\AfxTransBroker\ACSHook
SET CURRCMAPP=%CURPROJBASE%\Platform-Core-Patch\Remote Console Manager
SET LOGFILE=%CURPROJBASE%\RemoteConsoleBuild_Release.log
SET COPYCMD=/Y
SET VC7DIR=C:\"Program Files"\"Microsoft Visual Studio .NET 2003"


rem Add other includes
SET SAVELIB=%LIB%
SET SAVEINCLUDE=%INCLUDE%
SET SAVEPATH=%PATH%

call %VC7DIR%\common7\tools\vsvars32.bat
SET INCLUDE=%INCLUDE%;c:\scot\adk\include;
SET LIB=%LIB%;c:\scot\adk\lib;c:\scot\adk\lib\release7


echo Deleting %LOGFILE%
del %LOGFILE% /F /Q


echo Cleaning and rebuilding Remote Console Manager - Unicode Release'
%MSD% "%CURRCMAPP%\RemoteConsoleMgr.sln" /REBUILD "Unicode Release" /UseEnv /OUT %LOGFILE%

echo Copying Debug symbols to install folder
md %CURPROJBASE%\Symbols\
md %CURPROJBASE%\Symbols\Dll\
md %CURPROJBASE%\Symbols\ocx\
xcopy "%CURRCMAPP%\ocx - RemoteConsoleMgr\Release_Unicode\RemoteConsoleMgrU.pdb" %CURPROJBASE%\Symbols\ocx\ /R /V /Y
xcopy "%CURRCMAPP%\dll - RCMgr\Release_Unicode\RCMgrU.pdb" %CURPROJBASE%\Symbols\Dll\ /R /V /Y
xcopy "%CURRCMAPP%\dll - StateMachine\Release_Unicode\statem.pdb" %CURPROJBASE%\Symbols\Dll\ /R /V /Y
xcopy "%CURRCMAPP%\dll - Utils\Release_Unicode\Utils.pdb" %CURPROJBASE%\Symbols\Dll\ /R /V /Y
xcopy "%CURRCMAPP%\exe - TestRCM\Release_Unicode\TestRCM.pdb" %CURPROJBASE%\Symbols\exe\ /R /V /Y

echo Copy RemoteConsoleMgr release binary files to output folder
xcopy "%CURRCMAPP%\ocx - RemoteConsoleMgr\Release_Unicode\RemoteConsoleMgrU.ocx" %CURPROJBASE%\Scot\Controls\ /R /V /Y
xcopy "%CURRCMAPP%\dll - RCMgr\Release_Unicode\RCMgrU.dll" %CURPROJBASE%\Scot\dll\ /R /V /Y
xcopy "%CURRCMAPP%\dll - StateMachine\Release_Unicode\statemU.dll" %CURPROJBASE%\Scot\dll\ /R /V /Y
xcopy "%CURRCMAPP%\dll - Utils\Release_Unicode\UtilsU.dll" %CURPROJBASE%\Scot\dll\ /R /V /Y
xcopy "%CURRCMAPP%\exe - TestRCM\Release_Unicode\TestRCMU.exe" %CURPROJBASE%\Scot\bin\ /R /V /Y


SET BROOT=""
SET MSD=""
SET LOGFILE=""
SET CURPROJBASE=""
SET CURPROJSCOT=""
SET LIB=%SAVELIB%
SET INCLUDE=%SAVEINCLUDE%
SET PATH=%SAVEPATH%
