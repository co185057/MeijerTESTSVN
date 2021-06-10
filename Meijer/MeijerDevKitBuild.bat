@echo off
echo ---------------------------------------------------------------
echo =
echo = Meijer FastLane Development Kit Build
echo =
echo ---------------------------------------------------------------
SET BROOT=\Builds
SET MSD="D:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin\MSDev.exe"
SET MSREBASE="D:\Program Files\Microsoft Visual Studio\VC98\Bin\ReBase.exe"
SET MSVCVARS="D:\Program Files\Microsoft Visual Studio\VC98\Bin\vcvars32.bat"
SET CURPROJBASE=%1
SET CURPROJSCOT=%CURPROJBASE%\Source\ScotApp
SET LOGFILE=%CURPROJBASE%\BuildScotAllDebug
SET COPYCMD=/Y

rem Add other includes
SET SAVELIB=%LIB%
SET SAVEINCLUDE=%INCLUDE%

SET INCLUDE=%INCLUDE%;D:\Program Files\Microsoft Visual Studio\VC98\MFC\SRC;

call %MSVCVARS%

del %LOGFILE%-UnicodeDebugScotLib.log /F /Q
del %LOGFILE%-UnicodeDebugSSF.log /F /Q
del %LOGFILE%-UnicodeDebugScot.log /F /Q

echo Deleting SCOT binaries
del  /S /F /Q C:\Scot\Bin\ScotAppUD.exe
del  /S /F /Q C:\Scot\Bin\ScotServicesUD.dll
del  /S /F /Q C:\Scot\Bin\SCOTSSFUD.dll
del  /S /F /Q C:\Scot\Bin\TransBrokerUD.dll
del  /S /F /Q C:\Scot\Dll\ScotAppMsgUD.dll
del  /S /F /Q C:\Scot\Dll\ScotServicesMsgUD.dll 
del  /S /F /Q C:\Scot\Dll\ScotSSFMsgUD.dll 
del  /S /F /Q C:\Scot\Dll\TransBrokerMsgU.dll 

echo Copy additional base TB control dependencies
xcopy %CURPROJBASE%\Source\AfxTransBroker\tbControls\Debug_Unicode\tbControlsUD.dll %CURPROJBASE%\Scot\Controls\ /R /V /Y
xcopy %CURPROJBASE%\Source\AfxTransBroker\OposServiceObject\NCRTBDevice\Debug_UNICODE\NCRTBDeviceUD.dll %CURPROJBASE%\Scot\Controls\ /R /V /Y

echo Building Debug Core and TB for '%CURPROJBASE%' . . .

echo Removing readonly flags for all files under '%CURPROJBASE%'...
attrib -R /S /D %CURPROJBASE%\*.*

echo ---------------------------------------------------------------
echo =
echo = 3-Step SCOTAPP Debug Build Script
echo =
echo ---------------------------------------------------------------
echo Cleaning and rebuilding 'SCOTApp - Win32 Unicode Debug BuildLib'
%MSD% "%CURPROJSCOT%\ScotApp.dsw" /MAKE "SCOTApp - Win32 Unicode Debug BuildLib"  /OUT %LOGFILE%-UnicodeDebugScotLib.log /REBUILD /CLEAN

echo Cleaning and rebuilding 'SCOTSSF - Win32 Unicode Debug'
%MSD% "%CURPROJSCOT%\ScotApp.dsw" /MAKE "SCOTSSF - Win32 Unicode Debug" /OUT %LOGFILE%-UnicodeDebugSSF.log /REBUILD /CLEAN

echo Cleaning and rebuilding 'SCOTApp - Win32 Unicode Debug'
%MSD% "%CURPROJSCOT%\ScotApp.dsw" /MAKE "SCOTApp - Win32 Unicode Debug" /OUT %LOGFILE%-UnicodeDebugScot.log /REBUILD /CLEAN

echo ---------------------------------------------------------------
echo =
echo = Rebasing FastLane/TB binaries and generating debug files.
echo =
echo ---------------------------------------------------------------
md C:\Scot\Symbols\
%MSREBASE% -b 0x400000 -x C:\Scot\Symbols\ C:\Scot\Bin\ScotAppUD.exe
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Bin\ScotServicesUD.dll
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Bin\SCOTSSFUD.dll
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Bin\TransBrokerUD.dll
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Dll\ScotAppMsgUD.dll
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Dll\ScotServicesMsgUD.dll 
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Dll\ScotSSFMsgUD.dll 
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Dll\TransBrokerMsgU.dll 

echo ---------------------------------------------------------------
echo =
echo = Copying FastLane and TB binary files to build folder.
echo =
echo ---------------------------------------------------------------
xcopy C:\Scot\Bin\ScotAppUD.exe %CURPROJBASE%\Scot\Bin\ /R /V /Y
xcopy C:\Scot\Bin\ScotServicesUD.dll %CURPROJBASE%\Scot\Bin\ /R /V /Y
xcopy C:\Scot\Bin\ScotSSFUD.dll %CURPROJBASE%\Scot\Bin\ /R /V /Y
xcopy C:\Scot\Bin\TransBrokerUD.dll %CURPROJBASE%\Scot\Bin\ /R /V /Y

xcopy C:\Scot\Controls\WeightInfoD.dll %CURPROJBASE%\Scot\Controls\ /R /V /Y
xcopy C:\Scot\Controls\DBMgrClientD.ocx %CURPROJBASE%\Scot\Controls\ /R /V /Y
xcopy C:\Scot\Controls\SmartScaleD.ocx %CURPROJBASE%\Scot\Controls\ /R /V /Y

xcopy C:\Scot\Dll\ScotAppMsgUD.dll %CURPROJBASE%\Scot\Dll\ /R /V /Y
xcopy C:\Scot\Dll\ScotServicesMsgUD.dll %CURPROJBASE%\Scot\Dll\ /R /V /Y
xcopy C:\Scot\Dll\ScotSSFMsgUD.dll %CURPROJBASE%\Scot\Dll\ /R /V /Y
xcopy C:\Scot\Dll\TransBrokerMsgU.dll %CURPROJBASE%\Scot\Dll\ /R /V /Y

echo ---------------------------------------------------------------
echo =
echo = Copying FastLane and TB debug files to build folder.
echo =
echo ---------------------------------------------------------------
xcopy C:\Scot\Symbols %CURPROJBASE%\Symbols /E /R /V /Y
del C:\Scot\Symbols /S /F /Q
rd C:\Scot\Symbols /s /q


echo ---------------------------------------------------------------
echo =
echo = Building the Meijer FastLane Development Kit Setup Package 
echo =
echo ---------------------------------------------------------------
"D:\Program Files\Setup Factory 8.0\SUF80Design.exe" /BUILD "%CURPROJBASE%\Build\MeijerDevKitSetup.sf6" "/CONST:%CURPROJBASE%\Build\LaneSetup.ini"


echo ---------------------------------------------------------------
echo =
echo = Build is complete.
echo = Build log can be found at %LOGFILE%
echo = Custom log might also be available in the same folder.
echo =
echo ---------------------------------------------------------------

SET BROOT=""
SET MSD=""
SET LOGFILE=""
SET CURPROJBASE=""
SET CURPROJSCOT=""
SET LIB=%SAVELIB%
SET INCLUDE=%SAVEINCLUDE%
