@echo off
set ERRORLEVEL=0

:: ===================================================================
:: MergeXML.bat
:: ===================================================================

if "%APP_DRIVE%"=="" set APP_DRIVE=C:
if "%DATA_DRIVE%"=="" set DATA_DRIVE=C:
if "%OS_DRIVE%"=="" set OS_DRIVE=C:
if "%TEMP_DRIVE%"=="" set TEMP_DRIVE=C:
if "%BOOT_DRIVE%"=="" set BOOT_DRIVE=C:

set TEMP=%TEMP_DRIVE%\Temp
set TMP=%TEMP_DRIVE%\Temp
if not exist %TEMP% mkdir %TEMP%
if not exist %TMP% mkdir %TMP%

set COPYCMD=/Y
set INSTALLDIR=%APP_DRIVE%\Scot\Install
set INSTALLTRACELOG=%INSTALLDIR%\MergeXML.log
set PICKLIST=%APP_DRIVE%\scot\report\data\PickList.xml
set PICKLISTMERGE=%APP_DRIVE%\scot\config\MeijerPickList_Merged.xml

call :getDateTime
attrib -r %APP_DRIVE%\scot\report\data\PickList.xml
echo *****************************************************************************************>> %INSTALLTRACELOG%
echo Begin Date/Time = %CDCT% >> %INSTALLTRACELOG%

set INPUTFILE=
set OUTPUTFILE=
rem -------------------------------------------------------------------------------------------
rem Handle RAP data
rem -------------------------------------------------------------------------------------------
echo MergeXML PickList for RAP >> %INSTALLTRACELOG%

echo calling MergeXML.exe -e %PICKLISTMERGE% %PICKLIST%  >> %INSTALLTRACELOG%
%APP_DRIVE%\scot\bin\mergexml.exe -e  %PICKLISTMERGE%  %PICKLIST%  >> %INSTALLTRACELOG%
echo MergeXML.exe returns %ERRORLEVEL% >> %INSTALLTRACELOG%


rem -------------------------------------------------------------------------------------------
:end
call :getDateTime
echo End Date/Time = %CDCT% >> %INSTALLTRACELOG%
goto :eof

rem -------------------------------------------------------------------------------------------
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



