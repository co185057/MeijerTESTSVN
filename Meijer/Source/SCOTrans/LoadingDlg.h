#if !defined(AFX_LOADINGDLG_H__8E5F76AB_A8AC_11D1_90AC_00609761BB2E__INCLUDED_)
#define AFX_LOADINGDLG_H__8E5F76AB_A8AC_11D1_90AC_00609761BB2E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LoadingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLoadingDlg dialog

class CLoadingDlg : public CDialog
{
// Construction
public:
	void OffsetPos(int currentPos);
	void InitVariantProgress(int size);
	void StepIt(int currentItem);
	void InitProgress(int numItems);
	CLoadingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLoadingDlg)
	enum { IDD = IDD_DFLISTLOAD };
	CProgressCtrl	m_LoadProgress;
	CString	m_MsgTxt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoadingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoadingDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_Interval;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADINGDLG_H__8E5F76AB_A8AC_11D1_90AC_00609761BB2E__INCLUDED_)
