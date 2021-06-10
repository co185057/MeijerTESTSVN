'******************************************************************************* 
'Customer: Meijer
'Script: RAP_reg_reset.vbs
'Purpose: Resets the specific registry entries
'Author:  Dennis Shovelin
'Created: 9/2007
'*******************************************************************************
' Change Control

'************END CHANGE CONTROL*************************************************

Set oShell = CreateObject ("Wscript.shell")

'*******************************************************************************
'Setup Variables
'*******************************************************************************

Dim File, oshell
Dim Section
Dim Key
Dim savedcomputername
Dim savedScannerClient

dim configfilefound

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


file=""
section=""
key=""
value= ""



'**************************************************************************
'  Update Registry for Installation Subroutine
'**************************************************************************
sub reg_update()


With oShell
'	
 	.RegWrite sCompNameRegPath & "ComputerName\ComputerName", savedcomputername
 	.RegWrite sCompNameRegPath & "ActiveComputerName\ComputerName", savedcomputername
 	.RegWrite sTcpipParamsRegPath & "Hostname", savedcomputername
 	.RegWrite sTcpipParamsRegPath & "NV Hostname", savedcomputername
 	.RegWrite sTcpipParamsRegPath & "NV Hostname", savecomputername
'  RAP Updates
	.RegWrite RAPSCANNERPATH & "ClientName", savedscannerclient
End With

end sub

'*******************End reg_update Subroutine***********************************

'*************************************************************************
'    Read Ini Function
'*************************************************************************
Function ReadINI(file, section, key)
readini = ""
set FileSysObj = CreateObject("Scripting.FileSystemObject")

If FileSysObj.FileExists(file) then
	Set ini = FileSysObj.OpenTextFile( file, 1, False)

	' Return array of sections if section and keys are empty
	if section="" then
		set reSection		=new RegExp
		reSection.Global    =True
		reSection.IgnoreCase=True
		reSection.Pattern   ="\[([a-zA-Z0-9 ]*)\]"

		Do While ini.AtEndofStream =False
			line = ini.ReadLine

			if reSection.Test(line) then
				tempSection=tempSection & reSection.Replace(line, "$1") & ","
			end if
		loop
		ini.close
		tempSection=left(tempSection, len(tempSection)-1)
		ReadINI=split(tempSection,",")
		set reSection=nothing
		exit function
	end if 
' Return array of keys if keys are empty
	if key="" then

		set reSection		=new RegExp
		reSection.Global    =True
		reSection.IgnoreCase=True
		reSection.Pattern   ="\s*\[\s*" & section & "\s*\]"

		Do While ini.AtEndofStream =False
			line = ini.ReadLine

			if reSection.Test(line) then
				line=ini.ReadLine

				do while instr(line,"[")=0
					tempKeys=tempKeys & trim(left(line,instr(line,"=")-1)) & ","
					line=ini.ReadLine
				loop
				tempKeys=left(tempKeys,(len(tempkeys)-1)) ' Remove last comma
				ReadINI =split(tempKeys,",")
				exit function
			end if
		loop
	end if 
'=========================================================================

' READINI Part for file, section, key

	set reSection		=new RegExp
	reSection.Global    =False
	reSection.IgnoreCase=True
	'reSection.Pattern   ="\s*[\s*" & section & "\s*]"
	reSection.Pattern   ="\s*\[\s*" & section & "\s*\]"

	set reKey 		=new RegExp
	reKey.Global	=False
	reKey.IgnoreCase=True
	reKey.Pattern="\s*" & key & "\s*=\s*"


	Do While ini.AtEndofStream = False

		line = ini.ReadLine

			if reSection.Test(line) = True then

				line=ini.ReadLine
				do while instr(line,"[")=0

					if reKey.Test(line) then

						ReadINI=trim(mid(line,instr(line,"=")+1))

						exit do
					end if

					line=ini.ReadLine
				Loop

		exit do
		end if
	loop
	ini.Close
	set reSection=nothing
	set reKey    =nothing
end if ' If FileSysObj

end function

'***************************End readini function******************************

'*****************************************************************************
' WRITEINI ( file, section, item, value )
' file = path and name of ini file
' section = [Section] must be in brackets in the ini file
' key = the variable to read;
' ispaddr = the value to assign to the item.
'******************************************************************************
Sub WriteIni( file, section, key, value )
set FileSysObj = CreateObject("Scripting.FileSystemObject")
in_section = False
section_exists = False


item_exists = (ReadIni( file, section, key )  <> "")

wrote = False
path = Mid( file, 1, InStrRev( file, "\" ) )
if FileSysObj.FileExists(path & "temp_ini.ini") then 
     FileSysobj.DeleteFile(path & "temp_ini.ini")
end if
Set read_ini = FileSysObj.OpenTextFile( file, 1, True, TristateFalse ) 
Set write_ini = FileSysObj.CreateTextFile( path & "temp_ini.ini", False )

set reWSection		 =new RegExp
reWSection.Global	 =False
reWSection.IgnoreCase=True
reWSection.Pattern	 ="\s*[\s*" & section & "\s*]"

set reItem =new RegExp
reItem.Global =False
reItem.IgnoreCase=True
reItem.Pattern ="\s*" & key & "\s*="

While read_ini.AtEndOfStream = False
line = read_ini.ReadLine

     If wrote = False Then
          If reWSection.Test(line) Then
               section_exists = True
               in_section = True
          ElseIf InStr( line, "[" )> 0 Then
               in_section = False
          End If
     End If

     If in_section Then
          If item_exists = False Then
               write_ini.WriteLine line
               write_ini.WriteLine key & "=" & value
               wrote = True
               in_section = False
               msgbox "Writing " & line
               ElseIf reItem.Test(line) Then
                    write_ini.WriteLine key & "=" & value
                    wrote = True
                    in_section = False
          Else
               write_ini.WriteLine line
          End If
     Else
          write_ini.WriteLine line
     End If
Wend

If section_exists = False Then ' section doesn't exist
     section=trim(section)
     item	=trim(item)

     write_ini.WriteLine
     write_ini.WriteLine "[" & section & "]"
     write_ini.WriteLine key & "=" & value
End If

read_ini.Close
write_ini.Close
FileSysObj.DeleteFile file
FileSysObj.MoveFile path & "temp_ini.ini", file
set reWSection=nothing
set reItem=nothing

End Sub

'******************End writeini subroutine**************************

'**********************************************************************
'  Config File read
'**********************************************************************
'	This subroutine is used to read a configuration file for 
'	fastlane_setup.vbs.  It accepts 1 argument which is as follows:
'		
'
sub configflrd()

Dim sampleline
Dim configfileFSO, configfile
Set configfileFSO = WScript.CreateObject("Scripting.FileSystemObject")
If configfileFSO.DriveExists("c:\") Then
	If configfileFSO.FileExists("c:\install\" & "RAPsavedreg" & ".cfg") Then 
                configfilefound = "Y"
        else
                configfilefound = "N"
	end if
end if
end sub
'**********************************************************************


'*************************************************************************
'  MAIN PROCESSING SECTION
'*************************************************************************

'  Read config file
configflrd

If configfilefound = "Y" then

'****************GET computer name
	file = "c:\install\" & "RAPsavedreg" & ".cfg"
	section= "config"
	key= "CompName"
	savedcomputername = readini(file, section, key)

'****************GET Scanner Client
	file = "c:\install\" & "RAPsavedreg" & ".cfg"
	section= "config"
	key= "ScannerClient"
	savedScannerClient = readini(file, section, key)



	 reg_update


 else
 
 msgbox  "Configuration File Not Found"
 
 end if
 
wscript.quit