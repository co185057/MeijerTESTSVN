# Microsoft Developer Studio Project File - Name="SCOTSSF" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SCOTSSF - Win32 Unicode Debug_Test_CardDispenser
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Scotssf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Scotssf.mak" CFG="SCOTSSF - Win32 Unicode Debug_Test_CardDispenser"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SCOTSSF - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTSSF - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTSSF - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTSSF - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTSSF - Win32 Release_Test_CardDispenser" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTSSF - Win32 Debug_Test_CardDispenser" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTSSF - Win32 Unicode Debug_Test_CardDispenser" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTSSF - Win32 Unicode Release_Test_CardDispenser" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SCOTx Software/Source/Release 3.0/Core/Source/Scotssf", YSPBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SCOTSSF - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\SCOTApp" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_SCOTSSF_" /D "NewUI" /D "NewSec" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 SCOTServices.lib SCOTApp.lib CTransBroker.lib TransBroker.lib RPSW.lib PSXMFC.Lib SCOTPriceSound.lib version.lib winmm.lib PageClient.lib FastlaneUtils.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386 /libpath:"..\lib\release" /libpath:"..\ui\lib" /libpath:"c:\scot\adk\lib\release"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy release\SCOTSSF.lib ..\lib\release /r	xcopy               release\scotssf.dll c:\scot\bin\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\SCOTApp" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_SCOTSSF_" /D "NewUI" /D "NewSec" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 SCOTServicesD.lib SCOTAppD.lib CTransBrokerD.lib TransBrokerD.lib RPSWD.lib PSXMFCD.Lib SCOTPriceSoundD.lib version.lib winmm.lib PageClient.lib FastlaneUtilsD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug/SCOTSSFD.dll" /libpath:"..\lib\debug" /libpath:"..\ui\lib" /libpath:"C:\SCOT\ADK\Lib\debug"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy debug\SCOTSSFd.lib ..\lib\debug\*.* /r	xcopy               debug\scotssfd.dll c:\scot\bin\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SCOTSSF___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "SCOTSSF___Win32_Debug_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode"
# PROP Intermediate_Dir "Debug_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\SCOTApp" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_SCOTSSF_" /D "NewUI" /FD /c
# SUBTRACT BASE CPP /Fr /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\SCOTApp" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /D "_AFXEXT" /D "_SCOTSSF_" /D "NewUI" /D "NewSec" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SCOTServicesD.lib SCOTAppD.lib CTransBrokerD.lib TransBrokerD.lib RPSWD.lib PSXMFCD.Lib SCOTPriceSoundD.lib version.lib winmm.lib PageClient.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\SCOTSSFD.def" /out:"Debug/SCOTSSFD.dll" /libpath:"..\lib\debug" /libpath:"..\ui\lib" /libpath:"C:\SCOT\ADK\Lib\debug"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 SCOTServicesUD.lib SCOTAppUD.lib RPSWUD.lib CTransBrokerUD.lib TransBrokerUD.lib PSXMFCUD.Lib SCOTPriceSoundUD.lib version.lib winmm.lib PageClient.lib FastlaneUtilsUD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\SCOTSSFUD.def" /out:"Debug_Unicode/SCOTSSFUD.dll" /libpath:"..\lib\debug" /libpath:"..\ui\lib" /libpath:"C:\SCOT\ADK\Lib\debug"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Debug_Unicode\SCOTSSFUD.lib ..\lib\debug\*.* /r	xcopy               Debug_Unicode\scotssfud.dll c:\scot\bin\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SCOTSSF___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "SCOTSSF___Win32_Release_Unicode"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode"
# PROP Intermediate_Dir "Release_Unicode"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\SCOTApp" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_SCOTSSF_" /D "NewUI" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\SCOTApp" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /D "_AFXEXT" /D "_SCOTSSF_" /D "NewUI" /D "NewSec" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SCOTServices.lib SCOTApp.lib CTransBroker.lib TransBroker.lib RPSW.lib PSXMFC.Lib SCOTPriceSound.lib version.lib winmm.lib PageClient.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386 /libpath:"..\lib\release" /libpath:"..\ui\lib" /libpath:"c:\scot\adk\lib\release"
# ADD LINK32 SCOTServicesU.lib SCOTAppU.lib RPSWU.lib CTransBrokerU.lib TransBrokerU.lib PSXMFCU.Lib SCOTPriceSoundU.lib version.lib winmm.lib PageClient.lib FastlaneUtilsU.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386 /def:".\SCOTSSFU.def" /out:"Release_Unicode/ScotssfU.dll" /libpath:"..\lib\release" /libpath:"..\ui\lib" /libpath:"c:\scot\adk\lib\release"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Release_Unicode\SCOTSSFU.lib ..\lib\release /r	xcopy               Release_Unicode\scotssfu.dll c:\scot\bin\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Release_Test_CardDispenser"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SCOTSSF___Win32_Release_Test_CardDispenser"
# PROP BASE Intermediate_Dir "SCOTSSF___Win32_Release_Test_CardDispenser"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Test_CardDispenser"
# PROP Intermediate_Dir "Release_Test_CardDispenser"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\SCOTApp" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_SCOTSSF_" /D "NewUI" /D "NewSec" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\SCOTApp" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_SCOTSSF_" /D "NewUI" /D "NewSec" /D "_TESTSSF" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SCOTServices.lib SCOTApp.lib CTransBroker.lib TransBroker.lib RPSW.lib PSXMFC.Lib SCOTPriceSound.lib version.lib winmm.lib PageClient.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386 /libpath:"..\lib\release" /libpath:"..\ui\lib" /libpath:"c:\scot\adk\lib\release"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 SCOTServices.lib SCOTApp.lib CTransBroker.lib TransBroker.lib RPSW.lib PSXMFC.Lib SCOTPriceSound.lib version.lib winmm.lib PageClient.lib FASTLANEUtils.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386 /libpath:"..\lib\release" /libpath:"..\ui\lib" /libpath:"c:\scot\adk\lib\release"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Release_Test_CardDispenser\SCOTSSF.lib ..\lib\release /r	xcopy               Release_Test_CardDispenser\scotssf.dll c:\scot\bin\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Debug_Test_CardDispenser"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SCOTSSF___Win32_Debug_Test_CardDispenser"
# PROP BASE Intermediate_Dir "SCOTSSF___Win32_Debug_Test_CardDispenser"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Test_CardDispenser"
# PROP Intermediate_Dir "Debug_Test_CardDispenser"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\SCOTApp" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_SCOTSSF_" /D "NewUI" /D "NewSec" /FD /c
# SUBTRACT BASE CPP /Fr /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\SCOTApp" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_SCOTSSF_" /D "NewUI" /D "NewSec" /D "_TESTSSF" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SCOTServicesD.lib SCOTAppD.lib CTransBrokerD.lib TransBrokerD.lib RPSWD.lib PSXMFCD.Lib SCOTPriceSoundD.lib version.lib winmm.lib PageClient.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug/SCOTSSFD.dll" /libpath:"..\lib\debug" /libpath:"..\ui\lib" /libpath:"C:\SCOT\ADK\Lib\debug"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 SCOTServicesD.lib SCOTAppD.lib CTransBrokerD.lib TransBrokerD.lib RPSWD.lib PSXMFCD.Lib SCOTPriceSoundD.lib version.lib winmm.lib PageClient.lib FastlaneUtilsD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug_Test_CardDispenser/SCOTSSFD.dll" /libpath:"..\lib\debug" /libpath:"..\ui\lib" /libpath:"C:\SCOT\ADK\Lib\debug"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Debug_Test_CardDispenser\SCOTSSFd.lib ..\lib\debug\*.* /r	xcopy               Debug_Test_CardDispenser\scotssfd.dll c:\scot\bin\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Unicode Debug_Test_CardDispenser"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SCOTSSF___Win32_Unicode_Debug_Test_CardDispenser"
# PROP BASE Intermediate_Dir "SCOTSSF___Win32_Unicode_Debug_Test_CardDispenser"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode_Test_CardDispenser"
# PROP Intermediate_Dir "Debug_Unicode_Test_CardDispenser"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\SCOTApp" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /D "_AFXEXT" /D "_SCOTSSF_" /D "NewUI" /D "NewSec" /FD /c
# SUBTRACT BASE CPP /Fr /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\SCOTApp" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /D "_AFXEXT" /D "_SCOTSSF_" /D "NewUI" /D "NewSec" /D "_TESTSSF" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SCOTServicesUD.lib SCOTAppUD.lib RPSWUD.lib CTransBrokerUD.lib TransBrokerUD.lib PSXMFCUD.Lib SCOTPriceSoundUD.lib version.lib winmm.lib PageClient.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\SCOTSSFUD.def" /out:"Debug_Unicode/SCOTSSFUD.dll" /libpath:"..\lib\debug" /libpath:"..\ui\lib" /libpath:"C:\SCOT\ADK\Lib\debug"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 SCOTServicesUD.lib SCOTAppUD.lib RPSWUD.lib CTransBrokerUD.lib TransBrokerUD.lib PSXMFCUD.Lib SCOTPriceSoundUD.lib version.lib winmm.lib PageClient.lib FASTLANEUtilsUD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\SCOTSSFUD.def" /out:"Debug_Unicode_Test_CardDispenser/SCOTSSFUD.dll" /libpath:"..\lib\debug" /libpath:"..\ui\lib" /libpath:"C:\SCOT\ADK\Lib\debug"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Debug_Unicode_Test_CardDispenser\SCOTSSFUD.lib ..\lib\debug\*.* /r	xcopy               Debug_Unicode_Test_CardDispenser\scotssfud.dll c:\scot\bin\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Unicode Release_Test_CardDispenser"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SCOTSSF___Win32_Unicode_Release_Test_CardDispenser0"
# PROP BASE Intermediate_Dir "SCOTSSF___Win32_Unicode_Release_Test_CardDispenser0"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode_Test_CardDispenser"
# PROP Intermediate_Dir "Release_Unicode_Test_CardDispenser"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\SCOTApp" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /D "_AFXEXT" /D "_SCOTSSF_" /D "NewUI" /D "NewSec" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\SCOTApp" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /D "_AFXEXT" /D "_SCOTSSF_" /D "NewUI" /D "NewSec" /D "_TESTSSF" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SCOTServicesU.lib SCOTAppU.lib RPSWU.lib CTransBrokerU.lib TransBrokerU.lib PSXMFCU.Lib SCOTPriceSoundU.lib version.lib winmm.lib PageClient.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386 /def:".\SCOTSSFU.def" /out:"Release_Unicode/ScotssfU.dll" /libpath:"..\lib\release" /libpath:"..\ui\lib" /libpath:"c:\scot\adk\lib\release"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 SCOTServicesU.lib SCOTAppU.lib RPSWU.lib CTransBrokerU.lib TransBrokerU.lib PSXMFCU.Lib SCOTPriceSoundU.lib version.lib winmm.lib PageClient.lib FASTLANEUtilsU.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386 /def:".\SCOTSSFU.def" /out:"Release_Unicode_Test_CardDispenser/ScotssfU.dll" /libpath:"..\lib\release" /libpath:"..\ui\lib" /libpath:"c:\scot\adk\lib\release"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Release_Unicode_Test_CardDispenser\SCOTSSFU.lib ..\lib\release /r	xcopy               Release_Unicode_Test_CardDispenser\scotssfu.dll c:\scot\bin\*.* /r
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "SCOTSSF - Win32 Release"
# Name "SCOTSSF - Win32 Debug"
# Name "SCOTSSF - Win32 Unicode Debug"
# Name "SCOTSSF - Win32 Unicode Release"
# Name "SCOTSSF - Win32 Release_Test_CardDispenser"
# Name "SCOTSSF - Win32 Debug_Test_CardDispenser"
# Name "SCOTSSF - Win32 Unicode Debug_Test_CardDispenser"
# Name "SCOTSSF - Win32 Unicode Release_Test_CardDispenser"
# Begin Group "States"

