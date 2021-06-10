#if !defined(AFX_FINDDIALOG_H__917566A3_B2F1_11D1_90AE_00609761BB2E__INCLUDED_)
#define AFX_FINDDIALOG_H__917566A3_B2F1_11D1_90AE_00609761BB2E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FindDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFindDialog dialog

class CFindDialog : public CDialog
{
// Construction
public:
	BOOL FindInList(CString& string, CList<CString, CString&>& list);
	void AddUnique(CString& string, CComboBox& comboBox,CList<CString, CString&>& list);
	void SetDynaframeName(CString& name);
	void SetDynaframeID(CString& ID);
	void SetDynaframeTransNum(CString& transNum);
	CFindDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFindDialog)
	enum { IDD = IDD_FIND };
	CComboBox	m_DynaTransNumComboBox;
	CComboBox	m_DynaNameComboBox;
	CComboBox	m_DynaIdComboBox;
	int		m_FindType;
	CString	m_DynaName;
	int		m_FindDirection;
	CString	m_DynaId;
	CString	m_DynaTransNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFindDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDynaname();
	afx_msg void OnDynaid();
	afx_msg void OnDynatransnum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
  CList<CString, CString&> m_NameList;
  CList<CString, CString&> m_IdList;
  CList<CString, CString&> m_TransNumList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDDIALOG_H__917566A3_B2F1_11D1_90AE_00609761BB2E__INCLUDED_)
