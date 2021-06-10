# Microsoft Developer Studio Project File - Name="TransactionBroker" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TransactionBroker - Win32 Unicode Debug BuildLib
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TransactionBroker.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TransactionBroker.mak" CFG="TransactionBroker - Win32 Unicode Debug BuildLib"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TransactionBroker - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TransactionBroker - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TransactionBroker - Win32 Debug BuildLib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TransactionBroker - Win32 Release BuildLib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TransactionBroker - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TransactionBroker - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TransactionBroker - Win32 Unicode Debug BuildLib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TransactionBroker - Win32 Unicode Release BuildLib" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SCOTx Software/Source/Release 3.0/Core/Source/AfxTransBroker", ZRPBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TransactionBroker - Win32 Release"

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
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "tbCommon" /I "emulation" /I ".\\" /I "..\include" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /I "messages" /I "CustomerTransBroker" /I "SolutionTransBroker" /I "BaseTransBroker" /I "TBCommon" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_TRANSBROKER_PROVIDER_" /D "_WIN32_DCOM" /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386 /out:"Release/TransBroker.dll" /libpath:"..\lib\Release" /libpath:"$(MEIJER605)\adk\lib\Release"
# SUBTRACT LINK32 /incremental:yes
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Release\TransBroker.lib ..\lib\Release /r	xcopy  Release\TransBroker.dll $(MEIJER605)\bin\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TransactionBroker - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "tbCommon" /I "emulation" /I ".\\" /I "..\include" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /I "messages" /I "CustomerTransBroker" /I "SolutionTransBroker" /I "BaseTransBroker" /I "TBCommon" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_TRANSBROKER_PROVIDER_" /D "_WIN32_DCOM" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"MSVCRT.lib" /out:"Debug/TransBrokerD.dll" /libpath:"..\lib\debug" /libpath:"$(MEIJER605)\ADK\Lib\debug"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Debug\TransBrokerD.lib ..\lib\Debug\*.* /r	xcopy Debug\TransBrokerD.dll $(MEIJER605)\bin\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TransactionBroker - Win32 Debug BuildLib"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Transact"
# PROP BASE Intermediate_Dir "Transact"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Transact"
# PROP Intermediate_Dir "Transact"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "." /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_TRANSBROKER_PROVIDER_" /FR /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "emulation\DuplexClientPipe" /I "..\scotssf" /I "Messages\BaseMessages" /I "Messages\SolutionMessages" /I "Messages\CustomerMessages" /I "emulation" /I "emulation\taps" /I "emulation\GTS" /I "emulation\OPOS" /I "emulation\POS" /I "emulation\HD_JPOS" /I "MsgHandlers" /I "\traceWriter\com exe server" /I ".\\" /I "..\include" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /I "messages" /I "CustomerTransBroker" /I "SolutionTransBroker" /I "BaseTransBroker" /I "TBCommon" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_TRANSBROKER_PROVIDER_" /D "_WIN32_DCOM" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 CmClassD.lib NCREventLogging.lib acscom32.lib version.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"MSVCRT.lib" /out:"Debug/TransBrokerD.dll" /pdbtype:sept /libpath:"..\lib\debug" /libpath:"\SCOT\ADK\Lib\debug"
# ADD LINK32 version.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /nodefaultlib:"MSVCRT.lib" /out:"Transact/TransBrokerD.dll" /libpath:"..\lib\debug" /libpath:"$(MEIJER605)\ADK\Lib\debug" /libpath:"$(MEIJER605)\controls" /verbose:lib
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Transact\TransBrokerD.lib ..\lib\Debug\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TransactionBroker - Win32 Release BuildLib"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Transac0"
# PROP BASE Intermediate_Dir "Transac0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Transac0"
# PROP Intermediate_Dir "Transac0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Od /I "." /I "..\include" /I "..\scotssf" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_TRANSBROKER_PROVIDER_" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "emulation\duplexclientpipe" /I "emulation" /I "..\scotssf" /I "tbCommon" /I ".\\" /I "..\include" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /I "messages" /I "CustomerTransBroker" /I "SolutionTransBroker" /I "BaseTransBroker" /I "TBCommon" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_TRANSBROKER_PROVIDER_" /D "_WIN32_DCOM" /FR /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 CmClass.lib NCREventLogging.lib acscom32.lib version.lib /nologo /subsystem:windows /dll /incremental:yes /machine:I386 /out:"Release/TransBroker.dll" /libpath:"..\lib\Release" /libpath:"\scot\adk\lib\Release"
# ADD LINK32 version.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Transac0\TransBroker.dll" /libpath:"..\lib\Release" /libpath:"$(MEIJER605)\adk\lib\Release"
# SUBTRACT LINK32 /incremental:yes
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Transac0\TransBroker.lib ..\lib\Release\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TransactionBroker - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TransactionBroker___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "TransactionBroker___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode"
# PROP Intermediate_Dir "Debug_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "tbCommon" /I "emulation" /I ".\\" /I "..\include" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /I "messages" /I "CustomerTransBroker" /I "SolutionTransBroker" /I "BaseTransBroker" /I "TBCommon" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_TRANSBROKER_PROVIDER_" /Fr /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "tbCommon" /I "emulation" /I ".\\" /I "..\include" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /I "messages" /I "CustomerTransBroker" /I "SolutionTransBroker" /I "BaseTransBroker" /I "TBCommon" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_TRANSBROKER_PROVIDER_" /D "UNICODE" /D "_UNICODE" /D "_WIN32_DCOM" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 NCREventLoggingD.lib version.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"MSVCRT.lib" /out:"Debug/TransBrokerD.dll" /libpath:"..\lib\debug" /libpath:"$(MEIJER605)\ADK\Lib\debug"
# ADD LINK32 version.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /nodefaultlib:"MSVCRT.lib" /out:"Debug_Unicode\TransBrokerUD.dll" /libpath:"..\lib\debug" /libpath:"$(MEIJER605)\ADK\Lib\debug"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Debug_Unicode\TransBrokerUD.lib ..\lib\Debug\*.* /r	xcopy  Debug_Unicode\TransBrokerUD.dll $(MEIJER605)\bin\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TransactionBroker - Win32 Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TransactionBroker___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "TransactionBroker___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode"
# PROP Intermediate_Dir "Release_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "tbCommon" /I "emulation" /I ".\\" /I "..\include" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /I "messages" /I "CustomerTransBroker" /I "SolutionTransBroker" /I "BaseTransBroker" /I "TBCommon" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_TRANSBROKER_PROVIDER_" /YX"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "tbCommon" /I "emulation" /I ".\\" /I "..\include" /I "$(MEIJER605)\adk\include" /I "messages" /I "CustomerTransBroker" /I "SolutionTransBroker" /I "BaseTransBroker" /I "TBCommon" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_TRANSBROKER_PROVIDER_" /D "UNICODE" /D "_UNICODE" /D "_WIN32_DCOM" /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 NCREventLogging.lib version.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386 /out:"Release/TransBroker.dll" /libpath:"..\lib\Release" /libpath:"$(MEIJER605)\adk\lib\Release"
# SUBTRACT BASE LINK32 /incremental:yes
# ADD LINK32 version.lib /nologo /subsystem:windows /dll /map /debug /debugtype:both /machine:I386 /out:"Release_Unicode\TransBrokerU.dll" /libpath:"..\lib\Release" /libpath:"$(MEIJER605)\adk\lib\Release"
# SUBTRACT LINK32 /incremental:yes
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Release_Unicode\TransBrokerU.lib ..\lib\Release /r	xcopy Release_Unicode\TransBrokerU.dll $(MEIJER605)\bin\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TransactionBroker - Win32 Unicode Debug BuildLib"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TransactionBroker___Win32_Unicode_Debug_BuildLib"
# PROP BASE Intermediate_Dir "TransactionBroker___Win32_Unicode_Debug_BuildLib"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode_BuildLib"
# PROP Intermediate_Dir "Debug_Unicode_BuildLib"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "emulation\DuplexClientPipe" /I "..\scotssf" /I "Messages\BaseMessages" /I "Messages\SolutionMessages" /I "Messages\CustomerMessages" /I "emulation" /I "emulation\taps" /I "emulation\GTS" /I "emulation\OPOS" /I "emulation\POS" /I "emulation\HD_JPOS" /I "MsgHandlers" /I "\traceWriter\com exe server" /I ".\\" /I "..\include" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /I "messages" /I "CustomerTransBroker" /I "SolutionTransBroker" /I "BaseTransBroker" /I "TBCommon" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_TRANSBROKER_PROVIDER_" /Fr /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "emulation\DuplexClientPipe" /I "..\scotssf" /I "Messages\BaseMessages" /I "Messages\SolutionMessages" /I "Messages\CustomerMessages" /I "emulation" /I "emulation\taps" /I "emulation\GTS" /I "emulation\OPOS" /I "emulation\POS" /I "emulation\HD_JPOS" /I "MsgHandlers" /I "\traceWriter\com exe server" /I ".\\" /I "..\include" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /I "messages" /I "CustomerTransBroker" /I "SolutionTransBroker" /I "BaseTransBroker" /I "TBCommon" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_TRANSBROKER_PROVIDER_" /D "UNICODE" /D "_UNICODE" /D "_WINDLL" /D "_AFXDLL" /D "_WIN32_DCOM" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 NCREventLoggingD.lib version.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /nodefaultlib:"MSVCRT.lib" /out:"Transact/TransBrokerD.dll" /libpath:"..\lib\debug" /libpath:"$(MEIJER605)\ADK\Lib\debug" /verbose:lib
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 version.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /nodefaultlib:"MSVCRT.lib" /out:"Debug_Unicode_BuildLib\TransBrokerUD.dll" /libpath:"..\lib\debug" /libpath:"$(MEIJER605)\ADK\Lib\debug" /verbose:lib
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy .\Debug_Unicode_BuildLib\TransBrokerUD.lib ..\lib\debug\*.* /r
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TransactionBroker - Win32 Unicode Release BuildLib"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TransactionBroker___Win32_Unicode_Release_BuildLib"
# PROP BASE Intermediate_Dir "TransactionBroker___Win32_Unicode_Release_BuildLib"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode_BuildLib"
# PROP Intermediate_Dir "Release_Unicode_BuildLib"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "emulation\duplexclientpipe" /I "emulation" /I "..\scotssf" /I "tbCommon" /I ".\\" /I "..\include" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /I "messages" /I "CustomerTransBroker" /I "SolutionTransBroker" /I "BaseTransBroker" /I "TBCommon" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_TRANSBROKER_PROVIDER_" /FR /YX"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "emulation\duplexclientpipe" /I "emulation" /I "..\scotssf" /I "tbCommon" /I ".\\" /I "..\include" /I "$(MEIJER605)\adk\include" /I "C:\Program Files\DevStudio\VC\mfc\src" /I "messages" /I "CustomerTransBroker" /I "SolutionTransBroker" /I "BaseTransBroker" /I "TBCommon" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_TRANSBROKER_PROVIDER_" /D "UNICODE" /D "_UNICODE" /D "_WIN32_DCOM" /FR /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 NCREventLogging.lib version.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Transac0\TransBroker.dll" /libpath:"..\lib\Release" /libpath:"$(MEIJER605)\adk\lib\Release"
# SUBTRACT BASE LINK32 /incremental:yes
# ADD LINK32 version.lib /nologo /subsystem:windows /dll /map /machine:I386 /out:"Release_Unicode_BuildLib\TransBrokerU.dll" /libpath:"..\lib\Release" /libpath:"$(MEIJER605)\adk\lib\Release"
# SUBTRACT LINK32 /incremental:yes
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy Release_Unicode_BuildLib\TransBrokerU.lib ..\lib\release\*.* /r
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "TransactionBroker - Win32 Release"
# Name "TransactionBroker - Win32 Debug"
# Name "TransactionBroker - Win32 Debug BuildLib"
# Name "TransactionBroker - Win32 Release BuildLib"
# Name "TransactionBroker - Win32 Unicode Debug"
# Name "TransactionBroker - Win32 Unicode Release"
# Name "TransactionBroker - Win32 Unicode Debug BuildLib"
# Name "TransactionBroker - Win32 Unicode Release BuildLib"
# Begin Group "Base Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Base Current Classes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentCashDrawer.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentCashDrawer.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentDisplay.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentFiscalPrinter.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentFiscalPrinter.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentItem.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentItem.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentJournal.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentJournal.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentKeyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentKeyboard.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentKeylock.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentKeylock.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentMicr.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentMicr.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentMSR.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentMSR.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentOperator.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentOperator.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentOptions.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentPINPad.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentPINPad.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentPrinter.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentPrinter.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentReceipt.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentReceipt.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentSigCap.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentSigCap.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentSlip.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentSlip.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentTender.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentTender.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentTotals.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentTotals.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentTransaction.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseCurrentTransaction.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\CurrentPosState.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\CurrentPosState.h
# End Source File
# End Group
# Begin Group "Base MH Classes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHCashDrawer.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHCashDrawer.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHDisplay.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHFiscalPrinter.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHFiscalPrinter.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHGenMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHGenMsg.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHItem.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHItem.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHJournal.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHJournal.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHKeyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHKeyboard.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHKeylock.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHKeylock.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHMicr.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHMicr.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHMSR.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHMSR.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHOperator.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHOperator.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHOptions.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHPINPad.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHPINPad.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHPrinter.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHPrinter.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHReceipt.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHReceipt.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHSigCap.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHSigCap.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHSlip.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHSlip.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHTender.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHTender.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHTotals.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHTotals.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHTransaction.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMHTransaction.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\BaseTransBroker\BaseKeyDefines.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMessages.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMessages.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMsgMap.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseMsgMap.h
# End Source File
# Begin Source File

