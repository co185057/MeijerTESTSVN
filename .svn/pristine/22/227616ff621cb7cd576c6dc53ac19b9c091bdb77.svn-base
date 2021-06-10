// RemoteConsoleMgr.cpp : Implementation of CRemoteConsoleMgrApp and DLL registration.

#include "stdafx.h"
#include "RemoteConsoleMgr.h"
#include "RCMRegistry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CRemoteConsoleMgrApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x9581BE83, 0xB594, 0x4613, { 0x97, 0x27, 0xFB, 0xD7, 0x80, 0x8B, 0xC8, 0xDD } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;
HTRACE g_hTrace;



// CRemoteConsoleMgrApp::InitInstance - DLL initialization

BOOL CRemoteConsoleMgrApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
        g_hTrace = Trace::initialize(_T("trace.log"), HKEY_RCM);
	}

	return bInit;
}



// CRemoteConsoleMgrApp::ExitInstance - DLL termination

int CRemoteConsoleMgrApp::ExitInstance()
{
	// TODO: Add your own module termination code here.
    Trace::terminate(g_hTrace);

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
