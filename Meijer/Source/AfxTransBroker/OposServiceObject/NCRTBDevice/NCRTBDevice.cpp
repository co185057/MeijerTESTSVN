// NCRTBDevice.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "NCRTBDevice.h"
#include "NCRTBDeviceSO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CNCRTBDeviceApp

BEGIN_MESSAGE_MAP(CNCRTBDeviceApp, CWinApp)
	//{{AFX_MSG_MAP(CNCRTBDeviceApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNCRTBDeviceApp construction

CNCRTBDeviceApp::CNCRTBDeviceApp()
{
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CNCRTBDeviceApp object

CNCRTBDeviceApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNCRTBDeviceApp initialization

BOOL CNCRTBDeviceApp::InitInstance()
{

	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleObjectFactory::RegisterAll();

	//antonio start
		if (!AfxSocketInit())
	{
		return FALSE;
	}

    return TRUE;
	//antonio end
}

/////////////////////////////////////////////////////////////////////////////
// Special entry points required for inproc servers

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}

// by exporting DllRegisterServer, you can use regsvr.exe
STDAPI DllRegisterServer(void)
{
	int			iRet;
	HKEY		hKey;
	IID			iidObject;
	CString		strIID, strKey, strEntry;

	
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COleObjectFactory::UpdateRegistryAll();

	// get IID of the POS Keyboard object

    iidObject = CNCRTBDeviceSO::GetClassID();
        //CPosKeyboard::GetClassID();

	// format the IID in system registry format

	strIID.Format(_T("{%08.8X-%04.4X-%04.4X-%02.2X%02.2X-%02.2X%02.2X%02.2X%02.2X%02.2X%02.2X}"),
                  iidObject.Data1,
                  iidObject.Data2,
                  iidObject.Data3,
                  iidObject.Data4[0],
                  iidObject.Data4[1],
                  iidObject.Data4[2],
                  iidObject.Data4[3],
                  iidObject.Data4[4],
                  iidObject.Data4[5],
                  iidObject.Data4[6],
                  iidObject.Data4[7]);

	// format registry key name

#if !defined(_WIN32_WCE)
	strKey   = "SOFTWARE\\Classes\\CLSID\\" + strIID + "\\InProcServer32";
#else
	strKey   = _T("CLSID\\") + strIID + _T("\\InProcServer32");
#endif

	strEntry = _T("Apartment");
	iRet     = E_UNEXPECTED;

	// open the key
#if !defined(_WIN32_WCE)
    if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strKey, 0, KEY_ALL_ACCESS, &hKey ) == ERROR_SUCCESS)
#else
	if (::RegOpenKeyEx(HKEY_CLASSES_ROOT, strKey, 0, KEY_ALL_ACCESS, &hKey ) == ERROR_SUCCESS)
#endif
    {
		// set apartment threading model

		::RegSetValueEx(hKey,
						_T("ThreadingModel"),
						0,
						REG_SZ,
						(CONST BYTE*)(LPCTSTR)strEntry,
						strEntry.GetLength() * sizeof(TCHAR) + sizeof(TCHAR));

		// close the key

		::RegCloseKey(hKey);

		// set return status

		iRet = S_OK;
    }

	return iRet;
}

