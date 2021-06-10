Option Explicit

'
'Switches
'
' Setting the following variable to False will attempt to fetch the ADK files from VSS.
' Setting it True will run the installation from the fl-integration folder.
Dim bInstallADK : bInstallADK = False

'VSS Flags
Const VSSFLAG_TIMEMOD = 8
Const VSSFLAG_REPREPLACE = 128
Const VSSFLAG_RECURSYES = 8192
Const VSSFLAG_RECURSNO = 4096
Const VSSFLAG_FORCEDIRNO = 16384

'
' Let's make things simple.  We use two VSS databases, one for IM (TBs) 
' and one for the remaining fastlane Application
'
Const VSS_DB_FL = "\\susdlg402\FastlaneVSS\srcsafe.ini"
Const VSS_DB_IM = "\\susdlg801\FastLaneIM\srcsafe.ini"

'
' We will also either be pulling FastLane source from Patches or Releases
'
Const FLSRC_REL = "$\FastLane\Construction\Development"
Const FLSRC_PATCH = "$\FastLane\Patches"	'for patches we need the patch name
Const IMSRC_CUST = "$\Integration_Solution\"
Const IMSRC_BASE = "$\Integration_Base\VER_03.00\"


Dim oShell : Set oShell = WScript.CreateObject("WScript.Shell")
Dim BUILD_ROOT : BUILD_ROOT = Left(oShell.CurrentDirectory,2) + "\Builds\"
WScript.Echo "Build root folder is: " + BUILD_ROOT
Set oShell = Nothing



Dim strDB : strDB = VSS_DB_FL

Dim oFSO : Set oFSO = CreateObject("Scripting.FileSystemObject")

Dim strUser : strUser = Trim(InputBox("VSS Username:", "VSS Login", "sa185011"))
Dim strPass : strPass = Trim(InputBox("VSS Password:", "VSS Login"))
Dim strVer :  strVer =  Trim(InputBox("What is the source label? (e.g. 'Build_4.00.00.1.001.053')", "VSS GET Label","Build_4.00.00.1.001.053"))

Dim strPassIM : strPassIM = Trim(InputBox("FastlaneIM VSS Password:", "VSS Login"))

Dim strBaseVersion : strBaseVersion = ""

strBaseVersion = Trim(InputBox("What is the TB Base Label to use? (e.g. 'BASE3.0.1')", "VSS GET Pinned Version","BASE3.0.1"))

Dim strCustomer : strCustomer = Trim(WScript.Arguments(0))
Dim strCustLabel : strCustLabel = Trim(WScript.Arguments(1))

'Dim strCustomer : strCustomer =  Trim(InputBox("What is the TB Customer project name? (e.g. 'ACS_Solution')", "VSS GET Label",WScript.Arguments(0)))
'Dim strCustLabel : strCustLabel =  Trim(InputBox("What is the TB Customer label? (e.g. 'ACS4.0.0')", "VSS GET Label",WScript.Arguments(1)))

Dim strAdkPath : strAdkPath = ""
Dim oSHL
If bInstallADK Then
	Set oSHL = CreateObject("Shell.Application")
	
	Dim oFolder : Set oFolder = oSHL.BrowseForFolder(0, "Locate the folder containing the ADK's 'setup.exe'"+CHR(13) + "WARNING: Must be a mapped drive and not a UNC path!", 0, &H0011)
	
	strAdkPath = oFolder.ParentFolder.ParseName(oFolder.Title).Path
	If strAdkPath = "" Then
		'If the user selected a file, we must get that file's parent so we can
		'work with it's path (since these functions only work on folders).
        Dim o : Set o = oFolder.ParentFolder
        strAdkPath = oFolder.Title
        strAdkPath = oFSO.BuildPath(o.ParentFolder.ParseName(o.Title).Path, strAdkPath)
	End If

End If


WScript.Echo "Opening database"
Dim oDB 
Set oDB = OpenDatabase(strDB, strUser, strPass)

WScript.Echo "Validating label"
If Not ValidateLabel(strVer) Then 
	WScript.Quit 1
End If

Dim strProjRoot : strProjRoot = ""
Dim strADK : strADK = ""
Dim strPatch : strPatch = ""

'
' Build the project path
'
WScript.Echo "Building project path"
If IsPatch(strVer) Then
	strPatch = Trim(InputBox("Is the patch on a VSS branch (leave blank if not)? If so what is the patch name? (e.g. '4.0 Patch A')", "Patch Label detected", ""))
	If strPatch = "" Then
		strProjRoot = FLSRC_REL
	Else
		strProjRoot = FLSRC_PATCH + "\" + strPatch + "\Build"
	End If
