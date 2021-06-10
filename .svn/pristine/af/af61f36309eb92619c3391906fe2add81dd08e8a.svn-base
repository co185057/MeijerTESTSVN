'******************************************************************************* 
'Purpose: Script to setup the UPCONserver.dat file on the RAP/Attendant Station.
'Author:  Dennis Shovelin
'Created: 8/2005
'*******************************************************************************



Set oShell = Wscript.CreateObject("Wscript.Shell")



sCCS = "HKLM\SYSTEM\CurrentControlSet\"
sTcpipParamsRegPath = sCCS & "Services\Tcpip\Parameters\"
sCompNameRegPath = sCCS & "Control\ComputerName\"

name = oshell.Regread (sCompNameRegPath & "ComputerName\ComputerName") 

Const ForReading = 1, ForWriting = 2, ForAppending = 8
  Dim fso, f, Msg
  Set fso = wscript.CreateObject("Scripting.FileSystemObject")   
  
  
  Set f = fso.OpenTextFile("c:\SCOT\UPCON\emptyupconserver.bat", ForAppending, True)
  f.Writeline "set UpconIP=" & name
  
	Wscript.quit
	
 

'End upcon2.vbs