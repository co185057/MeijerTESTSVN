@echo off
SET BROOT=D:\Builds\Build_Fastlane
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

rem del %LOGFILE%-UnicodeReleaseScotLib.log /F /Q
rem del %LOGFILE%-UnicodeReleaseSSF.log /F /Q
rem del %LOGFILE%-UnicodeReleaseScot.log /F /Q

echo Building Core and TB for '%CURPROJBASE%' . . .

echo Removing readonly flags for all files under '%CURPROJBASE%'...
attrib -R /S /D %CURPROJBASE%\*.*

echo SKIPPING - Running Custom Build All script '%CURPROJCUST%\CustomBuildAll.bat'...
rem start "Custom Build Step" /WAIT %CURPROJCUST%\CustomBuildAll.bat %CURPROJBASE%

echo ---------------------------------------------------------------
echo =
echo = 3-Step SCOTAPP Build Script
echo =
echo ---------------------------------------------------------------
echo SKIPPING - Cleaning and rebuilding 'SCOTApp - Win32 Unicode Release BuildLib'
rem %MSD% "%CURPROJSCOT%\ScotApp.dsw" /USEENV /MAKE "SCOTApp - Win32 Unicode Release BuildLib"  /OUT %LOGFILE%-UnicodeReleaseScotLib.log /REBUILD /CLEAN

echo SKIPPING - Cleaning and rebuilding 'SCOTSSF - Win32 Unicode Release'
rem %MSD% "%CURPROJSCOT%\ScotApp.dsw" /USEENV /MAKE "SCOTSSF - Win32 Unicode Release" /OUT %LOGFILE%-UnicodeReleaseSSF.log /REBUILD /CLEAN

echo SKIPPING - Cleaning and rebuilding 'SCOTApp - Win32 Unicode Release'
rem %MSD% "%CURPROJSCOT%\ScotApp.dsw" /USENV /MAKE "SCOTApp - Win32 Unicode Release" /OUT %LOGFILE%-UnicodeReleaseScot.log /REBUILD /CLEAN

echo ---------------------------------------------------------------
echo =
echo = Rebasing FastLane/TB binaries and generating debug files.
echo =
echo ---------------------------------------------------------------
md "%CURPROJBASE%\Symbols"
%MSREBASE% -v -b 0x400000 -xa "%CURPROJBASE%\Symbols" "%CURPROJBASE%\Source\ScotApp\Release_UNICODE\ScotAppU.exe"
%MSREBASE% -b 0x10000000 -xa "%CURPROJBASE%\Symbols" "%CURPROJBASE%\Source\ScotServices\Release_UNICODE\ScotServicesU.dll"
%MSREBASE% -b 0x10000000 -xa "%CURPROJBASE%\Symbols" "%CURPROJBASE%\Source\SCOTSSF\Release_UNICODE\SCOTSSFU.dll"
%MSREBASE% -b 0x10000000 -xa "%CURPROJBASE%\Symbols" "%CURPROJBASE%\Source\AfxTransBroker\Release_UNICODE\TransBrokerU.dll"
%MSREBASE% -b 0x10000000 -xa "%CURPROJBASE%\Symbols" "%CURPROJBASE%\Source\ScotAppMsg\Release_UNICODE\ScotAppMsgU.dll"
%MSREBASE% -b 0x10000000 -xa "%CURPROJBASE%\Symbols" "%CURPROJBASE%\Source\ScotServicesMsg\Release_UNICODE\ScotServicesMsgU.dll" 
%MSREBASE% -b 0x10000000 -xa "%CURPROJBASE%\Symbols" "%CURPROJBASE%\Source\ScotSSFMsg\Release_UNICODE\ScotSSFMsgU.dll"
%MSREBASE% -b 0x10000000 -xa "%CURPROJBASE%\Symbols" "%CURPROJBASE%\Source\AfxTransBrokerMsg\Release_UNICODE\TransBrokerMsgU.dll" 


echo ---------------------------------------------------------------
echo =
echo = Copying FastLane and TB binary files to build folder.
echo =
echo ---------------------------------------------------------------
xcopy "%CURPROJBASE%\Source\ScotApp\Release_UNICODE\ScotAppU.exe" %CURPROJBASE%\Scot\Bin\ /R /V /Y
xcopy "%CURPROJBASE%\Source\ScotServices\Release_UNICODE\ScotServicesU.dll" %CURPROJBASE%\Scot\Bin\ /R /V /Y
xcopy "%CURPROJBASE%\Source\SCOTSSF\Release_UNICODE\SCOTSSFU.dll" %CURPROJBASE%\Scot\Bin\ /R /V /Y
xcopy "%CURPROJBASE%\Source\AfxTransBroker\Release_UNICODE\TransBrokerU.dll" %CURPROJBASE%\Scot\Bin\ /R /V /Y

xcopy "%CURPROJBASE%\Source\ScotAppMsg\Release_UNICODE\ScotAppMsgU.dll" %CURPROJBASE%\Scot\Dll\ /R /V /Y
xcopy "%CURPROJBASE%\Source\ScotServicesMsg\Release_UNICODE\ScotServicesMsgU.dll"  %CURPROJBASE%\Scot\Dll\ /R /V /Y
xcopy "%CURPROJBASE%\Source\ScotSSFMsg\Release_UNICODE\ScotSSFMsgU.dll" %CURPROJBASE%\Scot\Dll\ /R /V /Y
xcopy "%CURPROJBASE%\Source\AfxTransBrokerMsg\Release_UNICODE\TransBrokerMsgU.dll"  %CURPROJBASE%\Scot\Dll\ /R /V /Y




echo ---------------------------------------------------------------
echo =
echo = Building the Setup Packages for Lane And Rap Installation
echo =
echo ---------------------------------------------------------------
if exist "%CURPROJBASE%\Build\LaneSetup.suf" SUFDesign.exe /BUILD %CURPROJBASE%\Build\LaneSetup.suf /CONST:%CURPROJBASE%\Build\LaneSetup.ini
if exist "%CURPROJBASE%\Build\RapSetup.suf"  SUFDesign.exe /BUILD %CURPROJBASE%\Build\RapSetup.suf /CONST:%CURPROJBASE%\Build\RapSetup.ini
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
