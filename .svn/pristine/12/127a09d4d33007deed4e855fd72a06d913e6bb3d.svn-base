@echo off

goto :begin
 
FastLane Monitor Collection Tool
 
091903 1.0.0
091903 1.0.1 Added STFCrunch.exe
092203 1.0.2 Forcing TEMP/TMP to be c:\TEMP
092203 1.0.3 Updated STFCrunch.exe
092203 1.0.4 Updated STFCrunch.exe
101403 1.0.5 Added Attrib -r for diagfile.ini
102003 1.0.6 Copy the log to c:\temp during uninstall
120103 1.0.7 Copy GDF to c:\temp\fastlane (intead of c:\temp) if the dir exits.
120103 1.0.8 Uninstall, remove temp\fastlane files
041904 1.0.9 Added Transport Component and TerminalInfo.dat
071105 1.1.10 Add support for 5 partition environment variables.
042006 1.1.11 Fix 5 partition issues
110209 1.1.15 Added enhanced reporting with /e option to produce LaneDNA.csv file; /s to collect
              summary data and /w to collect windows path. RFC419614
111909 1.1.16 Corrected 4-year-old copy problem with the weight mismatch bak file.
              Changed echo to installhistory to reflect %version% instead of hardcoded v.....
120109 1.1.17 RFC422300 Remove FirmwareUpdateReports.txt from the collection
010210 1.1.18 TAR423386 Fix a performance issue with VS_FTP.exe
012510 1.1.19 RFC422412 Clearing scotapp.stf.bak file after collecting it.
020210 2.0.1  RFC424361 Modified install/uninstall processes to log better.
031610 2.0.2  TAR429552 Modified Uninstall.bat to remove extra echo that might have caused an uninstall error
040810 2.0.3  TAR431419 New VS_Ftp with performance fixes.
050310 2.0.4  TAR433398 STFCrunch was failing. We need to collect the stf files even if it they aren't crunched.
                        We also want to delete the scotapp.bak from both logs and bin to reduce .zip file size.
              TAR433248 TransportConfig.ini (and all monitor dir files) were begin deleted in each install
              TAR433402 Zip file name was accidently changed from MMDDYY to MMDDYYYY when logging was enhanced.
                        This tar changed it back.
              TAR431955 MonitorSummary.dat produced as Unicode by TerminalInfoU -s needs to be converted to ANSI
                        for filever to consume it correctly.
050610 2.0.5  TAR431955 Minor adjustment to ensure this redirect works on all systems. Removed the >nul redirect.
                        Also modified the .stf copy/crunch logic to ensure that files under scot\logs are copied
                        as they should be.
051110 2.0.6  RFC428898 Allow retries for vs_ftp sendfile processing and delete any monitor zip files older than 30 days
              TAR433398 Further tweaking of the STFCrunch process, resulted in success running this as a scheduled task.
051910 2.0.7  TAR434979 Corrected missing label in MonitorTransport.bat
030811 2.0.8  RFC458809 To comply with PCI requirements, we're removing the runonce entry that calls the
                        schedulemonitor.bat file and we're also removing the schedulemonitor.bat file, since it is not
                        necessary to create a schedule entry at install. We're also changing the frequency to once
                        a week, on Monday, at installation, to obviate the need to change the schedule after install.
                        Any code that was here to support the ability to change the schedule after install, has been
                        removed. Any unused files are now being removed through the monitor install.
                        We also removed all tabs from this batch file. Please be careful to NOT re-insert tabs.
040411 2.0.9 TAR456782  VS_FTP needed better logging, and better error checking to reveal transmission errors and
                        and errors related to validating that the transmission succeeded. New VS_FTP version is 1.0.11.

:begin

set Version=2.0.9
echo.
echo Monitor v. %Version%
echo.

rem Set Environment Variables
if "%APP_DRIVE%"==""  set APP_DRIVE=C:
if "%DATA_DRIVE%"=="" set DATA_DRIVE=C:
if "%OS_DRIVE%"==""   set OS_DRIVE=C:
if "%TEMP_DRIVE%"=="" set TEMP_DRIVE=C:
if "%BOOT_DRIVE%"=="" set BOOT_DRIVE=C:
set TEMP=%TEMP_DRIVE%\TEMP
set TMP=%TEMP_DRIVE%\TEMP
if not exist %TEMP% mkdir %TEMP%
if not exist %TMP% mkdir %TMP%

set Action=
set MONITOR=%APP_DRIVE%\scot\monitor
set ZIPDIR=%TEMP_DRIVE%\temp\monitor
if not exist %MONITOR% md %MONITOR%
if not exist %MONITOR% goto :ErrorCreatingDir %MONITOR%
set LOG=%DATA_DRIVE%\scot\logs\MONITOR.log

set SCHEDULE_TIME_E1=/every:m
set SCHEDULE_TIME_E2=/every:w,su
set SCHEDULE_TIME_E3=/every:m,w,f
set SCHEDULE_TIME_E7=/every:m,t,w,th,f,s,su 

