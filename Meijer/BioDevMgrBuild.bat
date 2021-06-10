@echo off
echo ---------------------------------------------------------------
echo =
echo = Custom Build Script for Biometric Device Manager C# components
echo =
echo ---------------------------------------------------------------
SET VS8DIR=D:\Program Files\Microsoft Visual Studio 8
SET VS8="%VS8DIR%\Common7\IDE\devenv.exe"
SET CURPROJBASE=%1
SET CURPROJAPP=%CURPROJBASE%\Platform-Core-Patch\BiometricDeviceManager
SET LOGFILE=%CURPROJBASE%\CustomScotAll-BioDevMgr.log
SET COPYCMD=/Y

rem Add other includes
SET SAVELIB=%LIB%
SET SAVEINCLUDE=%INCLUDE%
SET SAVEPATH=%PATH%

call "%VS8DIR%\Common7\Tools\vsvars32.bat"
SET INCLUDE=%INCLUDE%;c:\scot\adk\include;
SET LIB=%LIB%;c:\scot\adk\lib;c:\scot\adk\lib\release7

echo Deleting %LOGFILE%
IF EXIST %LOGFILE% del %LOGFILE% /F /Q

echo Cleaning and rebuilding BiometricDeviceManager Solution
%VS8% "%CURPROJAPP%\BiometricDeviceManager.sln" /REBUILD "Release" /UseEnv /OUT %LOGFILE%

echo Copying BiometricDeviceManager Debug symbols to install folder
md %CURPROJBASE%\Symbols\
md %CURPROJBASE%\Symbols\bin\
xcopy "%CURPROJAPP%\BioDevMgr\bin\Release\BioDevMgr.pdb" %CURPROJBASE%\Symbols\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Connector.RemoteMessage\bin\Release\BioDevMgr.Connector.RemoteMessage.pdb" %CURPROJBASE%\Symbols\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Connector.Test\bin\Release\BioDevMgr.Connector.Test.pdb" %CURPROJBASE%\Symbols\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Controller.SalesMonitor\bin\Release\BioDevMgr.Controller.SalesMonitor.pdb" %CURPROJBASE%\Symbols\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Driver.DigitalPersona\bin\Release\BioDevMgr.Driver.DigitalPersona.pdb" %CURPROJBASE%\Symbols\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Driver.Test\bin\Release\BioDevMgr.Driver.Test.pdb" %CURPROJBASE%\Symbols\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Driver.ACSBiomet\bin\Release\BioDevMgr.Driver.ACSBiomet.pdb" %CURPROJBASE%\Symbols\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Driver.ACSIO\bin\Release\BioDevMgr.Driver.ACSIO.pdb" %CURPROJBASE%\Symbols\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.PluginFramework\bin\Release\BioDevMgr.PluginFramework.pdb" %CURPROJBASE%\Symbols\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.ReaderTester\bin\Release\BioDevMgr.ReaderTester.pdb" %CURPROJBASE%\Symbols\bin\ /R /V /Y
xcopy %CURPROJBASE%\Symbols\bin\BioDevMgr* C:\SCOT\bin /R /V /Y

echo Copy BiometricDeviceManager binary output files to installation recipe folder
rem SCOT folder
xcopy "%CURPROJAPP%\BioDevMgr\bin\Release\BioDevMgr.exe" %CURPROJBASE%\SCOT\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Connector.RemoteMessage\bin\Release\BioDevMgr.Connector.RemoteMessage.dll" %CURPROJBASE%\SCOT\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Connector.Test\bin\Release\BioDevMgr.Connector.Test.dll" %CURPROJBASE%\SCOT\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Controller.SalesMonitor\bin\Release\BioDevMgr.Controller.SalesMonitor.dll" %CURPROJBASE%\SCOT\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Driver.DigitalPersona\bin\Release\BioDevMgr.Driver.DigitalPersona.dll" %CURPROJBASE%\SCOT\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Driver.Test\bin\Release\BioDevMgr.Driver.Test.dll" %CURPROJBASE%\SCOT\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Driver.ACSBiomet\bin\Release\BioDevMgr.Driver.ACSBiomet.dll" %CURPROJBASE%\SCOT\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Driver.ACSIO\bin\Release\BioDevMgr.Driver.ACSIO.dll" %CURPROJBASE%\SCOT\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.PluginFramework\bin\Release\BioDevMgr.PluginFramework.dll" %CURPROJBASE%\SCOT\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.ReaderTester\bin\Release\BioDevMgr.ReaderTester.exe" %CURPROJBASE%\SCOT\bin\ /R /V /Y
rem RAP folder
xcopy "%CURPROJAPP%\BioDevMgr\bin\Release\BioDevMgr.exe" %CURPROJBASE%\RAP\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Connector.RemoteMessage\bin\Release\BioDevMgr.Connector.RemoteMessage.dll" %CURPROJBASE%\RAP\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Connector.Test\bin\Release\BioDevMgr.Connector.Test.dll" %CURPROJBASE%\RAP\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Controller.SalesMonitor\bin\Release\BioDevMgr.Controller.SalesMonitor.dll" %CURPROJBASE%\RAP\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Driver.DigitalPersona\bin\Release\BioDevMgr.Driver.DigitalPersona.dll" %CURPROJBASE%\RAP\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Driver.Test\bin\Release\BioDevMgr.Driver.Test.dll" %CURPROJBASE%\RAP\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Driver.ACSBiomet\bin\Release\BioDevMgr.Driver.ACSBiomet.dll" %CURPROJBASE%\RAP\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.Driver.ACSIO\bin\Release\BioDevMgr.Driver.ACSIO.dll" %CURPROJBASE%\RAP\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.PluginFramework\bin\Release\BioDevMgr.PluginFramework.dll" %CURPROJBASE%\RAP\bin\ /R /V /Y
xcopy "%CURPROJAPP%\BioDevMgr.ReaderTester\bin\Release\BioDevMgr.ReaderTester.exe" %CURPROJBASE%\RAP\bin\ /R /V /Y


xcopy %CURPROJBASE%\SCOT\bin\BioDevMgr* C:\SCOT\bin /R /V /Y

rem Restore environment variables
SET VS8DIR=""
SET VS8=""
SET LOGFILE=""
SET LIB=%SAVELIB%
SET INCLUDE=%SAVEINCLUDE%
SET PATH=%SAVEPATH%
