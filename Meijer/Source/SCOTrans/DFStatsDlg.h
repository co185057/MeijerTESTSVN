#if !defined(AFX_DFSTATSDLG_H__E59AAF31_A9FE_11D1_90AC_00609761BB2E__INCLUDED_)
#define AFX_DFSTATSDLG_H__E59AAF31_A9FE_11D1_90AC_00609761BB2E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DFStatsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDFStatsDlg dialog

class CDFStatsDlg : public CDialog
{
// Construction
public:
	CDFStatsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDFStatsDlg)
	enum { IDD = IDD_DFSTATS };
	CString	m_AvgTime;
	int		m_NumTimes;
	int		m_ID;
	CString	m_Name;
	CString	m_MaxTime;
	CString	m_MinTime;
	CString	m_TotalTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDFStatsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDFStatsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DFSTATSDLG_H__E59AAF31_A9FE_11D1_90AC_00609761BB2E__INCLUDED_)
