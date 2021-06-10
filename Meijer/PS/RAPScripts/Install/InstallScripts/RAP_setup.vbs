'******************************************************************************* 
'Customer: Meijer
'Script: RAP_setup.vbs
'Purpose: Automates the tailoring of RAP to the specific store environment
'Author:  Dennis Shovelin
'Created: 1/2006
'*******************************************************************************
'History:
'POS183382  Work Request:68237  Name:Robert Susanto  Date: June 2, 2015
'*******************************************************************************
'               Change Control
'*******************************************************************************
' 7/11/2007	Changed the number of RAPS from 4 to 9
'
' 1/12/2009  -	Steve Antonakakis - Changed LaunchPad.xml and RapNet modication code
'			 to merge changes to  existing LaunchPad.xml document instead of replacing.
'			Script will add default setting values when settings are missing. It will also
'			update the Lanes section reusing blank Lane/Host entries when available.
' 3/31/2009  -  Dennis Shovelin - Modified FileTransportService.xml to use secondary
' 	         server name instead of IP address.
' POS183382 change filetransportserver to acs user
' 10/18/16 - Kyle Hmmang - Updated to run ndinit so RAP can work with ACS Apldr
' 12/21/16 - Kyle Hammang - Added step to reinstall VNC if RAP is XR7
' 1/31/17 - Kyle Hammang - Updates script to create config entities for R6 convertibles 240-249
'*******************************************************************************
Set oShell = CreateObject ("Wscript.shell")

'*******************************************************************************
'Setup Variables
'*******************************************************************************
Dim CI, s1, s2, sPath, A1, i, r, A2
Dim launchpad
Dim sampleline
Dim ObjFSO
Dim File
Dim Section
Dim Key
DIM savedcomputername(10)
Dim savedcomputername1
dim savedcomputername2
dim savedcomputername3
dim savedcomputername4
dim savedcomputername5
dim savedcomputername6
dim savedcomputername7
dim savedcomputername8
dim savedprimaryname
dim savedsecondaryname
dim savedRAPname1
dim savedRAPname2
dim numberoflanes
dim savenumberofraps
Dim servermsg
Dim title 
dim style 
dim response 
dim strnewcompname
dim DNSADDRprimary
dim DNSADDRsecondary
dim ispaddr
dim WINSADDR
Dim POS_NO
Dim STORE_NUM
DIM RAP_NO
DIM Starting_lane_no
DIM computed_lane_no
DIM RAP_NODE


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
ipaddr = ""
sRapNet = NCR & "RapNet\"

DNSADDRprimary = ""
DNSADDRsecondary = ""
WINSADDR = ""
primaryserver = ""
secondaryserver = ""
ispaddr = ""


file=""
section=""
key=""
value= ""
savedcomputername1 = ""
savedcomputername2 = ""
savedcomputername3 = ""
savedcomputername4 = ""
POS_NO = ""
securityserver = ""
rptserver = ""
strnewcompname = ""
POS_NO = ""
STORE_NUM = ""

'*******************************************************************************
'   Prompt for store number Subroutine
'*******************************************************************************
sub get_store()
Do
GoodInput = "Y"
STORE_NUM = InputBox("Enter Store Number: Must be 4 digits Leading Zeroes", "Store Number", "1")
TESTLEN = len(STORE_NUM)
	
	if TESTLEN = 0 then
		Error1=MsgBox("Store Number must be entered", 48, "Error!")
		GoodInput = "N"
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

end sub

