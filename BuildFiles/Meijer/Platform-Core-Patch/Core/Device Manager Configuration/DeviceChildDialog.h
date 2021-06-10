#if !defined(AFX_DEVICECHILDDIALOG_H__757D8085_33AF_11D3_99C1_00A0C9EDD467__INCLUDED_)
#define AFX_DEVICECHILDDIALOG_H__757D8085_33AF_11D3_99C1_00A0C9EDD467__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DeviceChildDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeviceChildDialog dialog

class CDeviceChildDialog : public CDialog
{
// Construction
public:
	CDeviceChildDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeviceChildDialog)
	enum { IDD = IDD_DIALOG_LIST_DEVICE };
	CString	m_sClassName;
	int	m_nSelection;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeviceChildDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeviceChildDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
  afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	DECLARE_MESSAGE_MAP()
  LRESULT OnCtlColorDlg(WPARAM wParam, LPARAM lParam);
  static HBRUSH m_hBrushButton;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICECHILDDIALOG_H__757D8085_33AF_11D3_99C1_00A0C9EDD467__INCLUDED_)
