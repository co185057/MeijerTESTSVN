# Microsoft Developer Studio Project File - Name="SCOTApp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SCOTApp - Win32 Unicode Debug BuildLib
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SCOTApp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SCOTApp.mak" CFG="SCOTApp - Win32 Unicode Debug BuildLib"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SCOTApp - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SCOTApp - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "SCOTApp - Win32 Release BuildLib" (based on "Win32 (x86) Application")
!MESSAGE "SCOTApp - Win32 Debug BuildLib" (based on "Win32 (x86) Application")
!MESSAGE "SCOTApp - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "SCOTApp - Win32 Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "SCOTApp - Win32 Unicode Release BuildLib" (based on "Win32 (x86) Application")
!MESSAGE "SCOTApp - Win32 Unicode Debug BuildLib" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SCOTx Software/Source/Release 3.0/Core/Source/SCOTApp", VSPBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SCOTApp - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_SCOT" /D "NewUI" /D "NewSec" /YX"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 SCOTServices.lib CTransBroker.lib TransBroker.lib SCOTSSF.lib RPSW.lib psxmfc.Lib SCOTPriceSound.lib version.lib winmm.lib PageClient.lib SCOTUtil.lib ws2_32.lib ScotBusyStatus.lib RemoteConnection.lib remotemessage.lib FastlaneUtils.lib AuthenticationClientLibrary.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /libpath:"..\lib\release" /libpath:"..\ui\lib" /libpath:"c:\scot\adk\lib\release"
# SUBTRACT LINK32 /pdb:none /incremental:yes
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy .\release\SCOTAPP.lib ..\lib\release\*.* /r	xcopy                release\scotapp.exe c:\scot\bin\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTApp - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_SCOT" /D "NewUI" /D "NewSec" /Fr /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 SCOTServicesD.lib CTransBrokerD.lib TransBrokerD.lib SCOTSSFD.lib RPSWD.lib SCOTPriceSoundD.lib RemoteConnectionD.lib version.lib winmm.lib PageClient.lib PSXMFCD.lib SCOTUtilD.lib ws2_32.lib ScotBusyStatusD.lib RemoteMessageD.lib FastlaneUtilsD.lib AuthenticationClientLibraryD.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/SCOTAppD.exe" /libpath:"..\lib\debug" /libpath:"..\ui\lib" /libpath:"C:\SCOT\ADK\Lib\debug"
# SUBTRACT LINK32 /pdb:none /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy .\debug\SCOTAPPD.lib ..\lib\debug /r	xcopy                debug\ScotappD.exe c:\scot\bin\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTApp - Win32 Release BuildLib"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SCOTApp0"
# PROP BASE Intermediate_Dir "SCOTApp0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "SCOTApp0"
# PROP Intermediate_Dir "SCOTApp0"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "c:\scot\adk\include" /I "..\include" /I "..\scotssf" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_SCOT" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_SCOT" /D "_BUILDLIB_" /D "NewUI" /D "NewSec" /YX"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SCOTPS.Lib SCOTPriceSound.lib version.lib winmm.lib CTransBroker.lib TransBroker.lib SCOTSSF.lib /nologo /subsystem:windows /incremental:yes /machine:I386 /libpath:"c:\scot\adk\lib\release" /libpath:"..\lib\release"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 SCOTServices.lib CTransBroker.lib TransBroker.lib RPSW.lib psxmfc.Lib SCOTPriceSound.lib version.lib winmm.lib PageClient.lib SCOTUtil.lib ws2_32.lib ScotBusyStatus.lib RemoteConnection.lib RPSW.lib RemoteMessage.lib RemoteMessage.lib FastlaneUtils.lib AuthenticationClientLibrary.lib /nologo /subsystem:windows /machine:I386 /libpath:"..\lib\release" /libpath:"..\ui\lib" /libpath:"c:\scot\adk\lib\release"
# SUBTRACT LINK32 /pdb:none /incremental:yes
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy SCOTApp0\SCOTAPP.lib ..\lib\release\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTApp - Win32 Debug BuildLib"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SCOTApp_"
# PROP BASE Intermediate_Dir "SCOTApp_"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SCOTApp_"
# PROP Intermediate_Dir "SCOTApp_"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\scotssf" /I "..\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /I "c:\scot\adk\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_SCOT" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_SCOT" /D "_BUILDLIB_" /D "NewUI" /D "NewSec" /Fr /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SCOTPS.Lib SCOTPriceSound.lib version.lib winmm.lib CTransBrokerD.lib TransBrokerD.lib SCOTSSFD.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/SCOTAppD.exe" /libpath:"c:\scot\adk\lib\debug" /libpath:"..\lib\debug"
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 SCOTServicesD.lib CTransBrokerD.lib TransBrokerD.lib RPSWD.lib SCOTPriceSoundD.lib version.lib winmm.lib PageClient.lib SCOTUtilD.lib ws2_32.lib ScotBusyStatusD.lib RemoteConnectionD.lib PSXMFCD.lib RemoteMessageD.lib FastlaneUtilsd.lib AuthenticationClientLibraryD.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"SCOTAPP_/SCOTAppD.exe" /libpath:"..\lib\debug" /libpath:"..\ui\lib" /libpath:"c:\scot\adk\lib\debug"
# SUBTRACT LINK32 /pdb:none /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy .\SCOTAPP_\SCOTAPPD.lib ..\lib\debug\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTApp - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SCOTApp___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "SCOTApp___Win32_Debug_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode"
# PROP Intermediate_Dir "Debug_Unicode"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_SCOT" /D "NewUI" /Fr /YX"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /D "_SCOT" /D "NewUI" /D "NewSec" /Fr /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SCOTServicesD.lib SCOTRAD.lib CTransBrokerD.lib TransBrokerD.lib SCOTSSFD.lib RPSWD.lib SCOTPriceSoundD.lib version.lib winmm.lib PageClient.lib PSXD.lib SCOTUtilD.lib ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/SCOTAppD.exe" /libpath:"..\lib\debug" /libpath:"..\ui\lib" /libpath:"C:\SCOT\ADK\Lib\debug"
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 SCOTServicesUD.lib RPSWUD.lib CTransBrokerUD.lib TransBrokerUD.lib SCOTSSFUD.lib SCOTPriceSoundUD.lib version.lib winmm.lib PageClient.lib PSXMFCUD.lib SCOTUtilUD.lib ws2_32.lib ScotBusyStatusUD.lib RemoteConnectionUD.lib RemoteMessageUD.lib RemoteMessageUD.lib FastlaneUtilsUD.lib AuthenticationClientLibraryUD.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"Debug_Unicode/SCOTAppUD.exe" /libpath:"..\lib\debug" /libpath:"..\ui\lib" /libpath:"C:\SCOT\ADK\Lib\debug"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy .\Debug_Unicode\SCOTAPPUD.lib ..\lib\debug /r /y	xcopy Debug_Unicode\ScotappUD.exe c:\scot\bin\*.* /r /y	xcopy Debug_Unicode\ScotappUD.pdb c:\scot\bin\*.* /r /y
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTApp - Win32 Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SCOTApp___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "SCOTApp___Win32_Release_Unicode"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode"
# PROP Intermediate_Dir "Release_Unicode"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_SCOT" /D "NewUI" /YX"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /D "_SCOT" /D "NewUI" /D "NewSec" /YX"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SCOTServices.lib SCOTRA.lib CTransBroker.lib TransBroker.lib SCOTSSF.lib RPSW.lib psx.Lib SCOTPriceSound.lib version.lib winmm.lib PageClient.lib SCOTUtil.lib ws2_32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /libpath:"..\lib\release" /libpath:"..\ui\lib" /libpath:"c:\scot\adk\lib\release"
# SUBTRACT BASE LINK32 /pdb:none /incremental:yes
# ADD LINK32 SCOTServicesU.lib RPSWU.lib CTransBrokerU.lib TransBrokerU.lib SCOTSSFU.lib psxmfcu.Lib SCOTPriceSoundU.lib version.lib winmm.lib PageClient.lib SCOTUtilU.lib ws2_32.lib ScotBusyStatusU.lib RemoteConnectionU.lib RemoteMessageU.lib RemoteMessageU.lib FastlaneUtilsU.lib AuthenticationClientLibraryU.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /map /debug /debugtype:both /machine:I386 /out:"Release_Unicode/SCOTAppU.exe" /libpath:"..\lib\release" /libpath:"..\ui\lib" /libpath:"c:\scot\adk\lib\release"
# SUBTRACT LINK32 /pdb:none /incremental:yes
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy .\Release_Unicode\SCOTAPPU.lib ..\lib\release\*.* /r /y	xcopy Release_Unicode\scotappu.exe c:\scot\bin\*.* /r /y	xcopy Release_Unicode\scotappu.pdb c:\scot\bin\*.* /r /y
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTApp - Win32 Unicode Release BuildLib"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SCOTApp___Win32_Release_Unicode_BuildLib"
# PROP BASE Intermediate_Dir "SCOTApp___Win32_Release_Unicode_BuildLib"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode_BuildLib"
# PROP Intermediate_Dir "Release_Unicode_BuildLib"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_SCOT" /D "_BUILDLIB_" /D "NewUI" /YX"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /D "_SCOT" /D "_BUILDLIB_" /D "NewUI" /D "NewSec" /YX"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SCOTServices.lib scotra.lib CTransBroker.lib TransBroker.lib RPSW.lib psx.Lib SCOTPriceSound.lib version.lib winmm.lib PageClient.lib SCOTUtil.lib ws2_32.lib /nologo /subsystem:windows /machine:I386 /libpath:"..\lib\release" /libpath:"..\ui\lib" /libpath:"c:\scot\adk\lib\release"
# SUBTRACT BASE LINK32 /pdb:none /incremental:yes
# ADD LINK32 SCOTServicesU.lib CTransBrokerU.lib TransBrokerU.lib RPSWU.lib psxmfcu.Lib SCOTPriceSoundU.lib version.lib winmm.lib PageClient.lib SCOTUtilU.lib ws2_32.lib ScotBusyStatusU.lib RemoteConnectionU.lib RemoteMessageU.lib FastlaneUtilsU.lib AuthenticationClientLibraryU.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"Release_Unicode_BuildLib/SCOTAppU.exe" /libpath:"..\lib\release" /libpath:"..\ui\lib" /libpath:"c:\scot\adk\lib\release"
# SUBTRACT LINK32 /pdb:none /incremental:yes
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Release_Unicode_BuildLib\SCOTAPPU.lib ..\lib\release\*.* /r /y
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTApp - Win32 Unicode Debug BuildLib"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SCOTApp___Win32_Debug_Unicode_BuildLib"
# PROP BASE Intermediate_Dir "SCOTApp___Win32_Debug_Unicode_BuildLib"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode_BuildLib"
# PROP Intermediate_Dir "Debug_Unicode_BuildLib"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_SCOT" /D "_BUILDLIB_" /D "NewUI" /Fr /YX"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "..\ui\include" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /D "_SCOT" /D "_BUILDLIB_" /D "NewUI" /D "NewSec" /Fr /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SCOTServicesD.lib scotrad.lib CTransBrokerD.lib TransBrokerD.lib RPSWD.lib PSXD.Lib SCOTPriceSoundD.lib version.lib winmm.lib PageClient.lib SCOTUtilD.lib ws2_32.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"SCOTAPP_/SCOTAppD.exe" /libpath:"..\lib\debug" /libpath:"..\ui\lib" /libpath:"c:\scot\adk\lib\debug"
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 SCOTServicesUD.lib CTransBrokerUD.lib TransBrokerUD.lib RPSWUD.lib PSXMFCUD.Lib SCOTPriceSoundUD.lib version.lib winmm.lib PageClient.lib SCOTUtilUD.lib ws2_32.lib ScotBusyStatusUD.lib RemoteConnectionUD.lib RemoteMessageUD.lib FastlaneUtilsUD.lib AuthenticationClientLibraryUD.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /map /debug /machine:I386 /out:"Debug_Unicode_BuildLib/SCOTAppUD.exe" /libpath:"..\lib\debug" /libpath:"..\ui\lib" /libpath:"c:\scot\adk\lib\debug"
# SUBTRACT LINK32 /pdb:none /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy .\Debug_Unicode_BuildLib\SCOTAPPUD.lib ..\lib\debug\*.* /r /y
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "SCOTApp - Win32 Release"
# Name "SCOTApp - Win32 Debug"
# Name "SCOTApp - Win32 Release BuildLib"
# Name "SCOTApp - Win32 Debug BuildLib"
# Name "SCOTApp - Win32 Unicode Debug"
# Name "SCOTApp - Win32 Unicode Release"
# Name "SCOTApp - Win32 Unicode Release BuildLib"
# Name "SCOTApp - Win32 Unicode Debug BuildLib"
# Begin Group "Main Program"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AppRestartCount.cpp
# End Source File
# Begin Source File