# PROP Default_Filter ""
# Begin Group "Base etc."

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SMInProgress.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMInProgress.h
# End Source File
# Begin Source File

SOURCE=.\SMProcessing.cpp
# End Source File
# Begin Source File

SOURCE=.\SMProcessing.h
# End Source File
# Begin Source File

SOURCE=.\SMState.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMState.h
# End Source File
# Begin Source File

SOURCE=.\SMStateCU.cpp
# End Source File
# Begin Source File

SOURCE=.\SMStateCU.h
# End Source File
# Begin Source File

SOURCE=.\SMStateDM.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMStateDM.h
# End Source File
# Begin Source File

SOURCE=.\SMStatePS.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMStatePS.h
# End Source File
# Begin Source File

SOURCE=.\SMStateRP.cpp
# End Source File
# Begin Source File

SOURCE=.\SMStateRP.h
# End Source File
# Begin Source File

SOURCE=.\SMStateSA.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMStateSA.h
# End Source File
# Begin Source File

SOURCE=.\SMStateTB.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMStateTB.h
# End Source File
# Begin Source File

SOURCE=.\SMStateUE.h
# End Source File
# Begin Source File

SOURCE=.\UserExits.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# End Group
# Begin Group "Store Mode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SMSmAbort.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSmAbort.h
# End Source File
# Begin Source File

