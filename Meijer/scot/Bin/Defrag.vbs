'*******************************************************************************

'Script: Defrag.vbs
'Purpose:             Run Defrag tool
'Created:             06/22/2012
'Author:              Robert Susanto

'*******************************************************************************

'Initialize
Const DefragLogFile = "C:\scot\logs\defrag.log"
Const traceFile = "C:\scot\logs\defragtrace.log"
Const DefragFinalLog = "C:\acs\data\ssco_defrag.log"

Set objFSO = WScript.CreateObject("Scripting.FileSystemObject")
Set ObjTraceFile = objFSO.CreateTextFile(traceFile , true)

'**************************************************************************
'  Run Defrag Analysis
'**************************************************************************
Function run_defrag()

Set oShell = WScript.CreateObject ("Wscript.shell")

if IsProcessRunning("defrag.exe") then
    KillProcess "defrag.exe"
end if
WScript.sleep 5000

if ( objFSO.FileExists(DefragLogFile) = True ) Then
	Set objLocalLog = objFSO.CreateTextFile(DefragLogFile, true)
	objLocalLog.Close

end if

'run defrag analysis
RunString = "%comspec% /c defrag.exe C: -a -v"
Log("Run Defrag Analysis = " & RunString)

run_defrag = 0
run_defrag = oShell.Run (RunString & " >> " & DefragLogFile & " 2>&1", 0, TRUE)

End Function

'**************************************************************************
'  analyse defrag data
'**************************************************************************
Function analyse_defragresult()

Set objReadFile = objFSO.OpenTextFile(DefragLogFile, 1, False)

analyse_defragresult = 0

'Reads the file until the end.
Do Until objReadFile.AtEndOfStream

	'Sets the line being read to a variable named strLine.
	strLine = objReadFile.ReadLine

	'Trims the strLine variable to remove any leading or trailing spaces.
	strLine = trim(strLine)
	
	Log("Original Text = " & strLine)

	'Set the search condition. In this case, we're looking for "Total fragmentation" in the string
	'of text which was set to strLine.
	If instr(strLine, "Total fragmentation             	=") Then		

		'find the = sign
		nLocation = instr(strLine, "=")
		Log("Any percentage sign in location: " &nLocation )
		
		'save the original line
		strOrgLine = strLine

		'exctract the total fragmentation value
		strLine = Mid(strLine, nLocation+1, 3)
	
		'trim the strLine variable to remove any leading or trailing spaces.
		strLine = trim(strLine)

		Log("Total Fragmentation = " & strLine & "%" )
		
		defragThreshold = 40
		'If the Total Fragmentation is less or equal then 40, run the defrag
		Log( "Number of argument = " & WScript.Arguments.Count )
		If WScript.Arguments.Count = 1 then
			defragThreshold = WScript.Arguments(0)	
			Log( "Argument Value = " & WScript.Arguments(0) )
		End If

		If CInt(strLine) <= CInt(defragThreshold) Then
			Set oShell = WScript.CreateObject ("Wscript.shell")
			RunString = "%comspec% /c defrag.exe C: -v"
			Log("Run Defrag Analysis with the fix = " & RunString)

			analyse_defragresult = oShell.Run (RunString & " >> " & DefragLogFile & " 2>&1", 0, TRUE)			
		else			
			'Write the total fragmention to the acs file, Total fragmentation             	= 1 %
			Set ObjWriteFile = objFSO.CreateTextFile(DefragFinalLog, true)
			strOrgLine = strOrgLine
			objWriteFile.Write strOrgLine
			objWriteFile.Close
			Log("Log to ACS Server Error Event Log: " & strOrgLine)
		End If

		Exit Do
	End If
Loop

objReadFile.Close

end Function

'*==================================================================
'* Name         : IsProcessRunning
'* In           : Process name (look in task manager)
'* Out          : True/False
'* Purpose      : Check if a process is running
'*==================================================================
Function IsProcessRunning(sProcess)
    Dim oProcess
    IsProcessRunning = False
    For Each oProcess In GetObject("winmgmts://").InstancesOf("win32_process")
         If UCase(oProcess.Name) = UCase(sProcess) Then
             IsProcessRunning = True
                Set oProcess = Nothing
                Exit Function
         End If
     Next
     Set oProcess = Nothing
End Function

'*==================================================================
'* Name            : KillProcess
'* In              : Process name (look in task manager)
'* Out             :
'* Purpose         : Kill a process by name
'*==================================================================
Sub KillProcess(sName)
    On Error Resume Next
    Dim oProcesses, oProcess
    Set oProcesses = GetObject("WinMgmts:root/CIMV2").ExecQuery( _
        "SELECT * FROM Win32_Process WHERE Name='" & sName &"'")
    For Each oProcess In oProcesses
        If UCase(oProcess) = UCase(sProcess) Then
            oProcess.Terminate
        End If
    Next
    Set oProcesses = Nothing
End Sub

'**************************************************************************
'  write to log file
'**************************************************************************
sub Log(traceString)
	traceLine = Time() & " " & traceString

	ObjTraceFile.WriteLine traceLine 
end sub

'**************************************************************************
'  Process defrag return code
'**************************************************************************
sub Defrag_Return_Code(return)

If return = 0 then
	Log("Defrag completed successfully")
else
	Set ObjWriteFile = objFSO.CreateTextFile(DefragFinalLog, true)

	if return = 1 then
		errorLine = "Defrag aborted with error level " & return & " (defrag was cancelled manually) "
	elseif return = 2 then
		errorLine = "Defrag aborted with error level " & return & " (there was a command line error. Check your command line for valid switches and drives)"
	elseif return = 3 then
		errorLine = "Defrag aborted with error level " & return & " (there was an unknown error)"
	elseif return = 4 then
		errorLine = "Defrag aborted with error level " & return & " (defrag could not run due to insufficient memory resources)"
	'errorlevel 5 is not currently used
	elseif return = 5 then
		errorLine = "Defrag aborted with error level " & return & " (general error)" 
	elseif return = 6 then
		errorLine = "Defrag aborted with error level " & return & "(System error: either the account used to run defrag is not an administrator, there is a problem loading the resource DLL, or a defrag engine could not be accessed. Check for proper user permissions and run Sfc.exe to validate system files)" 
	elseif return = 7 then
		errorLine = "Defrag aborted with error level " & return & " (There is not enough free space on the drive. Defrag needs 15% free space to run on a volume)"
	else
		errorLine = "Defrag aborted with an unknown error level: " & return
	end if

	Log("Log to ACS Server Error Event Log: " & errorLine)
	objWriteFile.Write errorLine	
	objWriteFile.Close
end if


end sub



'*************************************************************************
'  MAIN PROCESSING SECTION
'*************************************************************************

Log( "*************** START **********************" )
'**********run defrag*******************
Return = run_defrag
Defrag_Return_Code(Return)

if( Return = 0 ) then
	'**********analyze defrag*******************
	Return = analyse_defragresult
	Defrag_Return_Code(Return)	
end if

Log("Finish")
ObjTraceFile.Close

wscript.quit
