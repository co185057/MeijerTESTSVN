call :WriteDateTimeToHistoryLog 
ECHO Checking ACS FOLDER !!! >> C:\scot\install\ACSRestore.log
if NOT exist "C:\ACS" (
		ECHO Restoring ACS FOLDER !!! >> C:\scot\install\ACSRestore.log
		XCOPY c:\SCOT\install\backup\ACS\bin\*acsapldr.* c:\ACS\bin /I /R /V /Y  >> C:\scot\install\ACSRestore.log
		
		start c:\acs\bin\acsapldr.exe -c -g posi_fastlane.apl -l c:\acs\Logs\aploader.log -s -x
		call :WriteDateTimeToHistoryLog 
) ELSE if NOT exist "C:\ACS\bin\acsapldr.exe" (
		ECHO Restoring ACSAPLDR Application !!! >> C:\scot\install\ACSRestore.log
		XCOPY c:\SCOT\install\backup\ACS\bin\*acsapldr.* c:\ACS\bin /I /R /V /Y  >> C:\scot\install\ACSRestore.log

		start c:\acs\bin\acsapldr.exe -c -g posi_fastlane.apl -l c:\acs\Logs\aploader.log -s -x
		call :WriteDateTimeToHistoryLog 
)	


:: ======================================================================
:WriteDateTimeToHistoryLog
:: ======================================================================
  set CDCT=
  for /f "tokens=*" %%i in ('date /t') do set date1=%%i 
  set CD=%date1%
  for /f "tokens=*" %%i in ('time /t') do set time1=%%i 
  set CT=%time1% 
  set CDCT=Date/Time = %CD%%CT: =%

  echo %CDCT% >> C:\scot\install\ACSRestore.log
goto :EOF

	