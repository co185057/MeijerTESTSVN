#pragma once

// RemoteConsoleMgr.h : main header file for RemoteConsoleMgr.DLL

#if !defined( __AFXCTL_H__ )
#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols


// CRemoteConsoleMgrApp : See RemoteConsoleMgr.cpp for implementation.

class CRemoteConsoleMgrApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;
extern HTRACE g_hTrace;

