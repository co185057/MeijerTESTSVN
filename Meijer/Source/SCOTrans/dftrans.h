// dftrans.h : main header file for the DFTRANS application
//

#if !defined(AFX_DFTRANS_H__8E5F7699_A8AC_11D1_90AC_00609761BB2E__INCLUDED_)
#define AFX_DFTRANS_H__8E5F7699_A8AC_11D1_90AC_00609761BB2E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDftransApp:
// See dftrans.cpp for the implementation of this class
//

class CDftransApp : public CWinApp
{
public:
	void LaunchDefaultViewer(CString& fileName);
	int m_MinPrintWarnItems;
	void UpdateRegOptions(BOOL bFromReg);
	CString FileExtension(CString& path);
	CString BaseName(CString& path);
	CDftransApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDftransApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDftransApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileRefresh();
	afx_msg void OnUpdateFileRefresh(CCmdUI* pCmdUI);
	afx_msg void OnEditOptions();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl* m_pListCtrl;
	void SetListCtrl(CListCtrl* pListCtrl);
	CString m_BakFileViewer;
	CString m_LogFileViewer;
	CString	m_csAppDeathText;
	CString	m_csAppRestartText;
	CString	m_csTransEndText;
	CString	m_csLaneClosedText;

  COLORREF m_AppDeathColor;
  COLORREF m_AppRestartColor;
  COLORREF m_TransEndColor;
  COLORREF m_LaneClosedColor;

private:
	CSingleDocTemplate* m_pDocTemplate;
};

extern CDftransApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DFTRANS_H__8E5F7699_A8AC_11D1_90AC_00609761BB2E__INCLUDED_)
