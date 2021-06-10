set UPCONWORK=c:\scot\work
set UPCON_DIAGFILE=c:\scot\upcon\upcondiagfile.ini
set GETDIAGFILES=c:\scot\upcon\GetDiagFilesU.exe
set DIAG_DIR=c:\temp\fastlane
if not exist %DIAG_DIR% md %DIAG_DIR%
set TRACEUPCONLOGS=%UPCONWORK%\getupconlogs.log
rem The expected %TEMP% value is TEMP=C:\Temp for tokens processing

if "%1"=="/Success" goto :eof
if "%1"=="/Failure" call :getUpconGDFs
 
goto :eof

:GetUpconGDFs
  call :log +GetUpconGDFs
  set GetUpconGDFs_ERRORLEVEL=0

  call %GETDIAGFILES% -f %UPCON_DIAGFILE% > %TEMP%\.UpconZip 2>>%TRACEUPCONLOGS%
  set GetUpconGDFs_ERRORLEVEL=%ERRORLEVEL%
  if exist %TEMP%\.UpconZip type %TEMP%\.UpconZip >>%TRACEUPCONLOGS%
  if NOT "%GetUpconGDFs_ERRORLEVEL%"=="0" call :log fail to get upcon log files & goto :eof

  FOR /F "tokens=2" %%a IN (%TEMP%\.UpconZip) DO set UPCONGDFOUTPUT=%%a
  FOR /F "tokens=6 delims=\" %%a IN ('echo %UPCONGDFOUTPUT%') DO set UPCONZipName=%%a
  ren %UPCONGDFOUTPUT% Upcon-%UPCONZipName% 2>>%TRACEUPCONLOGS%
  move %TEMP%\Upcon-%UPCONZipName% %DIAG_DIR% 2>>%TRACEUPCONLOGS%
  set GetUpconGDFs_ERRORLEVEL=%ERRORLEVEL%
  if "%GetUpconGDFs_ERRORLEVEL%"=="0" call :log %DIAG_DIR%\Upcon-%UPCONZipName% generated on Upcon failure

  call :log -GetUpconGDFs
  goto :eof

:log
	call :GUDT
	echo %CDCT% %* >> %TRACEUPCONLOGS%
	goto :eof

:GUDT
  echo off
	set CDCT=
	for /f "tokens=*" %%i in ('date /t') do set t=%%i
	set SEP=%t:~4,1%
	echo %SEP%
	set TID=2
	if "%SEP%"=="/" set TID=1
	for /f "tokens=%TID%" %%i in ('date /t') do set date1=%%i
	set CD=%date1%

	for /f "tokens=1" %%i in ('time /t') do set time1=%%i 

	set CT=%time1% 
	set CDCT=%CD: =% %CT: =%
  	echo on
	goto :eof

