'******************************************************************************* 
'Customer: Meijer
'Script: RAP_reg_save.vbs
'Purpose: This script saves the Tailored registry entry values to an ini 
'         type file
'Author:  Dennis Shovelin 
'Created: 9/2007
'*******************************************************************************
' Change Control

'************END CHANGE CONTROL*************************************************
Set oShell = CreateObject ("Wscript.shell")

'*******************************************************************************
'Setup Variables
'*******************************************************************************


Dim ObjFSO

Dim savedcomputername
Dim savedScannerClient




sCCS = "HKLM\SYSTEM\CurrentControlSet\"
sTcpipParamsRegPath = sCCS & "Services\Tcpip\Parameters\"
sCompNameRegPath = sCCS & "Control\ComputerName\"
name = oshell.Regread (sCompNameRegPath & "ComputerName\ComputerName") 

NCR = "HKLM\SOFTWARE\NCR\"
scotIpath = NCR & "SCOT\Installation\"
scotCpath = NCR & "SCOT\CurrentVersion\"
scotCorepath = NCR & "SCOT - CoreApplication\"
sTcpipParamsRegPath = sCCS & "Services\Tcpip\Parameters\"
sCompNameRegPath = sCCS & "Control\ComputerName\"
scotpath = NCR & "SCOT\"
RAPSCANNERPATH = "HKLM\SOFTWARE\OLEFORRETAIL\SERVICEOPOS\SCANNER\RAPSCANNER\"

'**************************************************************************
'  Save Registry for Installation Subroutine
'**************************************************************************
sub reg_save()
name = oshell.Regread (sCompNameRegPath & "ComputerName\ComputerName") 


'	
 savedcomputername = oshell.RegRead (sCompNameRegPath & "ComputerName\ComputerName")

'  RAP Updates
savedScannerClient = oshell.RegRead (RAPSCANNERPATH & "ClientName")


end sub

'*******************End reg_update Subroutine***********************************
'**********************************************************************
'  Save Registry File write
'**********************************************************************
'	This subroutine is used to create a saved registry file for 
'	fastlane_setup.vbs.  It accepts 4 argument which is as follows:
'		Computer Name - The Active Computer Name
'               Security Server - The Saved Security Server Name
'               Report Server - The Saved Report Server Name
'               terminal number - physical terminal nubmer in quad
'
sub saveregwrite(savedcomputername, savedscannerclient)

Dim sampleline
Dim configfileFSO
Set configfileFSO = WScript.CreateObject("Scripting.FileSystemObject")
If configfileFSO.DriveExists("c:\") Then
If configfileFSO.FileExists("c:\install\" & "RAPsavedreg" & ".cfg") Then configfileFSO.DeleteFile("c:\install\" & "RAPsavedreg" & ".cfg")
Set configfile = configfileFSO.CreateTextFile("c:\install\" & "RAPsavedreg" & ".cfg")
sampleline = "[config]" 
configfile.writeline(sampleline)
sampleline = "CompName=" & savedcomputername
configfile.writeline(sampleline)
sampleline = "ScannerClient=" & SavedScannerClient
configfile.writeline(sampleline)

end if
end sub
'***************************configfile function******************************


'*************************************************************************
'  MAIN PROCESSING SECTION
'*************************************************************************



'  Save Registry entries

reg_save

'  Write Saved Config File

saveregwrite savedcomputername, savedscannerclient
wscript.quit