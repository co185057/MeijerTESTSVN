'******************************************************************************* 
'Customer: Meijer
'Script: FastLane_setup.vbs
'Purpose: Automates the tailoring of FastLanes to the specific store environment
'Author:  Dennis Shovelin
'Created: 3/2006
'*******************************************************************************
'History:
'POS245658                     Name:Kyle Hammang        Date: February 1, 2016
'POS77412  Work Request:58657  Name:Matt Condra         Date: September 24, 2013
'POS33327  Work Request:50864  Name:Robert Susanto      Date: July 31, 2012
'POS38137  Work Request:50864  Name:Robert Susanto      Date: July 19, 2012
'POS33327  Work Request:50864  Name:Robert Susanto      Date: July 19, 2012
'POS15793  Work Request:15196  Name:Matt Condra         Date: July 15, 2011
'POS14441  Work Request:15196  Name:Matt Condra         Date: June 10, 2011
'POS10658  Work Request:15196  Name:Matt Condra         Date: June 10, 2011
'POS10658  Work Request:15196  Name:Matt Condra         Date: June 8, 2011
'*******************************************************************************
' Change Control
' 3/19/2007	Changed the RAP prompting subroutine to accept RAP numbes from
'               1 to 4.
' 5/21/2007	Added the subroutine to prompt for Lane Type.
'		Added the code to execute tab_setup.bat which will execute the 
'		convert-to-tab process.
' 8/28/2007 Change the RAP prompting subroutine to accept RAP numbers from 
'		     1 to 9.
' 1/12/2009  -	Steve Antonakakis - Modified LaunchPad.xml modication code to merge changes  
'			to existing LaunchPad.xml document instead of replacing. Script will add default 
'			setting values when settings are missing. It will also update the Lanes section 
'			reusing blank Lane/Host entries when available.
' 3/31/2009 Dennis Shovelin - replaced FileTransportService to use secondery server name
'           instead of IP address
' 6/30/2014 -Rick Ruble - On PosReady7, Sub get_ipaddr, pulling Mac address instead of IPAddress. Re-wrote routine
' 1/8/2015 -Kyle Hammang - On PosReady7, added code to call takeawaybelt_enable and disable
' 2/1/2016 -Kyle Hammang - Added section to install R4 trilight drivers on 7343 lanes
' 10/10/2016 -Kyle Hammang - Modified to delete all AJBFIPAY directories from fastlane when setup is run
' 12/21/16 -Kyle Hammang - Added step to reinstall VNC if lane is R6
'************END CHANGE CONTROL*************************************************

'*******************************************************************************
'Setup Variables
'*******************************************************************************

Dim oShell
Dim CI, s1, s2, sPath, A1, i, r, A2
Dim launchpad
Dim sampleline
Dim ObjFSO
Dim File
Dim Section
Dim Key
Dim savedcomputername
dim savedprimaryname
dim savedsecondaryname
dim savedRAPname1
dim savedRAPname2
dim savednumberofRAPS
dim savedFastLaneNumber1
dim savedFastLaneNumber2
dim savedFastLaneNumber3
dim savedFastLaneNumber4
Dim servermsg
Dim title 
dim style 
dim response 
dim strnewcompname
dim DNSADDRprimary
dim DNSADDRsecondary
dim WINSADDR 
dim macaddress
dim newPOS_NO
dim first_digit
dim second_digit
DIM Pos_NO
Dim STORE_NUM
Dim ispaddr
DIM RAP_NO
Dim input_lane_type
Dim fs
Dim LaneType
Dim regValue

Set fs = CreateObject("Scripting.FileSystemObject")
Set oShell = CreateObject ("Wscript.shell")


'change 9/27-  add command line arguments to script Lane/store/type from Deployment Server  NDR - ITS


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
ipaddr = ""

systemControl= "HKLM\SYSTEM\"

DNSADDRprimary = ""
DNSADDRsecondary = ""
WINSADDR = ""
primaryserver = ""
secondaryserver = ""

file=""
section=""
key=""
value= ""
savedcomputername = ""
POS_NO = ""
STORE_NUM = ""
securityserver = ""
rptserver = ""
ispaddr = ""

'*******************************************************************************
'   Prompt for store number Subroutine
'*******************************************************************************
sub get_store()
if wscript.Arguments.Count = 4 then 
  STORE_NUM = wscript.Arguments.Item(0)
