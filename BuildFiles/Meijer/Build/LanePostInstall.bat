rem Implement any quick installation items to be run post install here

rem Param1 = %AppDir%
rem Param2 = %TempDir%
rem Param3 = %CheckAtLane%

REM %2\SoundLossFix.exe
rem Wait for the SoundLossFix to be applied...
REM Sleep 6
REM del /F /Q %2\SoundLossFix.exe

rem Copy the image files based on the Current Display Configuration
%1\bin\getxml.exe "/s" "%1\config\fastlane3.1.xml" "//@Position" > %2\~349getxml.txt
set /p GetXML=<%2\~349getxml.txt
del /F /Q %2\~349getxml.txt

echo Detected Display Position %GetXML%

rem IF "0,0,1024,768" == "%GetXML%" (
rem	echo Detected 1024x768 - Copying config and image files
rem	xcopy /R /Y %1\Config\10x7\*.* %1\Config\
rem	xcopy /R /Y %1\image\10x7\*.* %1\image\
rem ) ELSE (
rem	echo Detected 800x600 - Copying config and image files
rem	xcopy /R /Y %1\Config\8x6\*.* %1\Config\
rem	xcopy /R /Y %1\image\8x6\*.* %1\image\
rem )

rem POS34817 copy the 1024x768 files only
echo Default 1024x768 - Copying config and image files
xcopy /R /Y %1\Config\10x7\*.* %1\Config\
xcopy /R /Y %1\image\10x7\*.* %1\image\


Set GetXML=

echo Cleanup any old base files no longer being used
del /f /q %1\Config\Standard\FastLane3.1.xml
del /f /q %1\Config\LaunchPadMsg0409.000

echo Reregister the OPOS controls in C:\scot\controls
for %%i in (C:\SCOT\Controls\Opos*.ocx) do regsvr32 /s %%i
for %%i in (C:\SCOT\Controls\SCOT*.ocx) do regsvr32 /s %%i
regsvr32 /s C:\SCOT\Controls\NCRPOSKeyboard.ocx
regsvr32 /s C:\SCOT\Controls\XMPOSPrinter.ocx

echo Register Unicode version of Remote Console Manager
regsvr32 /s c:\SCOT\Controls\RemoteConsoleMgrU.ocx

REM Fix for TAR319681
attrib C:\ACS\*.* -R /S /D

rem delete acsapploader.bat from support account startup 
c:
cd\
cd C:\Documents and Settings\support\Start Menu\Programs\Startup
del /F /Q ACS*.*
cd\

rem delete launchpadnet.bat from the startup because the acsapldr will be the one to trigger it
cd\
cd C:\Documents and Settings\scot\Start Menu\Programs\Startup
del /F /Q launch*.*


rem Configure Settings Second Printer at Lane if enabled
IF /I "%3" == "yes" GOTO CheckAtLane 
IF /I "%3" == "y" GOTO CheckAtLane
GOTO SkipCheckAtLane
:CheckAtLane
set INSTLOG=c:\scot\install\InstallHistory.log
echo. >> %INSTLOG%
echo ********************************************************************  >> %INSTLOG%
echo %date% %time% >> %INSTLOG%


echo Updating the RAP Printer emulation/software to 7167 Mode... >> %INSTLOG%
%1\bin\BinOutSerial.exe NCRPOSPrinter.1 %1\bin\SetTo7167Mode.bin
type result.log >> %INSTLOG%
del /f /q result.log
echo Update complete -- Manually verify Emulation/Software settings in  >> %INSTLOG%
echo the printer hardware Diagnostics Form >> %INSTLOG%
echo ********************************************************************  >> %INSTLOG%

:SkipCheckAtLane


rem Ensure the C:\SCOT\Util directory has been created on the machine; required for UpCon
IF NOT EXIST c:\scot\util (
	echo Creating the C:\SCOT\Util folder to be ready for UpCon
	md c:\SCOT\util
)
