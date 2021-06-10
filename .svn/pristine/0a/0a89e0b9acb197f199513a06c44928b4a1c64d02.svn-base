Option Explicit

const launchPadConfigFile = "c:\scot\config\LaunchPad.xml"
const rapNetConfigFile = "c:\scot\config\RapNet.xml"
const rapNetExeFile = "c:\scot\bin\RapNet.exe"

'Create the stylesheet ot format the output file
Dim sStylesheet : sStyleSheet = _
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

' VBScript source code
Dim oDoc : Set oDoc	= CreateObject("MSXML2.DomDocument.4.0")
Dim oStyle : Set oStyle = CreateObject("MSXML2.DomDocument.4.0")
Dim oFSO : Set oFSO = CreateObject("Scripting.FileSystemObject")

'Load the formatting stylesheet
Dim bStyle : bStyle = oStyle.loadXML(sStyleSheet)

'Merge new settings to LaunchPad
If oDoc.load(launchPadConfigFile) Then
	WScript.Echo "Updating " & launchPadConfigFile & "..."
	Dim bRet
	bRet = AddOption(oDoc, "UseBiometrics", "false", "Use UseBiometrics Login", false)
	bRet = bRet or AddOption(oDoc, "AllowEmplIdSignOn", "false", "AllowEmplIdSignOn for login", false) 
	bRet = bRet or AddOption(oDoc, "AllowHandKeyEMPLID", "true", "AllowHandKeyEMPLID for login", false)
	bRet = bRet or AddOption(oDoc, "ScannerClass", "CommonLibsNet.ScannerShared", "The scanner class to be used. Options: ScannerShared, ScannerThread", false)
	bRet = bRet or AddOption(oDoc, "NcrRemoteScanner", "SOFTWARE\NCR\NcrLaunchPadScanner", "Launch Pad scanner profile (default: SOFTWARE\NCR\NcrRemoteScanner)", true)
    bRet = bRet or AddOption(oDoc, "UtilityReturnAlwaysEnabled", "true", "Allow return to ScotApp/RapNet when a utility executable is running? (default is false).", true)
	bRet = bRet or AddOption(oDoc, "BiometricDeviceConfigured", "false", "Has a Biometric device? (default is false)", true)
    bRet = bRet or AddOption(oDoc, "LaunchPadRAPAutoSignOffSeconds", "0", "Auto sign off RAP and Launchpad in seconds (default is 0, disabled)", false)
    
	If bRet Then
		Dim sOut : sOut = ""
		If bStyle Then
			sOut = oDoc.transformNode(oStyle)
		End If
		If sOut <> "" Then
			WScript.Echo "MergeNewEmplidXml: Formatting XML file and saving."
			Dim oFile : Set oFile = oFSO.CreateTextFile(launchPadConfigFile, true, false)
			oFile.Write(sOut)
			oFile.Close
		Else
			WScript.Echo "MergeNewEmplidXml: Saving XML file (no formatting)."
			oDoc.save(launchPadConfigFile)
		End If
	Else
		WScript.Echo "MergeNewEmplidXml: All required options already exist in XML file."
	End If
Else
	WScript.Echo "MergeNewEmplidXml: Failed to load XML file: " & launchPadConfigFile
End If

'If we're on the RAP, merge the settings into the RapNet file as well
If oFSO.FileExists(rapNetExeFile) Then
	WScript.Echo "MergeNewEmplidXml: Found file " & rapNetExeFile & ". We will be updating " & rapNetConfigFile
	If oDoc.load(rapNetConfigFile) Then
		WScript.Echo "Updating " & rapNetConfigFile & "..."
		bRet = AddOption(oDoc, "UseBiometrics", "false", "Use UseBiometrics Login", false)
		bRet = bRet or AddOption(oDoc, "AllowEmplIdSignOn", "false", "AllowEmplIdSignOn for login", false) 
		bRet = bRet or AddOption(oDoc, "AllowHandKeyEMPLID", "true", "AllowHandKeyEMPLID for login", false)
		bRet = bRet or AddOption(oDoc, "ScannerClass", "CommonLibsNet.ScannerShared", "The scanner class to be used. Options: ScannerShared, ScannerThread", false)
		bRet = bRet or AddOption(oDoc, "NcrRemoteScanner", "SOFTWARE\NCR\NCRRapRemoteScanner", "RapNet scanner profile (default: SOFTWARE\NCR\NcrRemoteScanner)", false)
        bRet = bRet or AddOption(oDoc, "DisplayTransactionCount", "true", "Display total transaction count of all lanes", false)
        bRet = bRet or AddOption(oDoc, "TimePeriodForThroughputCalculation", "60", "Time interval in minutes to evaluate the total transaction count", false)
        bRet = bRet or AddOption(oDoc, "TrxCountThresholdForHighAlert", "20", "Transaction count threshold for high alert", false)
        bRet = bRet or AddOption(oDoc, "TrxCountThresholdForCaution", "40", "Transaction count threshold for caution", false)
        bRet = bRet or AddOption(oDoc, "BiometricDeviceConfigured", "false", "Has a Biometric device? (default is false)", true)
		
		If bRet Then
			sOut = ""
			If bStyle Then
				sOut = oDoc.transformNode(oStyle)
			End If
			If sOut <> "" Then
				WScript.Echo "MergeNewEmplidXml: Formatting XML file and saving."
				Set oFile = oFSO.CreateTextFile(rapNetConfigFile, true, false)
				oFile.Write(sOut)
				oFile.Close
			Else
				WScript.Echo "MergeNewEmplidXml: Saving XML file (no formatting)."
				oDoc.save(rapNetConfigFile)
			End If
		Else
			WScript.Echo "MergeNewEmplidXml: All required options already exist in XML file."
		End If		
	Else
		WScript.Echo "MergeNewEmplidXml: Failed to load XML file: " & rapNetConfigFile
	End If
Else
	WScript.Echo "MergeNewEmplidXml: We are not on the RAP, skipping the " & rapNetConfigFile & " update."
End If

Set oStyle = Nothing
Set oFSO = Nothing
Set oDoc = Nothing


Function AddOption(oDoc, sOption, sValue, sComment, bForceOverwrite )
	AddOption = False
	Dim oOptions : Set oOptions = oDoc.selectSingleNode("/RapNet/Options")
	If Not oOptions Is Nothing Then
		Dim oOption : Set oOption = oOptions.selectSingleNode(sOption)
		If oOption Is Nothing Then
			WScript.Echo "MergeNewEmplidXml: Adding new Option '" & sOption & "' with value '" & sValue & "'"
			Set oOption = oDoc.createElement(sOption)
			oOption.text = sValue
			oOptions.appendChild oDoc.createComment(sComment)
			oOptions.appendChild oOption
			AddOption = True
		ElseIf bForceOverwrite Then
			WScript.Echo "MergeNewEmplidXml: Overwriting existing Option '" & sOption & "' with value '" & sValue & "'"
			oOption.text = sValue
			AddOption = True
		Else 
			WScript.Echo "MergeNewEmplidXml: Option '" & sOption & "' found in XML file."
		End If
	Else
		WScript.Echo "MergeNewEmplidXml: Failed to read /RapNet/Options node in XML file."
	End If
End Function