SOURCE=.\Emulation\BasePOSEmulation.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseTBAPI.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseTBConnectFL.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseTBConnectFL.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseTBConnectPos.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseTBConnectPos.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseTBDefs.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseTBProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseTBProperties.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseTBScotMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseTBScotMsg.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseTrxBroker.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\BaseTrxBroker.h
# End Source File
# Begin Source File

SOURCE=.\Emulation\CustomerPOSEmulation.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\MsgHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\MsgHandler.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\OverrideScotStringList.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\OverrideScotStringList.h
# End Source File
# Begin Source File

SOURCE=.\TBCommon\SafeArrayVariant.cpp
# End Source File
# Begin Source File

SOURCE=.\TBCommon\SafeArrayVariant.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\ScotStringList.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\SingleInstance.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\StdAfx.cpp

!IF  "$(CFG)" == "TransactionBroker - Win32 Release"

!ELSEIF  "$(CFG)" == "TransactionBroker - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "TransactionBroker - Win32 Debug BuildLib"

# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "TransactionBroker - Win32 Release BuildLib"

!ELSEIF  "$(CFG)" == "TransactionBroker - Win32 Unicode Debug"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "TransactionBroker - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "TransactionBroker - Win32 Unicode Debug BuildLib"

# ADD BASE CPP /Yc
# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "TransactionBroker - Win32 Unicode Release BuildLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\TBAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\TBApiCore.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\TBDefs.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\TBInProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\TBInProgress.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\TBUtilities.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\TBUtilities.h
# End Source File
# Begin Source File