set LOGRetention=30
set FILELOCATION=%TEMP_DRIVE%\Temp
set ALTFILELOCATION=%TEMP_DRIVE%\Temp\fastlane

if exist %APP_DRIVE%\scot\monitor attrib -r %APP_DRIVE%\scot\monitor\*.* > nul 2>&1
if exist %TEMP_DRIVE%\temp\monitor attrib -r %TEMP_DRIVE%\temp\monitor\*.* > nul 2>&1

rem Default Time Period Count
set T_DEFAULT=10:00
set P_DEFAULT=3

if "%1"=="install"    set Action=install
if "%1"=="uninstall"  set Action=uninstall
if "%1"=="run"        set Action=run
if "%Action%"==""     goto :usage

rem Clear Args Vars
set debug=N
set T=
set P=
set C=

shift 

:read_args

set Arg1=
set Arg1=%1
if not "%Arg1%"=="" set Arg1=%Arg1: =%
set Arg2=
set Arg2=%2
if not "%Arg2%"=="" set Arg2=%Arg2: =%

if "%Arg1%"=="" goto :read_args_done

rem Time
if "%Arg1%"=="time" set T=%Arg2% & shift
rem Period
if "%Arg1%"=="period" set P=%Arg2% & shift
rem Count
if "%Arg1%"=="count" set C=%Arg2% & shift
rem debug Mode
if "%Arg1%"=="debug" set debug=Y

shift

goto :read_args
:read_args_done

rem odd case if the value is missing
if "%T%"==""  set T=%T_DEFAULT%
if "%T%"==" " set T=%T_DEFAULT%
if "%P%"==""  set P=%P_DEFAULT%
if "%P%"==" " set P=%P_DEFAULT%

rem leave this rem below for easy debugging of Actions/Times (by removing the rem)
rem echo Action=%Action% Time=%T% Period=%P%

if "%debug%"=="Y" echo on

call :log
call :log ******************************************************************************
call :log MONITOR %*
call :log ******************************************************************************

set T=%T: =%
set P=%P: =%

rem Check for good P
set Pok=0
if "%P%"=="1"   set Pok=1
if "%P%"=="2"   set Pok=1
if "%P%"=="3"   set Pok=1
if "%P%"=="7"   set Pok=1
if "%Pok%"=="0" set Action=0

set SCHEDULE_TIME=
if "%P%"=="1" set SCHEDULE_TIME=%T% %SCHEDULE_TIME_E1%
if "%P%"=="2" set SCHEDULE_TIME=%T% %SCHEDULE_TIME_E2%
if "%P%"=="3" set SCHEDULE_TIME=%T% %SCHEDULE_TIME_E3%
if "%P%"=="7" set SCHEDULE_TIME=%T% %SCHEDULE_TIME_E7%

if "%Action%"=="install"    goto :Install
if "%Action%"=="uninstall"  goto :UnInstall
if "%Action%"=="run"        goto :Run

:usage
echo.
echo Usage:
echo monitor.bat install 
echo monitor.bat uninstall (remove scheduler job and files)
echo monitor.bat run (collect files to c:\temp)
echo monitor.bat help
echo.

goto :eof


rem ***************************************************************************
rem                                   Install
rem ***************************************************************************