SOURCE=.\SMSmAborted.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSmAborted.h
# End Source File
# Begin Source File

SOURCE=.\SMSmAssistMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmAssistMenu.h
# End Source File
# Begin Source File

SOURCE=.\SMSmAssistMode.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSmAssistMode.h
# End Source File
# Begin Source File

SOURCE=.\SMSmAuthorization.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSmAuthorization.h
# End Source File
# Begin Source File

SOURCE=.\SMSmCardManagement.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmCardManagement.h
# End Source File
# Begin Source File

SOURCE=.\SMSmCashierPassword.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmCashierPassword.h
# End Source File
# Begin Source File

SOURCE=.\SMSmCashManagement.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmCashManagement.h
# End Source File
# Begin Source File

SOURCE=.\SMSmCashManagementSafePay.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmCashManagementSafePay.h
# End Source File
# Begin Source File

SOURCE=.\SMSmConfirmSignature.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSmConfirmSignature.h
# End Source File
# Begin Source File

SOURCE=.\SMSmConfirmSuspendedTransaction.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmConfirmSuspendedTransaction.h
# End Source File
# Begin Source File

SOURCE=.\SMSmCouponItems.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSmCouponItems.h
# End Source File
# Begin Source File

SOURCE=.\SMSmDataEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmDataEntry.h
# End Source File
# Begin Source File