SOURCE=.\TBCommon\TraceSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\TBCommon\TraceSystem.h
# End Source File
# End Group
# Begin Group "Other Include Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TBCommon\autolock.h
# End Source File
# Begin Source File

SOURCE=.\BaseTransBroker\tbMessageMap.h
# End Source File
# End Group
# Begin Group "Customer Files"

# PROP Default_Filter ""
# Begin Group "Customer Current Classes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentCashDrawer.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentCashDrawer.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentDisplay.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentFiscalPrinter.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentFiscalPrinter.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentItem.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentItem.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentJournal.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentJournal.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentKeyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentKeyboard.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentKeylock.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentKeylock.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentMicr.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentMicr.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentMSR.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentMSR.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentOperator.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentOperator.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentOptions.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentPINPad.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentPINPad.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentPrinter.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentPrinter.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentReceipt.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentReceipt.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentSigCap.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentSigCap.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentSlip.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentSlip.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentTender.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentTender.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentTotals.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentTotals.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentTransaction.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerCurrentTransaction.h
# End Source File
# End Group
# Begin Group "Customer MH Classes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHCashDrawer.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHCashDrawer.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHDisplay.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHFiscalPrinter.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHFiscalPrinter.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHGenMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHGenMsg.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHItem.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHItem.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHJournal.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHJournal.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHKeyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHKeyboard.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHKeylock.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHKeylock.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHMicr.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHMicr.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHMSR.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHMSR.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHOperator.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHOperator.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHOptions.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHPINPad.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHPINPad.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHPrinter.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHPrinter.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHReceipt.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHReceipt.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHSigCap.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHSigCap.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHSlip.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHSlip.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHTender.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHTender.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHTotals.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHTotals.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHTransaction.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMHTransaction.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerKeyDefines.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMsgMap.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerMsgMap.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerTBAPI.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerTBConnectFL.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerTBConnectFL.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerTBConnectPOS.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerTBConnectPOS.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerTBDefs.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerTBProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerTBProperties.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerTBScotMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerTBScotMsg.h
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerTrxBroker.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomerTransBroker\CustomerTrxBroker.h
# End Source File
# End Group
# Begin Group "Messages"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Messages\CashDrawerMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\CashDrawerMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\CMOSMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\CMOSMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\CustomerDefinedMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\CustomerDefinedMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\DisplayMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\DisplayMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\FiscalPrinterMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\FiscalPrinterMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\ItemDetailsMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\ItemDetailsMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\KeyboardMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\KeyboardMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\KeylockMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\KeylockMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\MicrMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\MicrMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\MSRMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\MSRMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\PinpadMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\PinpadMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\PrinterMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\PrinterMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\ReturnStateMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\ReturnStateMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\RewardLineDetailsMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\RewardLineDetailsMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\ScaleMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\ScaleMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\ScannerMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\ScannerMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\TBGenMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\TBGenMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\TBMsgObj.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\TBMsgObj.h
# End Source File
# Begin Source File