:install

    rem We should only attempt to write to log files on a DATA_DRIVE.
    set HISTORYLOG=%DATA_DRIVE%\scot\install\InstallHistory.log
    set HISTORYTRACE=%DATA_DRIVE%\scot\install\InstallTrace.log

    call :getdatetime
    echo %DT% ********************************************************* >> %HISTORYLOG%
    echo %DT% ********************************************************* >> %HISTORYTRACE%

    echo %DT% Installing Monitor Tool %Version% >> %HISTORYLOG%
    echo %DT% Installing Monitor Tool %Version% >> %HISTORYTRACE%
    echo.
    echo Installing Monitor, Please wait...
    echo.

    rem TAR433248
    rem Don't delete the directory if it exists. It might have customer modified files in it.
    rem if exist %MONITOR% del /f /q %MONITOR%\*.*
    if not exist %MONITOR% mkdir %MONITOR%
    if not exist %MONITOR% goto :ErrorCreatingDir %MONITOR%

    rem +RFC458809
    rem Clean up files we no longer use 
    echo Cleaning up old files
    call :log Cleaning up old files

    if exist %TEMP_DRIVE%\Temp\scheduler.reg del /f %TEMP_DRIVE%\Temp\scheduler.reg >> %LOG% 2>&1 
    if exist %TEMP_DRIVE%\Temp\uninstall.bat del /f %TEMP_DRIVE%\Temp\uninstall.bat >> %LOG% 2>&1  

    if exist %MONITOR%\schedulemonitor.bat del /f %MONITOR%\schedulemonitor.bat >> %LOG% 2>&1
    if exist %DATA_DRIVE%\scot\logs\schedulemonitor.log del /f %DATA_DRIVE%\scot\logs\schedulemonitor.log >> %LOG% 2>&1
    
    if exist %MONITOR%\SCOTUtil.dll del /f %MONITOR%\SCOTUtil.dll >> %LOG% 2>&1
    if exist %MONITOR%\zip32.dll del /f %MONITOR%\zip32.dll >> %LOG% 2>&1
    if exist %MONITOR%\CryptSvc.reg del /f %MONITOR%\CryptSvc.reg >> %LOG% 2>&1
    if exist %MONITOR%\mergeini.exe del /f %MONITOR%\mergeini.exe >> %LOG% 2>&1
    if exist %MONITOR%\GetDT.exe del /f %MONITOR%\GetDT.exe >> %LOG% 2>&1
    if exist %MONITOR%\GetDiagFiles.exe del /f %MONITOR%\GetDiagFiles.exe >> %LOG% 2>&1
    if exist %MONITOR%\.count.current del /f %MONITOR%\.count.current >> %LOG% 2>&1
    if exist %MONITOR%\.count.max del /f %MONITOR%\.count.max >> %LOG% 2>&1
    rem -RFC458809
    
    call :log Begin %Version% Install

    rem Copy Files
    call :log Copy files to %MONITOR%
    copy %ZIPDIR%\*.exe %MONITOR%
    copy %ZIPDIR%\*.dll %MONITOR%
    copy %ZIPDIR%\*.bat %MONITOR%
    copy %ZIPDIR%\*.reg %MONITOR%
    copy %ZIPDIR%\*.ini %MONITOR%
    copy %ZIPDIR%\*.ocx %MONITOR%
    copy %MONITOR%\msinet.ocx %APP_DRIVE%\scot\controls
    copy %MONITOR%\SMTPX.DLL %APP_DRIVE%\scot\controls
    copy %MONITOR%\encodex.DLL %APP_DRIVE%\scot\controls
    copy %MONITOR%\Mabryobj.DLL %APP_DRIVE%\scot\controls
    copy %MONITOR%\mimex.DLL %APP_DRIVE%\scot\controls
    call :log Copied files to %MONITOR%

    rem register SMTP component
    if exist %APP_DRIVE%\scot\controls\SMTPX.DLL regsvr32 /s %APP_DRIVE%\scot\controls\SMTPX.DLL
    if exist %APP_DRIVE%\scot\controls\encodex.DLL regsvr32 /s %APP_DRIVE%\scot\controls\encodex.DLL
    if exist %APP_DRIVE%\scot\controls\mabryobj.DLL regsvr32 /s %APP_DRIVE%\scot\controls\mabryobj.DLL
    if exist %APP_DRIVE%\scot\controls\mimex.DLL regsvr32 /s %APP_DRIVE%\scot\controls\mimex.DLL
    if exist %APP_DRIVE%\scot\controls\msinet.ocx regsvr32 /s %APP_DRIVE%\scot\controls\msinet.ocx

   
 :SetUpSchedule
    rem
    rem Scheduler Configuration for XP and NT
    rem
    echo %DT% Setting Task Scheduler services to automatic >> %HISTORYTRACE%
    call :log Seting Task Scheduler to automatic
    regedit /s %MONITOR%\NTScheduler.On.reg

    rem Clean Earlier Install
    call :log Call :remove_jobs
    call :remove_jobs
    call :log :remove_jobs done

    rem Add Task to the schedule
    rem Note, Scheduler service must be running for this to work.

    rem +RFC458809
    
    call :log Call "AT" command to schedule the task to run monitor.
    
    set MONSCH=at %SCHEDULE_TIME% \"%WINDIR%\system32\cmd.exe /c %APP_DRIVE%\scot\monitor\run.bat\"

    call :log Calling %MONSCH%
    call %MONSCH% >nul 2>&1
    
    rem -RFC458809
    
    set error=%errorlevel%
 
    call :log Return code %error% 
    if not "%error%"=="0" goto :ErrorSetNTScheduler

    rem Update Scotopts.dat
    call :log Updating Scotopts.dat
    set OPT=%APP_DRIVE%\scot\config\scotopts.dat
    if exist %OPT% attrib -r %OPT%
    echo [Operations]>%TEMP_DRIVE%\Temp\.dat
    echo LogWLDBmismatch=Y>>%TEMP_DRIVE%\Temp\.dat
    echo WLDBmismatchMaxFileSize=500>>%TEMP_DRIVE%\Temp\.dat
    if exist %OPT% %APP_DRIVE%\scot\bin\mergeini.exe %TEMP_DRIVE%\Temp\.dat %OPT%
    call :log Update Scotopts.dat Done

    rem Clean up temp files used for scheduling.
    if exist %TEMP_DRIVE%\Temp\.at del /f /q %TEMP_DRIVE%\Temp\.at >> %LOG
    if exist %TEMP_DRIVE%\Temp\.dat del /f /q %TEMP_DRIVE%\Temp\.dat >> %LOG
    if exist %TEMP_DRIVE%\Temp\.job del /f /q %TEMP_DRIVE%\Temp\.job >> %LOG
    
    call :log Install done
   
    echo.
    at
    echo.
    echo Install Done (5s pause)
    echo.
    %MONITOR%\sleep 5
        
    call :getdatetime
    echo %DT% Installation details are located in %DATA_DRIVE%\scot\logs\monitor.log >> %HISTORYTRACE%

    echo %DT% Done>>%HISTORYLOG%
    echo %DT% Done>>%HISTORYTRACE%
    
    call :log Removing %ZIPDIR% directory after install...   
    rem Go up one level, so we can delete where we are
    cd ..
    rmdir /S /Q %ZIPDIR%  
    
    rem ***************** Block Ends *****************************

    goto :eof

