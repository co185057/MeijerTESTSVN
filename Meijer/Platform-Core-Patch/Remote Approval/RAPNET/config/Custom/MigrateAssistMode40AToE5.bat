rem MergeXML.bat for RAP NET

if exist c:\scot\firmware\util\SetPartitionDrivers.bat call  c:\scot\firmware\util\SetPartitionDrivers.bat 
if exist %APP_DRIVE%\scot\firmware\util\SetPartitionDrivers.bat call  %APP_DRIVE%\scot\firmware\util\SetPartitionDrivers.bat 

set COPYCMD=/Y
set INSTALLDIR=%APP_DRIVE%\Scot\Install
set INSTALLTRACELOG=%INSTALLDIR%\MigrateAM40AToE5.log

set RAP0=%APP_DRIVE%\scot\config\Rap.000
set RAPAM40A=%APP_DRIVE%\scot\config\Rap3.1.000
set RAPLOCALFONTFileName=LocalFontSettings.xml
set RAPREMOTESMFileName=RemoteSMConfig.xml
set RAP40AFileName=Rap3.1.000

call :getDateTime
attrib -r %APP_DRIVE%\scot\config\*.*
echo *****************************************************************************************>> %INSTALLTRACELOG%
echo Begin Date/Time = %CDCT% >> %INSTALLTRACELOG%

set INPUTFILE=
set OUTPUTFILE=

if exist %APP_DRIVE%\scot\bin\scotapp.exe goto :MergeXMLLane
if not exist %APP_DRIVE%\scot\bin\scotapp.exe goto :MergeXMLRAP
goto :eof

:MergeXMLLane
echo No call to MergeXML for Lane >> %INSTALLTRACELOG%
goto :end


:MergeXMLRAP
echo MergeXML for RAP >> %INSTALLTRACELOG%

echo calling MergeXML.exe -b %RAPAM40A% for backward compatbility >> %INSTALLTRACELOG%
if exist %RAPAM40A% call %APP_DRIVE%\scot\bin\MergeXML.exe -b %RAPAM40A% >> %INSTALLTRACELOG% 2>>&1
echo MergeXML.exe returns (%ERRORLEVEL%) >> %INSTALLTRACELOG%

echo calling MergeXML.exe -s %RAPREMOTESMFileName% %RAP0% for including the file >> %INSTALLTRACELOG%
if exist %RAP0% call %APP_DRIVE%\scot\bin\MergeXML.exe -s %RAPREMOTESMFileName% %RAP0% >> %INSTALLTRACELOG% 2>>&1
echo MergeXML.exe returns (%ERRORLEVEL%) >> %INSTALLTRACELOG%

echo calling MergeXML.exe -s %RAPLocalFontFileName% %RAP0% for including the file >> %INSTALLTRACELOG%
if exist %RAP0% call %APP_DRIVE%\scot\bin\MergeXML.exe -s %RAPLocalFontFileName% %RAP0% >> %INSTALLTRACELOG% 2>>&1
echo MergeXML.exe returns (%ERRORLEVEL%) >> %INSTALLTRACELOG%

echo calling MergeXML.exe -s %RAP40AFileName% %RAP0% for including the file >> %INSTALLTRACELOG%
if exist %RAP0% call %APP_DRIVE%\scot\bin\MergeXML.exe -s %RAP40AFileName% %RAP0% >> %INSTALLTRACELOG% 2>>&1
echo MergeXML.exe returns (%ERRORLEVEL%) >> %INSTALLTRACELOG%

goto :end

:end
call :getDateTime
echo End Date/Time = %CDCT% >> %INSTALLTRACELOG%
goto :eof


:getDateTime
@echo off
set CDCT=
for /f "tokens=*" %%i in ('date /t') do set date1=%%i 
set CD=%date1%
for /f "tokens=*" %%i in ('time /t') do set time1=%%i 
set CT=%time1% 
set CDCT=%CD%%CT: =%
@echo on
goto :eof

