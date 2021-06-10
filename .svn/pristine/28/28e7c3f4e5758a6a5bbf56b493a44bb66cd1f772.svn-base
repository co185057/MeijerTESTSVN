# Microsoft Developer Studio Project File - Name="TransBrokerMsg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TransBrokerMsg - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TransBrokerMsg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TransBrokerMsg.mak" CFG="TransBrokerMsg - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TransBrokerMsg - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TransBrokerMsg - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TransBrokerMsg - Win32 Release BuildLib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TransBrokerMsg - Win32 Debug BuildLib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TransBrokerMsg - Win32 Unicode Debug BuildLib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TransBrokerMsg - Win32 Unicode Release " (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TransBrokerMsg - Win32 Unicode Release BuildLib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TransBrokerMsg - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TransBrokerMsg - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FD /c
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy  TransBrokerMsg.h  ..\include /r /y	xcopy          Release\TransBrokerMsg.dll $(MEIJER605)\dll\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Debug"

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
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FD /c
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /debug /machine:I386
# SUBTRACT LINK32 /incremental:no
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy TransBrokerMsg.h   ..\include\*.* /r /y	xcopy          Debug\TransBrokerMsg.dll $(MEIJER605)\dll\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Release BuildLib"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TransBro"
# PROP BASE Intermediate_Dir "TransBro"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "TransBro"
# PROP Intermediate_Dir "TransBro"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy TransBrokerMsg.h     ..\include\*.* /r /y
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Debug BuildLib"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TransBr0"
# PROP BASE Intermediate_Dir "TransBr0"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "TransBr0"
# PROP Intermediate_Dir "TransBr0"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FD /c
# SUBTRACT BASE CPP /Fr /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /incremental:no /machine:I386 /pdbtype:sept
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /map /debug /machine:I386
# SUBTRACT LINK32 /incremental:no
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy TransBrokerMsg.h     ..\include\*.* /r /y
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Unicode Debug BuildLib"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TransBrokerMsg___Win32_Debug_Unicode_BuildLib"
# PROP BASE Intermediate_Dir "TransBrokerMsg___Win32_Debug_Unicode_BuildLib"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode_BuildLib"
# PROP Intermediate_Dir "Debug_Unicode_BuildLib"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FD /c
# SUBTRACT BASE CPP /Fr /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /map /debug /machine:I386
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"Debug_Unicode_BuildLib/TransBrokerMsgU.dll"
# SUBTRACT LINK32 /incremental:no
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy TransBrokerMsg.h     ..\include\*.* /r /y
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Unicode Release "

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TransBrokerMsg___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "TransBrokerMsg___Win32_Release_Unicode"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode"
# PROP Intermediate_Dir "Release_Unicode"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386 /out:"Release_Unicode/TransBrokerMsgU.dll"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy  TransBrokerMsg.h  ..\include /r	xcopy          Release_Unicode\TransBrokerMsgU.dll $(MEIJER605)\dll\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Unicode Release BuildLib"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TransBrokerMsg___Win32_Release_BuildLib_Unicode"
# PROP BASE Intermediate_Dir "TransBrokerMsg___Win32_Release_BuildLib_Unicode"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode_BuildLib"
# PROP Intermediate_Dir "Release_Unicode_BuildLib"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /WX /GX /O2 /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Release_BuildLib_Unicode/TransBrokerMsgU.dll"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy TransBrokerMsg.h     ..\include\*.* /r /y
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TransBrokerMsg___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "TransBrokerMsg___Win32_Debug_Unicode"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode"
# PROP Intermediate_Dir "Debug_Unicode"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FD /c
# SUBTRACT BASE CPP /Fr /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /debug /machine:I386
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug_Unicode/TransBrokerMsgU.dll"
# SUBTRACT LINK32 /incremental:no
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy TransBrokerMsg.h   ..\include\*.* /r /y	xcopy          Debug_Unicode\TransBrokerMsgU.dll $(MEIJER605)\dll\*.* /r
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "TransBrokerMsg - Win32 Release"
# Name "TransBrokerMsg - Win32 Debug"
# Name "TransBrokerMsg - Win32 Release BuildLib"
# Name "TransBrokerMsg - Win32 Debug BuildLib"
# Name "TransBrokerMsg - Win32 Unicode Debug BuildLib"
# Name "TransBrokerMsg - Win32 Unicode Release "
# Name "TransBrokerMsg - Win32 Unicode Release BuildLib"
# Name "TransBrokerMsg - Win32 Unicode Debug"
# Begin Source File

SOURCE=.\TransBrokerMsg.h

!IF  "$(CFG)" == "TransBrokerMsg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Release BuildLib"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Debug BuildLib"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Unicode Debug BuildLib"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Unicode Release "

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Unicode Release BuildLib"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Unicode Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TransBrokerMsg.mc

!IF  "$(CFG)" == "TransBrokerMsg - Win32 Release"

# Begin Custom Build
InputPath=.\TransBrokerMsg.mc
InputName=TransBrokerMsg

BuildCmds= \
	mc $(InputName)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"msg00001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Debug"

# Begin Custom Build
InputPath=.\TransBrokerMsg.mc
InputName=TransBrokerMsg

BuildCmds= \
	mc $(InputName)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"msg00001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Release BuildLib"

# Begin Custom Build
InputPath=.\TransBrokerMsg.mc
InputName=TransBrokerMsg

BuildCmds= \
	mc $(InputName)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"msg00001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Debug BuildLib"

# Begin Custom Build
InputPath=.\TransBrokerMsg.mc
InputName=TransBrokerMsg

BuildCmds= \
	mc $(InputName)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"msg00001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Unicode Debug BuildLib"

# Begin Custom Build
InputPath=.\TransBrokerMsg.mc
InputName=TransBrokerMsg

BuildCmds= \
	mc $(InputName)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"msg00001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Unicode Release "

# Begin Custom Build
InputPath=.\TransBrokerMsg.mc
InputName=TransBrokerMsg

BuildCmds= \
	mc $(InputName)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"msg00001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Unicode Release BuildLib"

# Begin Custom Build
InputPath=.\TransBrokerMsg.mc
InputName=TransBrokerMsg

BuildCmds= \
	mc $(InputName)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"msg00001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "TransBrokerMsg - Win32 Unicode Debug"

# Begin Custom Build
InputPath=.\TransBrokerMsg.mc
InputName=TransBrokerMsg

BuildCmds= \
	mc $(InputName)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"msg00001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TransBrokerMsg.rc
# End Source File
# End Target
# End Project