SOURCE=.\AppRestartCount.h
# End Source File
# Begin Source File

SOURCE=.\Common.h
# End Source File
# Begin Source File

SOURCE=.\CommonNCR.h
# End Source File
# Begin Source File

SOURCE=.\ConfigData.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigData.h
# End Source File
# Begin Source File

SOURCE=.\ConfigDynamic.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigObject.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigObject.h
# End Source File
# Begin Source File

SOURCE=.\DataEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\DataEntry.h
# End Source File
# Begin Source File

SOURCE=.\InstoreBarcodes.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemObject.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemObject.h
# End Source File
# Begin Source File

SOURCE=.\Library.h
# End Source File
# Begin Source File

SOURCE=.\MPProcedures.cpp
# End Source File
# Begin Source File

SOURCE=.\MPProcedures.h
# End Source File
# Begin Source File

SOURCE=.\NamedStringContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\NamedStringContainer.h
# End Source File
# Begin Source File

SOURCE=.\PSConfigData.cpp
# End Source File
# Begin Source File

SOURCE=.\PSConfigData.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SCOTApp.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTApp.h
# End Source File
# Begin Source File

SOURCE=.\res\SCOTApp.ico
# End Source File
# Begin Source File

SOURCE=.\SCOTApp.rc
# End Source File
# Begin Source File

