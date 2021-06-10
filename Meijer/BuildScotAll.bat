@echo off
SET BROOT=d:\Builds
SET MSD="d:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin\MSDev.exe"
SET MSREBASE="d:\Program Files\Microsoft Visual Studio\VC98\Bin\ReBase.exe"
SET MSVCVARS="d:\Program Files\Microsoft Visual Studio\VC98\Bin\vcvars32.bat"
SET CURPROJBASE=%BROOT%\%~1\%~2
SET CURPROJCUST=%BROOT%\%~1
SET CURPROJSCOT=%CURPROJBASE%\Source\ScotApp
SET LOGFILE=%CURPROJBASE%\BuildScotAll
SET COPYCMD=/Y

rem Add other includes
SET SAVELIB=%LIB%
SET SAVEINCLUDE=%INCLUDE%

set INCLUDE=
set LIB=
call %MSVCVARS%
set INCLUDE=%INCLUDE%;%MSVCDir%\MFC\SRC

del %LOGFILE%-ReleaseScotLib.log /F /Q
del %LOGFILE%-ReleaseSSF.log /F /Q
del %LOGFILE%-ReleaseScot.log /F /Q

echo Building Core and TB for '%CURPROJBASE%' . . .

echo Removing readonly flags for all files under '%CURPROJBASE%'...
attrib -R /S /D %CURPROJBASE%\*.*

echo Running Custom Build All script '%CURPROJCUST%\CustomBuildAll.bat'...
start "Custom Build Step" /WAIT %CURPROJBASE%\Build\CustomBuildAll.bat %CURPROJBASE%

echo ---------------------------------------------------------------
echo =
echo = 3-Step SCOTAPP Build Script
echo =
echo ---------------------------------------------------------------
echo Cleaning and rebuilding 'SCOTApp - Win32 Release BuildLib'
%MSD% "%CURPROJSCOT%\ScotApp.dsw" /USEENV /MAKE "SCOTApp - Win32 Release BuildLib"  /OUT %LOGFILE%-ReleaseScotLib.log /REBUILD /CLEAN

echo Cleaning and rebuilding 'SCOTSSF - Win32 Release'
%MSD% "%CURPROJSCOT%\ScotApp.dsw" /USEENV /MAKE "SCOTSSF - Win32 Release" /OUT %LOGFILE%-ReleaseSSF.log /REBUILD /CLEAN

echo Cleaning and rebuilding 'SCOTApp - Win32 Release'
%MSD% "%CURPROJSCOT%\ScotApp.dsw" /USEENV /MAKE "SCOTApp - Win32 Release" /OUT %LOGFILE%-ReleaseScot.log /REBUILD /CLEAN

echo ---------------------------------------------------------------
echo =
echo = Rebasing FastLane/TB binaries and generating debug files.
echo =
echo ---------------------------------------------------------------
md "%CURPROJBASE%\Symbols"
%MSREBASE% -b 0x400000 -x "%CURPROJBASE%\Symbols" "%CURPROJBASE%\Source\ScotApp\Release\ScotApp.exe"
%MSREBASE% -b 0x10000000 -x "%CURPROJBASE%\Symbols" "%CURPROJBASE%\Source\ScotServices\Release\ScotServices.dll"
%MSREBASE% -b 0x10000000 -x "%CURPROJBASE%\Symbols" "%CURPROJBASE%\Source\SCOTSSF\Release\SCOTSSF.dll"
%MSREBASE% -b 0x10000000 -x "%CURPROJBASE%\Symbols" "%CURPROJBASE%\Source\AfxTransBroker\Release\TransBroker.dll"
%MSREBASE% -b 0x10000000 -x "%CURPROJBASE%\Symbols" "%CURPROJBASE%\Source\ScotAppMsg\Release\ScotAppMsg.dll"
%MSREBASE% -b 0x10000000 -x "%CURPROJBASE%\Symbols" "%CURPROJBASE%\Source\ScotServicesMsg\Release\ScotServicesMsg.dll" 
%MSREBASE% -b 0x10000000 -x "%CURPROJBASE%\Symbols" "%CURPROJBASE%\Source\ScotSSFMsg\Release\ScotSSFMsg.dll"
%MSREBASE% -b 0x10000000 -x "%CURPROJBASE%\Symbols" "%CURPROJBASE%\Source\AfxTransBrokerMsg\Release\TransBrokerMsg.dll" 

echo ---------------------------------------------------------------
echo =
echo = Copying FastLane and TB binary files to build folder.
echo =
echo ---------------------------------------------------------------
xcopy "%CURPROJBASE%\Source\ScotApp\Release\ScotApp.exe" %CURPROJBASE%\Scot\Bin\ /R /V /Y
xcopy "%CURPROJBASE%\Source\ScotServices\Release\ScotServices.dll" %CURPROJBASE%\Scot\Bin\ /R /V /Y
xcopy "%CURPROJBASE%\Source\SCOTSSF\Release\SCOTSSF.dll" %CURPROJBASE%\Scot\Bin\ /R /V /Y
xcopy "%CURPROJBASE%\Source\AfxTransBroker\Release\TransBroker.dll" %CURPROJBASE%\Scot\Bin\ /R /V /Y

xcopy "%CURPROJBASE%\Source\WeightInfo\Release\WeightInfo.dll" %CURPROJBASE%\Scot\Controls\ /R /V /Y
xcopy "%CURPROJBASE%\Source\DBMgrClient\Release\DBMgrClient.ocx" %CURPROJBASE%\Scot\Controls\ /R /V /Y
xcopy "%CURPROJBASE%\Source\SmartScale\Release\SmartScale.ocx" %CURPROJBASE%\Scot\Controls\ /R /V /Y

xcopy "%CURPROJBASE%\Source\ScotAppMsg\Release\ScotAppMsg.dll" %CURPROJBASE%\Scot\Dll\ /R /V /Y
xcopy "%CURPROJBASE%\Source\ScotServicesMsg\Release\ScotServicesMsg.dll"  %CURPROJBASE%\Scot\Dll\ /R /V /Y
xcopy "%CURPROJBASE%\Source\ScotSSFMsg\Release\ScotSSFMsg.dll" %CURPROJBASE%\Scot\Dll\ /R /V /Y
xcopy "%CURPROJBASE%\Source\AfxTransBrokerMsg\Release\TransBrokerMsg.dll"  %CURPROJBASE%\Scot\Dll\ /R /V /Y


echo ---------------------------------------------------------------
echo =
echo = Building the Setup Packages for Lane And Rap Installation
echo =
echo ---------------------------------------------------------------
if exist "%CURPROJBASE%\Build\LaneSetup.suf" "D:\Program Files\Setup Factory 9\SUFDesign.exe" "%CURPROJBASE%\Build\LaneSetup.suf" "/B:%CURPROJBASE%\Build\LaneSetup.ini" /M
if exist "%CURPROJBASE%\Build\RapSetup.suf" "D:\Program Files\Setup Factory 9\SUFDesign.exe" "%CURPROJBASE%\Build\RapSetup.suf" "/B:%CURPROJBASE%\Build\RapSetup.ini" /M
pushd "%CURPROJBASE%\Build"
if exist CustomInstall.bat start "Custom Build Install Step" /WAIT CustomInstall.bat "%~1" "%~2"
popd

echo ---------------------------------------------------------------
echo =
echo = Build is complete.
echo = Build log can be found at %CURPROJBASE%
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