SOURCE=.\Messages\TenderDetailsMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\TenderDetailsMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\ToneIndicatorMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\ToneIndicatorMsg.h
# End Source File
# Begin Source File

SOURCE=.\Messages\TotalDetailsMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Messages\TotalDetailsMsg.h
# End Source File
# End Group
# Begin Group "Emulation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Emulation\DuplexClientPipe\APipeSink.h
# End Source File
# Begin Source File

SOURCE=.\Emulation\DuplexClientPipe\DuplexClientPipe.cpp
# End Source File
# Begin Source File

SOURCE=.\Emulation\DuplexClientPipe\DuplexClientPipe.h
# End Source File
# Begin Source File

SOURCE=.\Emulation\DuplexClientPipe\PipeMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\Emulation\DuplexClientPipe\PipeMessage.h
# End Source File
# Begin Source File

SOURCE=.\Emulation\DuplexClientPipe\PipeQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\Emulation\DuplexClientPipe\PipeQueue.h
# End Source File
# End Group
# Begin Group "Solution Files"

# PROP Default_Filter ""
# Begin Group "Solution MH Classes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHCashDrawer.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHCashDrawer.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHDisplay.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHGenMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHGenMsg.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHItem.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHItem.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHMicr.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHMicr.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHOperator.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHOperator.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHOptions.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHReceipt.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHReceipt.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHSlip.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHSlip.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHTender.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHTender.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHTotals.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHTotals.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHTransaction.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMHTransaction.h
# End Source File
# End Group
# Begin Group "Solution Current Classes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionCurrentDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionCurrentDisplay.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionCurrentReceipt.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionCurrentReceipt.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionCurrentSigCap.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionCurrentSigCap.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionCurrentSlip.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionCurrentSlip.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMsgMap.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMsgMap.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMultiBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionMultiBitmap.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionTBConnectPOS.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionTBConnectPOS.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionTBDefs.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionTBProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionTBProperties.h
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionTBSharedDefs.H
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionTrxBroker.cpp
# End Source File
# Begin Source File

SOURCE=.\SolutionTransBroker\SolutionTrxBroker.h
# End Source File
# End Group
# End Target
# End Project
