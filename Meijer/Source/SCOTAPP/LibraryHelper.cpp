#include "StdAfx.h"
#include "LibraryHelper.h"

// PTL_COVERAGE_IGNORE_START

// This class is a very thin wrapper around windows file APIs, which allows
// us to use a fake version of this class in our other tests.
// No unit tests will be written for this class, because it simply delegates to
// windows API calls.

CLibraryHelper::CLibraryHelper()
{
}

CLibraryHelper::~CLibraryHelper()
{
}


HWND CLibraryHelper::FindWindow(LPCTSTR lpszClasName, LPCTSTR lpszWindowName)
{
    return ::FindWindow(lpszClasName, lpszWindowName);
}


HMODULE CLibraryHelper::LoadLibrary(LPCTSTR lpFileName)
{
    return ::LoadLibrary(lpFileName);
}

BOOL CLibraryHelper::FreeLibrary(HMODULE hModule)
{
    return ::FreeLibrary(hModule);
}

FARPROC CLibraryHelper::GetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
    return ::GetProcAddress(hModule, lpProcName);
}

// PTL_COVERAGE_IGNORE_END
