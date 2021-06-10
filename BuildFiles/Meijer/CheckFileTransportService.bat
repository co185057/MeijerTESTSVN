@echo off
REM Detect File Transport Service and making sure that it stays running. 

sc query "File Transport Service"  > c:\scot\bin\FileTransportServiceStatus1.log 
REM get the STATE line 
findstr "STATE" c:\scot\bin\FileTransportServiceStatus1.log > c:\scot\data\fts.txt 
If %ERRORLEVEL% EQU 0 goto :GetStatus
goto :RegisterIt

:GetStatus
findstr "STOPPED" c:\scot\data\fts.txt
If %ERRORLEVEL% EQU 0 goto :StartIt
findstr "RUNNING" c:\scot\data\fts.txt
If %ERRORLEVEL% EQU 0 goto :eof

:StartIt
sc start "File Transport Service"
::delay for 3 secs
ping -n 3 127.0.0.1>NUL
sc query "File Transport Service"  > c:\scot\bin\FileTransportServiceStatus2.log 
goto :eof	
	
:RegisterIt
c:\scot\controls\FileTransportService.exe /service
net start "File Transport Service"
sc config "File Transport Service" start= auto
::delay for 2 secs
ping -n 2 127.0.0.1>NUL 
sc query "File Transport Service"  > c:\scot\bin\FileTransportServiceStatus2.log
goto :eof