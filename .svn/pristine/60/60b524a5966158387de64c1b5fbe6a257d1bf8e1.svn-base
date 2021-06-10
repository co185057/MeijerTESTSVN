rem Implement any quick installation items to be run post install here

rem Param1 = %AppDir%
rem Param2 = %TempDir%

REM %2\SoundLossFix.exe
rem Wait for the SoundLossFix to be applied...
REM Sleep 6
REM del /F /Q %2\SoundLossFix.exe

rem Copy the image files based on the Current Display Configuration
%1\bin\getxml.exe "/s" "%1\config\fastlane3.1.xml" "//@Position" > %2\~349getxml.txt
set /p GetXML=<%2\~349getxml.txt
del /F /Q %2\~349getxml.txt

echo Detected Display Position %GetXML%

IF "0,0,1024,768" == "%GetXML%" (
	echo Detected 1024x768 - Copying config and image files
	xcopy /R /Y %1\Config\10x7\*.* %1\Config\
	xcopy /R /Y %1\image\10x7\*.* %1\image\
) ELSE (
	echo Detected 800x600 - Copying config and image files
	xcopy /R /Y %1\Config\8x6\*.* %1\Config\
	xcopy /R /Y %1\image\8x6\*.* %1\image\
)
Set GetXML=

echo Reregister the OPOS controls in C:\scot\controls
for %%i in (C:\SCOT\Controls\Opos*.ocx) do regsvr32 /s %%i
for %%i in (C:\SCOT\Controls\SCOT*.ocx) do regsvr32 /s %%i
regsvr32 /s C:\SCOT\Controls\NCRPOSKeyboard.ocx

echo Register Unicode version of Remote Console Manager
regsvr32 /s c:\SCOT\Controls\RemoteConsoleMgrU.ocx

rem Ensure the C:\SCOT\Util directory has been created on the machine; required for UpCon
IF NOT EXIST c:\scot\util (
	echo Creating the C:\SCOT\Util folder to be ready for UpCon
	md c:\SCOT\util
)
