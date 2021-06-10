'******************************************************************************* 
'Customer: NCR Corp
'Script: upconcontrol.vbs
'Purpose: Pulls Updates and Executes UpCon
'Author:  Dennis Shovelin
'Created: 8/2005
'*******************************************************************************
' Change Control
'  10/2007    - Made it more generic for General Use.
'  11/02/2006 - Changed the file name for the update log from c:\temp\update.log to
'               c:\scot\work\upconcontrol.log.  Added a backup also
'               Modified the subroutine setup-log
'  11/02/2006 - Created new subroutine to delete the z: drive.  This routine will be
'               executed when upconcontrol starts and when upconcontrol ends
'  11/02/2006 - Added subroutine to parse a text string and search for a specific text.
'               This subroutine will be used to search the log for the word failure
'  11/02/2006 - Added new subroutine to add the upconstatus.dat text to the upconcontrol.log
'               to show status.
'  11/02/2006 - Added code to check the upconstatus.dat log for failure.  If a failure occurs then
'               the bundle will not be moved to applied bundles.
'  11/02/2006 - Added subroutine to copy and rename the upconcontrol.log to show store and date in 
'               the file name and send this log file to a central repository.
'  11/02/2006 - Added subroutine to get the store number from computer name on
'               the upcon server. 
'  11/13/2006 - Added backup of Upcon logs as done by Atlanta.  Modified upcon_cleanup subroutine
'  11/14/2006 - Added creation of c:\temp\fastlane\logs directory if it doesn't exist
'  08/14/2008 - Made change to check for updates folder before trying to delete
'  10/30/2008 - Made change from a internal command to a called batch file to get the update list
'               The reason for this is that it was possible to leave an orphaned cmd.exe on the 
'               Upcon server.
Option Explicit

Dim objFSO, objIE, objIE2, verFile, logFile, objWshShell, updateFolder, updateFile, objtext, tracefile
Dim oShell, objInputFile, inputData, objFileSystem, objupconstatus, upconstatus, dshell
Dim strInputFile, strData, Return, wshshell, wshnetwork, alldrives, newfolder
dim test, f
dim NCR
dim scotpath
dim sCCs
dim sCompNameRegPath
dim pos
dim searchtext
dim searchfor
dim failure_found
dim compname
dim store_number
dim driveletter
driveletter = "Z:"

NCR = "HKLM\SOFTWARE\NCR\"
scotpath = NCR & "SCOT\"
sCCS = "HKLM\SYSTEM\CurrentControlSet\"
sCompNameRegPath = sCCS & "Control\ComputerName\"
Const OPEN_FILE_FOR_READING = 1
Set oShell = CreateObject ("Wscript.shell")
Set objFSO = WScript.CreateObject("Scripting.FileSystemObject")

'*******************************************************************************
'           Logging Setup
'*******************************************************************************
sub setup_log()
'Create log file on PC
Set objWshShell = WScript.CreateObject("WScript.Shell")
if objFSO.FileExists("c:\scot\work\upconcontrol.log.bak") Then 
        objFSO.DeleteFile("c:\scot\work\upconcontrol.log.bak")
end if
If objFSO.FileExists("c:\scot\work\upconcontrol.log") Then 
        objFSO.copyfile "c:\scot\work\upconcontrol.log", "c:\scot\work\upconcontrol.log.bak"
	objFSO.DeleteFile("c:\scot\work\upconcontrol.log")

end if
Set logFile = objFSO.CreateTextFile("c:\scot\work\upconcontrol.log")
logFile.WriteLine "Update process began  --  " & Time & " " & Date
logFile.WriteBlankLines 1

end sub
'*******************************************************************************
'       Read upconstatus.dat file and write to upconcontrol.log
'*******************************************************************************
sub read_upconstatus()
dim statustext
failure_found = "N"
set objupconstatus = WScript.CreateObject("Scripting.FileSystemObject")
if objupconstatus.FileExists("c:\scot\work\upconstatus.dat") Then 
set upconstatus = objupconstatus.opentextfile("c:\scot\work\upconstatus.dat",1)
	do while not upconstatus.AtEndOfStream
		statustext = upconstatus.readline()
		logfile.writeline statustext
		searchtext = statustext
		searchfor = "failure"
		search_routine
		if pos > 1 then
                    failure_found = "y"
                end if
	loop
upconstatus.close
else
	logfile.WriteLine "upconstatus.dat file not found"
	logfile.WriteBlankLines 1
end if