rem ***************************************************************************
rem                                   UnInstall
rem ***************************************************************************

:uninstall

    echo Uninstalling Monitor Collector
    echo.
    rem We should only attempt to write to log files on a DATA_DRIVE.
    set HISTORYLOG=%DATA_DRIVE%\scot\install\InstallHistory.log
    set HISTORYTRACE=%DATA_DRIVE%\scot\install\InstallTrace.log
    
    call :getdatetime
    echo %DT% ********************************************************* >> %HISTORYLOG%
    echo %DT% ********************************************************* >> %HISTORYTRACE%

    call :log Begin Uninstall
    echo %DT% UnInstalling Monitor Tool %Version% >> %HISTORYLOG%
    echo %DT% UnInstalling Monitor Tool %Version% >> %HISTORYTRACE% 

    rem Remove Weight MissMatch Capture
    set OPT=%APP_DRIVE%\scot\config\scotopts.dat
    attrib -r %OPT%
    echo [Operations]>%TEMP_DRIVE%\Temp\.dat
    echo LogWLDBmismatch=N>>%TEMP_DRIVE%\Temp\.dat
    echo WLDBmismatchMaxFileSize=5000>>%TEMP_DRIVE%\Temp\.dat
    %APP_DRIVE%\scot\bin\mergeini.exe %TEMP_DRIVE%\Temp\.dat %OPT%

    rem Remove Jobs
    call :remove_jobs

    rem move uninstall.bat to %temp% so that we can delete it from monitor too.
    rem otherwise, it will be open when we attempt to delete the files
    rem under scot\monitor

    copy %MONITOR%\uninstall.bat %TEMP% >nul 2>&1
    call :log Called %TEMP%\uninstall.bat

    start %TEMP%\uninstall.bat
    echo Done
    rem Do not add any actions after this. They will not execute correctly, since
    rem Monitor.bat has been deleted at this point.
    goto :eof

:remove_jobs
    rem Only Remove Monitor Jobs
    call :log remove_jobs begin
    AT >> %LOG%
    if exist %TEMP_DRIVE%\Temp\.at del /f %TEMP_DRIVE%\Temp\.at
    AT > %TEMP_DRIVE%\Temp\.at
    for /f "tokens=1,* delims= " %%i in (%TEMP_DRIVE%\Temp\.at) do call :remove_job %%i %%j
    AT >> %LOG%
    call :log remove_jobs end
    goto :eof

:remove_job
    rem remove the job if "monitor\run.bat" is found, but use 2 backslashes to look.
    if "%1"=="" goto :eof
    call :log Job Found %*
    if exist %TEMP_DRIVE%\Temp\.job del /f %TEMP_DRIVE%\Temp\.job
    echo %* > %TEMP_DRIVE%\Temp\.job
    if not exist %TEMP_DRIVE%\Temp\.job goto :eof
    findstr /R /I /C:"monitor\\run.bat" %TEMP_DRIVE%\Temp\.job  >>%LOG% 2>&1
    set error=%errorlevel%
    if not "%error%"=="0" goto :eof
    at %1 /delete /yes  >>%LOG% 2>&1
    set error=%errorlevel%
    if "%error%"=="0" call :log Job Removed %*
    if "%1"=="Error" at %2 /delete /yes  >>%LOG% 2>&1
    set error=%errorlevel%
    if "%error%"=="0" call :log Job Removed %*
    goto :eof

rem ***************************************************************************
rem                           MONITOR (collect Files)
rem ***************************************************************************

