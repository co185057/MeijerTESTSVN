rem Implement any quick installation items to be run post install here

rem Param1 = %%AppDir%%
rem Param2 = %%TempDir%%
rem Param2 = %%AppResolution%%

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

rem Ensure the C:\SCOT\Util directory has been created on the machine; required for UpCon
IF NOT EXIST c:\scot\util (
	echo Creating the C:\SCOT\Util folder to be ready for UpCon
	md c:\SCOT\util
)