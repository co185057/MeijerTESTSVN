# Microsoft Developer Studio Project File - Name="CommonOPOS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=CommonOPOS - Win32 Debug UNICODE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CommonOPOS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CommonOPOS.mak" CFG="CommonOPOS - Win32 Debug UNICODE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CommonOPOS - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "CommonOPOS - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "CommonOPOS - Win32 Debug UNICODE" (based on "Win32 (x86) Static Library")
!MESSAGE "CommonOPOS - Win32 Release UNICODE" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Base TB/Source/OposServiceObject/CommonOPOS", UCPAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CommonOPOS - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "." /I "..\include" /I "\scot\adk\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "CommonOPOS - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\include" /I "\scot\adk\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\CommonOPOSD.lib"

!ELSEIF  "$(CFG)" == "CommonOPOS - Win32 Debug UNICODE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CommonOPOS___Win32_Debug_UNICODE"
# PROP BASE Intermediate_Dir "CommonOPOS___Win32_Debug_UNICODE"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_UNICODE"
# PROP Intermediate_Dir "Debug_UNICODE"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\include" /I "\scot\adk\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\include" /I "\scot\adk\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\CommonOPOSD.lib"
# ADD LIB32 /nologo /out:"Debug_UNICODE\CommonOPOSUD.lib"

!ELSEIF  "$(CFG)" == "CommonOPOS - Win32 Release UNICODE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CommonOPOS___Win32_Release_UNICODE"
# PROP BASE Intermediate_Dir "CommonOPOS___Win32_Release_UNICODE"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_UNICODE"
# PROP Intermediate_Dir "Release_UNICODE"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "." /I "..\include" /I "\scot\adk\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "." /I "..\include" /I "\scot\adk\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release_UNICODE\CommonOPOSU.lib"

!ENDIF 

# Begin Target

# Name "CommonOPOS - Win32 Release"
# Name "CommonOPOS - Win32 Debug"
# Name "CommonOPOS - Win32 Debug UNICODE"
# Name "CommonOPOS - Win32 Release UNICODE"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\GenericCO.cpp
# End Source File
# Begin Source File

SOURCE=.\IConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\OposDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\OposDeviceThread.cpp
# End Source File
# Begin Source File

SOURCE=.\OposDiagnostics.cpp
# End Source File
# Begin Source File

SOURCE=.\OposEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\OposOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\OposRegistry.cpp
# End Source File
# Begin Source File

SOURCE=.\OposRegistryCO.cpp
# End Source File
# Begin Source File

SOURCE=.\OposSerialInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\OposService.cpp
# End Source File
# Begin Source File

SOURCE=.\OposSharedData.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialRegistry.cpp
# End Source File
# Begin Source File

SOURCE=.\SharedMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\GenericCO.h
# End Source File
# Begin Source File

SOURCE=.\IConnection.h
# End Source File
# Begin Source File

SOURCE=.\OposDevice.h
# End Source File
# Begin Source File

SOURCE=.\OposDeviceThread.h
# End Source File
# Begin Source File

SOURCE=.\OposDiagnostics.h
# End Source File
# Begin Source File

SOURCE=.\OposEvent.h
# End Source File
# Begin Source File

SOURCE=.\OposEvent.inl
# End Source File
# Begin Source File

SOURCE=.\OposOutput.h
# End Source File
# Begin Source File

SOURCE=.\OposOutput.inl
# End Source File
# Begin Source File

SOURCE=.\OposRegistry.h
# End Source File
# Begin Source File

SOURCE=.\OposRegistry.inl
# End Source File
# Begin Source File

SOURCE=.\OposSerialInterface.h
# End Source File
# Begin Source File

SOURCE=.\OposService.h
# End Source File
# Begin Source File

SOURCE=.\OposService.inl
# End Source File
# Begin Source File

SOURCE=.\OposSharedData.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SerialRegistry.h
# End Source File
# Begin Source File

SOURCE=.\SharedMemory.h
# End Source File
# Begin Source File

SOURCE=.\std_opos.h
# End Source File
# Begin Source File

SOURCE=.\std_opos.inl
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TCheckHealth.h
# End Source File
# Begin Source File

SOURCE=.\TClassPtr.h
# End Source File
# Begin Source File

SOURCE=.\TControl.h
# End Source File
# Begin Source File

SOURCE=.\TDeviceMap.h
# End Source File
# Begin Source File

SOURCE=.\TOposDlgThread.h
# End Source File
# Begin Source File

SOURCE=.\TService.h
# End Source File
# End Group
# End Target
# End Project