end sub
'*******************************************************************************
'       Pulling Standard Update
'*******************************************************************************  
sub get_updates()
If objFSO.DriveExists("c:\") Then
	logFile.WriteLine "Getting Server File List -- " & Time & " " & Date
	set oShell=WScript.CreateObject ("WSCript.Shell")
'	Return = oShell.Run ("cmd /K dir c:\bundles\*.exe /o:n /b>c:\temp\update.lst",0 ,false)
	f = oshell.run ("c:\scot\upcon\upcongetfiles.bat", 0, true)
	set oShell=nothing
	logFile.WriteLine "Server File List Retrieved -- " & Time & " " & Date
'	Const OPEN_FILE_FOR_READING = 1
	set objFileSystem = CreateObject("Scripting.fileSystemObject")
	wscript.sleep 3000
	Set objInputFile = objFileSystem.OpenTextFile("c:\temp\update.lst",OPEN_FILE_FOR_READING)
	do while not objinputfile.AtEndOfStream
		strData = objinputfile.readline() 

test = len(trim(strdata))
	 	if len(trim(strData)) > 0 then

			If not (objFSO.FileExists("c:\temp\update.lst" & strData)) Then
				logFile.WriteLine "Pulling " & strData

				Popup strData
'
				PullFile "c:\bundles", strData
'		 		objIE.Quit ()
			Else 
				logFile.WriteLine strData & " -- Not pulled"
			End If
		end if


	loop
	set objFileSystem = nothing
	set objInputFile = nothing
end if
end sub


'******************************************************************************
'                Cleanup OLD UpCon Logs
'******************************************************************************
sub UpCon_Cleanup()

If objFSO.FileExists("c:\scot\work\upcon.log")Then 
set traceFile = objFSO.GetFile("c:\scot\work\upcon.log")
	If traceFile.Size > 3000000 Then 
		If objFSO.FileExists("c:\scot\work\upcon.log.bak") Then 
			objFSO.DeleteFile "c:\scot\work\upcon.log.bak"
		End If
		traceFile.Copy "c:\scot\work\upcon.log.bak"
		objFSO.DeleteFile "c:\scot\work\upcon.log"
	Else
		Set objText = traceFile.OpenAsTextStream( 8, -2 )
		objText.Write vbCrLf & "Starting new run of UpCon" & vbCrLf & vbCrLf
		Set objText = Nothing
	End If
End If

If objFSO.FileExists("c:\scot\work\upconsummary.log")Then 
set traceFile = objFSO.GetFile("c:\scot\work\upconsummary.log")
	If traceFile.Size > 3000000 Then 
		If objFSO.FileExists("c:\scot\work\upconsummary.log.bak") Then 
			objFSO.DeleteFile "c:\scot\work\upconsummary.log.bak"
		End If
		traceFile.Copy "c:\scot\work\upconsummary.log.bak"
		objFSO.DeleteFile "c:\scot\work\upconsummary.log"
	Else
		Set objText = traceFile.OpenAsTextStream( 8, -2 )
		objText.Write vbCrLf & "Starting new run of UpCon" & vbCrLf & vbCrLf
		Set objText = Nothing
	End If
End If


end sub
'******************************************************************************
'                Cleanup Updates
'******************************************************************************
sub cleanup_updates()
if objFSO.folderexists("c:\updates\") then 
	objFSO.DeleteFolder "c:\updates", True
end if

end sub
'******************************************************************************
'                Search Subroutine
'******************************************************************************
sub Search_routine()
pos = instr(1,searchtext,searchfor,1)
end sub
'******************************************************************************
'                 Delete z: subroutine
'******************************************************************************
sub Delete_Z_Routine()
dim i
set wshshell = wscript.createobject("wscript.shell")
set wshnetwork = wscript.createobject("wscript.network")
set alldrives = wshnetwork.enumnetworkdrives
for i = 0 to alldrives.count-1 step 2
if alldrives.item(i) = driveletter then
wshnetwork.removenetworkdrive alldrives.item(i), "true"
end if
next
end sub
'******************************************************************************
'                Copy to Applied Bundles
'******************************************************************************
sub copy_to_applied()
 		If objFSO.FileExists("c:\bundles\" & strData) Then 
 			objFSO.copyfile "c:\bundles\" & strData, "c:\bundles\appliedbundles\" & strData, True
 			objFSO.Deletefile "c:\bundles\" & strData
 		end if
end sub

'******************************************************************************
'   Updating Screen
'******************************************************************************
Function Popup (updateName)

Set objIE = WScript.CreateObject("InternetExplorer.Application")
'With objIE
'	.left      = 300
'	.top       = 50
'	.width     = 800
'	.height    = 600
'	.menubar   = False
'	.toolbar   = False
'	.statusbar = False
'	.Resizable = False
'	.navigate("about:blank")
'	.Visible   = True
'	.FullScreen = True
'	While .Busy: Wend
'End With
end function

'**************************************************************************************
'This function pulls new bundles and applies them
'**************************************************************************************
Function PullFile(upFileDir, upFileName)
Dim lngReturn, strErrorFile, objErrorFile, strTextStream, upconrun, nontbdata, nontbshell, nontbobjinputfile, nontbobjfilesystem
'objFSO.CopyFile upFileDir & "\" & upFileName, "c:\updates\" & upFileName, True

	logFile.WriteLine "Starting Update " & Time & " " & Date & " For " & upFileName
	
	logFile.WriteLine "Unzipping " & upfileName
	lngReturn = objWshShell.Run("c:\bundles\" & upFileName, 2, True)
				
	logFile.WriteLine "Starting Upcon " & Time & " " & Date & " For " & upFileName
	upconrun = objWshShell.Run("c:\scot\upcon\upcon.exe US\scot scot", 2, True)
	Read_Upconstatus
	if failure_found = "N" then
            copy_to_applied
	end if 
	logFile.WriteLine "Upcon Ended " & Time & " " & Date & " For " & upFileName
	set nontbobjfilesystem = nothing
	set nontbobjinputfile = nothing
	
	cleanup_Updates
	logFile.WriteLine "Ending Update " & Time & " " & Date & " For " & upFileName
	
end Function

'**************************************************************************************
'
'     MAIN PROCESSING
'
'**************************************************************************************
Delete_Z_Routine

'*****************execute log setup****************************************************
setup_log

'*****************Cleanup Old UpCon Logs***********************************************
UpCon_Cleanup

'*****************execute get updates**************************************************
get_updates

'*****************End of Processing****************************************************
logFile.WriteBlankLines 1
logFile.WriteLine "Update Process Ended " & Time & " " & Date
logfile.close

Delete_Z_Routine
wscript.quit