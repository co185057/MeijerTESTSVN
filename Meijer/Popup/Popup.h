// Popup.h : main header file for the POPUP application
//

#if !defined(AFX_POPUP_H__0B33DE7A_F2E9_11D4_8943_00A0C9EDD46B__INCLUDED_)
#define AFX_POPUP_H__0B33DE7A_F2E9_11D4_8943_00A0C9EDD46B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPopupApp:
// See Popup.cpp for the implementation of this class
//

class CPopupApp : public CWinApp
{
public:
	CPopupApp();
	long m_lTimerID;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPopupApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUP_H__0B33DE7A_F2E9_11D4_8943_00A0C9EDD46B__INCLUDED_)