'**************************************************************************
'   GET RAP Information
'**************************************************************************
sub get_RAP_info
do
GoodInput = "Y"
RAP_NO = InputBox("Enter the number of the RAP: Must be 1 to 7 ", "RAP Number", "1")
TESTLEN = len(RAP_NO)

	if TESTLEN = 0 then
		Error1=MsgBox("RAP Number must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	if RAP_NO = "" then
		 Error1=MsgBox("RAP Number must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	if RAP_NO > "7" then
		Error1=MsgBox("RAP Number must be either 1 to 7", 48, "Error!")
		GoodInput = "N"
	end if
	
	if TESTLEN > 1 then
		Error1=MsgBox("RAP Number must be 1 digits", 48, "Error!")
		GoodInput = "N"
	end if
Loop Until GoodInput = "Y"

do
GoodInput = "Y"
numberoflanes = InputBox("Enter the Number of Lanes in this POD Max 10", "Number of Lanes", "00")
TESTLEN = len(numberoflanes)

	if TESTLEN = 0 then
		Error1=MsgBox("Number of Lanes must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	if numberoflanes = "" then
		 Error1=MsgBox("Number of Lanes must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	
	

	if TESTLEN > 2 then
		Error1=MsgBox("Number of Lanes must be 10 or less", 48, "Error!")
		GoodInput = "N"
	end if
Loop Until GoodInput = "Y"


do
GoodInput = "Y"
Starting_lane_no = InputBox("Enter the Starting Lane Number: 3 digits", "Starting Lane Number", "000")
TESTLEN = len(Starting_lane_no)

	if TESTLEN = 0 then
		Error1=MsgBox("Starting Lane Number must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	if Starting_lane_no = "" then
		 Error1=MsgBox("Starting Lane Number must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	
	if TESTLEN < 3 then
		Error1=MsgBox("Starting Number must be 3 digits", 48, "Error!")
		GoodInput = "N"
	end if

	if TESTLEN > 3 then
		Error1=MsgBox("Starting Number must be 3 digits", 48, "Error!")
		GoodInput = "N"
	end if
Loop Until GoodInput = "Y"


end sub
'**************************************************************************
'   Build Computer Names
'**************************************************************************
sub Build_Computer_names()
	dim i 
	dim done
	mynum = int(numberoflanes - 1)
	i = 0
	done = "N"
	computed_lane_no = starting_lane_no
	strnewcompname = "Store" & STORE_NUM & "RAP" & RAP_NO
	do 
	savedcomputername(i) = "Store" & STORE_NUM & "POS" & LPad(computed_lane_no, "0", 3)
	computed_lane_no = computed_lane_no + 1
	i = i + 1
	if i > mynum then
	  done = "Y"
	end if
	loop until done = "Y"
end sub
'**************************************************************************
'  Update Registry for Installation Subroutine
'**************************************************************************
sub reg_update()



With oShell
'	
 	.RegWrite sCompNameRegPath & "ComputerName\ComputerName", strNewCompName
 	.RegWrite sCompNameRegPath & "ActiveComputerName\ComputerName", strNewCompName
 	.RegWrite sTcpipParamsRegPath & "Hostname", strNewCompName
 	.RegWrite sTcpipParamsRegPath & "NV Hostname", strNewcompName
 	.RegWrite sTcpipParamsRegPath & "NV Hostname", strNewcompName
'  RAP Updates
	.RegWrite RAPSCANNERPATH & "ClientName", strNewCompName
	.RegWrite sRapNet & "ReportServer", strNewCompName

 

 	
End With

end sub

'*******************End reg_update Subroutine***********************************


'*******************************************************************************
'  Get IP ADDRESS Subroutine
'*******************************************************************************
sub get_ipaddr()

set IPConfigSet = GetObject("winmgmts:{impersonationLevel=impersonate}").ExecQuery _
  ("select IPAddress from Win32_NetworkAdapterConfiguration where IPEnabled=TRUE")

for each IPConfig in IPConfigSet
 if Not IsNull(IPConfig.IPAddress) then 
  for i=LBound(IPConfig.IPAddress) to UBound(IPConfig.IPAddress)
  ipaddr = IPConfig.IPAddress(i)
'  msgbox ipaddr
  next
 end if
next



'WScript.Echo IPaddr
    ' finds the length between the 1st and 2nd octet (eg. 168.244.51.200) is only 4 chars to the "."
    location1 = InStr(IPaddr, ".")
    firstip = Mid(Ipaddr, 1)
    firstoctet = left(firstip, (location1 - 1))

    ' pulls off the first octet and the "." (eg. leaves 244.51.200)
    secIP = Mid(IPaddr, (location1 + 1))
    
    ' finds the length of the 2nd octet (eg. gets that 244 is only 3 chars)
    temp = InStr(secIP, ".")
    ' strips off the second octet from the IP address (eg. returns 244)
    SECoctet = Left(secIP, (temp - 1))
    'MsgBox "this should be just the second octet " & SECoctet
    ' finds the length between the 2nd and 3rd octet (eg. 168.244.51.200) is only 3 chars to the "."
    location2 = InStr(secIP, ".")
    ' pulls everything but the 3rd and 4th octet (eg. 51.200)
    thirdIP = Mid(secIP, (location2 + 1))
    ' finds the length of the third octet (eg. gets that 51 is only 2 chars)
    temp = InStr(thirdIP, ".")
    ' strips off the second octet from the IP address (eg. returns 51)
    THIRDoctet = Left(thirdIP, (temp - 1))
    'MsgBox "this should be the third octet " & THIRDoctet
    'gets the length of the 3rd octet
    location3 = InStr(thirdIP, ".")
    ' pulls out the last octet
    fourthIP = Mid(thirdIP, (location3 + 1))

primaryserver = firstoctet & "." & secoctet & "." & thirdoctet & "." & "201"
secondaryserver = firstoctet & "." & secoctet & "." & thirdoctet & "." & "202"
ispaddr = firstoctet & "." & secoctet & "." & thirdoctet & "." & "202"

end sub

'***********************End get_ipdaddr subroutine************************




'**********************************************************************
'  LaunchPad XML file write
'**********************************************************************
sub launchpadxml()
dim i 
dim done
mynum = int(numberoflanes - 1)
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

'**************DO LOOP TO FILL IN

i = 0
done = "N"
do 
sampleline = "             <Lane>"
launchpad.writeline(sampleline)
sampleline = "                 <Host>" & savedcomputername(i) & "</Host>"
launchpad.writeline(sampleline)
sampleline = "             </Lane>"
launchpad.writeline(sampleline)
i = i + 1
if i > mynum then
  done = "Y"
end if
loop until done = "Y"
'***************END DO LOOP

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
'***************************end Launchpad function******************************
'**********************************************************************
'  RAPNET XML file write
'**********************************************************************
sub RAPnetxml()
dim i 
dim done
mynum = int(numberoflanes - 1)
Set objFSO = WScript.CreateObject("Scripting.FileSystemObject")

If objFSO.FileExists("c:\scot\config\RAPnet.xml") Then objFSO.DeleteFile("c:\scot\config\RAPnet.xml")
Set rapnet = objFSO.CreateTextFile("c:\scot\config\RAPnet.xml")
sampleline = "<?xml version=""1.0""?>"
rapnet.writeline(sampleline)
sampleline = "<RapNet>"
rapnet.writeline(sampleline)
sampleline = "     <!--  This is the list of connected Fastlanes -->"
rapnet.writeline(sampleline)
sampleline = "          <Lanes>"
rapnet.writeline(sampleline)

'**************DO LOOP TO FILL IN

i = 0
done = "N"
do 
sampleline = "             <Lane>"
rapnet.writeline(sampleline)
sampleline = "                 <Host>" & savedcomputername(i) & "</Host>"
rapnet.writeline(sampleline)
sampleline = "             </Lane>"
rapnet.writeline(sampleline)
i = i + 1
if i > mynum then
  done = "Y"
end if
loop until done = "Y"
'***************END DO LOOP


sampleline = "          </Lanes>"
rapnet.writeline(sampleline)
sampleline = "          <Options>"
rapnet.writeline(sampleline)
sampleline = "             <!-- Use Alphanumeric keyboard for signon -->"
rapnet.writeline(sampleline)
sampleline = "             <AlphaNumericKeyboard>false</AlphaNumericKeyboard>"
rapnet.writeline(sampleline)
sampleline = "             <!-- Timeout in ms for signon operation -->"
rapnet.writeline(sampleline)
sampleline = "             <SignOnTimeout>5000</SignOnTimeout>"
rapnet.writeline(sampleline)
sampleline = "             <!-- Time in ms between Psx ConnectRemote invocations -->"
rapnet.writeline(sampleline)
sampleline = "             <ConnectionPeriod>5000</ConnectionPeriod>"
rapnet.writeline(sampleline)
sampleline = "             <!-- Timeout in ms for ConnectRemote operation -->"
rapnet.writeline(sampleline)
sampleline = "             <ConnectRemoteTimeout>5000</ConnectRemoteTimeout>"
rapnet.writeline(sampleline)
sampleline = "             <!-- Synchronize our local time with FastLane at signon -->"
rapnet.writeline(sampleline)
sampleline = "             <SynchronizeTimeServer>false</SynchronizeTimeServer>"
rapnet.writeline(sampleline)
sampleline = "             <!-- Sound played when a barcode is scanned. Silent if empty. -->"
rapnet.writeline(sampleline)
sampleline = "             <ScanSound>Scan.wav</ScanSound>"
rapnet.writeline(sampleline)
sampleline = "             <!-- Sound played when an immediate intervention is received. Silent if empty. -->"
rapnet.writeline(sampleline)
sampleline = "             <ImmediateInterventionSound>SecurityHigh.wav</ImmediateInterventionSound>"
rapnet.writeline(sampleline)
sampleline = "             <!-- Sound played when a delayed intervention is received. Silent if empty. -->"
rapnet.writeline(sampleline)
sampleline = "             <DelayedInterventionSound>SecurityLow.wav</DelayedInterventionSound>"
rapnet.writeline(sampleline)
sampleline = "             <!-- Delay in ms before an intervention sound is played. -->"
rapnet.writeline(sampleline)
sampleline = "             <InterventionSoundDelay>2000</InterventionSoundDelay>"
rapnet.writeline(sampleline)
sampleline= "           </Options>"
rapnet.writeline(sampleline)
sampleline= "</RapNet>"
rapnet.writeline(sampleline)

end sub
'***************************End RAP NET Function
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
' rapnetxml_update and launchpadxml_update common objects - Steve Antonakakis
'**********************************************************************
Const launchPadConfigFile = "c:\scot\config\LaunchPad.xml"
Const rapNetConfigFile = "c:\scot\config\RapNet.xml"
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


'**********************************************************************
' LaunchPad XML file manipulation helper function - Steve Antonakakis
'**********************************************************************
Sub launchpadxml_update()

	'Merge required settings to LaunchPad
	If oDoc.load(launchPadConfigFile) Then
		'WScript.Echo "Updating " & launchPadConfigFile & "..."
		Dim bRet : bRet = False
		'Add all configured lanes
		For i = 0 To numberoflanes - 1
			bRet = bRet or XmlAddLaneIfMissing(oDoc, savedcomputername(i))
		Next
		
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
		bRet = bRet or XmlAddOption(oDoc, "UtilityReturnAlwaysEnabled", "true", "Allow return to ScotApp/RapNet when a utility executable is running? (default is false).", false)
		bRet = bRet or XmlAddOption(oDoc, "BiometricDeviceConfigured", "false", "Has a Biometric device? (default is false)", true)
		bRet = bRet or XmlAddOption(oDoc, "LaunchPadRAPAutoSignOffSeconds", "0", "Auto sign off RAP and Launchpad in seconds (default is 0, disabled)", false)
		bRet = bRet or XmlAddOption(oDoc, "UseBiometrics", "false", "Use UseBiometrics Login", false)
		bRet = bRet or XmlAddOption(oDoc, "AllowEmplIdSignOn", "false", "AllowEmplIdSignOn for login", false) 
		bRet = bRet or XmlAddOption(oDoc, "AllowHandKeyEMPLID", "true", "AllowHandKeyEMPLID for login", false)
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

'**********************************************************************
'  RapNet XML file manipulation helper function - Steve Antonakakis
'**********************************************************************
Sub rapnetxml_update()

	'Merge required settings to LaunchPad
	If oDoc.load(rapNetConfigFile) Then
		'WScript.Echo "Updating " & rapNetConfigFile & "..."
		Dim bRet : bRet = False
		
		'Add all configured lanes
		For i = 0 To numberoflanes - 1
			bRet = bRet or XmlAddLaneIfMissing(oDoc, savedcomputername(i))
		Next
		
		bRet = bRet or XmlAddOption(oDoc, "AlphaNumericKeyboard", "false", "Use Alphanumeric keyboard for signon", false) 
		bRet = bRet or XmlAddOption(oDoc, "SignOnTimeout", "5000", "Timeout in ms for signon operation", false)
		bRet = bRet or XmlAddOption(oDoc, "EnterIdUppercase", "false", "EnterId screen initially uppercase", false)
		bRet = bRet or XmlAddOption(oDoc, "EnterPasswordUppercase", "false", "EnterPassword screen initially uppercase", false)
		bRet = bRet or XmlAddOption(oDoc, "ConnectionPeriod", "5000", "Time in ms between Psx ConnectRemote invocations", false)
		bRet = bRet or XmlAddOption(oDoc, "ConnectRemoteTimeout", "5000", "Timeout in ms for ConnectRemote operation", false)
		bRet = bRet or XmlAddOption(oDoc, "SynchronizeTimeServer", "false", "Synchronize our local time with FastLane at signon", false)
		bRet = bRet or XmlAddOption(oDoc, "ScanSound", "Scan.wav", "Sound played when a barcode is scanned. Silent if empty", false)
		bRet = bRet or XmlAddOption(oDoc, "ImmediateInterventionSound", "SecurityHigh.wav", "Sound played when an immediate intervention is received. Silent if empty.", false)
		bRet = bRet or XmlAddOption(oDoc, "DelayedInterventionSound", "SecurityLow.wav", "Sound played when a delayed intervention is received. Silent if empty.", false)
		bRet = bRet or XmlAddOption(oDoc, "InterventionSoundDelay", "2000", "Delay in ms before an intervention sound is played.", false)
		bRet = bRet or XmlAddOption(oDoc, "AltF4ExitAllowed", "true", "Allow the Alt-F4 keystroke entry to exit the application? (default is true)", false)
		bRet = bRet or XmlAddOption(oDoc, "DisplayTransactionCount", "true", "Display total transaction count of all lanes", false)
		bRet = bRet or XmlAddOption(oDoc, "MultipleInterventionHandling", "false", "Allow user to switch from the current Exclusive Lane Intervention handling to handle another Intervention of another Lane.", false)
		bRet = bRet or XmlAddOption(oDoc, "NcrRemoteScanner", "SOFTWARE\NCR\NCRRapRemoteScanner", "RapNet scanner profile (default: SOFTWARE\NCR\NCRRapRemoteScanner)", false)
		bRet = bRet or XmlAddOption(oDoc, "TimePeriodForThroughputCalculation", "60", "Time interval in minutes to evaluate the total transaction count", false)
		bRet = bRet or XmlAddOption(oDoc, "TrxCountThresholdForHighAlert", "20", "Transaction count threshold for high alert", false)
		bRet = bRet or XmlAddOption(oDoc, "TrxCountThresholdForCaution", "40", "Transaction count threshold for caution", false)
		bRet = bRet or XmlAddOption(oDoc, "BiometricDeviceConfigured", "false", "Has a Biometric device? (default is false)", true)
		bRet = bRet or XmlAddOption(oDoc, "UseBiometrics", "false", "Use UseBiometrics Login", false)
		bRet = bRet or XmlAddOption(oDoc, "AllowEmplIdSignOn", "false", "AllowEmplIdSignOn for login", false) 
		bRet = bRet or XmlAddOption(oDoc, "AllowHandKeyEMPLID", "true", "AllowHandKeyEMPLID for login", false)
		bRet = bRet or XmlAddOption(oDoc, "ScannerClass", "CommonLibsNet.ScannerShared", "The scanner class to be used. Options: ScannerShared, ScannerThread", false)

		If bRet Then
			Dim sOut : sOut = ""
			If bStyle Then
				sOut = oDoc.transformNode(oStyle)
			End If
			If sOut <> "" Then
				'WScript.Echo "rapnetxml_update(): Formatting XML file and saving."
				Dim oFile : Set oFile = oFSO.CreateTextFile(rapNetConfigFile, true, false)
				oFile.Write(sOut)
				oFile.Close
			Else
				'WScript.Echo "rapnetxml_update(): Saving XML file (no formatting)."
				oDoc.save(rapNetConfigFile)
			End If
		Else
			'WScript.Echo "rapnetxml_update(): All required options already exist in XML file."
		End If
	Else
		WScript.Echo "rapnetxml_update(): Failed to load XML file: " & rapNetConfigFile
	End If	
	

End Sub

'**************************************************************************************************
'  Create configentities for node ID's 240-249 specific to Meijer R6 convertibles - Kyle Hammang
'**************************************************************************************************
Sub r6_convertible_configentity()
  Set fso = CreateObject("Scripting.FileSystemObject")

  'delete *240.xml-*249.xml if config entities exist from previous stores
  xcount = 240
  On error resume next
  do
    fso.DeleteFile "C:\scot\config\ConfigEntity-Store*" & xcount & ".xml", True 
    xcount = xcount + 1
  loop until xcount > 249
  On error goto 0

  'write the lane name to the file for convertible node ID's 240-249
  xcount = 240
  do 
    strnewcompname = "Store" & STORE_NUM & "POS" & xcount

    Set rFile=fso.OpenTextFile("ConfigEntity-Starter.txt",1) 'ForReading
    buf=rFile.ReadAll
    rFile.Close

    arr=Split(buf,vbCrLf)
    arr(2)="   <conf:Entity Id=""" & strnewcompname & """ Type=""Lane""  LastTimeUpdated=""2017-01-01T12:00:00"">"

    Set rFile=fso.OpenTextFile("ConfigEntity-Starter.txt",2) 'ForWriting
    For x=0 To Ubound(arr)
      rFile.WriteLine arr(x)
    Next 'x
    rFile.Close

    'copy starter file into place and rename
    fso.CopyFile "C:\install\installscripts\ConfigEntity-Starter.txt", "C:\scot\config\ConfigEntity-Store" & STORE_NUM & "POS" & xcount & ".xml"

    xcount = xcount + 1
  loop until xcount > 249

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

'**********************************************************************
'  String padding functions - Steve Antonakakis
'**********************************************************************
Function LPad (MyValue, MyPadChar, MyPaddedLength)
	LPad = string(MyPaddedLength - Len(MyValue),MyPadChar) & MyValue
End Function

Function RPad (MyValue, MyPadChar, MyPaddedLength)
	RPad = MyValue & string(MyPaddedLength - Len(MyValue), MyPadChar)
End Function

'**********************************************************************
'  Adjust RAP node to match 251-259
'**********************************************************************
Function Node_ID_Conv ()
	RAP_NODE = 229 + RAP_NO
End Function

'*************************************************************************
'"*************************************************************************
'  MAIN PROCESSING SECTION
'*************************************************************************
'*************************************************************************

get_store
get_RAP_Info
build_computer_names
get_ipaddr


'*****************Updates Registry Entries********************************

reg_update


'*****************Create Launchpad.xml************************************

'launchpadxml
launchpadxml_update

'*****************Create RAPnet.xml***************************************

'rapnetxml
rapnetxml_update

filetransportxml

'*****************Run NDINIT to configure Terminal for ACS APLDR on RAP
Node_ID_Conv
f = oshell.run ("c:\acs\bin\ndinit.exe " & RAP_NODE, 1, true)


'If the RAP is release 6 run VNC install to regenerate keys and license and create convertible entities
Set LaneType = CreateObject("WScript.Shell")
on error resume next
 regValue = LaneType.RegRead("HKEY_LOCAL_MACHINE\SOFTWARE\NCR\HWType")
on error goto 0
if regvalue = "SCOT6" then
  'reinstall vnc license
  VNCInstallrun = oShell.Run ("c:\install\installscripts\VNC_Install\FullRealVNC4Install.bat", 2, True)
  
  'create R6 convertible config entities
  r6_convertible_configentity
end if

'*****************Following sets up DNS and WINS in the fastlanes
'dnswinscmd = "c:\install\installscripts\dnswins.bat" & " " & DNSADDRprimary & " " & DNSADDRsecondary & " " & WINSADDR 
'	a = oshell.run (dnswinscmd, 1,true)

'****************************POS32528**************************************************
printer = oshell.run ("c:\scot\bin\FindPrinter.bat",1,true)

'*****************Run Reboot Script***************************************
       c = oshell.run ("c:\install\installscripts\restart.vbs", 1, true)
 
wscript.quit