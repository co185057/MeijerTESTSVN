# Microsoft Developer Studio Project File - Name="SCOTAppMsg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SCOTAppMsg - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SCOTAppMsg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SCOTAppMsg.mak" CFG="SCOTAppMsg - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SCOTAppMsg - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTAppMsg - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTAppMsg - Win32 Debug BuildLib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTAppMsg - Win32 Release BuildLib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTAppMsg - Win32 Unicode Release BuildLib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTAppMsg - Win32 Unicode Debug BuildLib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTAppMsg - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCOTAppMsg - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SCOTx Software/Source/Release 3.0/Core/Source/SCOTAppMsg", KYUBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SCOTAppMsg - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386
# SUBTRACT LINK32 /incremental:yes
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy SCOTAppMsg.h     ..\include /r	xcopy   release\scotappmsg.dll c:\scot\dll\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /debug /machine:I386
# SUBTRACT LINK32 /incremental:no
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy SCOTAppMsg.h     ..\include /r	xcopy debug\scotappmsg.dll   c:\scot\dll\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Debug BuildLib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SCOTAppM"
# PROP BASE Intermediate_Dir "SCOTAppM"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SCOTAppM"
# PROP Intermediate_Dir "SCOTAppM"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "." /I "..\include" /I "..\scotssf" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /map /debug /machine:I386
# SUBTRACT LINK32 /incremental:no
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy SCOTAppMsg.h     ..\include\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Release BuildLib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SCOTApp0"
# PROP BASE Intermediate_Dir "SCOTApp0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "SCOTApp0"
# PROP Intermediate_Dir "SCOTApp0"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "." /I "..\include" /I "..\scotssf" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /machine:I386
# SUBTRACT LINK32 /incremental:yes
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy SCOTAppMsg.h  ..\include\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Unicode Release BuildLib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SCOTAppMsg___Win32_Release_Unicode_BuildLib"
# PROP BASE Intermediate_Dir "SCOTAppMsg___Win32_Release_Unicode_BuildLib"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode_BuildLib"
# PROP Intermediate_Dir "Release_Unicode_BuildLib"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /machine:I386
# SUBTRACT BASE LINK32 /incremental:yes
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Debug_Unicode_BuildLib/SCOTAppMsgU.dll"
# SUBTRACT LINK32 /incremental:yes
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy SCOTAppMsg.h  ..\include\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Unicode Debug BuildLib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SCOTAppMsg___Win32_Debug_Unicode_BuildLib"
# PROP BASE Intermediate_Dir "SCOTAppMsg___Win32_Debug_Unicode_BuildLib"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode_BuildLib"
# PROP Intermediate_Dir "Debug_Unicode_BuildLib"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT BASE CPP /Fr /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /map /debug /machine:I386
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"Debug_Unicode_BuildLib/SCOTAppMsgud.dll"
# SUBTRACT LINK32 /incremental:no
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy SCOTAppMsg.h     ..\include\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SCOTAppMsg___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "SCOTAppMsg___Win32_Release_Unicode"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode"
# PROP Intermediate_Dir "Release_Unicode"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\include" /I "..\scotssf" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386
# SUBTRACT BASE LINK32 /incremental:yes
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386 /out:"Release_Unicode/SCOTAppMsgu.dll"
# SUBTRACT LINK32 /incremental:yes
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy SCOTAppMsg.h     ..\include /r	xcopy   Release_Unicode\scotappmsgU.dll c:\scot\dll\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SCOTAppMsg___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "SCOTAppMsg___Win32_Debug_Unicode"
# PROP BASE Ignore_Export_Lib 1
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode"
# PROP Intermediate_Dir "Debug_Unicode"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT BASE CPP /Fr /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\scotssf" /I "c:\scot\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /debug /machine:I386
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug_Unicode/SCOTAppMsgud.dll"
# SUBTRACT LINK32 /incremental:no
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy SCOTAppMsg.h     ..\include /r	xcopy debug_unicode\scotappmsgUD.dll   c:\scot\dll\*.* /r
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "SCOTAppMsg - Win32 Release"
# Name "SCOTAppMsg - Win32 Debug"
# Name "SCOTAppMsg - Win32 Debug BuildLib"
# Name "SCOTAppMsg - Win32 Release BuildLib"
# Name "SCOTAppMsg - Win32 Unicode Release BuildLib"
# Name "SCOTAppMsg - Win32 Unicode Debug BuildLib"
# Name "SCOTAppMsg - Win32 Unicode Release"
# Name "SCOTAppMsg - Win32 Unicode Debug"
# Begin Source File

SOURCE=.\SCOTAppMsg.h

!IF  "$(CFG)" == "SCOTAppMsg - Win32 Release"

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Debug BuildLib"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Release BuildLib"

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Unicode Release BuildLib"

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Unicode Debug BuildLib"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Unicode Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SCOTAppMsg.mc

!IF  "$(CFG)" == "SCOTAppMsg - Win32 Release"

# Begin Custom Build
InputPath=.\SCOTAppMsg.mc
InputName=SCOTAppMsg

BuildCmds= \
	mc $(InputPath)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"msg00001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Debug"

# Begin Custom Build
InputPath=.\SCOTAppMsg.mc
InputName=SCOTAppMsg

BuildCmds= \
	mc $(InputPath)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"msg00001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Debug BuildLib"

# Begin Custom Build
InputPath=.\SCOTAppMsg.mc
InputName=SCOTAppMsg

BuildCmds= \
	mc $(InputPath)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"msg00001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Release BuildLib"

# Begin Custom Build
InputPath=.\SCOTAppMsg.mc
InputName=SCOTAppMsg

BuildCmds= \
	mc $(InputPath)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"msg00001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Unicode Release BuildLib"

# Begin Custom Build
InputPath=.\SCOTAppMsg.mc
InputName=SCOTAppMsg

BuildCmds= \
	mc $(InputPath)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"msg00001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Unicode Debug BuildLib"

# Begin Custom Build
InputPath=.\SCOTAppMsg.mc
InputName=SCOTAppMsg

BuildCmds= \
	mc $(InputPath)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"msg00001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Unicode Release"

# Begin Custom Build
InputPath=.\SCOTAppMsg.mc
InputName=SCOTAppMsg

BuildCmds= \
	mc $(InputPath)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"msg00001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Unicode Debug"

# Begin Custom Build
InputPath=.\SCOTAppMsg.mc
InputName=SCOTAppMsg

BuildCmds= \
	mc $(InputPath)

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

SOURCE=.\SCOTAppMsg.rc

!IF  "$(CFG)" == "SCOTAppMsg - Win32 Release"

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Debug"

# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /d "_AFXDLL"

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Debug BuildLib"

# ADD BASE RSC /l 0x409 /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_AFXDLL"

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Release BuildLib"

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Unicode Release BuildLib"

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Unicode Debug BuildLib"

# ADD BASE RSC /l 0x409 /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_AFXDLL"

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "SCOTAppMsg - Win32 Unicode Debug"

# ADD BASE RSC /l 0x409 /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_AFXDLL"

!ENDIF 

# End Source File
# End Target
# End Project
