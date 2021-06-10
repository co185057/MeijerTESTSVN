@echo ON

rem setlocal ENABLEEXTENSIONS enabledelayedexpansion
setlocal enabledelayedexpansion
set KEY_NAME="HKLM\SOFTWARE\NCR\SCOT\CurrentVersion\SCOTAPP\Controls\SmartScale"
set VALUE_NAME=DisableBagScale
set FILE_NAME=acsopts
set var=C
FOR /F "usebackq skip=4 tokens=1-3" %%A IN (`REG QUERY %KEY_NAME% /v %VALUE_NAME% 2^>nul`) DO (
    set ValueName=%%A
    set ValueType=%%B
    set ValueValue=%%C
)

set K_NAME="HKLM\SOFTWARE\NCR\SCOT\CurrentVersion\SCOTTB"
set V_NAME=TerminalNumber
FOR /F "usebackq skip=4 tokens=1-3" %%A IN (`REG QUERY %K_NAME% /v %V_NAME% 2^>nul`) DO (
    set VName=%%A
    set VType=%%B
    set TermNum=%%C
)

set TermNumFile=%FILE_NAME%.%TermNum%

if exist c:\scot\config\acsopts.%TermNum% (

c:\scot\bin\Ini2Reg.exe /ri TB EnableBagScale /f c:\scot\config\acsopts.%TermNum% > .\out.dat
@echo errorlevel acsopts.%TermNum% =!ERRORLEVEL! >> c:\scot\logs\enablebagscale.log
if !ERRORLEVEL!==0 (
FOR /F "tokens=1,2" %%a IN (.\out.dat) DO set var=%%a
)
if NOT !var! == Y ( 
@echo Not Y >> c:\scot\logs\enablebagscale.log
if NOT !var! == N  (
@echo Not N >> c:\scot\logs\enablebagscale.log
c:\scot\bin\Ini2Reg.exe /ri TB EnableBagScale /f c:\scot\config\acsopts.dat > .\out.dat
@echo errorlevel acsopt.dat = !ERRORLEVEL! >> c:\scot\logs\enablebagscale.log
if !ERRORLEVEL!==0 (
FOR /F "tokens=1,2" %%a IN (.\out.dat) DO set var=%%a
)
)
)
) else (
c:\scot\bin\Ini2Reg.exe /ri TB EnableBagScale /f c:\scot\config\acsopts.dat > .\out.dat
@echo errorlevel acsopt.dat no xxx =!ERRORLEVEL! >> c:\scot\logs\enablebagscale.log
if !ERRORLEVEL!==0 (
FOR /F "tokens=1,2" %%a IN (.\out.dat) DO set var=%%a
)
)

@echo EnableBagScale Option is set as:!var! >> c:\scot\logs\enablebagscale.log
if !var! EQU N (
 if %ValueValue% EQU 0x0 (
REM @echo registry needs to set!!!
REM @echo registry is being set.....
rem REGEDIT.EXE /S DisableScale.reg
c:\scot\bin\Config_No_BagScale.bat

if exist c:\scot\config\securitysmtab.xml.nobagscale (
REN c:\scot\config\securitysmtab.xml securitysmtab.xml.bagscaleenable
REN c:\scot\config\securitysmtab.xml.nobagscale securitysmtab.xml
)

@echo DisableScale registry applied. >> c:\scot\logs\enablebagscale.log
) ELSE (
@echo registry is already set!!! >> c:\scot\logs\enablebagscale.log
)
) ELSE (
if %ValueValue% EQU 0x1 (
@echo registry needs to set to enable bag scale. >> c:\scot\logs\enablebagscale.log
REGEDIT.EXE /S c:\scot\config\EnableBagScale.reg

if exist c:\scot\config\securitysmtab.xml.bagscaleenable (
REN c:\scot\config\securitysmtab.xml securitysmtab.xml.nobagscale
REN c:\scot\config\securitysmtab.xml.bagscaleenable securitysmtab.xml
)

) ELSE (
@echo registry to enable bag scale is already set >> c:\scot\logs\enablebagscale.log
)
)