SOURCE=.\res\SCOTApp.rc2
# End Source File
# Begin Source File

SOURCE=.\SCOTAPPDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTAPPDlg.h
# End Source File
# Begin Source File

SOURCE=.\SCOTDataEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTDataEntry.h
# End Source File
# Begin Source File

SOURCE=.\SigReceiptDataFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SigReceiptDataFile.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Device Manager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\dmx\CardDispenser.cpp
# End Source File
# Begin Source File

SOURCE=.\dmx\CardDispenser.h
# End Source File
# Begin Source File

SOURCE=.\dmx\CashDrawerWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\dmx\CashDrawerWrapper.h
# End Source File
# Begin Source File

SOURCE=.\dmx\CashRecyclerWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\dmx\CashRecyclerWrapper.h
# End Source File
# Begin Source File

SOURCE=.\devmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\devmgr.h
# End Source File
# Begin Source File

SOURCE=.\dmx\DispatchHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\dmx\DispatchHelper.h
# End Source File
# Begin Source File

SOURCE=.\dmx\DMInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\dmx\DMInterface.h
# End Source File
# Begin Source File

SOURCE=.\DMX\DMiPrinter.cpp
# End Source File
# Begin Source File

SOURCE=.\DMMessageExtension.cpp
# End Source File
# Begin Source File

