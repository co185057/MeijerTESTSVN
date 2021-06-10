 

::---------------------------------------------------------------
rem Detect if the 7168 USB printer is connected 
::---------------------------------------------------------------
set TEMPDATA=%APP_DRIVE%\%APP_DIR%\logs\FindPrinter.log
set TEMPD=%APP_DRIVE%\%APP_DIR%\logs\FindPrinter.log
set SCOTBIN=%APP_DRIVE%\scot\bin
set VALUE=
%SCOTBIN%\Drivermgr.exe find @USB "USB\VID_0404&PID_0311" >%TEMPDATA% 2>>&1
REM POS204956 use findstr to handle new drivermgr
REM FOR /F "tokens=1" %%a IN (%TEMPDATA%) DO if not "%%a"=="1" set PORTDATA=%%a
findstr /C:"0 matching device" %TEMPDATA%
if %ERRORLEVEL% == 1 findstr /C:"not found" %TEMPDATA%
set PORTDATA=%ERRORLEVEL%
set ID="%PORTDATA%"
 
set ISACTIVE=1
if "%PORTDATA%"=="0" set ISACTIVE=0
if "%PORTDATA%"==Drivermgr.exe set ISACTIVE=0
if "%ISACTIVE%"=="0" goto :COM_Printer

if "%ISACTIVE%"=="1" goto :USB_Printer


rem -----------------------------------------------------------------
rem USB Printer
rem -----------------------------------------------------------------
:USB_Printer
echo Found 7168 USB PRinter, set the registry >>%TEMPDATA%
regedit /s %APP_DRIVE%\scot\config\USBPrinter.reg 
goto :eof

rem -----------------------------------------------------------------
rem COM Printer
rem -----------------------------------------------------------------
:COM_Printer
echo Found 7167 COM PRinter, set the registry >>%TEMPDATA%
regedit /s %APP_DRIVE%\scot\config\COMPrinter.reg 
goto :eof


