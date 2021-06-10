// ApiDumpDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "ApiRecorder.h"

#define MAX_PARM_LIST 100

using namespace utils;


// CApiDumpDlg dialog
class CApiDumpDlg 
    : public CDialog
    , public CBaseObject
{
// Construction
public:
	CApiDumpDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_APIDUMP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();

private:

    bool FindNextRecord(SRecord *pRecord);
    void FormatNextRecord(int);

private:

    CEdit m_editMacroName;
    char *m_pszMacroBuffer;
    char *m_pszEndRecord;

    char *m_pszCurrentRecord;

};
