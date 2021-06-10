'*******************************************************************************

'Script: Set_Security_RAP1.vbs

'Purpose:             Changes the Registry to point at POS Server 202 for Security Server

'Created:             01/12/2010

'Author:               Ryan.Cichewicz@meijer.com

'*******************************************************************************

 

Set oShell = CreateObject ("Wscript.shell")

set objShell = WScript.CreateObject("WScript.Shell")

Set objFSO = WScript.CreateObject("Scripting.FileSystemObject")

Set WshShell = WScript.CreateObject("WScript.Shell")

 

Dim ObjFSO

Dim Store_Num1
Dim Store_Num2

Dim S2

'Dim strComputerName

 

extract_failed = 0

Const ForReading = 1

const ForWriting = 2

const ForAppending = 8

 

strComputerName = objShell.ExpandEnvironmentStrings( "%COMPUTERNAME%" )

'Store_Num1 = WshShell.Regread("HKEY_LOCAL_MACHINE\SOFTWARE\NCR\SCOT\StoreNumber")  
WScript.Echo "Computer Name '" & strComputerName & "'"

Store_Num1 = Left(strComputerName,9)

Store_Num2 = Right(Store_Num1,4)
WScript.Echo "Final Store Number after trimming '" & Store_Num2 & "'"
 

'Lab

'S2 = "W" & Store_Num2 & "IPOSS0002"

 

 

'Enterprise

S2 = "W" & Store_Num2 & "EPOSS0002"
WScript.Echo "202 Server Name '" & S2 & "'"

 

'change registry so it looks at the  W 'STOREXXXX' EPOSS0002

WshShell.RegWrite "HKEY_LOCAL_MACHINE\SOFTWARE\NCR\SCOT - CoreApplication\ObservedOptions\ServerName", S2
WScript.Echo "Written '" & S2 & "' to the HKLM\SOFTWARE\NCR\SCOT - CoreApplication\ObservedOptions\ServerName"

WshShell.RegWrite "HKEY_LOCAL_MACHINE\SOFTWARE\NCR\SCOT\CurrentVersion\SCOTAPP\ClientName",  S2
WScript.Echo "Written '" & S2 & "' to the HKLM\SOFTWARE\NCR\SCOT\CurrentVersion\SCOTAPP\ClientNamee"

WshShell.RegWrite "HKEY_LOCAL_MACHINE\SOFTWARE\NCR\SCOT\Installation\CoreSecurityServer\ServerName",  S2
WScript.Echo "Written '" & S2 & "' to the HKLM\SOFTWARE\NCR\SCOT\Installation\CoreSecurityServer\ServerName"
