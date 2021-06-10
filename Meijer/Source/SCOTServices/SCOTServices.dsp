# Microsoft Developer Studio Project File - Name="SCOTServices" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SCOTServices - Win32 Unicode Debug BuildLib
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SCOTServices.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SCOTServices.mak" CFG="SCOTServices - Win32 Unicode Debug BuildLib"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SCOTServices - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTServices - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTServices - Win32 Debug BuildLib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTServices - Win32 Release BuildLib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTServices - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTServices - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTServices - Win32 Unicode Release BuildLib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTServices - Win32 Unicode Debug BuildLib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""$/SCOTx Software/Source/Release 3.0/Core/Source/SCOTServices", LHJHAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SCOTServices - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "$(MEIJER605)\adk\include" /I "..\scotapp" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 RPSW.lib version.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386 /libpath:"..\lib\release" /libpath:"$(MEIJER605)\ADK\Lib\release"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy release\SCOTServices.lib ..\lib\Release /r	xcopy           release\SCOTServices.dll $(MEIJER605)\bin\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTServices - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "$(MEIJER605)\adk\include" /I "..\scotapp" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /fo"Debug/SCOTServicesD.res" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 RPSWD.lib version.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"MSVCRT.lib" /def:".\SCOTServicesD.def" /out:"Debug/SCOTServicesD.dll" /libpath:"..\lib\debug" /libpath:"$(MEIJER605)\ADK\Lib\debug"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Debug\SCOTServicesD.dll $(MEIJER605)\bin\*.* /r	xcopy       Debug\SCOTServicesD.lib ..\lib\Debug /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTServices - Win32 Debug BuildLib"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SCOTServ"
# PROP BASE Intermediate_Dir "SCOTServ"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SCOTServ"
# PROP Intermediate_Dir "SCOTServ"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\include" /I "$(MEIJER605)\adk\include" /I "..\scotapp" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "$(MEIJER605)\adk\include" /I "..\scotapp" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /fo"SCOTServ/SCOTServicesD.res" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RPSW.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\SCOTServicesD.def" /pdbtype:sept /libpath:"..\lib\debug" /libpath:"$(MEIJER605)\ADK\Lib\debug"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 RPSWD.lib version.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /nodefaultlib:"MSVCRT.lib" /def:".\SCOTServicesD.def" /out:"SCOTServ/SCOTServicesD.dll" /libpath:"..\lib\debug" /libpath:"$(MEIJER605)\ADK\Lib\debug"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy ScotServ\SCOTServicesD.lib ..\lib\Debug /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTServices - Win32 Release BuildLib"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SCOTSer0"
# PROP BASE Intermediate_Dir "SCOTSer0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "SCOTSer0"
# PROP Intermediate_Dir "SCOTSer0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "$(MEIJER605)\adk\include" /I "..\scotapp" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "$(MEIJER605)\adk\include" /I "..\scotapp" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RPSW.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\lib\release" /libpath:"\SCOT\ADK\Lib\release"
# ADD LINK32 RPSW.lib version.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\lib\release" /libpath:"$(MEIJER605)\ADK\Lib\release"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy SCOTSer0\SCOTServices.lib ..\lib\Release\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTServices - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SCOTServices___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "SCOTServices___Win32_Debug_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode"
# PROP Intermediate_Dir "Debug_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "$(MEIJER605)\adk\include" /I "..\scotapp" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /ZI /Od /I "..\include" /I "$(MEIJER605)\adk\include" /I "..\scotapp" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /fo"Debug/SCOTServicesD.res" /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /fo"Debug/SCOTServicesD.res" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RPSWD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"MSVCRT.lib" /def:".\SCOTServicesD.def" /out:"Debug/SCOTServicesD.dll" /libpath:"..\lib\debug" /libpath:"$(MEIJER605)\ADK\Lib\debug"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 RPSWUD.lib version.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"MSVCRT.lib" /out:"Debug_Unicode/SCOTServicesUD.dll" /libpath:"..\lib\debug" /libpath:"$(MEIJER605)\ADK\Lib\debug"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Debug_Unicode\SCOTServicesUD.dll $(MEIJER605)\bin\*.* /r	xcopy       Debug_Unicode\SCOTServicesUD.lib ..\lib\Debug /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTServices - Win32 Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SCOTServices___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "SCOTServices___Win32_Release_Unicode"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode"
# PROP Intermediate_Dir "Release_Unicode"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "$(MEIJER605)\adk\include" /I "..\scotapp" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /WX /GX /O2 /I "..\include" /I "$(MEIJER605)\adk\include" /I "..\scotapp" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RPSW.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386 /libpath:"..\lib\release" /libpath:"$(MEIJER605)\ADK\Lib\release"
# ADD LINK32 RPSWU.lib version.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386 /out:"Release_Unicode/SCOTServicesU.dll" /libpath:"..\lib\release" /libpath:"$(MEIJER605)\ADK\Lib\release"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Release_Unicode\SCOTServicesU.lib ..\lib\Release /r	xcopy           Release_Unicode\SCOTServicesU.dll $(MEIJER605)\bin\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTServices - Win32 Unicode Release BuildLib"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SCOTServices___Win32_Release_Unicode_BuildLib"
# PROP BASE Intermediate_Dir "SCOTServices___Win32_Release_Unicode_BuildLib"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode_BuildLib"
# PROP Intermediate_Dir "Release_Unicode_BuildLib"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "$(MEIJER605)\adk\include" /I "..\scotapp" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /WX /GX /O2 /I "..\include" /I "$(MEIJER605)\adk\include" /I "..\scotapp" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RPSW.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\lib\release" /libpath:"$(MEIJER605)\ADK\Lib\release"
# ADD LINK32 RPSWU.lib version.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Release_Unicode_BuildLib/SCOTServicesU.dll" /libpath:"..\lib\release" /libpath:"$(MEIJER605)\ADK\Lib\release"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Release_Unicode_BuildLib\SCOTServicesU.lib ..\lib\Release\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTServices - Win32 Unicode Debug BuildLib"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SCOTServices___Win32_Debug_Unicode_BuildLib"
# PROP BASE Intermediate_Dir "SCOTServices___Win32_Debug_Unicode_BuildLib"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode_BuildLib"
# PROP Intermediate_Dir "Debug_Unicode_BuildLib"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "$(MEIJER605)\adk\include" /I "..\scotapp" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /ZI /Od /I "..\include" /I "$(MEIJER605)\adk\include" /I "..\scotapp" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /fo"SCOTServ/SCOTServicesD.res" /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /fo"SCOTServ/SCOTServicesD.res" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RPSWD.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /nodefaultlib:"MSVCRT.lib" /def:".\SCOTServicesD.def" /out:"SCOTServ/SCOTServicesD.dll" /libpath:"..\lib\debug" /libpath:"$(MEIJER605)\ADK\Lib\debug"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 RPSWUD.lib version.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /nodefaultlib:"MSVCRT.lib" /out:"Debug_Unicode/SCOTServicesUD.dll" /libpath:"..\lib\debug" /libpath:"$(MEIJER605)\ADK\Lib\debug"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Debug_Unicode_BuildLib\SCOTServicesUD.lib ..\lib\Debug /r
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "SCOTServices - Win32 Release"
# Name "SCOTServices - Win32 Debug"
# Name "SCOTServices - Win32 Debug BuildLib"
# Name "SCOTServices - Win32 Release BuildLib"
# Name "SCOTServices - Win32 Unicode Debug"
# Name "SCOTServices - Win32 Unicode Release"
# Name "SCOTServices - Win32 Unicode Release BuildLib"
# Name "SCOTServices - Win32 Unicode Debug BuildLib"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ErrorObject.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgObject.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTServices.cpp
# End Source File
# Begin Source File

SOURCE=.\SCOTServices.rc

!IF  "$(CFG)" == "SCOTServices - Win32 Release"

!ELSEIF  "$(CFG)" == "SCOTServices - Win32 Debug"

!ELSEIF  "$(CFG)" == "SCOTServices - Win32 Debug BuildLib"

!ELSEIF  "$(CFG)" == "SCOTServices - Win32 Release BuildLib"

!ELSEIF  "$(CFG)" == "SCOTServices - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "SCOTServices - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "SCOTServices - Win32 Unicode Release BuildLib"

!ELSEIF  "$(CFG)" == "SCOTServices - Win32 Unicode Debug BuildLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\stackwalk.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TraceObject.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Include\ErrorObject.h
# End Source File
# Begin Source File

SOURCE=..\Include\MsgObject.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SCOTServices.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Include\TraceObject.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\SCOTServices.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