SOURCE=.\DMMessageExtension.h
# End Source File
# Begin Source File

SOURCE=.\DMProcedures.cpp
# End Source File
# Begin Source File

SOURCE=.\DMProcedures.h
# End Source File
# Begin Source File

SOURCE=.\DMTriColorLight.cpp
# End Source File
# Begin Source File

SOURCE=.\DMTriColorLight.h
# End Source File
# Begin Source File

SOURCE=.\DMX\DMX.cpp
# End Source File
# Begin Source File

SOURCE=.\DMX\DmxOposIF.cpp
# End Source File
# Begin Source File

SOURCE=.\DMX\DmxOposIF.h
# End Source File
# Begin Source File

SOURCE=.\dmx\EASWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\dmx\EASWrapper.h
# End Source File
# Begin Source File

SOURCE=.\DMX\KeylockWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\DMX\KeylockWrapper.h
# End Source File
# Begin Source File

SOURCE=.\DMX\MicrWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\DMX\MicrWrapper.h
# End Source File
# Begin Source File

SOURCE=.\dmx\MotionSensorWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\dmx\MotionSensorWrapper.h
# End Source File
# Begin Source File

SOURCE=.\DMX\MSRWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\DMX\MSRWrapper.h
# End Source File
# Begin Source File

SOURCE=.\DMX\printer.cpp
# End Source File
# Begin Source File

SOURCE=.\DMX\printer.h
# End Source File
# Begin Source File

SOURCE=.\dmx\ScaleWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\dmx\ScaleWrapper.h
# End Source File
# Begin Source File

SOURCE=.\DMX\ScannerWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\DMX\ScannerWrapper.h
# End Source File
# Begin Source File

SOURCE=.\dmx\UiThread.cpp
# End Source File
# Begin Source File

SOURCE=.\dmx\UiThread.h
# End Source File
# End Group
# Begin Group "Presentation Services"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DPSCompatibility.h
# End Source File
# Begin Source File

SOURCE=.\OverrideScotStringList.cpp
# End Source File
# Begin Source File

SOURCE=.\OverrideScotStringList.h
# End Source File
# Begin Source File

SOURCE=.\PSAssistModeBase.cpp
# End Source File
# Begin Source File

SOURCE=.\PSCustomerModeBase.cpp
# End Source File
# Begin Source File

SOURCE=.\PSIntList.h
# End Source File
# Begin Source File

SOURCE=.\PSItemList.cpp
# End Source File
# Begin Source File

SOURCE=.\PSItemList.h
# End Source File
# Begin Source File

SOURCE=.\PSLoadStrings.cpp
# End Source File
# Begin Source File

SOURCE=.\PSLoadStrings.h
# End Source File
# Begin Source File

SOURCE=.\PSProceduresBase.cpp
# End Source File
# Begin Source File

SOURCE=.\PSProceduresBase.h
# End Source File
# Begin Source File

SOURCE=.\PSStoreModeBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTMultiStringList.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTMultiStringList.h
# End Source File
# Begin Source File

SOURCE=.\ScotStringList.cpp
# End Source File
# Begin Source File

SOURCE=..\Include\ScotStringList.h
# End Source File
# End Group
# Begin Group "Services"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CommonServices.h
# End Source File
# Begin Source File

SOURCE=.\SCOTAppBusyStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTAppBusyStatus.h
# End Source File
# Begin Source File

SOURCE=.\SCOTAPPErrorObject.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTAPPErrorObject.h
# End Source File
# Begin Source File

SOURCE=.\SCOTEncryption.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTEncryption.h
# End Source File
# Begin Source File

SOURCE=.\VolumeControl.cpp
# End Source File
# Begin Source File

SOURCE=.\VolumeControl.h
# End Source File
# End Group
# Begin Group "Documents"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Scot Finish.doc"
# End Source File
# Begin Source File

SOURCE=.\TestingItemSales.txt
# End Source File
# End Group
# Begin Group "States"

# PROP Default_Filter ""
# Begin Group "Base etc."

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SMInProgressBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMInProgressBase.h
# End Source File
# Begin Source File

SOURCE=.\SMStateBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMStateBase.h
# End Source File
# Begin Source File

SOURCE=.\SMStateCUBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMStateCUBase.h
# End Source File
# Begin Source File

SOURCE=.\SMStateDMBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMStateDMBase.h
# End Source File
# Begin Source File

