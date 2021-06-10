// Popup.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Popup.h"
#include "PopupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static VOID CALLBACK MakePopupTopMost( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
/////////////////////////////////////////////////////////////////////////////
// CPopupApp

BEGIN_MESSAGE_MAP(CPopupApp, CWinApp)
	//{{AFX_MSG_MAP(CPopupApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupApp construction

CPopupApp::CPopupApp()
:m_lTimerID(0)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPopupApp object

CPopupApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPopupApp initialization

BOOL CPopupApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CPopupDlg dlg;
	m_pMainWnd = &dlg;

	// added for SCOT 2.4 - used by SMA

	// allow only one instance of CPopupDlg
	HWND hwnd = ::FindWindow(NULL, _T("NCR Message"));
	if (hwnd)
	{
	::PostMessage(hwnd, WM_CLOSE, 0 , 0);
	}

	m_lTimerID = SetTimer(NULL, 0, 3000, &MakePopupTopMost);

	int nResponse = dlg.DoModal();
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

VOID CALLBACK MakePopupTopMost( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{	
   //KillTimer(NULL, m_lTimerID);

   SetWindowPos(FindWindow(NULL, _T("NCR Message")), HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
       
}