else
Do
GoodInput = "Y"
STORE_NUM = InputBox("Enter Store Number: Must be 4 digits Leading Zeroes", "Store Number", "1")
TESTLEN = len(STORE_NUM)

  if TESTLEN = 0 then
		Error1=MsgBox("Store Number must be entered", 48, "Error!")
		wscript.quit
	end if
	if STORE_NUM = "" then
		 Error1=MsgBox("Store Number must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	if TESTLEN < 4 then
		Error1=MsgBox("Store Number must be 4 digits Leading Zeroes", 48, "Error!")
		GoodInput = "N"
		end if
	if TESTLEN > 4 then
		Error1=MsgBox("Store Number must be 4 digits Leading Zeroes", 48, "Error!")
		GoodInput = "N"
	end if
  Loop Until GoodInput = "Y"

end if

end sub

'******************End get_store subroutine*********************************


'*******************************************************************************
'   Prompt for terminal number Subroutine
'*******************************************************************************

sub get_term()
if wscript.Arguments.Count = 4 then 
  Pos_NO = wscript.Arguments.Item(1)
else
Do
GoodInput = "Y"
Lanefound = "N"
msgin = "Enter Terminal Number - 3 digit Examples: 021, 121"
POS_NO = InputBox(msgin, "FastLane Setup - Fastlane Number", "999")

TESTLEN = len(POS_NO)
	if TESTLEN = 0 then
		Error1=MsgBox("Terminal Number must be entered", 48, "Error!")
		wscript.quit
	end if
	if TESTLEN >3 then
		Error1=MsgBox("Terminal Number must be 3 digits", 48, "Error!")
		GoodInput = "N"
	end if
	if TESTLEN < 2 then
		Error1=MsgBox("Terminal Number must be 3 digits", 48, "Error!")
		GoodInput = "N"	
	end if
  goodinput = "Y"

Loop Until GoodInput = "Y"
end if

end sub


'******************End get_term subroutine*********************************

'*******************************************************************************
'   Prompt for RAP number Subroutine
'*******************************************************************************

sub get_RAP()
if wscript.Arguments.Count = 4 then 
  RAP_NO = wscript.Arguments.Item(2)
else
  Do
  GoodInput = "Y"
  RAPfound = "N"
  msgin = "Enter This Terminals RAP Number - Valid entries are 1 to 9"
  RAP_NO = InputBox(msgin, "FastLane Setup - RAP Number", "1")

  TESTLEN = len(RAP_NO)
	if TESTLEN = 0 then
		Error1=MsgBox("RAP Number must be entered", 48, "Error!")
		wscript.quit
	end if
	if TESTLEN >1 then
		Error1=MsgBox("RAP Number must be 1 digits", 48, "Error!")
		GoodInput = "N"
	end if
	if RAP_NO > "9" then
		Error1=MsgBox("RAP Number must be 1 to 9", 48, "Error!")
		GoodInput = "N"	
	end if
  goodinput = "Y"

  Loop Until GoodInput = "Y"
end if

end sub


'******************End get_RAP subroutine*********************************
'*******************************************************************************
'   Prompt for  Lane Type Subroutine
'   POS10658: Fixed input_lane_type conditional so that selection 3 is an error.
'*******************************************************************************

sub prompt_Lane_Type()
if wscript.Arguments.Count = 4 then 
  input_lane_type = wscript.Arguments.Item(3)
else
  Do
  GoodInput = "Y"
  input_lane_type = InputBox("Enter Lane Type: Valid Entries are:" & _
  		vbCrLf & vbCrLf & _
		"1) SCAN & BAG" & vbCRLf & _
		"2) TAB" & vbCrLf & _
		"3) PAYMENT STATION" & vbCrLf & _
		"Please select Terminal Type" , "Fastlane Type", "0")
TESTLEN = len(input_lane_type)
	if TESTLEN = 0 then
		wscript.quit
	end if
	if input_lane_type = 0 then
		 Error1=Msgbox("Lane Type must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	if input_lane_type = "" then
		 Error1=MsgBox("Lane Type must be entered", 48, "Error!")
		GoodInput = "N"
	end if
 	if input_lane_type > "3" then
 		'POS77412: Updated error message to include 3.
 		Error1=MsgBox("Lane Type must be 1, 2, or 3", 48, "Error!")
 		GoodInput = "N"
 	end if
  Loop Until GoodInput = "Y"

end if	

end sub

'******************End  Lane Type subroutine*********************************
'******************Build Computer Name
sub Build_Computer_name()


strNewCompName = "Store" & STORE_NUM & "POS" & POS_NO

' sotf 10784-10785 - moving security db to 202 server - rrs
'securityserver = "W" & STORE_NUM & "EPOSS0002"
securityserver = "Store" & STORE_NUM & "RAP" & RAP_NO

rptserver = "Store" & STORE_NUM & "RAP" & RAP_NO

end sub

'**************************************************************************
'  Update Registry for Installation Subroutine
'**************************************************************************
sub reg_update()

'Delete all of the Store Minder Tallies for predictive services

    Dim fso, MyFile
    Set fso = CreateObject("Scripting.FileSystemObject")
    Set MyFile = fso.CreateTextFile("c:\temp\DeleteTallies.bat", True)
    MyFile.Write("REG DELETE ")
    MyFile.Write(chr(34))
    MyFile.Write("HKLM\SOFTWARE\NCR\NCR Store Minder Client\Tally")
    MyFile.Write(chr(34))
    MyFile.Write(" /f")

    MyFile.Close

    Set oShell = CreateObject ("Wscript.shell")
    oShell.Run "c:\temp\DeleteTallies.bat", 0, True

    fso.DeleteFile("c:\temp\DeleteTallies.bat")

With oShell
'	
 	.RegWrite sCompNameRegPath & "ComputerName\ComputerName", strNewCompName
 	.RegWrite sCompNameRegPath & "ActiveComputerName\ComputerName", strNewCompName
 	.RegWrite sTcpipParamsRegPath & "Hostname", strNewCompName
 	.RegWrite sTcpipParamsRegPath & "NV Hostname", strNewcompName
 	.RegWrite sTcpipParamsRegPath & "NV Hostname", strNewcompName
'  SCOT Security Server and Report Server Registry Updates

' Moving the security server to 202 server - sotf 10784/10785 - rrs
 	.RegWrite scotIpath & "CoreSecurityServer\ServerName", securityserver

 	.Regwrite scotIpath & "CoreReportServer\ReportServerName", RptServer

' Moving the security server to 202 server - sotf 10784/10785 - rrs
        .Regwrite scotCpath & "SCOTAPP\ClientName", securityserver

 	.Regwrite scotCpath & "SCOTAPP\Reporting\ReportServer", RptServer

' Moving the security server to 202 server - sotf 10784/10785 - rrs
 	.Regwrite scotCorepath & "ObservedOptions\ServerName", securityserver

        .Regwrite scotCorepath & "ObservedOptions\ReportServerName", RptServer
'  ACS Specific parameters
        .Regwrite scotCpath & "SCOTTB\TerminalNumber", POS_NO
'  Microsoft FBRESEAL WINLOGON AUTO LOGON reset
 	.Regwrite "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon\AutoAdminLogon", "1"
'  Write out store number for predictive services 
   .Regwrite "HKLM\Software\NCR\SCOT\StoreNumber", STORE_NUM
   .Regwrite "HKLM\Software\NCR\SCOT\RetailerName", "Meijer"
   .Regwrite "HKLM\Software\NCR\SCOT\TerminalIDNumber", POS_NO
   
  ' POS304890
 	.Regwrite scotCorepath & "ObservedOptions\MessageBrokerIP", securityserver 
	.Regwrite systemControl & "ControlSet001\Control\Session Manager\Environment\MessageBrokerIP", securityserver  
	.Regwrite systemControl & "ControlSet002\Control\Session Manager\Environment\MessageBrokerIP", securityserver  
	.Regwrite systemControl & "CurrentControlSet\Control\Session Manager\Environment\MessageBrokerIP", securityserver 
   
End With

end sub

'*******************End reg_update Subroutine***********************************


'*******************************************************************************
'  Get IP ADDRESS Subroutine
'*******************************************************************************


''=========================
Sub get_ipaddr()
'Get IPaddress info on XP and Win7 machines
'Assuming there is only one network card
''=========================
	
    Dim oShellExec, oWshShell
    Dim sLine, sTmpIPAddress, sIPAddress, sMacAddress
    Dim iResult

    Set oWshShell = CreateObject("WScript.Shell")
    Set oShellExec = oWshShell.exec("cmd /c " & "IPConfig /All ")

	  Do While Not oShellExec.StdOut.AtEndOfStream
      sLine = oShellExec.StdOut.ReadLine
      sLine = Replace(sLine, Chr(10), "") 'Remove LF
      sLine = Replace(sLine, Chr(13), "") 'Remove CR
      Select Case True
        Case InStr(1, sLine, "IP Address", 1) >0 'xp
          sIPAddress = Mid(sLine, InStr(sLine,":")+2)
        Case InStr(1, sLine, "IPv4 Address", 1) >0 'win7
          sIPAddress = Mid(sLine, InStr(sLine,":")+2)
          sIpAddress = Trim(Replace(sIpAddress, "(Preferred)", ""))
        Case InStr(1, sLine, "Physical Address", 1) >0 'both
          If Len(sMacAddress) <16 Then
            sMacAddress = Mid(sLine, InStr(sLine,":")+2)
          End If
      End Select
    Loop

    Select Case True
      Case Len(sIPAddress) <9 , Len(sMacAddress) <9
        MsgBox "ERROR: FAILED TO RETRIEVE IP or MACADDRESS. VERIFY NETWORK CONNECTION & RE_RUN THIS SCRIPT"
        Wscript.Quit
    End Select  

    primaryserver = Mid(sIPAddress, 1, InStrRev(sIPAddress, ".")) & "201"
    secondaryserver = Mid(sIPAddress, 1, InStrRev(sIPAddress, ".")) & "202"
    ispaddr = Mid(sIPAddress, 1, InStrRev(sIPAddress, ".")) & "202"

    Set oShellExec = Nothing

'MsgBox "sIPAddress>" & sIPAddress & "<" & " " & Len(sIPAddress)
'MsgBox "WIN7_sMacAddress>" & sMacAddress & "<" & " " & Len(sMacAddress)    
'MsgBox "primaryserver>" & primaryserver & "<" & " " & Len(primaryserver)
'MsgBox "secondaryserver>" & secondaryserver & "<" & " " & Len(secondaryserver)
'MsgBox "ispaddr>" & ispaddr & "<" & " " & Len(ispaddr)

End Sub


'Re-wrote routine for PosReady7. 6/30/14 Rick Ruble
'sub get_ipaddr()
'
'set IPConfigSet = GetObject("winmgmts:{impersonationLevel=impersonate}").ExecQuery _
'  ("select IPAddress from Win32_NetworkAdapterConfiguration where IPEnabled=TRUE")
'
'for each IPConfig in IPConfigSet
' if Not IsNull(IPConfig.IPAddress) then 
'  for i=LBound(IPConfig.IPAddress) to UBound(IPConfig.IPAddress)
'  ipaddr = IPConfig.IPAddress(i)
''  msgbox ipaddr
'  next
' end if
'next
'
'
'
''WScript.Echo IPaddr
'    ' finds the length between the 1st and 2nd octet (eg. 168.244.51.200) is only 4 chars to the "."
'    location1 = InStr(IPaddr, ".")
'    firstip = Mid(Ipaddr, 1)
'    firstoctet = left(firstip, (location1 - 1))
'
'    ' pulls off the first octet and the "." (eg. leaves 244.51.200)
'    secIP = Mid(IPaddr, (location1 + 1))
'    
'    ' finds the length of the 2nd octet (eg. gets that 244 is only 3 chars)
'    temp = InStr(secIP, ".")
'    ' strips off the second octet from the IP address (eg. returns 244)
'    SECoctet = Left(secIP, (temp - 1))
'    'MsgBox "this should be just the second octet " & SECoctet
'    ' finds the length between the 2nd and 3rd octet (eg. 168.244.51.200) is only 3 chars to the "."
'    location2 = InStr(secIP, ".")
'    ' pulls everything but the 3rd and 4th octet (eg. 51.200)
'    thirdIP = Mid(secIP, (location2 + 1))
'    ' finds the length of the third octet (eg. gets that 51 is only 2 chars)
'    temp = InStr(thirdIP, ".")
'    ' strips off the second octet from the IP address (eg. returns 51)
'    THIRDoctet = Left(thirdIP, (temp - 1))
'    'MsgBox "this should be the third octet " & THIRDoctet
'    'gets the length of the 3rd octet
'    location3 = InStr(thirdIP, ".")
'    ' pulls out the last octet
'    fourthIP = Mid(thirdIP, (location3 + 1))
'
'primaryserver = firstoctet & "." & secoctet & "." & thirdoctet & "." & "201"
'secondaryserver = firstoctet & "." & secoctet & "." & thirdoctet & "." & "202"
'ispaddr = firstoctet & "." & secoctet & "." & thirdoctet & "." & "202"
'
'end sub

'***********************End get_ipdaddr subroutine************************
'*******************************************************************************
'  Get MACADDRESS Info Subroutine
'*******************************************************************************
sub get_MACaddress()

strComputer = "."
Set objWMIService = GetObject("winmgmts:" _
 & "{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")
Set colNicConfigs = objWMIService.ExecQuery _
 ("SELECT * FROM Win32_NetworkAdapterConfiguration WHERE IPEnabled = True")
  
For Each objNicConfig In colNicConfigs
 
  Set colNics = objWMIService.ExecQuery _
   ("ASSOCIATORS OF " & "{Win32_NetworkAdapterConfiguration.Index='" & _
   objNicConfig.Index & "'}" & _
   " WHERE AssocClass=Win32_NetworkAdapterSetting")
 
  For Each objNic in colNics
     macaddr = objNic.MACAddress
 '   msgbox macaddr
 
  Next
 
Next


end sub



'***********************End get_ipconfig subroutine************************


'**********************************************************************
'  LaunchPad XML file write
'**********************************************************************
sub launchpadxml()
Set objFSO = WScript.CreateObject("Scripting.FileSystemObject")

If objFSO.FileExists("c:\scot\config\launchpad.xml") Then objFSO.DeleteFile("c:\scot\config\launchpad.xml")
Set launchpad = objFSO.CreateTextFile("c:\scot\config\launchpad.xml")
sampleline = "<?xml version=""1.0""?>"
launchpad.writeline(sampleline)
sampleline = "<RapNet>"
launchpad.writeline(sampleline)
sampleline = "     <!--  This is the list of connected Fastlanes -->"
launchpad.writeline(sampleline)
sampleline = "          <Lanes>"
launchpad.writeline(sampleline)
sampleline = "             <Lane>"
launchpad.writeline(sampleline)
sampleline = "                 <Host>" & strNewCompName & "</Host>"
launchpad.writeline(sampleline)
sampleline = "             </Lane>"
launchpad.writeline(sampleline)
sampleline = "             <Lane>"
launchpad.writeline(sampleline)
sampleline = "                 <Host></Host>"
launchpad.writeline(sampleline)
sampleline = "             </Lane>"
launchpad.writeline(sampleline)
sampleline = "             <Lane>"
launchpad.writeline(sampleline)
sampleline = "                 <Host></Host>"
launchpad.writeline(sampleline)
sampleline = "             </Lane>"
launchpad.writeline(sampleline)
sampleline = "             <Lane>"
launchpad.writeline(sampleline)
sampleline = "                 <Host></Host>"
launchpad.writeline(sampleline)
sampleline = "             </Lane>"
launchpad.writeline(sampleline)
sampleline = "          </Lanes>"
launchpad.writeline(sampleline)
sampleline = "          <Options>"
launchpad.writeline(sampleline)
sampleline = "             <!-- Use Alphanumeric keyboard for signon -->"
launchpad.writeline(sampleline)
sampleline = "             <AlphaNumericKeyboard>false</AlphaNumericKeyboard>"
launchpad.writeline(sampleline)
sampleline = "             <!-- Timeout in ms for signon operation -->"
launchpad.writeline(sampleline)
sampleline = "             <SignOnTimeout>5000</SignOnTimeout>"
launchpad.writeline(sampleline)
sampleline = "             <!-- Time in ms between Psx ConnectRemote invocations -->"
launchpad.writeline(sampleline)
sampleline = "             <ConnectionPeriod>5000</ConnectionPeriod>"
launchpad.writeline(sampleline)
sampleline = "             <!-- Timeout in ms for ConnectRemote operation -->"
launchpad.writeline(sampleline)
sampleline = "             <ConnectRemoteTimeout>5000</ConnectRemoteTimeout>"
launchpad.writeline(sampleline)
sampleline = "             <!-- Timeout in ms for the application to shutdown. If the application is not dead after this time, call process.kill(). -->"
launchpad.writeline(sampleline)
sampleline = "             <ShutDownTimeout>180000</ShutDownTimeout>"
launchpad.writeline(sampleline)
sampleline = "             <!-- Synchronize our local time with FastLane at signon -->"
launchpad.writeline(sampleline)
sampleline = "             <SynchronizeTimeServer>false</SynchronizeTimeServer>"
launchpad.writeline(sampleline)
sampleline= "           </Options>"
launchpad.writeline(sampleline)
sampleline= "</RapNet>"
launchpad.writeline(sampleline)

end sub
'***************************Launchpad function******************************

'**********************************************************************
'  RCMCONFIG.000 XML file write
'**********************************************************************
sub rcmconfig000()

first_digit = mid(POS_NO,1,1)
second_digit = mid(pos_no,2,1)
IF FIRST_DIGIT = 0 THEN
NEWpos_no = MID(pos_no,2,2)
	IF SECOND_DIGIT = 0 then
		NEWPOS_NO = mid(POS_NO,3,1)
	end if
else
newPOS_NO = POS_NO
end if

Set objFSO = WScript.CreateObject("Scripting.FileSystemObject")

If objFSO.FileExists("c:\scot\config\RCMconfig.000") Then objFSO.DeleteFile("c:\scot\config\RCMconfig.000")
Set rcmconfig = objFSO.CreateTextFile("c:\scot\config\RCMconfig.000")
sampleline = "<!-- ================================================================================ -->"
rcmconfig.writeline(sampleline)
sampleline = " <!--                                                                                  -->"
rcmconfig.writeline(sampleline)
sampleline = "<!-- File: RCMConfig.xml                                                         -->"
rcmconfig.writeline(sampleline)
sampleline = " <!--                                                                                  -->"
rcmconfig.writeline(sampleline)
sampleline = "<!-- Function: Provides the set of user-configurable parameters for the Remote Console -->"
rcmconfig.writeline(sampleline)
sampleline = "<!--    Manager component of the FastLane.                                            -->"
rcmconfig.writeline(sampleline)
sampleline = "<!--                                                                                  -->"
rcmconfig.writeline(sampleline)
sampleline = "<!-- Notes:                                                                           -->"
rcmconfig.writeline(sampleline)
sampleline = "<!--                                                                                  -->"
rcmconfig.writeline(sampleline)
sampleline = "<!--    1.  All timeout values are in milliseconds, ie. 1000 = 1s                     -->"
rcmconfig.writeline(sampleline)
sampleline = "<!--    2.  All weight values are in thousandths of a pound, ie.                     -->"
rcmconfig.writeline(sampleline)
sampleline = "<!--        1000 = 1lb                                                                -->"
rcmconfig.writeline(sampleline)
sampleline = "<!--                                                                                  -->"
rcmconfig.writeline(sampleline)
sampleline = "<!-- ================================================================================ -->"
rcmconfig.writeline(sampleline)
sampleline = "<rcm>"
rcmconfig.writeline(sampleline)
sampleline = "        <lane-name>" & newPOS_NO & "</lane-name>"
rcmconfig.writeline(sampleline)
sampleline = "        <assist-mode-control>3</assist-mode-control>"
rcmconfig.writeline(sampleline)
sampleline = "</rcm>"
rcmconfig.writeline(sampleline)
end sub

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
sampleline = "          <Monitor SourceMask=""C:\temp\*.zip"" Destination=""\\" & savedsecondaryname & "\temp\fastlane"" Move=""yes"" DestUser=""acs"" DestPass=""unity1"" />"
filetransport.writeline(sampleline)
sampleline= "     </Monitors>"
filetransport.writeline(sampleline)
sampleline = "</FileTransportService>"
filetransport.writeline(sampleline)

end sub

'**********************************************************************
'  XML file manipulation helper function - Steve Antonakakis
'**********************************************************************
Sub launchpadxml_update()
	
	Const launchPadConfigFile = "c:\scot\config\LaunchPad.xml"
	'Create the stylesheet to format the output file
	Dim sStylesheet : sStylesheet = _
			"<xsl:stylesheet version=""1.0"" xmlns:xsl=""http://www.w3.org/1999/XSL/Transform"">" & _
				"<xsl:output method=""xml""/>" & _
				"<xsl:param name=""indent-increment"" select=""'    '"" />" & _
				"<xsl:template match=""*|comment()|processing-instruction()"">" & _
					"<xsl:param name=""indent"" select=""'&#xA;'""/>" & _
					"<xsl:value-of select=""$indent""/>" & _
					"<xsl:copy>" & _
						"<xsl:copy-of select=""@*"" />" & _
						"<xsl:apply-templates>" & _
							"<xsl:with-param name=""indent"" select=""concat($indent, $indent-increment)""/>" & _
						"</xsl:apply-templates>" & _
						"<xsl:if test=""*"">" & _
							"<xsl:value-of select=""$indent""/>" & _
						"</xsl:if>" & _
					"</xsl:copy>" & _
				"</xsl:template>" & _
				"<xsl:template match=""text()[normalize-space(.)='']""/>" & _
			"</xsl:stylesheet>"

	Dim oDoc : Set oDoc	= CreateObject("MSXML2.DomDocument.4.0")
	Dim oStyle : Set oStyle = CreateObject("MSXML2.DomDocument.4.0")
	Dim oFSO : Set oFSO = CreateObject("Scripting.FileSystemObject")			

	'to use the XSLT utilities 
	oDoc.setProperty "SelectionNamespaces", "xmlns:ms='urn:schemas-microsoft-com:xslt'"
	
	'Load the formatting stylesheet
	Dim bStyle : bStyle = oStyle.loadXML(sStyleSheet)

	'Merge required settings to LaunchPad
	If oDoc.load(launchPadConfigFile) Then
		'WScript.Echo "Updating " & launchPadConfigFile & "..."
		Dim bRet
		bRet = XmlAddLaneIfMissing(oDoc, strnewcompname)
		bRet = bRet or XmlAddOption(oDoc, "AlphaNumericKeyboard", "false", "Use Alphanumeric keyboard for signon", false) 
		bRet = bRet or XmlAddOption(oDoc, "SignOnTimeout", "5000", "Timeout in ms for signon operation", false)
		bRet = bRet or XmlAddOption(oDoc, "ConnectionPeriod", "5000", "Time in ms between Psx ConnectRemote invocations", false)
		bRet = bRet or XmlAddOption(oDoc, "ConnectRemoteTimeout", "5000", "Timeout in ms for ConnectRemote operation", false)
		bRet = bRet or XmlAddOption(oDoc, "ShutDownTimeout", "180000", "Timeout in ms for the application to shutdown. If the application is not dead after this time, call process.kill()", false)
		bRet = bRet or XmlAddOption(oDoc, "SynchronizeTimeServer", "false", "Synchronize our local time with FastLane at signon", false)
		bRet = bRet or XmlAddOption(oDoc, "ScanSound", "Scan.wav", "Sound played when a barcode is scanned. Silent if empty", false)
		bRet = bRet or XmlAddOption(oDoc, "SignOnTBFirst", "true", "Signon via TB first: true ( default ) means registry will only be checked after TB signon receive times out or TB send fails to all lanes or no Lanes defined", false)
		bRet = bRet or XmlAddOption(oDoc, "AltF4ExitAllowed", "true", "Allow the Alt-F4 keystroke entry to exit the application? (default is true)", false)
		bRet = bRet or XmlAddOption(oDoc, "LegalCaptionRequired", "false", "Is it required to display the legal caption prior to the sign on menu? (default is false)", false)
		bRet = bRet or XmlAddOption(oDoc, "HashAlgorithm", "NONE", "Hash algorith to use when validating password in swusers/maintusers registry. Desktop possible values: NONE, SHA1, MD5, SHA256, SHA384, and SHA512 (default is NONE). HH possible values: NONE and SHA256(default is NONE).", false)
		bRet = bRet or XmlAddOption(oDoc, "NcrRemoteScanner", "SOFTWARE\NCR\NcrLaunchPadScanner", "Launch Pad scanner profile (default: SOFTWARE\NCR\NcrRemoteScanner)", true)
		bRet = bRet or XmlAddOption(oDoc, "UtilityReturnAlwaysEnabled", "true", "Allow return to ScotApp/RapNet when a utility executable is running? (default is false).", true)
		bRet = bRet or XmlAddOption(oDoc, "BiometricDeviceConfigured", "false", "Has a Biometric device? (default is false)", true)
		bRet = bRet or XmlAddOption(oDoc, "LaunchPadRAPAutoSignOffSeconds", "0", "Auto sign off RAP and Launchpad in seconds (default is 0, disabled)", false)
		bRet = bRet or XmlAddOption(oDoc, "UseBiometrics", "false", "Use UseBiometrics Login", false)
		bRet = bRet or XmlAddOption(oDoc, "AllowEmplIdSignOn", "true", "AllowEmplIdSignOn for login", true) 
		bRet = bRet or XmlAddOption(oDoc, "AllowHandKeyEMPLID", "false", "AllowHandKeyEMPLID for login", true)
		bRet = bRet or XmlAddOption(oDoc, "ScannerClass", "CommonLibsNet.ScannerShared", "The scanner class to be used. Options: ScannerShared, ScannerThread", false)

		If bRet Then
			Dim sOut : sOut = ""
			If bStyle Then
				sOut = oDoc.transformNode(oStyle)
			End If
			If sOut <> "" Then
				'WScript.Echo "launchpadxml_update(): Formatting XML file and saving."
				Dim oFile : Set oFile = oFSO.CreateTextFile(launchPadConfigFile, true, false)
				oFile.Write(sOut)
				oFile.Close
			Else
				'WScript.Echo "launchpadxml_update(): Saving XML file (no formatting)."
				oDoc.save(launchPadConfigFile)
			End If
		Else
			'WScript.Echo "launchpadxml_update(): All required options already exist in XML file."
		End If
	Else
		WScript.Echo "launchpadxml_update(): Failed to load XML file: " & launchPadConfigFile
	End If	
	

End Sub

'*==================================================================
'* Name         : IsProcessRunning
'* In           : Process name (look in task manager)
'* Out          : True/False
'* Purpose      : Check if a process is running
'* Comment      : POS15793
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
'* Comment         : POS15793
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

'**********************************************************************
'  XML file manipulation to update Lane/host entries - Steve Antonakakis
'**********************************************************************
Function XmlAddLaneIfMissing(oDoc, sLaneName)
	XmlAddLaneIfMissing = False
	Dim oLanes: Set oLanes = oDoc.selectSingleNode("/RapNet/Lanes")
	Dim oHost : Set oHost = oDoc.selectSingleNode("/RapNet/Lanes/Lane/Host[ms:string-compare(text(),'" & sLaneName & "', 'en-US', 'i')=0]")
	If oHost Is Nothing Then
		'WScript.Echo "Current host " & sLaneName & " is missing from LaunchPad.xml Lanes section. Adding it..."
		' Get a blank lane entry if one exists otherwise create a new one
		Set oHost = oDoc.selectSingleNode("/RapNet/Lanes/Lane/Host[normalize-space(text())='']")
		If oHost Is Nothing Then
			Dim oLane : Set oLane = oDoc.createElement("Lane")
			Set oHost = oDoc.createElement("Host")
			oLane.appendChild oHost
			oLanes.appendChild oLane
		End If
		oHost.text = sLaneName
		XmlAddLaneIfMissing = True
	Else
		'WScript.Echo "Current host " & sLaneName & " already in file."
	End If
End Function

'**********************************************************************
'  XML file manipulation helper function - Steve Antonakakis
'**********************************************************************
Function XmlAddOption(oDoc, sOption, sValue, sComment, bForceOverwrite)
	XmlAddOption = False
	Dim oOptions : Set oOptions = oDoc.selectSingleNode("/RapNet/Options")
	If Not oOptions Is Nothing Then
		Dim oOption : Set oOption = oOptions.selectSingleNode(sOption)
		If oOption Is Nothing Then
			'WScript.Echo "XmlAddOption(): Adding new Option '" & sOption & "' with value '" & sValue & "'"
			Set oOption = oDoc.createElement(sOption)
			oOption.text = sValue
			oOptions.appendChild oDoc.createComment(" " & sComment & " ")
			oOptions.appendChild oOption
			XmlAddOption = True
		ElseIf bForceOverwrite Then
			'WScript.Echo "XmlAddOption(): Overwriting existing Option '" & sOption & "' with value '" & sValue & "'"
			oOption.text = sValue
			XmlAddOption = True
		Else 
			'WScript.Echo "XmlAddOption(): Option '" & sOption & "' found in XML file."
		End If
	Else
		WScript.Echo "XmlAddOption(): Failed to read /RapNet/Options node in XML file."
	End If
End Function



'*************************************************************************
'  MAIN PROCESSING SECTION
'*************************************************************************

'*****************Get IP Address

get_ipaddr
'get_macaddress

'*****************Get Store Number

get_store
'*****************Get Terminal Number
get_term
get_RAP
prompt_lane_type
'*****************Updates Registry Entries
build_computer_name

reg_update

'********POS10658: Check if SCAN & BAG. If so, run bag_setup
if input_lane_type = "1" then
	bagsetuprun = oShell.Run ("c:\install\installscripts\bag_setup.bat", 2, True)
  If fs.FileExists("c:\scot\bin\takeawaybelt_disable.bat") Then
    bagenablerun = oShell.Run ("c:\scot\bin\takeawaybelt_disable.bat", 2, True)
  end if
end if

'********Check if TAB if so run Tab_setup
if input_lane_type = "2" then
	tabsetuprun = oShell.Run ("c:\install\installscripts\tab_setup.bat", 2, True)
  If fs.FileExists("c:\scot\bin\takeawaybelt_enable.bat") Then
    tabenablerun = oShell.Run ("c:\scot\bin\takeawaybelt_enable.bat", 2, True)
  end if
end if

'********POS245658: Check if lane is an R4, installs Trilight drivers if needed
Set LaneType = CreateObject("WScript.Shell")
regValue = LaneType.RegRead("HKEY_LOCAL_MACHINE\SOFTWARE\NCR\SCOT - Platform\ObservedOptions\HWType")
if regvalue = "SCOT4" then
  trilightdriverrun = oShell.Run ("c:\install\drivers\devices\R4_7343_trilight_driver_install.bat", 2, True)
end if

'********Check if Mobile Payment Station if so run paymentstation_setup
if input_lane_type = "3" then
	tabsetuprun = oShell.Run ("c:\install\installscripts\paymentstation_setup.bat", 2, True)
end if

'POS38137 - set the printer min receipt length 
printerrun = oshell.Run ("C:\scot\bin\PrinterMinLength.bat", 2, True)

'*****************Create Launchpad.xml
'launchpadxml
launchpadxml_update
'*****************Update Launchpad.xml

'*****************Create rcmconfig.000

rcmconfig000

'*****************Create File Transport Service xml

filetransportxml

'If the lane is release 6 run VNC install to regenerate keys and license
Set LaneType = CreateObject("WScript.Shell")
on error resume next
 regValue = LaneType.RegRead("HKEY_LOCAL_MACHINE\SOFTWARE\NCR\HWType")
on error goto 0
if regvalue = "SCOT6" then
  VNCInstallrun = oShell.Run ("c:\install\installscripts\VNC_Install\FullRealVNC4Install.bat", 2, True)
end if


'*****************Following sets up DNS and WINS in the fastlanes
'dnswinscmd = "c:\install\installscripts\dnswins.bat" & " " & DNSADDRprimary & " " & DNSADDRsecondary & " " & WINSADDR 
'	a = oshell.run (dnswinscmd, 1,true)

'*****************Run NDINIT to configure Terminal

	f = oshell.run ("c:\acs\bin\ndinit.exe " & Right(Pos_NO, 3), 1, true)

'oShell.Run ("c:\acs\bin\ndinit.exe"), 1, True







'*****************Run Autostartupon.bat for automatic startup of SCOT
	b = oshell.run ("c:\scot\tools\autostartup.on.bat", 1,true)

'Rerun Axeda packagage to re-register the terminal number
'Dim fso, MyFile
'Set fso = CreateObject("Scripting.FileSystemObject")
'Set MyFile = fso.CreateTextFile("c:\temp\ForceReinstall.bat", True)
'MyFile.Write("c:\scot\install\agent")
'MyFile.Write(chr(34))
'MyFile.Write("Predictive Services v2")
'MyFile.Write(chr(34))
'MyFile.Write("\SSCO_Diagnostic_Agent.exe /force")

'MyFile.Close

'fso.DeleteFile("c:\temp\ForceReinstall.bat")

Set fso = CreateObject("Scripting.FileSystemObject")
Set oShell = CreateObject ("Wscript.shell")

if fso.FileExists("c:\acs\data\cmosfile.dat") then
	' POS14441: This reimaged FastLane needs the old cmosfile.dat file deleted.
	' POS15793: Make sure sales app is shutdown so that it lets go of the cmosfile.dat file.

	if IsProcessRunning("scotappu.exe") then
		' Kill the FastLane app so it is not used after running this script.
		' Killing the FastLane app does not kill the sales app.
		KillProcess "scotappu.exe"
		if IsProcessRunning("pipeServer.exe") then
			KillProcess "pipeServer.exe"
		end if
	end if

	if IsProcessRunning("salesw2k.exe") then
		' Kill processes associated with the sales app & then kill sales app.
		if IsProcessRunning("OfflineEFT.exe") then
			KillProcess "OfflineEFT.exe"
		end if
		if IsProcessRunning("ACSIOC~1.exe") then
			KillProcess "ACSIOC~1.exe"
		end if
		if IsProcessRunning("ACSIOChannel.exe") then
			KillProcess "ACSIOChannel.exe"
		end if
		KillProcess "salesw2k.exe"
	end if

	' Wait up to 1 minute for sales app to terminate.
	iLoop = 0
	Do
		wscript.sleep 3000 ' Wait 3 seconds
		iLoop = iLoop + 3000
	Loop Until (IsProcessRunning("salesw2k.exe") = False) Or (iLoop > 60000)

	if iLoop > 60000 then
		WScript.Echo "Script FAILED -> End this script, Shutdown scotappu.exe, Shutdown salesw2k.exe, & Re-run fastlane_setup.vbs"
	end if

	fso.DeleteFile("c:\acs\data\cmosfile.dat")
end if

If fs.FileExists("c:\acs\data\BIOMET") then
  fso.DeleteFile("c:\acs\data\BIOMET")
End If
If fs.FileExists("c:\acs\data\OPAuth") then
  fso.DeleteFile("c:\acs\data\OPAuth")
End If

If fs.FileExists("c:\install\installscripts\ADKRebootFlag.txt") then
  fso.DeleteFile("c:\install\installscripts\ADKRebootFlag.txt")
End If

'POS296404 Old terminals AJB scripts need to be cleaned off lane after reimage
FIPAY_DIR_Del = oShell.Run ("c:\install\installscripts\Delete_Old_AJB-Fastlane.bat", 0, True)

'POS10658: These bat files must be run everytime after reimaging the FastLane. Also, change path from c:\Install\RemoteAgent to c:\scot\bin for bat files.
x = oShell.Run ("c:\scot\bin\UnifiedAgentSetup.bat", 0, True)
x = oShell.Run ("c:\scot\bin\SSCORemoteAgentSetup.bat", 0, True)


'Reregister 
y = oShell.Run ("c:\windows\regedit.exe /s c:\scot\reg\RSM_SNMP_config.reg", 0, True)

'POS33327: Reset the pagefault size depending of the system physical TOTAL RAM size
x = oshell.Run ("C:\scot\bin\ConfigurePageFile.vbs", 0, True)

'Run Reboot Script
       c = oshell.run ("c:\install\installscripts\restart.vbs", 1, true)

wscript.quit
