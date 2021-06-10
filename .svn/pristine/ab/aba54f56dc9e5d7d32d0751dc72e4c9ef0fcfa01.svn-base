#if !defined(AFX_OPTIONSDLG_H__39B61E52_6435_11D2_90DA_00609761BB2E__INCLUDED_)
#define AFX_OPTIONSDLG_H__39B61E52_6435_11D2_90DA_00609761BB2E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OptionsDlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

class COptionsDlg : public CDialog
{
// Construction
public:
	void OnTextColor(COLORREF& textColorRef, UINT nCtrlID);
	COptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsDlg)
	enum { IDD = IDD_OPTIONS };
	CString	m_BakFileViewer;
	CString	m_LogFileViewer;
	int		m_MinPrintWarnItems;
	CString	m_csAppDeathText;
	CString	m_csAppRestartText;
	CString	m_csTransEndText;
	CString	m_csLaneClosedText;
	//}}AFX_DATA

  COLORREF m_AppDeathColor;
  COLORREF m_AppRestartColor;
  COLORREF m_TransEndColor;
  COLORREF m_LaneClosedColor;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsDlg)
	afx_msg void OnBakBrowse();
	afx_msg void OnLogBrowse();
	afx_msg void OnAppRestartColor();
	afx_msg void OnAppDeathColor();
	afx_msg void OnLaneClosedColor();
	afx_msg void OnTransEndColor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void OnBrowse(int dlgItem, CString extension);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__39B61E52_6435_11D2_90DA_00609761BB2E__INCLUDED_)