:Run

    rem Set Collect Files
    rem STF and Weight MisMatch Files

    :run_do

    call :log Monitor %Version% begins
    rem Copy STF to the Monitor dir before we Crunch it, then Crunch/Compress the .stf file using STFcrunch
    
    rem +TAR433398
    rem added logging to this processing so that we can discover why STFCrunch is failing.
    rem Also prevented looking for the stf files under scot\logs, if we find it under scot\bin
    call :log Look for scotapp.stf under scot\bin  

    rem There are still cases in which the stf files are being left behind. We'll delete them before we continue
    del /f /q %MONITOR%\scotapp.*  >> %LOG% 2>&1

    if exist %APP_DRIVE%\scot\bin\scotapp.stf  copy  /Y %APP_DRIVE%\scot\bin\scotapp.stf %MONITOR%\scotapp.stf.1  >> %LOG% 2>&1 
    set error=%errorlevel%
    call :log copy scotapp.stf error level is %error%
    if %error%==0 call :CrunchSTF
    
    if exist %APP_DRIVE%\scot\bin\scotapp.stf.bak  copy /Y %APP_DRIVE%\scot\bin\scotapp.stf.bak %MONITOR%\scotapp.bak.1  >> %LOG% 2>&1 
    set error=%errorlevel%
    call :log copy scotapp.stf.bak error level is %error%
    if %error%==0 call :CrunchSTFBak    
    if "%FoundSTF%"=="Y" goto :WeightMM

    call :log Look for scotapp.stf under scot\logs
    
    if exist %DATA_DRIVE%\scot\logs\scotapp.stf copy /Y %DATA_DRIVE%\scot\logs\scotapp.stf %MONITOR%\scotapp.stf.1   >> %LOG% 2>&1 
    set error=%errorlevel%
    call :log copy scotapp.stf error level is %error%
    if %error%==0 call :CrunchSTF

    if exist %DATA_DRIVE%\scot\logs\scotapp.stf.bak copy /Y %DATA_DRIVE%\scot\logs\scotapp.stf.bak %MONITOR%\scotapp.bak.1 >> %LOG% 2>&1
    set error=%errorlevel%
    call :log copy scotapp.stf.bak error level is %error%
    if %error%==0 call :CrunchSTFBak

    goto :WeightMM

:CrunchSTF
    call %MONITOR%\STFcrunch /c %MONITOR%\scotapp.stf.1  %MONITOR%\scotapp.stf  >> %LOG% 2>&1
    set error=%errorlevel%
    rem If STFCrunch fails, then rename the .1 to .stf so that we have something to put into the zip file.
    call :log STFCrunch error level is %error%
    if not %error%==0 ren %MONITOR%\scotapp.stf.1  scotapp.stf  >> %LOG% 2>&1
    if %error%==0 set FoundSTF=Y
    goto :eof

:CrunchSTFBak
    call %MONITOR%\STFcrunch /c %MONITOR%\scotapp.bak.1  %MONITOR%\scotapp.stf.bak         >> %LOG% 2>&1
    set error=%errorlevel%
    rem If STFCrunch fails, then rename the bak.1 to stf.bak so that we have something to put into the zip file.
    call :log STFCrunch error level is %error%
    if not %error%==0 ren %MONITOR%\scotapp.bak.1  scotapp.stf.bak  >> %LOG% 2>&1
    if %error%==0 set FoundSTF=Y
    goto :eof