Else
	strProjRoot = FLSRC_REL
End If

'
' Create a new clean ADK folder and copy over the old one - date stamping to the current time and user
'
If bInstallADK Then
	WScript.Echo "Create a new clean ADK folder and copy over the old one - date stamping to the current time and user"
	If oFSO.FolderExists("C:\Scot") Then
		If oFSO.FolderExists("C:\Scot\ADK")	Then
			Dim strNew : strNew = strUser + "-" + CStr(Now())
			strNew = Replace(strNew, ":", ".")
			strNew = Replace(strNew, "\", ".")
			strNew = Replace(strNew, "/", ".")
			If Not RenameFolder(oFSO, "C:\Scot\ADK","C:\Scot\ADK-" + strNew) Then
				WScript.Echo "Failed to rename the existing ADK folder."
				WScript.Quit 1
			End If
			oFSO.CreateFolder("C:\Scot\ADK")
		Else
			oFSO.CreateFolder("C:\Scot\ADK")
		End If
	Else
		oFSO.CreateFolder("C:\Scot")
		oFSO.CreateFolder("C:\Scot\ADK")
	End If
		
	If Not oFSO.FolderExists("C:\Scot\ADK") Then
		WScript.Echo "Failed to create the C:\Scot\ADK folder."
		WScript.Quit 1
	End If

	oFSO.CreateFolder("C:\Scot\ADK\Include") 
	If Not oFSO.FolderExists("C:\Scot\ADK\Include") Then
		WScript.Echo "Failed to create the C:\Scot\ADK\Include folder."
		WScript.Quit 1
	End If
	
	oFSO.CreateFolder("C:\Scot\ADK\Lib")
	If Not oFSO.FolderExists("C:\Scot\ADK\Lib") Then
		WScript.Echo "Failed to create the C:\Scot\ADK\Lib folder."
		WScript.Quit 1
	End If
	

	WScript.Echo "Running the ADK installation from '" + strAdkPath + "'..."
	oSHL.ShellExecute strAdkPath + "\setup.exe", "","", "open", True
	'Dim oSHL : Set oSHL = CreateObject("Shell.Application")
	
	'oSHL.BrowseForFolder 0, "OpenFile", &H4031, &H0011
	
	'\\fl-integration\FLIM-FTP\FastLane Core Runtime Software (Patches)\Release 3.2\Current\FastLane 3.2 Patch B\FastLane 3.2 ADK CD Image Patch B BUILD 033 02262004\ADK
	'VBScript Example
	'Set WshNetwork = WScript.CreateObject("WScript.Network")
	'WshNetwork.MapNetworkDrive "S:", "\\server\share"
	
	'WshNetwork.RemoveNetworkDrive "S:", True, True
	'WshNetwork.RemoveNetworkDrive "\\server\share", True, True
End If

'
' Build the Build Target path based on TB customer and TB label
'
Dim strBuildTarget 

strBuildTarget = BUILD_ROOT + strCustomer + "\" + strCustLabel

WScript.Echo "The new build folder is: '" + strBuildTarget + "'"

'
' Get the Core App Source to the build location
'
WScript.Echo "Get the Core App Source to the build location"
GetProject strProjRoot + "\Core\Source", strVer, strBuildTarget + "\Source", TRUE


'
' Recursively delete the TransBroker folder
'
WScript.Echo "Recursively delete the TransBroker folder"
On Error Resume Next
oFSO.DeleteFolder strBuildTarget + "\Source\AfxTransBroker", True
On Error Goto 0

Set oDB = Nothing

'
' Get the transaction broker from the IM database
'

strDB = VSS_DB_IM
Set oDB = CreateObject("SourceSafe")
oDB.Open strDB, strUser, strPassIM

WScript.Echo "Get the Base TB Source to the build location"

GetProject IMSRC_BASE + "CURRENT_DEV\Source\", strBaseVersion, strBuildTarget + "\Source", TRUE

WScript.Echo "Get the Customer TB Source to the build location"
GetProject IMSRC_CUST + strCustomer + "\CURRENT_DEV\Source\", strCustLabel, strBuildTarget + "\Source", TRUE

WScript.Echo "Get all other installation files to the build location"
GetProject IMSRC_CUST + strCustomer + "\CURRENT_DEV\rap\", strCustLabel, strBuildTarget + "\Install\Rap", TRUE
GetProject IMSRC_CUST + strCustomer + "\CURRENT_DEV\SCOT\", strCustLabel, strBuildTarget + "\Install\Scot", TRUE
GetProject IMSRC_CUST + strCustomer + "\CURRENT_DEV\Common\", strCustLabel, strBuildTarget + "\Install\Common", TRUE
GetProject IMSRC_CUST + strCustomer + "\CURRENT_DEV\Build\", strCustLabel, strBuildTarget + "\Install\Build", TRUE

Set oDB = Nothing

Function OpenDatabase(strDB, strUser, strPass)
	
	Dim o : Set o = CreateObject("SourceSafe")
	
	On Error Resume Next	

	Dim iTries : iTries = 5
	Dim bAgain : bAgain = False
	Do
		o.Open strDB, strUser, strPass
		If Err.number > 1 Then
			Err.Clear
			WScript.Echo "Failed to open database, trying again..."
			iTries = iTries - 1
			If iTries <= 0 Then 
				bAgain = True
			End If
		Else
			bAgain = False
		End If
	Loop While bAgain

	On Error Goto 0
	
	Set OpenDatabase = o
	
End Function

Function RenameFolder(oFSO, strCur, strNew)

	RenameFolder = False
	Wscript.Echo "New folder: " + strNew
	If Not oFSO.FolderExists(strNew) Then
		WScript.Echo "Moving folder '" + strCur + "' to folder '" + strNew +"'"
		oFSO.MoveFolder strCur, strNew
		
		'The original folder should no longer be available
		If Not oFSO.FolderExists(strCur) Then
			RenameFolder = True
		End If			
	End If

End Function

Function IsPatch(ByVal strLabel)
	
	'Strip the end prefix temporarily for validation if necessary
	If Left(strLabel,4) = "END_" Then
		strLabel = Mid(strLabel, 5)
	End If
	IsPatch = Mid(strLabel,11,3) <> "000"
	
End Function

Function ValidateLabel(ByVal strLabel)
	
	'Strip the end prefix temporarily for validation if necessary
	If Left(strLabel,4) = "END_" Then
		strLabel = Mid(strLabel, 5)
	End If

	If Left(strLabel,6) = "Build_" Then
		strLabel = Mid(strLabel, 7)
	End If

	If Len(strLabel) <> 17 Then
		WScript.Echo "The Label is not 17 characters long."
		ValidateLabel = False
		Exit Function
	End If
	
	If Mid(strLabel, 2, 1) <> "." Or _
	   Mid(strLabel, 5, 1) <> "." Or _
	   Mid(strLabel, 8, 1) <> "." Or _
	   Mid(strLabel, 10, 1) <> "." Or _
	   Mid(strLabel, 14, 1) <> "." Then
	   WScript.Echo "The Label is not in the 9.99.99.9.999.999 format"
	   ValidateLabel = False
	   Exit Function
	End If
	
	ValidateLabel = True

End Function


Sub GetProject(strProject, strVersion, strLocalTarget, bRecurse)
  
  WScript.Echo "GetProject(" + strProject+ ", " + strVersion + ", " + strLocalTarget + ")"

  Dim oProjectLatest
  Dim oProjectVer
  Dim lFlags
  Dim strCheckVer
  
  Set oProjectLatest = oDB.VSSItem(strProject, FALSE)
  If IsEmpty(oProjectLatest) Then
	Wscript.Echo "ERROR:  Unknown project: " & strProject
	WScript.Quit 1
  End If

  If IsEmpty(strVersion) or Len(strVersion) = 0 Then
    Set oProjectVer = oProjectLatest
  Else
    Set oProjectVer = oProjectLatest.Version(strVersion)
	strCheckVer = oProjectVer.VersionNumber
	If (IsNumeric(strVersion) AND (strCheckVer <> strVersion)) OR IsEmpty(strCheckVer) Then
	  Wscript.Echo "ERROR: Unknown Version '"& strVersion & "' for project '" & strProject & "'"
	  Wscript.Quit 1
    End If
  End If
 
  if bRecurse Then
    lFlags = CLng(VSSFLAG_RECURSYES + VSSFLAG_REPREPLACE + VSSFLAG_TIMEMOD + VSSFLAG_FORCEDIRNO )
  else
    lFlags = CLng(VSSFLAG_RECURSNO + VSSFLAG_REPREPLACE + VSSFLAG_TIMEMOD + VSSFLAG_FORCEDIRNO )
  end If
  
  'VSSItem and VSSVersion objects behave differently and must be called differently.
  If IsEmpty(strVersion) or Len(strVersion) = 0 Then
	oProjectVer.Parent.LocalSpec = CStr(strLocalTarget)
	oProjectVer.Get oProjectVer.LocalSpec, CLng(lFlags)
  Else
	oProjectVer.LocalSpec = CStr(strLocalTarget)
	oProjectVer.Get oProjectVer.LocalSpec, CLng(lFlags)
  End If
  
End Sub