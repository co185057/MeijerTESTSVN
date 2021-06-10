'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
'                       Variable Declaration 
'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
 
Const ForReading = 1
Const ForWriting = 2
Const ForAppending = 8
Const logDir = "c:\scot\logs\"

strLog = logDir & "\ProcessQuery.Log"

Dim workingSetSizeThreshold
Dim objLogHandle
Dim oFSO
Set oFSO = Wscript.CreateObject("Scripting.FileSystemObject")
 

'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
'                       Main Body  
'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 

strComputer = "."
workingSetSizeThreshold = 200000000

LogIt( "*************** START **********************" )
If WScript.Arguments.Count = 1 then
	workingSetSizeThreshold = WScript.Arguments(0)
	LogIt( "WorkingSetSize Threshold Value = "  & CStr(workingSetSizeThreshold) )
End If
		
If IsProcessRunning("scotappu.exe") then
	Set objWMIService = GetObject("winmgmts:" _
	    & "{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")

	'Set colProcessList = objWMIService.ExecQuery("Select * from Win32_Process")
	Set objScotappu = objWMIService.ExecQuery("Select * from Win32_Process Where Name = 'SCOTAPPU.EXE' ")

	
	For Each objProcess in objScotappu
	    LogIt( "Process: " & objProcess.Name )
	    LogIt( "Process ID: " & objProcess.ProcessID  )
	    LogIt( "Thread Count: " & objProcess.ThreadCount )
	    LogIt( "Page File Size: " & objProcess.PageFileUsage )
	    LogIt( "Page Faults: " & objProcess.PageFaults )
	    LogIt( "Working Set Size: " & objProcess.WorkingSetSize )
	
    	    'check the working set size of scotappu	
	    If( CLng(objProcess.WorkingSetSize) > CLng(workingSetSizeThreshold) ) Then		
		Set ObjWriteFile = oFSO.CreateTextFile("c:\acs\data\ssco_memory.log", true)
		strMessage = "Working Storage Section(WSS) high at:" & objProcess.WorkingSetSize
		ObjWriteFile.Write strMessage
		ObjWriteFile.Close
		LogIt(strMessage)
	    End If	
	Next	
else
	LogIt( "SCOTAPPU Process is not running!!!")
End If

'call LogItMaintenance
LogItMaintenance()

wscript.quit


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


'******************************************************************************* 
'Purpose: 	Logs items to the log defined at the beginning of the script
'*******************************************************************************
Function LogIt(txt)  	   
	Set objLogHandle = oFSO.OpenTextFile(strLog,ForAppending,True)	
    objLogHandle.WriteLine(Now & " " & txt)
    objLogHandle.Close
End Function

'******************************************************************************* 
'Purpose: 	Log maintenance, create a backup when the size exceeds 1MB
'*******************************************************************************
Function LogItMaintenance()    
	'backing up log file if necessary
	Set ObjFile = oFSO.GetFile(strLog)
	If ObjFile.Size > 1000000 Then		
		strBakLogFile = strLog & ".bak"
		LogIt( strBakLogFile & " will be created" )
		if oFSO.FileExists( strBakLogFile ) Then
			oFSO.DeleteFile( strBakLogFile)
		End If
		oFSO.MoveFile strLog, strBakLogFile
	End If
End Function