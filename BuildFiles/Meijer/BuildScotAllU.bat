@echo off
SET BROOT=\Builds
SET MSD="d:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin\MSDev.exe"
SET MSREBASE="d:\Program Files\Microsoft Visual Studio\VC98\Bin\ReBase.exe"
SET MSVCVARS="d:\Program Files\Microsoft Visual Studio\VC98\Bin\vcvars32.bat"
SET CURPROJBASE=%BROOT%\%1\%2
SET CURPROJCUST=%BROOT%\%1
SET CURPROJSCOT=%CURPROJBASE%\Source\ScotApp
SET LOGFILE=%CURPROJBASE%\BuildScotAll
SET COPYCMD=/Y
SET SFD="D:\Program Files\Setup Factory 9\SUFDesign.exe"

rem Add other includes
SET SAVELIB=%LIB%
SET SAVEINCLUDE=%INCLUDE%

SET INCLUDE=%INCLUDE%;%CURPROJBASE%\Platform\include;
SET INCLUDE=%INCLUDE%;d:\Program Files\Microsoft Visual Studio\VC98\MFC\SRC;

call %MSVCVARS%

del %LOGFILE%-UnicodeReleaseScotLib.log /F /Q
del %LOGFILE%-UnicodeReleaseSSF.log /F /Q
del %LOGFILE%-UnicodeReleaseScot.log /F /Q
del %LOGFILE%-SF8SBuild.log /F /Q

echo Deleting SCOT binaries
del  /S /F /Q C:\Scot\Bin\ScotAppU.exe
del  /S /F /Q C:\Scot\Bin\ScotServicesU.dll
del  /S /F /Q C:\Scot\Bin\SCOTSSFU.dll
del  /S /F /Q C:\Scot\Bin\TransBrokerU.dll
del  /S /F /Q C:\Scot\Dll\ScotAppMsgU.dll
del  /S /F /Q C:\Scot\Dll\ScotServicesMsgU.dll 
del  /S /F /Q C:\Scot\Dll\ScotSSFMsgU.dll 
del  /S /F /Q c:\Scot\Dll\TransBrokerMsgU.dll 

echo Building Core and TB for '%CURPROJBASE%' . . .

echo Removing readonly flags for all files under '%CURPROJBASE%'...
attrib -R /S /D %CURPROJBASE%\*.*

echo Running Custom Build All script '%CURPROJCUST%\CustomBuildAll.bat'...
start "Custom Build Step" /WAIT %CURPROJCUST%\CustomBuildAll.bat %CURPROJBASE%

echo ---------------------------------------------------------------
echo =
echo = 3-Step SCOTAPP Build Script
echo =
echo ---------------------------------------------------------------
echo Cleaning and rebuilding 'SCOTApp - Win32 Unicode Release BuildLib'
%MSD% "%CURPROJSCOT%\ScotApp.dsw" /MAKE "SCOTApp - Win32 Unicode Release BuildLib"  /OUT %LOGFILE%-UnicodeReleaseScotLib.log /REBUILD /CLEAN

echo Cleaning and rebuilding 'SCOTSSF - Win32 Unicode Release'
%MSD% "%CURPROJSCOT%\ScotApp.dsw" /MAKE "SCOTSSF - Win32 Unicode Release" /OUT %LOGFILE%-UnicodeReleaseSSF.log /REBUILD /CLEAN

echo Cleaning and rebuilding 'SCOTApp - Win32 Unicode Release'
%MSD% "%CURPROJSCOT%\ScotApp.dsw" /MAKE "SCOTApp - Win32 Unicode Release" /OUT %LOGFILE%-UnicodeReleaseScot.log /REBUILD /CLEAN

echo ---------------------------------------------------------------
echo =
echo = Rebasing FastLane/TB binaries and generating debug files.
echo =
echo ---------------------------------------------------------------
md C:\Scot\Symbols\
%MSREBASE% -b 0x400000 -x C:\Scot\Symbols\ C:\Scot\Bin\ScotAppU.exe
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Bin\ScotServicesU.dll
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Bin\SCOTSSFU.dll
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Bin\TransBrokerU.dll
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Dll\ScotAppMsgU.dll
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Dll\ScotServicesMsgU.dll 
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Dll\ScotSSFMsgU.dll 
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Dll\TransBrokerMsgU.dll 

echo ---------------------------------------------------------------
echo =
echo = Copying FastLane and TB binary files to build folder.
echo =
echo ---------------------------------------------------------------
xcopy C:\Scot\Bin\ScotAppU.exe %CURPROJBASE%\Scot\Bin\ /R /V /Y
xcopy C:\Scot\Bin\ScotServicesU.dll %CURPROJBASE%\Scot\Bin\ /R /V /Y
xcopy C:\Scot\Bin\ScotSSFU.dll %CURPROJBASE%\Scot\Bin\ /R /V /Y
xcopy C:\Scot\Bin\TransBrokerU.dll %CURPROJBASE%\Scot\Bin\ /R /V /Y

xcopy C:\Scot\Controls\WeightInfo.dll %CURPROJBASE%\Scot\Controls\ /R /V /Y
xcopy C:\Scot\Controls\DBMgrClient.ocx %CURPROJBASE%\Scot\Controls\ /R /V /Y
xcopy C:\Scot\Controls\SmartScale.ocx %CURPROJBASE%\Scot\Controls\ /R /V /Y

xcopy C:\Scot\Dll\ScotAppMsgU.dll %CURPROJBASE%\Scot\Dll\ /R /V /Y
xcopy C:\Scot\Dll\ScotServicesMsgU.dll %CURPROJBASE%\Scot\Dll\ /R /V /Y
xcopy C:\Scot\Dll\ScotSSFMsgU.dll %CURPROJBASE%\Scot\Dll\ /R /V /Y
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
echo = Building the Setup Packages for Lane And Rap Installation
echo =
echo ---------------------------------------------------------------


%SFD% /BUILD %CURPROJBASE%\Build\LaneSetup.suf /CONST:%CURPROJBASE%\Build\LaneSetup.ini
%SFD% /BUILD %CURPROJBASE%\Build\RapSetup.suf /CONST:%CURPROJBASE%\Build\RapSetup.ini
%SFD% /BUILD %CURPROJBASE%\Build\SSCO_Config.suf

DEL D:\Upcon\updates\FastLane.6.0.5*
DEL D:\Upcon\*.exe
DEL D:\Upcon\*.zip
COPY /y %CURPROJBASE%\Build\FastLane.6.0.5* d:\Upcon\updates
CALL "D:\Upcon\CreateZipPackage.bat" "6.0.5.13.15"
DEL D:\Upcon\*.zip

::echo Running POST Custom Build script '%CURPROJCUST%\CustomBuildAll-Post.bat'...
::start "POST Custom Build Step" /WAIT %CURPROJCUST%\CustomBuildAll-Post.bat %CURPROJBASE%


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
