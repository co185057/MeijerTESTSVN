'******************************************************************************* 
'Customer: Meijer
'Script: resetFTSxml.vbs
'Purpose: Resets the Destination ID to computer name for File Transport Service
'Author:  Dennis Shovelin
'Created: 3/2009
'*******************************************************************************
' Change Control

'************END CHANGE CONTROL*************************************************
Set oShell = CreateObject ("Wscript.shell")

'*******************************************************************************
'Setup Variables
'*******************************************************************************

dim strCompName
dim STORE_NUM
dim savedsecondaryname
Dim sampleline
Dim ObjFSO

sCCS = "HKLM\SYSTEM\CurrentControlSet\"
sTcpipParamsRegPath = sCCS & "Services\Tcpip\Parameters\"
sCompNameRegPath = sCCS & "Control\ComputerName\"
strCompName = oshell.Regread (sCompNameRegPath & "ComputerName\ComputerName") 



'*****************************Get Computer Name**********************************
sub get_comp_name()

STORE_NUM = mid(strCompName, 2, 4)

end sub

'**********************************************************************************

'**********************************************************************
'  File Transport XML file write
'**********************************************************************
sub filetransportxml()
savedsecondaryname = "W" & STORE_NUM & "EPOSS0002"
Set objFSO = WScript.CreateObject("Scripting.FileSystemObject")

If objFSO.FileExists("c:\scot\config\filetransportservice.xml") Then objFSO.DeleteFile("c:\scot\config\filetransportservice.xml")
Set FileTransport = objFSO.CreateTextFile("c:\scot\config\filetransportservice.xml")
sampleline = "<?xml version=""1.0"" encoding=""utf-8"" ?>"
filetransport.writeline(sampleline)
sampleline = "<FileTransportService>"
filetransport.writeline(sampleline)
sampleline = "     <Monitors PeriodInSeconds=""1800"">"
filetransport.writeline(sampleline)
sampleline = "          <Monitor SourceMask=""C:\temp\*.zip"" Destination=""\\" & savedsecondaryname & "\temp\fastlane"" Move=""yes"" DestUser=""administrator"" DestPass=""Unity1"" />"
filetransport.writeline(sampleline)
sampleline= "     </Monitors>"
filetransport.writeline(sampleline)
sampleline = "</FileTransportService>"
filetransport.writeline(sampleline)

end sub

'*************************************************************************

'*************************************************************************
'*************************************************************************
'                      MAIN PROCESSING
'*************************************************************************
'*************************************************************************

get_comp_name

filetransportxml

wscript.quit