SOURCE=.\SMSmEnterBirthdate.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmEnterBirthdate.h
# End Source File
# Begin Source File

SOURCE=.\SMSmmAddWeight.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmmAddWeight.h
# End Source File
# Begin Source File

SOURCE=.\SMSmMediaStatus.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSmMediaStatus.h
# End Source File
# Begin Source File

SOURCE=.\SMSmmEditAdd.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmmEditAdd.h
# End Source File
# Begin Source File

SOURCE=.\SMSmmEditWtExp.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmmEditWtExp.h
# End Source File
# Begin Source File

SOURCE=.\SMSmmEditWtSubCk.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmmEditWtSubCk.h
# End Source File
# Begin Source File

SOURCE=.\SMSmmEditWtTolerance.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmmEditWtTolerance.h
# End Source File
# Begin Source File

SOURCE=.\SMSmmInvalidWtTol.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmmInvalidWtTol.h
# End Source File
# Begin Source File

SOURCE=.\SMSmmKeyInWtTol.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmmKeyInWtTol.h
# End Source File
# Begin Source File

SOURCE=.\SMSmOutOfService.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSmOutOfService.h
# End Source File
# Begin Source File

SOURCE=.\SMSmPrintReceipt.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmPrintReceipt.h
# End Source File
# Begin Source File

SOURCE=.\SMSmReportsMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmReportsMenu.h
# End Source File
# Begin Source File

SOURCE=.\SMSmRestrictedItems.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSmRestrictedItems.h
# End Source File
# Begin Source File

SOURCE=.\SMSmRunReports.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmRunReports.h
# End Source File
# Begin Source File

SOURCE=.\SMSmSecurityLogs.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSmSecurityLogs.h
# End Source File
# Begin Source File

SOURCE=.\SMSmSelectModeOfOperation.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmSelectModeOfOperation.h
# End Source File
# Begin Source File

SOURCE=.\SMSmSystemFunctions.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSmSystemFunctions.h
# End Source File
# Begin Source File

SOURCE=.\SMSmUpdateItem.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmUpdateItem.h
# End Source File
# Begin Source File

SOURCE=.\SMSmUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmUtility.h
# End Source File
# Begin Source File

SOURCE=.\SMSmVisualItems.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmVisualItems.h
# End Source File
# Begin Source File

SOURCE=.\SMSmVoidedItems.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSmVoidedItems.h
# End Source File
# Begin Source File

