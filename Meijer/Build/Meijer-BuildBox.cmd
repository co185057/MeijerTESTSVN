@echo off
SET BROOT=\Builds
SET MSD="C:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin\MSDev.exe"
SET MSREBASE="C:\Program Files\Microsoft Visual Studio\VC98\Bin\ReBase.exe"
SET MSVCVARS="C:\Program Files\Microsoft Visual Studio\VC98\Bin\vcvars32.bat"
SET PACKAGER="D:\Program Files\Setup Factory 8.0 Trial\SUF80Design.exe"
rem SET PACKAGER="C:\Program Files\Setup Factory 6.0\SUF60Design.exe"
SET CURPROJBASE=%BROOT%\%1\%2
SET CURPROJCUST=%BROOT%\%1
SET CURPROJSCOT=%CURPROJBASE%\Source
SET LOGFILE=%CURPROJBASE%\Build-SSCO-Log
SET COPYCMD=/Y

rem Add other includes
SET SAVELIB=%LIB%
SET SAVEINCLUDE=%INCLUDE%

SET INCLUDE=%INCLUDE%;C:\Program Files\Microsoft Visual Studio\VC98\MFC\SRC;

call %MSVCVARS%

echo Deleting existing log files
del "%LOGFILE%*" /F /Q

echo Deleting SSCO TB/SSF binaries
del  /S /F /Q C:\Scot\Bin\SCOTSSFU.dll
del  /S /F /Q C:\Scot\Bin\TransBrokerU.dll
del  /S /F /Q C:\Scot\Dll\ScotSSFMsgU.dll 
del  /S /F /Q c:\Scot\Dll\TransBrokerMsgU.dll 

echo Building TB/SSF for '%CURPROJBASE%' . . .

echo Setup files for building . . .
md c:\scot\controls\
copy /Y %CURPROJBASE%\Scot\Controls\WeightInfoD.dll C:\scot\controls
md \scot\controls\
copy /Y %CURPROJBASE%\Scot\Controls\ACSHookManager.dll \scot\controls
copy /Y %CURPROJSCOT%\MeijerDevKit\MeijerSSFTB.dsw	 %CURPROJSCOT%\
copy /Y %CURPROJSCOT%\MeijerDevKit\Scotssf.dsp 		 %CURPROJSCOT%\SCOTSSF\
copy /Y %CURPROJSCOT%\MeijerDevKit\TransactionBroker.dsp %CURPROJSCOT%\AfxTransBroker\
copy /Y %CURPROJSCOT%\MeijerDevKit\TransBrokerMsg.dsp 	 %CURPROJSCOT%\AfxTransBrokerMsg\
copy /Y %CURPROJSCOT%\MeijerDevKit\SCOTSSFMsg.dsp	 %CURPROJSCOT%\SCOTSSFMsg\

echo Removing readonly flags for all files under '%CURPROJBASE%'...
attrib -R /S /D %CURPROJBASE%\*.*

rem REMOVE echo Running Custom Build All script '%CURPROJCUST%\CustomBuildAll.bat'...
rem REMOVE start "Custom Build Step" /WAIT %CURPROJCUST%\CustomBuildAll.bat %CURPROJBASE%

echo ---------------------------------------------------------------
echo =
echo = TB/SSF Build Script - Release
echo =
echo ---------------------------------------------------------------
echo Cleaning and rebuilding 'SCOTSSF - Win32 Unicode Release'
%MSD% "%CURPROJSCOT%\MeijerSSFTB.dsw" /MAKE "SCOTSSF - Win32 Unicode Release"  /OUT %LOGFILE%-UnicodeReleaseSSF.log /REBUILD /CLEAN

echo Cleaning and rebuilding 'TransBroker - Win32 Unicode Release'
%MSD% "%CURPROJSCOT%\MeijerSSFTB.dsw" /MAKE "TransactionBroker - Win32 Unicode Release" /OUT %LOGFILE%-UnicodeReleaseTB.log /REBUILD /CLEAN

echo ---------------------------------------------------------------
echo =
echo = Rebasing TB and SSF binaries and generating debug files - Release
echo =
echo ---------------------------------------------------------------
md C:\Scot\Symbols\
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Bin\SCOTSSFU.dll
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Bin\TransBrokerU.dll
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Dll\ScotSSFMsgU.dll 
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Dll\TransBrokerMsgU.dll 

