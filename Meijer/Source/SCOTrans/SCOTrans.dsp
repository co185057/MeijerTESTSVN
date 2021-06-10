# Microsoft Developer Studio Project File - Name="SCOTrans" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SCOTrans - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SCOTrans.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SCOTrans.mak" CFG="SCOTrans - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SCOTrans - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SCOTrans - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SCOTrans - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "$(MEIJER605)\adk\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Z<none>
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=if exist $(MEIJER605)\bin    copy Release\SCOTrans.exe $(MEIJER605)\bin
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SCOTrans - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "$(MEIJER605)\adk\include" /D "_WINDOWS" /D "_AFXDLL" /D "WIN32" /D "_DEBUG" /D "SCOT_BUILD" /D "_SCOT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=if exist $(MEIJER605)\bin copy    Debug\SCOTrans.exe $(MEIJER605)\bin
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "SCOTrans - Win32 Release"
# Name "SCOTrans - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DFStatsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dftrans.cpp
# End Source File
# Begin Source File

SOURCE=.\dftrans.rc
# End Source File
# Begin Source File

SOURCE=.\dftransDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\dftransView.cpp
# End Source File
# Begin Source File

SOURCE=.\DynaTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\FindDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\GotoDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\scotrans.awk

!IF  "$(CFG)" == "SCOTrans - Win32 Release"

# Begin Custom Build - Building tbmklist.exe ...
ProjDir=.
InputPath=.\scotrans.awk

"$(ProjDir)\tbmklist.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(ProjDir)\awkc.exe -P $(ProjDir)\awkrun.nt -o $(ProjDir)\tbmklist.exe -f $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "SCOTrans - Win32 Debug"

# Begin Custom Build - Building tbmklist.exe ...
ProjDir=.
InputPath=.\scotrans.awk

"$(ProjDir)\tbmklist.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(ProjDir)\awkc.exe -P $(ProjDir)\awkrun.nt -o $(ProjDir)\tbmklist.exe -f $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ScotState.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VersionInfo.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DFStatsDlg.h
# End Source File
# Begin Source File

SOURCE=.\dftrans.h
# End Source File
# Begin Source File

SOURCE=.\dftransDoc.h
# End Source File
# Begin Source File

SOURCE=.\dftransView.h
# End Source File
# Begin Source File

SOURCE=.\DynaTrans.h
# End Source File
# Begin Source File

SOURCE=.\FindDialog.h
# End Source File
# Begin Source File

SOURCE=.\GotoDialog.h
# End Source File
# Begin Source File

SOURCE=.\LoadingDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScotState.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\tbStateListType.h
# End Source File
# Begin Source File

SOURCE=..\include\TransBroker.h

!IF  "$(CFG)" == "SCOTrans - Win32 Release"

USERDEP__TRANS="$(ProjDir)\tbmklist.exe"	
# Begin Custom Build - Creating tbStateList.h ...
ProjDir=.
InputPath=..\include\TransBroker.h

"$(ProjDir)\tbStateList.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	tbmklist $(InputPath) > $(ProjDir)\tbStateList.h

# End Custom Build

!ELSEIF  "$(CFG)" == "SCOTrans - Win32 Debug"

USERDEP__TRANS="$(ProjDir)\tbmklist.exe"	
# Begin Custom Build - Creating tbStateList.h ...
ProjDir=.
InputPath=..\include\TransBroker.h

"$(ProjDir)\tbStateList.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	tbmklist $(InputPath) > $(ProjDir)\tbStateList.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VersionInfo.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\103.bmp
# End Source File
# Begin Source File

SOURCE=.\res\104.bmp
# End Source File
# Begin Source File

SOURCE=.\res\105.bmp
# End Source File
# Begin Source File

SOURCE=.\res\106.bmp
# End Source File
# Begin Source File

SOURCE=.\res\107.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dftrans.ico
# End Source File
# Begin Source File

SOURCE=.\res\dftrans.rc2
# End Source File
# Begin Source File

SOURCE=.\res\dftransDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