rem Copy Weight MisMatches to the local dir
:WeightMM
    rem -TAR433398 ends here.
    if exist %MONITOR%\%COMPUTERNAME%.log del /f %MONITOR%\%COMPUTERNAME%.log > nul 2>&1
    if exist %MONITOR%\%COMPUTERNAME%.log.bak del /f %MONITOR%\%COMPUTERNAME%.log.bak > nul 2>&1
    if exist c:\scot\bin\%COMPUTERNAME%.log copy c:\scot\bin\%COMPUTERNAME%.log %MONITOR% > nul 2>&1
    if exist c:\scot\bin\%COMPUTERNAME%.log.bak copy c:\scot\bin\%COMPUTERNAME%.log.bak %MONITOR% > nul 2>&1
    if exist %APP_DRIVE%\scot\bin\%COMPUTERNAME%.log copy %APP_DRIVE%\scot\bin\%COMPUTERNAME%.log %MONITOR% > nul 2>&1
    if exist %APP_DRIVE%\scot\bin\%COMPUTERNAME%.log.bak copy %APP_DRIVE%\scot\bin\%COMPUTERNAME%.log.bak %MONITOR% > nul 2>&1
    if exist %DATA_DRIVE%\scot\logs\%COMPUTERNAME%.log copy %DATA_DRIVE%\scot\logs\%COMPUTERNAME%.log %MONITOR% > nul 2>&1
    if exist %DATA_DRIVE%\scot\logs\%COMPUTERNAME%.log.bak copy %DATA_DRIVE%\scot\logs\%COMPUTERNAME%.log.bak %MONITOR% > nul 2>&1

    rem Copy TerminalInfo.dat to the local dir

    if exist %MONITOR%\terminalinfo.dat del /f %MONITOR%\terminalinfo.dat > nul 2>&1
    if exist c:\scot\config\terminalinfo.dat copy c:\scot\config\terminalinfo.dat %MONITOR% > nul 2>&1
    if exist %APP_DRIVE%\scot\config\terminalinfo.dat copy %APP_DRIVE%\scot\config\terminalinfo.dat %MONITOR% > nul 2>&1
    if exist c:\scot\logs\terminalinfo.dat copy c:\scot\logs\terminalinfo.dat %MONITOR% > nul 2>&1
    if exist %DATA_DRIVE%\scot\logs\terminalinfo.dat copy %DATA_DRIVE%\scot\logs\terminalinfo.dat %MONITOR% > nul 2>&1

    rem Copy SummaryInfo.dat to the local dir

    if exist %MONITOR%\SummaryInfo.dat del /f %MONITOR%\SummaryInfo.dat > nul 2>&1
    if exist c:\scot\logs\SummaryInfo.dat copy c:\scot\logs\SummaryInfo.dat %MONITOR% > nul 2>&1
    if exist %DATA_DRIVE%\scot\logs\SummaryInfo.dat copy %DATA_DRIVE%\scot\logs\SummaryInfo.dat %MONITOR% > nul 2>&1

    rem TAR431955
    rem Convert MonitorSummary.dat file to Ansi
    call :log Converting MonitorSummary, if needed.
    if exist %MONITOR%\MonitorSummary.dat  type %MONITOR%\MonitorSummary.dat  > %MONITOR%\MonitorSummary1.dat
    if exist %MONITOR%\MonitorSummary1.dat copy %MONITOR%\MonitorSummary1.dat %MONITOR%\MonitorSummary.dat >>%LOG% 2>&1
        
    rem MONITOR DiagFile.ini

    set MONITORDiagFile=%MONITOR%\MONITORDiagfile.ini
    if exist %MONITORDiagFile% del /f %MONITORDiagFile%

    echo ; This comment line required if file converted to UNICODE.  Do not delete this line!>>%MONITORDiagFile%

    echo [Delete old files]>>%MONITORDiagFile%
    echo Type=DelOldFiles>>%MONITORDiagFile%
    echo Clear=Yes>>%MONITORDiagFile%
    echo Name=*Monitor*.zip>>%MONITORDiagFile%
    echo PathDir=%TEMP_DRIVE%\Temp>>%MONITORDiagFile%
    echo Days=%LOGRetention%>>%MONITORDiagFile%

    echo [MONITOR0]>>%MONITORDiagFile%
    echo Type=File>>%MONITORDiagFile%
    echo Name=%DATA_DRIVE%\scot\logs\monitor.log>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=No>>%MONITORDiagFile%

    echo [MONITOR1]>>%MONITORDiagFile%
    echo Type=File>>%MONITORDiagFile%
    echo Name=%MONITOR%\scotapp.stf>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    rem  RFC422412 - Clear the stf file from monitor directory
    echo Clear=Yes>>%MONITORDiagFile%

    echo [MONITOR2]>>%MONITORDiagFile%
    echo Type=File>>%MONITORDiagFile%
    echo Name=%MONITOR%\scotapp.stf.bak>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    rem  RFC422412 - Clear the stf.bak file from monitor directory
    echo Clear=Yes>>%MONITORDiagFile%

    rem  RFC422412 - Clear the stf.bak file from bin directory, but don't copy it
    echo [MONITORSTF]>>%MONITORDiagFile%
    echo Type=File>>%MONITORDiagFile%
    echo Name=%DATA_DRIVE%\scot\bin\scotapp.stf.bak>>%MONITORDiagFile%
    echo Copy=No>>%MONITORDiagFile%
    echo Clear=Yes>>%MONITORDiagFile%

    rem  RFC422412 - Clear the stf.bak file from logs directory, but don't copy it
    echo [MONITORSTF1]>>%MONITORDiagFile%
    echo Type=File>>%MONITORDiagFile%
    echo Name=%DATA_DRIVE%\scot\logs\scotapp.stf.bak>>%MONITORDiagFile%
    echo Copy=No>>%MONITORDiagFile%
    echo Clear=Yes>>%MONITORDiagFile%

    echo [MONITOR3]>>%MONITORDiagFile%
    echo Type=File>>%MONITORDiagFile%
    echo Name=%MONITOR%\%COMPUTERNAME%.log>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=No>>%MONITORDiagFile%

    echo [MONITOR4]>>%MONITORDiagFile%
    echo Type=File>>%MONITORDiagFile%
    echo Name=%MONITOR%\%COMPUTERNAME%.log.bak>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=No>>%MONITORDiagFile%

    echo [MONITOR5]>>%MONITORDiagFile%
    echo Type=File>>%MONITORDiagFile%
    echo Name=%MONITOR%\TerminalInfo.dat>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=No>>%MONITORDiagFile%

    echo [MONITOR6]>>%MONITORDiagFile%
    echo Type=File>>%MONITORDiagFile%
    echo Name=%MONITOR%\SummaryInfo.dat>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=No>>%MONITORDiagFile%

    echo [Platform Reg Key Dump]>>%MONITORDiagFile%
    echo Type=RegDump>>%MONITORDiagFile%
    echo Name=HKLM\SOFTWARE\NCR\SCOT - Platform>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=Yes>>%MONITORDiagFile%
    echo OutputFile=NCRPlatformRegDump.txt>>%MONITORDiagFile%

    echo [ADD Reg Key Dump]>>%MONITORDiagFile%
    echo Type=RegDump>>%MONITORDiagFile%
    echo Name=HKLM\SOFTWARE\NCR\ADD>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=Yes>>%MONITORDiagFile%
    echo OutputFile=NCRADDRegDump.txt>>%MONITORDiagFile%

    echo [Tally Reg Key Dump]>>%MONITORDiagFile%
    echo Type=RegDump>>%MONITORDiagFile%
    echo Name=HKLM\SOFTWARE\NCR\NCR Store Minder Client\Tally>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=Yes>>%MONITORDiagFile%
    echo OutputFile=NCRTallyRegDump.txt>>%MONITORDiagFile%  

    rem Adding new files and applications for RFC419614
    rem Filever will create a new output file, LaneDNA.csv
    echo [SCOT FileVer Info]>>%MONITORDiagFile%
    echo Type=Application>>%MONITORDiagFile%
    echo Name=%APP_DRIVE%\scot\bin\fileVer.exe>>%MONITORDiagFile%
    echo Copy=No>>%MONITORDiagFile%
    echo Clear=No>>%MONITORDiagFile%
    echo Arguments=/p %APP_DRIVE%\scot\config\FileVer.ini>>%MONITORDiagFile%

    rem Add LaneDNA.csv to the DiagFile.ini  
    echo [LANEDNA]>>%MONITORDiagFile%
    echo Type=File>>%MONITORDiagFile%
    echo Name=%DATA_DRIVE%\scot\monitor\LaneDNA.csv>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=No>>%MONITORDiagFile%

    rem Another requirement of this RFC419614 is
    rem to collect more Tally files that aren't
    rem kept in the registry.
    rem Add BNR placeholder for future BNR tally files
    echo [BNR]>>%MONITORDiagFile%
    echo Type=File>>%MONITORDiagFile%
    echo Name=%DATA_DRIVE%\scot\logs\BNR*.log>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=Yes>>%MONITORDiagFile%
    
    echo [BagScale Tally]>>%MONITORDiagFile%
    echo Type=File>>%MONITORDiagFile%
    echo Name=%DATA_DRIVE%\scot\logs\OPOS_BagScale_Tally.log>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=No>>%MONITORDiagFile%

    echo [CoinAcceptor Tally]>>%MONITORDiagFile%
    echo Type=File>>%MONITORDiagFile%
    echo Name=%DATA_DRIVE%\scot\logs\OPOS_CoinAcceptor_BCRInfo.dat>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=No>>%MONITORDiagFile%

    echo [CashAcceptor Tally]>>%MONITORDiagFile%
    echo Type=File>>%MONITORDiagFile%
    echo Name=%DATA_DRIVE%\scot\logs\CashAcceptorAudit.log>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=No>>%MONITORDiagFile%

    echo [Puloon CashDispenser Tally]>>%MONITORDiagFile%
    echo Type=File>>%MONITORDiagFile%
    echo Name=%DATA_DRIVE%\scot\logs\PuloonTallies.log>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=No>>%MONITORDiagFile%

    echo [F53CashDispenser Tally]>>%MONITORDiagFile%
    echo Type=RegDump>>%MONITORDiagFile%
    echo Name=HKLM\SOFTWARE\OleForRetail\ServiceOPOS\CashChanger\F53CurrDisp\Info_RunTimeSnapshot>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=Yes>>%MONITORDiagFile%
    echo OutputFile=NCRF53CashDispenserTallies.txt>>%MONITORDiagFile%

    rem  RFC424361 - added new tallies
    echo [UPS Power Device Tally]>>%MONITORDiagFile%
    echo Type=RegDump>>%MONITORDiagFile%
    echo Name=HKLM\SOFTWARE\OleForRetail\ServiceOPOS\POSPower\UPS>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=Yes>>%MONITORDiagFile%
    echo OutputFile=POSPowerTallies.txt>>%MONITORDiagFile%

    echo [Motorized Card Reader Tally]>>%MONITORDiagFile%
    echo Type=RegDump>>%MONITORDiagFile%
    echo Name=HKLM\SOFTWARE\OleForRetail\ServiceOPOS\MSR\MotorCardReader\Info_RunTimeSnapshot>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=Yes>>%MONITORDiagFile%
    echo OutputFile=MCRTallies.txt>>%MONITORDiagFile%

    echo [Serial MSR Tally]>>%MONITORDiagFile%
    echo Type=RegDump>>%MONITORDiagFile%
    echo Name=HKLM\SOFTWARE\OleForRetail\ServiceOPOS\MSR\NCRMSRDIPReader\Info_RunTimeSnapshot>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=Yes>>%MONITORDiagFile%
    echo OutputFile=SerialMSRTallies.txt>>%MONITORDiagFile%

    echo [SigCap Topaz Tally]>>%MONITORDiagFile%
    echo Type=RegDump>>%MONITORDiagFile%
    echo Name=HKLM\SOFTWARE\OleForRetail\ServiceOPOS\SignatureCapture\TopazSigCap\Info_RunTimeSnapshot>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=Yes>>%MONITORDiagFile%
    echo OutputFile=SigCapTopazTallies.txt>>%MONITORDiagFile%

    echo [Sankyo Check Reader Tally]>>%MONITORDiagFile%
    echo Type=RegDump>>%MONITORDiagFile%
    echo Name=HKLM\SOFTWARE\OleForRetail\ServiceOPOS\CheckScanner\OPOS_CkScanNCRSankyoUSB.305\Info_RunTimeSnapshot>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=Yes>>%MONITORDiagFile%
    echo OutputFile=SankyoCheckReaderTallies.txt>>%MONITORDiagFile%

    echo [Sankyo MICR Tally]>>%MONITORDiagFile%
    echo Type=RegDump>>%MONITORDiagFile%
    echo Name=HKLM\SOFTWARE\OleForRetail\ServiceOPOS\MICR\OPOS_MICRNCRSankyoUSB.305\Info_RunTimeSnapshot>>%MONITORDiagFile%
    echo Copy=Yes>>%MONITORDiagFile%
    echo Clear=Yes>>%MONITORDiagFile%
    echo OutputFile=SankyoMICRTallies.txt>>%MONITORDiagFile%
    
    rem RFC428898
    echo [Delete old files]>>%MONITORDiagFile% 
    echo Type=DelOldFiles>>%MONITORDiagFile%
    echo Clear=Yes>>%MONITORDiagFile%
    echo Name=*.Monitor.*.zip>>%MONITORDiagFile%
    echo PathDir=%TEMP_DRIVE%\Temp>>%MONITORDiagFile%
    echo Days=30>>%MONITORDiagFile%

    rem Usage: GetDiagFiles [-v] [-l] [-n] [-p] [-f <listfile>] [-o <outfile>]
    rem Typical Name LANENAME.MONITOR.010706.111633.zip

    call  :getdatetime
    set ZIPDT=%MM%%DD%%YY%.%HR%%MN%%SC%

    set zip=%TEMP%\%COMPUTERNAME%.MONITOR.%ZIPDT%.zip
    if exist %TEMP%\fastlane set zip=%TEMP%\fastlane\%COMPUTERNAME%.MONITOR.%ZIPDT%.zip
    
    if exist %ALTFILELOCATION% set FILELOCATION=%ALTFILELOCATION%

    call :log Creating file %zip%
    rem We're now calling the normal getdiagfile.exe from c:\scot\bin for RFC419614
    %APP_DRIVE%\scot\bin\getdiagfiles.exe -f %MONITORDiagFile% -o %zip%

    rem Now that we've acquired the scotapp.* files, delete all of them from Monitor dir
    del /f /q %APP_DRIVE%\scot\monitor\scotapp.* >> %LOG% 2>&1 
    rem Also delete other temporary files.
    del /f /q %APP_DRIVE%\scot\monitor\monitorsummary1.dat >> %LOG% 2>&1    

    call :log Start run transport module
    call %MONITOR%\MonitorTransport.bat %FILELOCATION%
    call :log End Run transport module

    call :log Monitor end
    echo.
    echo Monitor Run (%NBINC%/%MAX%) Done
    goto :eof

