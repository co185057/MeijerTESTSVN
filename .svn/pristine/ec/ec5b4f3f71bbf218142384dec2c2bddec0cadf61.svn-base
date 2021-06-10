rem Implement any quick installation items to be run post install here

rem Param1 = %%AppDir%%
rem Param2 = %%TempDir%%
rem Param3 = %%AppResolution%%
rem Param4 = #CUSTOMERVER#
rem Param5 = %%SkipHHRAP%%

rem %2\SoundLossFix.exe
rem Wait for the SoundLossFix to be applied...
rem Sleep 6
rem del /F /Q %2\SoundLossFix.exe

rem Copy the image files based on the Current Display Configuration
rem If the last parameter is not passed - try to extract the value from XML


IF "%3" == "8X6" (
	echo Detected 800x600 - Copying config and image files
	xcopy /R /Y %1\Config\8x6\*.* %1\Config\
	xcopy /R /Y %1\image\8x6\*.* %1\image\	
) ELSE (
	echo Detected 1024x768 - Copying config and image files
	xcopy /R /Y %1\Config\10x7\*.* %1\Config\
	xcopy /R /Y %1\image\10x7\*.* %1\image\
)

echo Cleanup any old base files no longer being used
del /f /q %1\Config\Enable_Intervention\LaneRap.xml
del /f /q %1\Config\Disable_Intervention\LaneRap.xml
del /f /q %1\Config\Enable_Intervention\RapMsg0409.000
del /f /q %1\Config\Disable_Intervention\RapMsg0409.000
del /f /q %1\Config\Enable_Intervention\RapMsg0409.xml
del /f /q %1\Config\Disable_Intervention\RapMsg0409.xml
del /f /q %1\Config\LaunchPadMsg0409.000
del /f /q %1\Config\RapMsg0409.000


rem Ensure the C:\SCOT\Util directory has been created on the machine; required for UpCon
IF NOT EXIST c:\scot\util (
	echo Creating the C:\SCOT\Util folder to be ready for UpCon
	md c:\SCOT\util
)

rem turn on data capture for printer at RAP
regedit /s c:\scot\reg\ScotRecDataCap_ON.reg

rem POS52199: Remove PSC scanner setting because new Datalogic Wireless scanner is being used at RAP.
rem Enable the PSC scanner at the RAP (used across the Meijer chain for all stores now)
rem regedit /s c:\scot\reg\PSC_Scanner_Rap.reg

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

IF "%5" NEQ "1" (
	IF EXIST "c:\Program Files\Microsoft ActiveSync\WCESMgr.exe" (
		rem
		rem Executing HHRAP Setup
		rem
		pushd
		md %1\Install\HHRAP
		cd %1\Install\HHRAP
		call InstallHHRapPatchMeijer.bat %4
		popd
	) ELSE (
		echo ActiveSync not present. Skipping HHRAP installation. >> %INSTLOG%
	)
) ELSE (
	echo Command line parameter instructed to skip HHRAP install. >> %INSTLOG%
)