SOURCE=.\SMStoreModeMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\SMStoreModeMenu.h
# End Source File
# Begin Source File

SOURCE=.\SMStoreModeState.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMStoreModeState.h
# End Source File
# End Group
# Begin Group "Customer Mode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SMAcknowledgeSig.cpp
# End Source File
# Begin Source File

SOURCE=.\SMAcknowledgeSig.h
# End Source File
# Begin Source File

SOURCE=.\SMAttract.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMAttract.h
# End Source File
# Begin Source File

SOURCE=.\SMBagAndEas.cpp
# End Source File
# Begin Source File

SOURCE=.\SMBagAndEas.h
# End Source File
# Begin Source File

SOURCE=.\SMCardMisRead.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMCardMisRead.h
# End Source File
# Begin Source File

SOURCE=.\SMCardProcessing.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMCardProcessing.h
# End Source File
# Begin Source File

SOURCE=.\SMCashPayment.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMCashPayment.h
# End Source File
# Begin Source File

SOURCE=.\SMCheckBasketReminder.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCheckBasketReminder.h
# End Source File
# Begin Source File

SOURCE=.\SMCmDataEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCmDataEntry.h
# End Source File
# Begin Source File

SOURCE=.\SMConfirmDebitAmount.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMConfirmDebitAmount.h
# End Source File
# Begin Source File

SOURCE=.\SMConfirmEBTAmount.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMConfirmEBTAmount.h
# End Source File
# Begin Source File

SOURCE=.\SMConfirmQuantity.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMConfirmQuantity.h
# End Source File
# Begin Source File

SOURCE=.\SMConfirmVoid.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMConfirmVoid.h
# End Source File
# Begin Source File

SOURCE=.\SMCouponNoMatch.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMCouponNoMatch.h
# End Source File
# Begin Source File

SOURCE=.\SMCouponNotAllowed.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMCouponNotAllowed.h
# End Source File
# Begin Source File

SOURCE=.\SMCouponTooHigh.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCouponTooHigh.h
# End Source File
# Begin Source File

SOURCE=.\SMCrateableItem.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCrateableItem.h
# End Source File
# Begin Source File

SOURCE=.\SMCustomMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCustomMessage.h
# End Source File
# Begin Source File

SOURCE=.\SMDepositCoupons.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMDepositCoupons.h
# End Source File
# Begin Source File

SOURCE=.\SMDepositGiftCard.cpp
# End Source File
# Begin Source File

SOURCE=.\SMDepositGiftCard.h
# End Source File
# Begin Source File

SOURCE=.\SMDropoffCoupons.cpp
# End Source File
# Begin Source File

SOURCE=.\SMDropoffCoupons.h
# End Source File
# Begin Source File

SOURCE=.\SMElectronicBenefitsTransfer.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMElectronicBenefitsTransfer.h
# End Source File
# Begin Source File

SOURCE=.\SMEnterAlphaNumericInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SMEnterAlphaNumericInfo.h
# End Source File
# Begin Source File

SOURCE=.\SMEnterCoupons.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMEnterCoupons.h
# End Source File
# Begin Source File

SOURCE=.\SMEnterCouponValue.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMEnterCouponValue.h
# End Source File
# Begin Source File

SOURCE=.\SMEnterPin.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMEnterPin.h
# End Source File
# Begin Source File

SOURCE=.\SMEnterQuantity.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMEnterQuantity.h
# End Source File
# Begin Source File

SOURCE=.\SMEnterWeight.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMEnterWeight.h
# End Source File
# Begin Source File

SOURCE=.\SMEnterWtForPriceEmbedded.cpp
# End Source File
# Begin Source File

SOURCE=.\SMEnterWtForPriceEmbedded.h
# End Source File
# Begin Source File

SOURCE=.\SMFinish.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMFinish.h
# End Source File
# Begin Source File

SOURCE=.\SMGetFoodStampState.cpp
# End Source File
# Begin Source File

SOURCE=.\SMGetFoodStampState.h
# End Source File
# Begin Source File

SOURCE=.\SMInsertCoupon.cpp
# End Source File
# Begin Source File

SOURCE=.\SMInsertCoupon.h
# End Source File
# Begin Source File

SOURCE=.\SMInsertGiftCard.cpp
# End Source File
# Begin Source File

SOURCE=.\SMInsertGiftCard.h
# End Source File
# Begin Source File

