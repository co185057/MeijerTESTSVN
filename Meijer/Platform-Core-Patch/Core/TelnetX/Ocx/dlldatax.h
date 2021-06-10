#if !defined(AFX_DLLDATAX_H__45CA8C20_916E_4B2D_96C1_4086CA80FA0B__INCLUDED_)
#define AFX_DLLDATAX_H__45CA8C20_916E_4B2D_96C1_4086CA80FA0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _MERGE_PROXYSTUB

extern _T("C") 
{
BOOL WINAPI PrxDllMain(HINSTANCE hInstance, DWORD dwReason, 
	LPVOID lpReserved);
STDAPI PrxDllCanUnloadNow(void);
STDAPI PrxDllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
STDAPI PrxDllRegisterServer(void);
STDAPI PrxDllUnregisterServer(void);
}

#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLDATAX_H__45CA8C20_916E_4B2D_96C1_4086CA80FA0B__INCLUDED_)
