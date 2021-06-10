// mywnd.cpp : implementation file
//

#include "stdafx.h"
#include "fireev.h"
#include "firectl.h"
#include "mywnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////
//message: WM_THREADFIREEVENT
//wParam:  FALSE means fire the event.  TRUE means destroy our window
//lParam:  Not used
#define WM_THREADFIREEVENT WM_USER+101

/////////////////////////////////////////////////////////////////////////////
// CMyWindow

CMyWindow::CMyWindow(CFireeventCtrl *pCtrl):m_pControl(pCtrl)
{
}

CMyWindow::~CMyWindow()
{
}


BEGIN_MESSAGE_MAP(CMyWindow, CWnd)
	//{{AFX_MSG_MAP(CMyWindow)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_THREADFIREEVENT,OnFireEventForThread)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyWindow message handlers

HWND CMyWindow::Create()
{
	//Register a window class
	LPCTSTR classname = 0;
	classname = AfxRegisterWndClass(0);

	//Create the window and return it's handle
	CWnd::CreateEx(NULL,classname,NULL,NULL,1,1,1,1,NULL,NULL);
	ASSERT(m_hWnd!=NULL);
	return m_hWnd;
}

LRESULT CMyWindow::OnFireEventForThread(WPARAM wParam, LPARAM lParam)
{
	//This is a one-message-does-everything handler.  If wParam is not
	//set, that means our message has been sent to fire the even.
	//If wParam is set that means we're being asked to destroy ourselves
	if(!wParam)
		m_pControl->FireLengthyProcessDone();
	else if(wParam==1)
		DestroyWindow();

	return TRUE;
}

void CMyWindow::PostNcDestroy() 
{
	//clean up this object
	delete this;
}