SOURCE=.\SMItemNotForSale.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMItemNotForSale.h
# End Source File
# Begin Source File

SOURCE=.\SMItemQuantityExceeded.cpp
# End Source File
# Begin Source File

SOURCE=.\SMItemQuantityExceeded.h
# End Source File
# Begin Source File

SOURCE=.\SMKeyInCode.cpp
# End Source File
# Begin Source File

SOURCE=.\SMKeyInCode.h
# End Source File
# Begin Source File

SOURCE=.\SMLimitedEBTBenefit.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMLimitedEBTBenefit.h
# End Source File
# Begin Source File

SOURCE=.\SMLookUpItem.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMLookUpItem.h
# End Source File
# Begin Source File

SOURCE=.\SMOutOfService.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMOutOfService.h
# End Source File
# Begin Source File

SOURCE=.\SMOutOfService2.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMOutOfService2.h
# End Source File
# Begin Source File

SOURCE=.\SMProduceFavorites.cpp
# End Source File
# Begin Source File

SOURCE=.\SMProduceFavorites.h
# End Source File
# Begin Source File

SOURCE=.\SMPutBagOnScale.cpp
# End Source File
# Begin Source File

SOURCE=.\SMPutBagOnScale.h
# End Source File
# Begin Source File

SOURCE=.\SMRemoteScannerConnect.cpp
# End Source File
# Begin Source File

SOURCE=.\SMRemoteScannerConnect.h
# End Source File
# Begin Source File

SOURCE=.\SMRequestSig.cpp
# End Source File
# Begin Source File

SOURCE=.\SMRequestSig.h
# End Source File
# Begin Source File

SOURCE=.\SMRestrictedNotAllowed.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMRestrictedNotAllowed.h
# End Source File
# Begin Source File

SOURCE=.\SMSavePreferences.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSavePreferences.h
# End Source File
# Begin Source File

SOURCE=.\SMScanAndBag.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMScanAndBag.h
# End Source File
# Begin Source File

SOURCE=.\SMScanCard.cpp
# End Source File
# Begin Source File

SOURCE=.\SMScanCard.h
# End Source File
# Begin Source File

SOURCE=.\SMSelectCardType.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSelectCardType.h
# End Source File
# Begin Source File

SOURCE=.\SMSelectContainer.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSelectContainer.h
# End Source File
# Begin Source File

SOURCE=.\SMSelectDebitType.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSelectDebitType.h
# End Source File
# Begin Source File

SOURCE=.\SMSelectFromList.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSelectFromList.h
# End Source File
# Begin Source File

SOURCE=.\SMSelectLanguage.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSelectLanguage.h
# End Source File
# Begin Source File

SOURCE=.\SMSelectPayment.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSelectPayment.h
# End Source File
# Begin Source File

SOURCE=.\SMSelectTare.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSelectTare.h
# End Source File
# Begin Source File

SOURCE=.\SMSellBags.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSellBags.h
# End Source File
# Begin Source File

SOURCE=.\SMSetAllItemsAside.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSetAllItemsAside.h
# End Source File
# Begin Source File

SOURCE=.\SMSuspend.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSuspend.h
# End Source File
# Begin Source File

SOURCE=.\SMSwipeCard.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSwipeCard.h
# End Source File
# Begin Source File

SOURCE=.\SMTakeCard.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMTakeCard.h
# End Source File
# Begin Source File

SOURCE=.\SMTakeChange.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMTakeChange.h
# End Source File
# Begin Source File

SOURCE=.\SMTakeLoyaltyCard.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMTakeLoyaltyCard.h
# End Source File
# Begin Source File

SOURCE=.\SMTakeReceipt.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMTakeReceipt.h
# End Source File
# Begin Source File

SOURCE=.\SMTenderBalance.cpp
# End Source File
# Begin Source File

SOURCE=.\SMTenderBalance.h
# End Source File
# Begin Source File

SOURCE=.\SMThankYou.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMThankYou.h
# End Source File
# Begin Source File

SOURCE=.\SMTimeRestrictedItem.cpp
# End Source File
# Begin Source File

SOURCE=.\SMTimeRestrictedItem.h
# End Source File
# Begin Source File

SOURCE=.\SMTransportItem.cpp
# End Source File
# Begin Source File

SOURCE=.\SMTransportItem.h
# End Source File
# Begin Source File