SOURCE=.\SMStateHTBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMStateHTBase.h
# End Source File
# Begin Source File

SOURCE=.\SMStatePSBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMStatePSBase.h
# End Source File
# Begin Source File

SOURCE=.\SMStateRABase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMStateRABase.h
# End Source File
# Begin Source File

SOURCE=.\SMStateRPBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMStateRPBase.h
# End Source File
# Begin Source File

SOURCE=.\SMStateSABase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMStateSABase.h
# End Source File
# Begin Source File

SOURCE=.\SMStateTBBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMStateTBBase.h
# End Source File
# Begin Source File

SOURCE=.\SMStateUEBase.h
# End Source File
# Begin Source File

SOURCE=.\UserExitsBase.cpp
# End Source File
# End Group
# Begin Group "Help mode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SMAssistedTenderBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMAssistedTenderBase.h
# End Source File
# Begin Source File

SOURCE=.\SMCollectGiftCardBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCollectGiftCardBase.h
# End Source File
# Begin Source File

SOURCE=.\SMConfirmAbortBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMConfirmAbortBase.h
# End Source File
# Begin Source File

SOURCE=.\SMContextHelpBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMContextHelpBase.h
# End Source File
# Begin Source File

SOURCE=.\SMContinueTransBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMContinueTransBase.h
# End Source File
# Begin Source File

SOURCE=.\SMDegradedContinueTransBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMDegradedContinueTransBase.h
# End Source File
# Begin Source File

SOURCE=.\SMFatalErrorBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMFatalErrorBase.h
# End Source File
# Begin Source File

SOURCE=.\SMHelpModeStateBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMHelpModeStateBase.h
# End Source File
# Begin Source File

SOURCE=.\SMHelpOnWayBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMHelpOnWayBase.h
# End Source File
# Begin Source File

SOURCE=.\SMHostOffLineBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMHostOffLineBase.h
# End Source File
# Begin Source File

SOURCE=.\SMLaneClosedBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMLaneClosedBase.h
# End Source File
# Begin Source File

SOURCE=.\SMOperatorPasswordStateBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMOperatorPasswordStateBase.h
# End Source File
# Begin Source File

SOURCE=.\SMPickingUpItemsBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMPickingUpItemsBase.h
# End Source File
# Begin Source File

SOURCE=.\SMRAPDataNeededBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMRAPDataNeededBase.h
# End Source File
# Begin Source File

SOURCE=.\SMRemoteScannerConnectBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMRemoteScannerConnectBase.h
# End Source File
# Begin Source File

SOURCE=.\SMScaleBrokenBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMScaleBrokenBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSecBagAreaBackupBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecBagAreaBackupBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSecBagBoxNotValidBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecBagBoxNotValidBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSecBagBoxThresholdBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecBagBoxThresholdBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSecItemRemovedThresholdBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecItemRemovedThresholdBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSecMisMatchWeightBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecMisMatchWeightBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSecNewItemEntryBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecNewItemEntryBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSecOperatorPWStateBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecOperatorPWStateBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSecSkipBaggingThresholdBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecSkipBaggingThresholdBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSecUnexpectedDecreaseBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecUnexpectedDecreaseBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSecUnExpectedIncreaseBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecUnExpectedIncreaseBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSecurityLevel2Base.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecurityLevel2Base.h
# End Source File
# Begin Source File

SOURCE=.\SMSecurityLevel3Base.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSecurityLevel3Base.h
# End Source File
# Begin Source File

SOURCE=.\SMSignChargeBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSignChargeBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage1Base.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage1Base.h
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage2Base.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage2Base.h
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage3Base.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage3Base.h
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage4Base.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage4Base.h
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage5Base.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage5Base.h
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage6Base.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessage6Base.h
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessageBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessageBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessageOpPassBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSystemMessageOpPassBase.h
# End Source File
# Begin Source File

SOURCE=.\SMUnDeActivatedItemApprovalBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMUnDeActivatedItemApprovalBase.h
# End Source File
# Begin Source File

SOURCE=.\SMVoidApprovalBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMVoidApprovalBase.h
# End Source File
# Begin Source File

SOURCE=.\SMVoidTransApprovalBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMVoidTransApprovalBase.h
# End Source File
# Begin Source File

SOURCE=.\SMWaitForApprovalBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMWaitForApprovalBase.h
# End Source File
# Begin Source File

SOURCE=.\SMWaitForRemoteAssistanceBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMWaitForRemoteAssistanceBase.h
# End Source File
# End Group
# Begin Group "Store mode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DMCashCount.cpp
# End Source File
# Begin Source File

SOURCE=.\DMCashCount.h
# End Source File
# Begin Source File

