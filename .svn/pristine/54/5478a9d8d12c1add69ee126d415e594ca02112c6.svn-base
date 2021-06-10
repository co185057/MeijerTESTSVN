// TestRCM.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TestRCM.h"
#include "TestRCMDlg.h"
#include "RCMRegistry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define HKEY_TRCM           _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\TestRCM")

HTRACE g_hTrace;


// CTestRCMApp

BEGIN_MESSAGE_MAP(CTestRCMApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTestRCMApp construction

CTestRCMApp::CTestRCMApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTestRCMApp object

CTestRCMApp theApp;


// CTestRCMApp initialization

BOOL CTestRCMApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

    g_hTrace = Trace::initialize(_T("trace.log"), HKEY_RCM);

	CTestRCMDlg dlg;
    dlg.SetTraceObject(g_hTrace);

    m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