SOURCE=.\SMUnknownItem.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMUnknownItem.h
# End Source File
# Begin Source File

SOURCE=.\SMUnknownPrice.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMUnknownPrice.h
# End Source File
# Begin Source File

SOURCE=.\SMVoidItem.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMVoidItem.h
# End Source File
# Begin Source File

SOURCE=.\SMVoidNoMatch.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMVoidNoMatch.h
# End Source File
# Begin Source File

SOURCE=.\SMVoucherPayment.cpp
# End Source File
# Begin Source File

SOURCE=.\SMVoucherPayment.h
# End Source File
# End Group
# Begin Group "Help Mode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SMAssistedTender.cpp
# End Source File
# Begin Source File

SOURCE=.\SMAssistedTender.h
# End Source File
# Begin Source File

SOURCE=.\SMCollectGiftCard.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCollectGiftCard.h
# End Source File
# Begin Source File

SOURCE=.\SMConfirmAbort.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMConfirmAbort.h
# End Source File
# Begin Source File

SOURCE=.\SMContextHelp.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMContextHelp.h
# End Source File
# Begin Source File

SOURCE=.\SMContinueTrans.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMContinueTrans.h
# End Source File
# Begin Source File

SOURCE=.\SMDegradedContinueTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\SMDegradedContinueTrans.h
# End Source File
# Begin Source File

SOURCE=.\SMFatalError.cpp
# End Source File
# Begin Source File

SOURCE=.\SMFatalError.h
# End Source File
# Begin Source File

SOURCE=.\SMHelpModeState.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMHelpModeState.h
# End Source File
# Begin Source File

SOURCE=.\SMHelpOnWay.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMHelpOnWay.h
# End Source File
# Begin Source File

SOURCE=.\SMHostOffLine.cpp
# End Source File
# Begin Source File

SOURCE=.\SMHostOffLine.h
# End Source File
# Begin Source File

SOURCE=.\SMLaneClosed.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMLaneClosed.h
# End Source File
# Begin Source File

SOURCE=.\SMOperatorPasswordState.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMOperatorPasswordState.h
# End Source File
# Begin Source File

SOURCE=.\SMPickingUpItems.cpp
# End Source File
# Begin Source File

SOURCE=.\SMPickingUpItems.h
# End Source File
# Begin Source File

SOURCE=.\SMRAPDataNeeded.cpp
# End Source File
# Begin Source File

SOURCE=.\SMRAPDataNeeded.h
# End Source File
# Begin Source File

SOURCE=.\SMScaleBroken.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMScaleBroken.h
# End Source File
# Begin Source File

SOURCE=.\SMSecBagAreaBackup.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecBagAreaBackup.h
# End Source File
# Begin Source File

SOURCE=.\SMSecBagBoxNotValid.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecBagBoxNotValid.h
# End Source File
# Begin Source File

SOURCE=.\SMSecBagBoxThreshold.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecBagBoxThreshold.h
# End Source File
# Begin Source File

SOURCE=.\SMSecItemRemovedThreshold.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecItemRemovedThreshold.h
# End Source File
# Begin Source File

SOURCE=.\SMSecMisMatchWeight.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecMisMatchWeight.h
# End Source File
# Begin Source File

SOURCE=.\SMSecNewItemEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecNewItemEntry.h
# End Source File
# Begin Source File

SOURCE=.\SMSecOperatorPWState.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecOperatorPWState.h
# End Source File
# Begin Source File

SOURCE=.\SMSecSkipBaggingThreshold.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecSkipBaggingThreshold.h
# End Source File
# Begin Source File

SOURCE=.\SMSecUnexpectedDecrease.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecUnexpectedDecrease.h
# End Source File
# Begin Source File

SOURCE=.\SMSecUnExpectedIncrease.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecUnExpectedIncrease.h
# End Source File
# Begin Source File

SOURCE=.\SMSecurityLevel2.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSecurityLevel2.h
# End Source File
# Begin Source File

SOURCE=.\SMSecurityLevel3.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSecurityLevel3.h
# End Source File
# Begin Source File

SOURCE=.\SMSignCharge.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSignCharge.h
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage.h
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage1.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage1.h
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage2.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage2.h
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage3.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage3.h
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage4.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage4.h
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage5.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage5.h
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage6.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage6.h
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessageOpPass.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessageOpPass.h
# End Source File
# Begin Source File