:log
    call :getdatetime  
    echo %DT% %* >> %LOG%
    goto :eof

:getdatetime
    rem Support Local Settings

    rem During install, getdt won't exist yet, so we have an alternate time
    rem But after install, it will exist. This means that at install,
    rem the date/time will be effected by the locale settings until getdt.exe
    rem is copied to the monitor directory.
    if not exist %APP_DRIVE%\scot\bin\getDT.exe goto :getAltDateTime
    for /f "tokens=1,2 delims= " %%i in ('%APP_DRIVE%\scot\bin\getDT.exe') do set date1=%%i & set time1=%%j

    SET MM=%date1:~0,2%
    SET DD=%date1:~3,2%
    rem TAR433402 Keep zip file name the same. MMDDYY
    SET YYYY=%date1:~6,4%
    SET YY=%date1:~8,2%

    SET HR=%time1:~0,2%
    SET MN=%time1:~3,2%
    SET SC=%time1:~6,2%

    set DT=
    rem TAR433402 Keep logging as MM-DD-YYYY
    set DT=%MM%-%DD%-%YYYY% %HR%:%MN%:%SC%
    goto :eof
    
:getAltDateTime
    rem Using AltDateTime, effected by Locale
    set DT=
    rem This for loop only works if the default date format
    rem contains 2 tokens, day of week, then date (Wed 01/01/2001 for instance)
    for /f "tokens=2" %%i in ('date /t') do set date1=%%i 

    set DTA=%date1:~0,10%   
    set CT=%TIME:~0,8%
    set DT=%date1:~0,10% %CT%
    rem set DT=%DTA%%CT%

    goto :eof

:ErrorSetNTScheduler
    call :log Failed to Set the Job in the NT Scheduler
    type %log%
    %MONITOR%\sleep 30
    goto :eof

:ErrorStartNTScheduler
    call :log Failed to Start NT Scheduler
    type %log%
    %MONITOR%\sleep 30
    goto :eof

:ErrorCreatingDir
    call :log Failed to Create Dir %*
    type %log%
    %ZIPDIR%\sleep 30
    goto :eof