SOURCE=.\SMSmAbortBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmAbortBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmAbortedBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmAbortedBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmAssistMenuBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmAssistMenuBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmAssistModeBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmAssistModeBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmAuthorizationBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmAuthorizationBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmCardManagementBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmCardManagementBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmCashierPasswordBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmCashierPasswordBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmCashManagementBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmCashManagementBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmCashManagementSafePayBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmCashManagementSafePayBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmConfirmSignatureBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmConfirmSignatureBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmConfirmSuspendedTransactionBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmConfirmSuspendedTransactionBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmCouponItemsBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmCouponItemsBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmDataEntryBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmDataEntryBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmEnterBirthdateBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmEnterBirthdateBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmmAddWeightBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmmAddWeightBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmMediaStatusBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmMediaStatusBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmmEditAddBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmmEditAddBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmmEditWtExpBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmmEditWtExpBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmmEditWtSubCkBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmmEditWtSubCkBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmmEditWtToleranceBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmmEditWtToleranceBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmmInvalidWtTolBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmmInvalidWtTolBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmmKeyInWtTolBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmmKeyInWtTolBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmOutOfServiceBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmOutOfServiceBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmPrintReceiptBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmPrintReceiptBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmReportsMenuBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmReportsMenuBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmRestrictedItemsBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmRestrictedItemsBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmRunReportsBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmRunReportsBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmSecurityLogsBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmSecurityLogsBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmSelectModeOfOperationBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmSelectModeOfOperationBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmSystemFunctionsBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmSystemFunctionsBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmUpdateItemBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmUpdateItemBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmUtilityBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmUtilityBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSmVisualItemsBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmVoidedItemsBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSmVoidedItemsBase.h
# End Source File
# Begin Source File

SOURCE=.\SMStoreModeMenuBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMStoreModeMenuBase.h
# End Source File
# Begin Source File

SOURCE=.\SMStoreModeStateBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMStoreModeStateBase.h
# End Source File
# End Group
# Begin Group "Customer mode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SMAcknowledgeSigBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMAcknowledgeSigBase.h
# End Source File
# Begin Source File

SOURCE=.\SMAttractBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMAttractBase.h
# End Source File
# Begin Source File

SOURCE=.\SMBagAndEasBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMBagAndEASBase.h
# End Source File
# Begin Source File

SOURCE=.\SMCardMisReadBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCardMisReadBase.h
# End Source File
# Begin Source File

SOURCE=.\SMCardProcessingBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCardProcessingBase.h
# End Source File
# Begin Source File

SOURCE=.\SMCashPaymentBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCashPaymentBase.h
# End Source File
# Begin Source File

SOURCE=.\SMCheckBasketReminderBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCheckBasketReminderBase.h
# End Source File
# Begin Source File

SOURCE=.\SMCmDataEntryBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCmDataEntryBase.h
# End Source File
# Begin Source File

SOURCE=.\SMConfirmDebitAmountBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMConfirmDebitAmountBase.h
# End Source File
# Begin Source File

SOURCE=.\SMConfirmEBTAmountBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMConfirmEBTAmountBase.h
# End Source File
# Begin Source File

SOURCE=.\SMConfirmQuantityBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMConfirmQuantityBase.h
# End Source File
# Begin Source File

SOURCE=.\SMConfirmVoidBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMConfirmVoidBase.h
# End Source File
# Begin Source File

SOURCE=.\SMCouponNoMatchBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCouponNoMatchBase.h
# End Source File
# Begin Source File

SOURCE=.\SMCouponNotAllowedBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCouponNotAllowedBase.h
# End Source File
# Begin Source File

SOURCE=.\SMCouponTooHighBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCouponTooHighBase.h
# End Source File
# Begin Source File

SOURCE=.\SMCrateableItemBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCrateableItemBase.h
# End Source File
# Begin Source File

SOURCE=.\SMCustomerModeState.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCustomerModeState.h
# End Source File
# Begin Source File

SOURCE=.\SMCustomMessageBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMCustomMessageBase.h
# End Source File
# Begin Source File

SOURCE=.\SMDepositCouponsBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMDepositCouponsBase.h
# End Source File
# Begin Source File

SOURCE=.\SMDepositGiftCardBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMDepositGiftCardBase.h
# End Source File
# Begin Source File

SOURCE=.\SMDropoffCouponsBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMDropoffCouponsBase.h
# End Source File
# Begin Source File

SOURCE=.\SMElectronicBenefitsTransferBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMElectronicBenefitsTransferBase.h
# End Source File
# Begin Source File

SOURCE=.\SMEnterAlphaNumericInfoBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMEnterAlphaNumericInfoBase.h
# End Source File
# Begin Source File

SOURCE=.\SMEnterCouponsBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMEnterCouponsBase.h
# End Source File
# Begin Source File

SOURCE=.\SMEnterCouponValueBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMEnterCouponValueBase.h
# End Source File
# Begin Source File

SOURCE=.\SMEnterIDBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMEnterIDBase.h
# End Source File
# Begin Source File

SOURCE=.\SMEnterPasswordBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMEnterPasswordBase.h
# End Source File
# Begin Source File

SOURCE=.\SMEnterPinBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMEnterPinBase.h
# End Source File
# Begin Source File

SOURCE=.\SMEnterQuantityBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMEnterQuantityBase.h
# End Source File
# Begin Source File