echo ---------------------------------------------------------------
echo =
echo = Copying TB and SSF binary files to build folder - Release
echo =
echo ---------------------------------------------------------------
xcopy C:\Scot\Bin\ScotSSFU.dll %CURPROJBASE%\Scot\Bin\ /R /V /Y
xcopy C:\Scot\Bin\TransBrokerU.dll %CURPROJBASE%\Scot\Bin\ /R /V /Y
xcopy C:\Scot\Dll\ScotSSFMsgU.dll %CURPROJBASE%\Scot\Dll\ /R /V /Y
xcopy C:\Scot\Dll\TransBrokerMsgU.dll %CURPROJBASE%\Scot\Dll\ /R /V /Y

echo ---------------------------------------------------------------
echo =
echo = Copying TB and SSF debug symbol files to build folder - Release
echo =
echo ---------------------------------------------------------------
xcopy C:\Scot\Symbols %CURPROJBASE%\Symbols /E /R /V /Y
del C:\Scot\Symbols /S /F /Q
rd C:\Scot\Symbols /s /q

echo ---------------------------------------------------------------
echo =
echo = Building the SSCO Setup Packages for Lane And Rap Installation
echo =
echo ---------------------------------------------------------------
echo Packaging MeijerLaneSetup.exe...
%PACKAGER% /BUILD "%CURPROJBASE%\Build\LaneSetup.sf8" "/CONST:%CURPROJBASE%\Build\LaneSetup.ini" /LOG:%LOGFILE%-LaneSetup.log
echo Packaging MeijerRapSetup.exe...
%PACKAGER% /BUILD "%CURPROJBASE%\Build\RapSetup.sf8" "/CONST:%CURPROJBASE%\Build\RapSetup.ini" /LOG:%LOGFILE%-RapSetup.log

REM ==== NOW THAT THE RELEASE HAS BEEN BUILT AND THE LANE/RAP SETUP   ====
REM ==== PACKAGES COMPLETE, WE CAN BUILD DEBUG AND PACKAGE THE DEVKIT ====

echo ---------------------------------------------------------------
echo =
echo = TB/SSF Build Script - Debug
echo =
echo ---------------------------------------------------------------
echo Cleaning and rebuilding 'SCOTSSF - Win32 Unicode Debug'
%MSD% "%CURPROJSCOT%\MeijerSSFTB.dsw" /MAKE "SCOTSSF - Win32 Unicode Debug"  /OUT %LOGFILE%-UnicodeReleaseSSFD.log /REBUILD /CLEAN

echo Cleaning and rebuilding 'TransBroker - Win32 Unicode Debug'
%MSD% "%CURPROJSCOT%\MeijerSSFTB.dsw" /MAKE "TransactionBroker - Win32 Unicode Debug" /OUT %LOGFILE%-UnicodeReleaseTBD.log /REBUILD /CLEAN


echo ---------------------------------------------------------------
echo =
echo = Rebasing TB and SSF binaries and generating debug files - Debug
echo =
echo ---------------------------------------------------------------
md C:\Scot\Symbols\
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Bin\SCOTSSFUD.dll
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Bin\TransBrokerUD.dll
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Dll\ScotSSFMsgUD.dll 
%MSREBASE% -b 0x10000000 -x C:\Scot\Symbols\ C:\Scot\Dll\TransBrokerMsgU.dll 

echo ---------------------------------------------------------------
echo =
echo = Copying TB and SSF binary files to build folder - Debug
echo =
echo ---------------------------------------------------------------
xcopy C:\Scot\Bin\ScotSSFUD.dll %CURPROJBASE%\Scot\Bin\ /R /V /Y
xcopy C:\Scot\Bin\TransBrokerUD.dll %CURPROJBASE%\Scot\Bin\ /R /V /Y
xcopy C:\Scot\Dll\ScotSSFMsgUD.dll %CURPROJBASE%\Scot\Dll\ /R /V /Y
xcopy C:\Scot\Dll\TransBrokerMsgU.dll %CURPROJBASE%\Scot\Dll\ /R /V /Y

echo ---------------------------------------------------------------
echo =
echo = Copying TB and SSF debug files to build folder.
echo =
echo ---------------------------------------------------------------
xcopy C:\Scot\Symbols %CURPROJBASE%\Symbols /E /R /V /Y
del C:\Scot\Symbols /S /F /Q
rd C:\Scot\Symbols /s /q

echo ---------------------------------------------------------------
echo =
echo = Building the SSCO Development Kit Setup Package 
echo =
echo ---------------------------------------------------------------
echo Packaging MeijerDevKitSetup.exe...
%PACKAGER% /BUILD "%CURPROJBASE%\Build\MeijerDevKitSetup.sf8" "/CONST:%CURPROJBASE%\Build\LaneSetup.ini" /LOG:%LOGFILE%-MeijerDevKitSetup.log

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