SOURCE=.\SMUnDeActivatedItemApproval.cpp
# End Source File
# Begin Source File

SOURCE=.\SMUnDeActivatedItemApproval.h
# End Source File
# Begin Source File

SOURCE=.\SMVoidApproval.cpp
# End Source File
# Begin Source File

SOURCE=.\SMVoidApproval.h
# End Source File
# Begin Source File

SOURCE=.\SMVoidTransApproval.cpp
# End Source File
# Begin Source File

SOURCE=.\SMVoidTransApproval.h
# End Source File
# Begin Source File

SOURCE=.\SMWaitForApproval.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SMWaitForApproval.h
# End Source File
# Begin Source File

SOURCE=.\SMWaitForRemoteAssistance.cpp
# End Source File
# Begin Source File

SOURCE=.\SMWaitForRemoteAssistance.h
# End Source File
# End Group
# Begin Group "New Screens"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SMExtensionState.cpp
# End Source File
# Begin Source File

SOURCE=.\SMExtensionState.h
# End Source File
# End Group
# End Group
# Begin Group "Main Program"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SCOTSSF.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SCOTSSF.def

!IF  "$(CFG)" == "SCOTSSF - Win32 Release"

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Unicode Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Release_Test_CardDispenser"

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Debug_Test_CardDispenser"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Unicode Debug_Test_CardDispenser"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Unicode Release_Test_CardDispenser"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SCOTSSF.h
# End Source File
# Begin Source File

SOURCE=.\SCOTSSF.rc
# End Source File
# Begin Source File

SOURCE=.\res\SCOTSSF.rc2
# End Source File
# Begin Source File

SOURCE=.\SCOTSSFD.def

!IF  "$(CFG)" == "SCOTSSF - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Debug"

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Release_Test_CardDispenser"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Debug_Test_CardDispenser"

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Unicode Debug_Test_CardDispenser"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Unicode Release_Test_CardDispenser"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SCOTSSFUD.def

!IF  "$(CFG)" == "SCOTSSF - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Release_Test_CardDispenser"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Debug_Test_CardDispenser"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Unicode Debug_Test_CardDispenser"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTSSF - Win32 Unicode Release_Test_CardDispenser"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Presentation Services"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PSAssistMode.cpp
# End Source File
# Begin Source File

SOURCE=.\PSCustomerMode.cpp
# End Source File
# Begin Source File

SOURCE=.\PSProcedures.cpp
# End Source File
# Begin Source File

SOURCE=.\PSProcedures.h
# End Source File
# Begin Source File

SOURCE=.\PSStoreMode.cpp
# End Source File
# End Group
# Begin Group "Customer Information"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CustomerInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerInfo.h
# End Source File
# End Group
# Begin Group "Test_CardDispenser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFAcceptGiftCard.cpp
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFAcceptGiftCard.h
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFCardDispenserHealthCheckFailed.cpp
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFCardDispenserHealthCheckFailed.h
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFCardDispenserReadFailed.cpp
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFCardDispenserReadFailed.h
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFCardDispenserTakeCard.cpp
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFCardDispenserTakeCard.h
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFCardRechargeDispenserTakeCard.cpp
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFCardRechargeDispenserTakeCard.h
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFGiftCardBalanceReadCard.cpp
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFGiftCardBalanceReadCard.h
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFGiftCardDisplayBalance.cpp
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFGiftCardDisplayBalance.h
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFGiftCardRechargeAmount.cpp
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFGiftCardRechargeAmount.h
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFIssueNewGiftCard.cpp
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFIssueNewGiftCard.h
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFIssueRecharge.cpp
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFIssueRecharge.h
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFNewGiftCardAmount.cpp
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFNewGiftCardAmount.h
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFSelectGiftCardActivity.cpp
# End Source File
# Begin Source File

SOURCE=.\Test_CardDispenser\SMSSFSelectGiftCardActivity.h
# End Source File
# End Group
# Begin Group "USSF"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SMStateUSSF.cpp
# End Source File
# Begin Source File

SOURCE=.\SMStateUSSF.h
# End Source File
# Begin Source File

SOURCE=.\SMUSSFManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SMUSSFManager.h
# End Source File
# Begin Source File

SOURCE=.\USSFUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\USSFUtility.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