SOURCE=.\SMEnterWeightBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMEnterWeightBase.h
# End Source File
# Begin Source File

SOURCE=.\SmEnterWtForPriceEmbeddedBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMEnterWtForPriceEmbeddedBase.h
# End Source File
# Begin Source File

SOURCE=.\SMFinishBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMFinishBase.h
# End Source File
# Begin Source File

SOURCE=.\SMGetFoodStampStateBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMGetFoodStampStateBase.h
# End Source File
# Begin Source File

SOURCE=.\SMInsertCouponBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMInsertCouponBase.h
# End Source File
# Begin Source File

SOURCE=.\SMInsertGiftCardBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMInsertGiftCardBase.h
# End Source File
# Begin Source File

SOURCE=.\SMItemNotForSaleBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMItemNotForSaleBase.h
# End Source File
# Begin Source File

SOURCE=.\SMItemQuantityExceededBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMItemQuantityExceededBase.h
# End Source File
# Begin Source File

SOURCE=.\SMKeyInCodeBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMKeyInCodeBase.h
# End Source File
# Begin Source File

SOURCE=.\SMLimitedEBTBenefitBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMLimitedEBTBenefitBase.h
# End Source File
# Begin Source File

SOURCE=.\SMLookUpItemBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMLookUpItemBase.h
# End Source File
# Begin Source File

SOURCE=.\SMOutOfService2Base.cpp
# End Source File
# Begin Source File

SOURCE=.\SMOutOfService2Base.h
# End Source File
# Begin Source File

SOURCE=.\SMOutOfServiceBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMOutOfServiceBase.h
# End Source File
# Begin Source File

SOURCE=.\SMProcessingBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMProcessingBase.h
# End Source File
# Begin Source File

SOURCE=.\SMProduceFavoritesBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMProduceFavoritesBase.h
# End Source File
# Begin Source File

SOURCE=.\SMPutBagOnScaleBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMPutBagOnScaleBase.h
# End Source File
# Begin Source File

SOURCE=.\SMRequestSigBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMRequestSigBase.h
# End Source File
# Begin Source File

SOURCE=.\SMRestrictedNotAllowedBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMRestrictedNotAllowedBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSavePreferencesBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSavePreferencesBase.h
# End Source File
# Begin Source File

SOURCE=.\SMScanAndBagBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMScanAndBagBase.h
# End Source File
# Begin Source File

SOURCE=.\SMScanCardBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMScanCardBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSelectCardTypeBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSelectCardTypeBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSelectContainerBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSelectContainerBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSelectDebitTypeBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSelectDebitTypeBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSelectFromListBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSelectFromListBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSelectLanguageBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSelectLanguageBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSelectPaymentBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSelectPaymentBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSelectTareBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSelectTareBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSellBagsBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSellBagsBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSetAllItemsAsideBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSetAllItemsAsideBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSuspendBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSuspendBase.h
# End Source File
# Begin Source File

SOURCE=.\SMSwipeCardBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSwipeCardBase.h
# End Source File
# Begin Source File

SOURCE=.\SMTakeCardBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMTakeCardBase.h
# End Source File
# Begin Source File

SOURCE=.\SMTakeChangeBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMTakeChangeBase.h
# End Source File
# Begin Source File

SOURCE=.\SMTakeLoyaltyCardBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMTakeLoyaltyCardBase.h
# End Source File
# Begin Source File

SOURCE=.\SMTakeReceiptBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMTakeReceiptBase.h
# End Source File
# Begin Source File

SOURCE=.\SMTenderBalanceBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMTenderBalanceBase.h
# End Source File
# Begin Source File

SOURCE=.\SMThankYouBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMThankYouBase.h
# End Source File
# Begin Source File

SOURCE=.\SMTimeRestrictedItemBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMTimeRestrictedItemBase.h
# End Source File
# Begin Source File

SOURCE=.\SMTransportItemBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMTransportItemBase.h
# End Source File
# Begin Source File

SOURCE=.\SMUnknownItemBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMUnknownItemBase.h
# End Source File
# Begin Source File

SOURCE=.\SMUnknownPriceBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMUnknownPriceBase.h
# End Source File
# Begin Source File

SOURCE=.\SMVoidItemBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMVoidItemBase.h
# End Source File
# Begin Source File

SOURCE=.\SMVoidNoMatchBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMVoidNoMatchBase.h
# End Source File
# Begin Source File

SOURCE=.\SMVoucherPaymentBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMVoucherPaymentBase.h
# End Source File
# End Group
# Begin Group "Training mode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TrainingMode.cpp
# End Source File
# Begin Source File

SOURCE=.\TrainingMode.h
# End Source File
# End Group
# Begin Group "ExtensionState"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DeviceCtxStoreXML.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceCtxStoreXML.h
# End Source File
# Begin Source File

SOURCE=.\DevicePropertyChanges.h
# End Source File
# Begin Source File

SOURCE=.\SMExtensionStateBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMExtensionStateBase.h
# End Source File
# End Group
# End Group
# Begin Group "Hard Totals"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HardTotals.cpp
# End Source File
# Begin Source File

