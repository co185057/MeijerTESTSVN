'******************************************************************************* 
'Customer: Meijer
'Script:   ChkdskFix.vbs
'Purpose:  Setup the FastLane harddrive to undergo a fix upon reboot.
'*******************************************************************************
'History:
'POS43830  Work Request:52635  Name:Matt Condra         Date: January 7, 2013
'*******************************************************************************

'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
'                       Variable Declaration 
'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
 
Const ForReading = 1
Const ForWriting = 2
Const ForAppending = 8
Const logDir = "c:\scot\logs\"

strLog = logDir & "\ChkdskFix.log"

Dim workingSetSizeThreshold
Dim objLogHandle
Dim oFSO
Set oFSO = Wscript.CreateObject("Scripting.FileSystemObject")
 

'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
'                       Main Body  
'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 

LogIt( "*************** START **********************" )

' This ChkdskFix.vbs script file will be executed based on the scheduled "ChkdskFix" task.
' This script will change the registry setting [HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\BootExecute]
' to [autocheck autochk /p \??\C:] so that during the next reboot, chkdsk.exe will run in fix mode to fix the harddrive.
' Meijer Operations will need to be informed of the potentials for:
' (a) chkdsk.exe to get stuck at boot-time waiting for manual input to continue.
' (b) chkdsk.exe to run at each boot due to a Microsoft malfunction in not automatically changing the registry setting back to
'     [autocheck autochk *] after chkdsk has run once.
sCCS = "HKLM\SYSTEM\CurrentControlSet\"
sSessionManagerRegPath = sCCS & "Control\Session Manager\"
Set oShell = CreateObject ("Wscript.shell")
oShell.RegWrite sSessionManagerRegPath & "BootExecute", "autocheck autochk /p \??\C:"

LogIt( "System registry setup so that harddrive will be fixed during next reboot." )
LogIt( "*************** END **********************" )

wscript.quit


'******************************************************************************* 
'Purpose: 	Logs information to the file defined at the start of the script
'*******************************************************************************
Function LogIt(txt)  	   
	Set objLogHandle = oFSO.OpenTextFile(strLog,ForAppending,True)	
    objLogHandle.WriteLine(Now & " " & txt)
    objLogHandle.Close
End Function
