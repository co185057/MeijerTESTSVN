// Utils.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Utils.h"

#if !defined(_WINDOWS) && defined(_CONSOLE)

static int nCount = 0;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        Trace::initialize();
        nCount++;
        ITRACE_DEVELOP("Attaching Process" + IString(nCount));
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
        Trace::terminate();
		break;
	}
    return TRUE;
}
#endif