SOURCE=.\HardTotals.h
# End Source File
# Begin Source File

SOURCE=.\HTProcedures.cpp
# End Source File
# Begin Source File

SOURCE=.\HTProcedures.h
# End Source File
# End Group
# Begin Group "Remote Approval"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PSXInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\PSXInterface.h
# End Source File
# Begin Source File

SOURCE=.\QPING.CPP
# End Source File
# Begin Source File

SOURCE=.\qping.h
# End Source File
# Begin Source File

SOURCE=.\RAInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\RAInterface.h
# End Source File
# Begin Source File

SOURCE=.\RAProcedures.cpp
# End Source File
# Begin Source File

SOURCE=.\RAProcedures.h
# End Source File
# Begin Source File

SOURCE=.\rcmgrwrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\rcmgrwrapper.h
# End Source File
# End Group
# Begin Group "Generic Device Support"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\civicmform.cpp
# End Source File
# Begin Source File

SOURCE=.\civicmform.h
# End Source File
# Begin Source File

SOURCE=.\OposControl.cpp
# End Source File
# Begin Source File

SOURCE=.\OposControl.h
# End Source File
# Begin Source File

SOURCE=.\opospinpad.cpp
# End Source File
# Begin Source File

SOURCE=.\opospinpad.h
# End Source File
# Begin Source File

SOURCE=.\SCOTAuxDisp.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTAuxDisp.h
# End Source File
# Begin Source File

SOURCE=.\SCOTAuxScreenList.h
# End Source File
# Begin Source File

SOURCE=.\SCOTDerivedForm.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTDerivedForm.h
# End Source File
# Begin Source File

SOURCE=.\SCOTDevFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTDevFactory.h
# End Source File
# Begin Source File

SOURCE=.\SCOTDevice.h
# End Source File
# Begin Source File

SOURCE=.\SCOTEncryptor.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTEncryptor.h
# End Source File
# Begin Source File

SOURCE=.\SCOTForm.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTForm.h
# End Source File
# Begin Source File

SOURCE=.\SCOTIVIForms.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTIVIForms.h
# End Source File
# Begin Source File

SOURCE=.\SCOTPromptDisp.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTPromptDisp.h
# End Source File
# Begin Source File

SOURCE=.\SCOTSigCap.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTSigCap.h
# End Source File
# Begin Source File

SOURCE=.\SCOTTakeawayBelt.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTTakeawayBelt.h
# End Source File
# Begin Source File

SOURCE=.\sigcap.cpp
# End Source File
# Begin Source File

SOURCE=.\sigcap.h
# End Source File
# Begin Source File

SOURCE=.\takeawaybeltcontrol.cpp
# End Source File
# Begin Source File

SOURCE=.\takeawaybeltcontrol.h
# End Source File
# End Group
# Begin Group "Report"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CollectorBase.cpp
# End Source File
# Begin Source File

SOURCE=.\CollectorBase.h
# End Source File
# Begin Source File

SOURCE=.\FLReportingErrors.h
# End Source File
# Begin Source File

SOURCE=.\FLTransporter.cpp
# End Source File
# Begin Source File

SOURCE=.\FLTransporter.h
# End Source File
# Begin Source File

SOURCE=.\Reporting.cpp
# End Source File
# Begin Source File

SOURCE=.\Reporting.h
# End Source File
# Begin Source File

SOURCE=.\ReportingErrors.h
# End Source File
# End Group
# Begin Group "Application Security"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\KVParmList.cpp
# End Source File
# Begin Source File

SOURCE=.\SecMgrInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\SecMgrInterface.h
# End Source File
# Begin Source File

SOURCE=.\SecMgrProcedures.cpp
# End Source File
# Begin Source File

SOURCE=.\SecMgrProcedures.h
# End Source File
# Begin Source File

SOURCE=.\securitymanager.cpp
# End Source File
# Begin Source File

SOURCE=.\securitymanager.h
# End Source File
# End Group
# Begin Group "Customer Information"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CustomerInfoBase.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerInfoBase.h
# End Source File
# Begin Source File

SOURCE=.\CXMLHTTPEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\CXMLHTTPEvent.h
# End Source File
# End Group
# Begin Group "USSF"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SMStateUSSFBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMStateUSSFBase.h
# End Source File
# Begin Source File

SOURCE=.\SMStateUSSFMacros.h
# End Source File
# Begin Source File

SOURCE=.\SMUSSFManagerBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SMUSSFManagerBase.h
# End Source File
# Begin Source File

SOURCE=.\USSFCommon.h
# End Source File
# Begin Source File

SOURCE=.\USSFProcedures.cpp
# End Source File
# Begin Source File

SOURCE=.\USSFProcedures.h
# End Source File
# Begin Source File

SOURCE=.\USSFUtilityBase.cpp
# End Source File
# Begin Source File

SOURCE=.\USSFUtilityBase.h
# End Source File
# End Group
# End Target
# End Project
