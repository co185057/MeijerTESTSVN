// PSAPI.h: interface for the CPSAPI class.
//
// This class includes many process and module helper functions.  
// This class has evolved from its original implementation by
// Lee, JaeKil 1998-1999 (opensource) and fully supports all 
// features in both NT4 and up.  It is important that
// any new functionality also adheres to this requirement. 
// - SteveA (SA185011)
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "psapi.h"

#include <vdmdbg.h>
#include <tlhelp32.h>
#include <vector>

static DWORD const PSAPI_MAX_PATH = 4096;
static DWORD const PSAPI_MAX_HANDLE_COUNT = 1024;

typedef struct tagPROCESSMODULEINFO
{
	HMODULE hModule;
	TCHAR szModuleName[PSAPI_MAX_PATH];
	TCHAR szPathName[PSAPI_MAX_PATH];
} PROCESSMODULEINFO, * PPROCESSMODULEINFO;

typedef struct tagPROCESSINFO
{
   TCHAR szProcessName[PSAPI_MAX_PATH];
   DWORD dwProcessID;
} PROCESSINFO, PPROCESSINFO;

typedef std::vector<PROCESSMODULEINFO> PROCESSMODULEINFOLIST, * PPROCESSMODULEINFOLIST;
typedef std::vector<PROCESSINFO> PROCESSINFOLIST, * PPROCESSINFOLIST;
typedef std::vector<HWND> WINDOWLIST, * PWINDOWLIST;


namespace PSAPI
{
	BOOL Load(void);
   void Unload();

	BOOL EnumDeviceDrivers(void);
	BOOL EnumProcesses(void);
	BOOL EnumProcessModules(DWORD dwProcessId);
   BOOL EnumProcessThreads(DWORD dwProcessId);
   HWND GetWindowByProcess(LPCTSTR szProcessName, LPCTSTR szWindowClass, LPCTSTR szWindowText, int iXpos = -1, int iYpos = -1);
   HWND GetWindowByProcess(DWORD dwProcessId, LPCTSTR szWindowClass, LPCTSTR szWindowText, int iXpos = -1, int iYpos = -1);
   DWORD GetFirstMatchProcessId(LPCTSTR szProcessName);
   PROCESSINFOLIST GetProcessList();
	PROCESSMODULEINFOLIST GetProcessModuleList(DWORD dwProcessId);
	PROCESSMODULEINFO FindLoadedModuleInfo(DWORD dwProcessId, LPCTSTR szPartialModuleName);
}
