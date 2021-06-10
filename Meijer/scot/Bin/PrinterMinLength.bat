@echo off
REM POS9979 Update the printer MinRecLength to be 7 for 4 inch
set HISTORYLOG=C:\scot\install\PrinterSetting.log
SET PrinterType=
C:\scot\bin\ini2reg.exe /rr "SOFTWARE\OleForRetail\ServiceOPOS\POSPrinter" ScotRec /f %APP_DRIVE%\scot\bin\config.ini > %TEMP%\.PrinterType
FOR /F "tokens=1,2" %%a IN (%TEMP%\.PrinterType) DO SET PrinterType=%%a
	del /q %TEMP%\.PrinterType

	IF NOT "%PrinterType%"=="" (
		echo Modify MinRecLength to 7 for PrinterType=%PrinterType% >> %HISTORYLOG%
		reg add "HKLM\SOFTWARE\OleForRetail\ServiceOPOS\POSPrinter\%PrinterType%" /v "MinRecLength" /t REG_SZ /d "7 ->0-None,1-6\",2-6.5\",3-7\",4-7.5\",5-8\",6-5\",7-4\",8-3\",9-2\",10-1\"" /f 
	 	reg query "HKLM\SOFTWARE\OLEForRetail\ServiceOPOS\POSPrinter\%PrinterType%" /v MinRecLength >> %HISTORYLOG%
    	)

REM POS9979 Update the printer MinRecLength to be 7 for 4 inch