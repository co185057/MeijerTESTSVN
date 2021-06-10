'
' Script to configure the SelfServ(tm) Checkout and Meijer EMPLID and Biometric settings
'
' Author: Steve Antonakakis (2009-03-19)
'
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

Dim bEmplidOn : bEmplidOn = False
Dim bEmplidOff : bEmplidOff = False
Dim bBiometOn : bBiometOn = False
Dim bBiometOff : bBiometOff = False
Dim bHandKeyOn : bHandKeyOn = False
Dim bHandKeyOff : bHandKeyOff = False

    
If ParametersAreInvalid() Then
  WScript.Echo ""
  WScript.Echo ""
  WScript.Echo "EmplidBiometConfig.vbs Script Usage Syntax: " 
  WScript.Echo ""
  WScript.Echo "cscript.exe EmplidBiometConfig.vbs [EMPLID_ON|EMPLID_OFF] [BIOMET_ON|BIOMET_OFF] [HANDKEY_ON|HANDKEY_OFF] "
  WScript.Echo ""
  WScript.Echo "  EMPLID_ON = Enables EMPLID for SelfServ(tm) Checkout and enables AllowHandKeyEMPLID by default."
  WScript.Echo "  EMPLID_OFF = Disables EMPLID for SelfServ(tm) Checkout and returns system to legacy operator/password support."
  WScript.Echo "  BIOMET_ON = Enables Meijer biometric support for SelfServ(tm) Checkout"
  WScript.Echo "  BIOMET_OFF = Disables Meijer biometric support for SelfServ(tm) Checkout"
  WScript.Echo "  HANDKEY_ON = Enables support for Allowing Hand Key Entry of EMPLID "
  WScript.Echo "  HANDKEY_OFF = Disables support for Allowing Hand Key Entry of EMPLID "
  WScript.Echo ""
  WScript.Echo "Sanity checks will also be performed on other settings dependencies to ensure there are no conflicting configurations."
  WScript.Echo "Possible settings that may be modified include: NcrRemoteScanner, BiometricDeviceConfigured, ScannerClass"
  WScript.Quit(0)
End If    
  
' VBScript source code
Dim oDoc : Set oDoc	= CreateObject("MSXML2.DomDocument.4.0")
Dim oStyle : Set oStyle = CreateObject("MSXML2.DomDocument.4.0")
Dim oFSO : Set oFSO = CreateObject("Scripting.FileSystemObject")

'Load the formatting stylesheet
Dim bStyle : bStyle = oStyle.loadXML(sStyleSheet)

'
' Correct any conflicting configuration settings
'
If bBiometOn Then
  If Not bEmplidOn Or bEmplidOff Then
    WScript.Echo "Biometric requires EMPLID be enabled. Forcing EMPLID_ON flag."
    bEmplidOn = True
  End If
End If

'Merge new settings to LaunchPad if file exists
If oDoc.load(launchPadConfigFile) Then
  UpdateConfigFile oDoc, launchPadConfigFile
Else
  WScript.Echo "Failed to load XML file: " & launchPadConfigFile
End If

'If we're on the RAP, merge the settings into the RapNet file as well
If oFSO.FileExists(rapNetConfigFile) Then
  If oDoc.load(rapNetConfigFile) Then
    UpdateConfigFile oDoc, rapNetConfigFile
  Else
    WScript.Echo "Failed to load XML file: " & rapNetConfigFile
  End If
End If  

Set oStyle = Nothing
Set oFSO = Nothing
Set oDoc = Nothing

Function UpdateConfigFile(oDoc, configFileName)
  UpdateConfigFile = True
  WScript.Echo "Updating " & configFileName & "..."
  '
  ' Enabled/Disable EMPLID if specified (if emplid is enabled - we will default 'hand key' for EMPLID to enabled and it may then be disabled if specified
  '
  If bEmplidOn Then
    AddOption oDoc, "AllowEmplIdSignOn", "true", "AllowEmplIdSignOn for login", true 
    AddOption oDoc, "AllowHandKeyEMPLID", "true", "AllowHandKeyEMPLID for login", true 
  ElseIf bEmplidOff Then
    AddOption oDoc, "AllowEmplIdSignOn", "false", "AllowEmplIdSignOn for login", true 
  End If
  If bEmplidOn And bEmplidOff Then
    WScript.Echo "Invalid combination. Both EMPLID_ON and EMPLID_OFF were specified. Ignoring EMPLID_OFF."
  End If
  '
  ' Enabled/Disable Biometrics if specified
  '
  If bBiometOn Then
    AddOption oDoc, "UseBiometrics", "true", "Use UseBiometrics Login", true
  ElseIf bBiometOff Then
    AddOption oDoc, "UseBiometrics", "false", "Use UseBiometrics Login", true
  End If
  If bBiometOn And bBiometOff Then
    WScript.Echo "Invalid combination. Both BIOMET_ON and BIOMET_OFF were specified. Ignoring BIOMET_OFF."
  End If
  '
  ' Enabled/Disable Allow Key Entry of EMPLID if specified
  '
  If bHandKeyOn Then
    AddOption oDoc, "AllowHandKeyEMPLID", "true", "AllowHandKeyEMPLID for login", true
  ElseIf bHandKeyOff Then
    AddOption oDoc, "AllowHandKeyEMPLID", "false", "AllowHandKeyEMPLID for login", true
  End If
  If bHandKeyOn And bHandKeyOff Then
    WScript.Echo "Invalid combination. Both HANDKEY_ON and HANDKEY_OFF were specified. Ignoring HANDKEY_OFF."
  End If
  
  'Ensure NCR Common Components Biometric feature is disabled
  AddOption oDoc, "BiometricDeviceConfigured", "false", "Has a Biometric device? (default is false)", true
  
  Dim sOut : sOut = ""
  If bStyle Then
    sOut = oDoc.transformNode(oStyle)
  End If
  If sOut <> "" Then
    WScript.Echo "MergeNewEmplidXml: Formatting XML file and saving."
    Dim oFile : Set oFile = oFSO.CreateTextFile(configFileName, true, false)
    oFile.Write(sOut)
    oFile.Close
  Else
    WScript.Echo "MergeNewEmplidXml: Saving XML file (no formatting)."
    oDoc.save(configFileName)
  End If
End Function


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

Function ParametersAreInvalid()
  ParametersAreInvalid = True
  
  Dim oArgs : Set oArgs = WScript.Arguments
  Dim sArg 
  
  For Each sArg in oArgs
    Select Case UCase(sArg)
      Case "EMPLID_ON"
        bEmplidOn = True
        ParametersAreInvalid = False
      Case "EMPLID_OFF"
        bEmplidOff = True
        ParametersAreInvalid = False
      Case "BIOMET_ON"
        bBiometOn = True
        ParametersAreInvalid = False
      Case "BIOMET_OFF"
        bBiometOff = True
        ParametersAreInvalid = False
      Case "HANDKEY_ON"
        bHandKeyOn = True
        ParametersAreInvalid = False
      Case "HANDKEY_OFF"
        bHandKeyOff = True
        ParametersAreInvalid = False
      Case Else
        WScript.Echo "Invalid argument '" + sArg + "'. Skipping..."
    End Select
  Next 
  
End Function
