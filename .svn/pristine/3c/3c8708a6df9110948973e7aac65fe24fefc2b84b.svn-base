'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
' Name: ConfigurePageFile.vbs 
' Comments: This script examines curent phyiscal RAM and will  
'               create a page file twice the size 
'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
Option Explicit 
'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
'                       Variable Declaration 
'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
 
Dim strName, strInitialSize, strMaximumSize, objRtvPageFile, strMsg, strLog 
Dim intRAM, int2XRAM 
Dim objRAM, objSysSet, objItem 
Dim valYN 
Dim PageFileSz 

Const ForReading = 1
Const ForWriting = 2
Const ForAppending = 8
Const logDir = "c:\scot\logs\"

strLog = logDir & "\PagingFilesConfig.Log"

Dim oFSO
Set oFSO = Wscript.CreateObject("Scripting.FileSystemObject")
Dim objLogHandle    
Dim ObjFile

'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
'                       Main Body  
'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
 
 LogIt( "*************** START **********************" )
 
' Puts Total MB of Physical RAM into intRAM variable 
GetTotalRam() 
 
' Puts Page Config into variables 
PageFileConfig() 
 
' Create Message 
CreateMsg() 
 
' Twice Physical RAM 
int2XRAM = intRam * 2.5 
strMsg = strMsg & vbcrlf & "Set the Page File to " & vbcrlf & "2.5 X Total Physical Memory = " & int2XRAM & " " & vbcrlf & vbcrlf  
LogIt(strMsg)
 
If int2XRAM < 1241 Then
	'Set current Page File to be two times RAM 
	setPageFileSz(int2XRAM) 
	strMsg = "Set the PageFile to 2x RAM"
	LogIt(strMsg)
Else
	'Set current Page File to be two times RAM 
	setPageFileSz(1240) 
	strMsg = "Set the PageFile to the recommended Max 1240"
	LogIt(strMsg)
End If	

PageFileConfig()
strMsg = "Here are your new Page File settings:" & vbcrlf & vbcrlf 
CreateMsg() 
LogIt(strMsg)
 
 'call logitmaintenance
 LogItMaintenance()
 
' Exit Script 
WScript.Quit(0) 
 
'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
'                 Sub Procedures and Functions 
'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
 
' Configure pagefile 
Function setPageFileSz (PageFileSz) 
        Dim objPageFile, objWMIService, colPageFiles 
 
        Set objWMIService = GetObject("winmgmts:" & "{impersonationLevel=impersonate}!\\.\root\cimv2") 
        Set colPageFiles = objWMIService.ExecQuery ("Select * from Win32_PageFileSetting") 
 
        For Each objPageFile in colPageFiles 
                objPageFile.InitialSize = PageFileSz 
                objPageFile.MaximumSize = PageFileSz 
		objPageFile.Put_
        Next 
End Function 
 
' Retrieves Total Phyiscal RAM and Converts to MB  
Sub GetTotalRAM() 
        For Each objRAM in GetObject("winmgmts:\\.\root\CIMV2").ExecQuery("SELECT TotalPhysicalMemory FROM Win32_ComputerSystem") 
                intRAM = (objRAM.TotalPhysicalMemory / 1024) / 1024 
                intRAM = FormatNumber(intRAM, 0) + 1 
        Next 
End Sub 
 
' Retrieves Page File Name, Initial Size and Maximum Size into str variables 
Sub PageFileConfig() 
        For Each objRtvPageFile in GetObject("winmgmts:\\.\root\CIMV2").ExecQuery("SELECT Name, InitialSize, MaximumSize FROM Win32_PageFileSetting",,48) 
                strName = objRtvPageFile.Name 
                strInitialSize = objRtvPageFile.InitialSize 
                strMaximumSize = objRtvPageFile.MaximumSize 
        Next 
End Sub 

' Put string together for message 
Sub CreateMsg() 
        strMsg = strMsg & "Current Location:" & strName & vbcrlf 
        strMsg = strMsg & "InitialSize: " & strInitialSize & vbcrlf 
        strMsg = strMsg & "MaximumSize: " & strMaximumSize & vbcrlf 
        strMsg = strMsg & "Total Physical Memory: " & intRAM & vbcrlf 
End Sub

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
