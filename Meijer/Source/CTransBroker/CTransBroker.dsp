# Microsoft Developer Studio Project File - Name="CTransBroker" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=CTransBroker - Win32 Unicode Debug BuildLib
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CTransBroker.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CTransBroker.mak" CFG="CTransBroker - Win32 Unicode Debug BuildLib"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CTransBroker - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "CTransBroker - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "CTransBroker - Win32 Debug BuildLib" (based on "Win32 (x86) Static Library")
!MESSAGE "CTransBroker - Win32 Release BuildLib" (based on "Win32 (x86) Static Library")
!MESSAGE "CTransBroker - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "CTransBroker - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "CTransBroker - Win32 Unicode Debug BuildLib" (based on "Win32 (x86) Static Library")
!MESSAGE "CTransBroker - Win32 Unicode Release BuildLib" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SCOTx Software/Source/Release 3.0/Core/Source/CTransBroker", ASPBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CTransBroker - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "..\scotapp" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Release\CTransBroker.lib   ..\lib\Release\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CTransBroker - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "..\include" /I "..\scotssf" /I "..\scotapp" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fr /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\CTransBrokerD.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Debug\CTransBrokerD.lib   ..\lib\Debug /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CTransBroker - Win32 Debug BuildLib"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CTransBr"
# PROP BASE Intermediate_Dir "CTransBr"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CTransBr"
# PROP Intermediate_Dir "CTransBr"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Z7 /Od /I "." /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "..\include" /I "..\scotssf" /I "..\scotapp" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fr /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\CTransBrokerD.lib"
# ADD LIB32 /nologo /out:"CTransBr\CTransBrokerD.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy CTransBr\CTransBrokerD.lib   ..\lib\Debug\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CTransBroker - Win32 Release BuildLib"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CTransB0"
# PROP BASE Intermediate_Dir "CTransB0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "CTransB0"
# PROP Intermediate_Dir "CTransB0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "." /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "..\scotapp" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy CTransB0\CTransBroker.lib   ..\lib\Release\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CTransBroker - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CTransBroker___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "CTransBroker___Win32_Debug_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode"
# PROP Intermediate_Dir "Debug_Unicode"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Z7 /Od /I "..\include" /I "..\scotssf" /I "..\scotapp" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fr /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /WX /GX /Z7 /Od /I "..\include" /I "..\scotssf" /I "..\scotapp" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /Fr /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\CTransBrokerD.lib"
# ADD LIB32 /nologo /out:"Debug_Unicode\CTransBrokerUD.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Debug_Unicode\CTransBrokerUD.lib   ..\lib\Debug /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CTransBroker - Win32 Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CTransBroker___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "CTransBroker___Win32_Release_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode"
# PROP Intermediate_Dir "Release_Unicode"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "..\scotapp" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /WX /GX /O2 /I "..\include" /I "..\scotssf" /I "..\scotapp" /I "$(MEIJER605)\adk\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release_Unicode\CTransBrokerU.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Release_Unicode\CTransBrokerU.lib   ..\lib\Release\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CTransBroker - Win32 Unicode Debug BuildLib"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CTransBroker___Win32_Debug_Unicode_BuildLib"
# PROP BASE Intermediate_Dir "CTransBroker___Win32_Debug_Unicode_BuildLib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode_BuildLib"
# PROP Intermediate_Dir "Debug_Unicode_BuildLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Z7 /Od /I "..\include" /I "..\scotssf" /I "..\scotapp" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fr /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /WX /GX /Z7 /Od /I "..\include" /I "..\scotssf" /I "..\scotapp" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /Fr /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"CTransBr\CTransBrokerD.lib"
# ADD LIB32 /nologo /out:"Debug_Unicode_BuildLib\CTransBrokerUD.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Debug_Unicode_BuildLib\CTransBrokerUD.lib   ..\lib\Debug\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CTransBroker - Win32 Unicode Release BuildLib"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CTransBroker___Win32_Release_Unicode_BuildLib"
# PROP BASE Intermediate_Dir "CTransBroker___Win32_Release_Unicode_BuildLib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode_BuildLib"
# PROP Intermediate_Dir "Release_Unicode_BuildLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "..\scotapp" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /WX /GX /O2 /I "..\include" /I "..\scotssf" /I "..\scotapp" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release_Unicode_BuildLib\CTransBrokerU.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Release_Unicode_BuildLib\CTransBrokerU.lib   ..\lib\Release\*.* /r
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "CTransBroker - Win32 Release"
# Name "CTransBroker - Win32 Debug"
# Name "CTransBroker - Win32 Debug BuildLib"
# Name "CTransBroker - Win32 Release BuildLib"
# Name "CTransBroker - Win32 Unicode Debug"
# Name "CTransBroker - Win32 Unicode Release"
# Name "CTransBroker - Win32 Unicode Debug BuildLib"
# Name "CTransBroker - Win32 Unicode Release BuildLib"
# Begin Source File

SOURCE=.\CTransBroker.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Include\CTransBroker.h
# End Source File
# End Target
# End Project
