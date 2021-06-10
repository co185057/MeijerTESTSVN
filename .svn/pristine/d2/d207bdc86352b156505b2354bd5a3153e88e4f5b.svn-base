// TestRCMDlg.h : header file
//

#pragma once
#include "RCMWrapper.h"
#include "ApiRecorder.h"
#include "afxwin.h"

#define MAX_PARM_LIST 100

class CPSX;

using namespace utils;

#define MAX_PARM_LIST 100


// CTestRCMDlg dialog
class CTestRCMDlg 
    : public CDialog
    , public CBaseObject
{
// Construction
public:
    CTestRCMDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
    enum { IDD = IDD_TESTRCM_DIALOG };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


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
    afx_msg void OnBnClickedButton3();
    afx_msg void OnBnClickedButton4();
    afx_msg void OnBnClickedButton5();
    afx_msg void OnBnClickedButton6();

    DECLARE_EVENTSINK_MAP()
    void OnRequestApproval(LPCTSTR bstrParms);
    void OnRequestSuspend(LPCTSTR bstrParms);
    void OnCancelSuspend(LPCTSTR bstrParms);
    void OnValidateOperator(LPCTSTR bstrParms);
    void OnUnsolicitedData(LPCTSTR bstrParms);
    void OnAssistMode(LPCTSTR bstrParms);
    void OnRemoteConsoleEvent(LPCTSTR bstrParms);

private:

    static BOOL LoadStringCallbackHandler( UINT nId, CString& strReturnedString );

    static void EventHandler(CPSX *pPSX
                           , const CString& strRemoteConnectionName
                           , const CString& strControlName
                           , const CString& strContextName
                           , const CString& strEventName
                           , const COleVariant& vParam );

    void LoadPSX();
    void DisplayNextMacro();
    void FindNextMacro(SRecord *pRecord);
    bool ExecuteNextMacro();

    CPSX *m_pPSX;

    CRemoteConsoleMgrWrapper m_ctrlRCM;

    char *m_pszMacroBuffer;
    char *m_pszEndRecord;

    char *m_pszCurrentRecord;

    CEdit m_editMacroName;

    CString m_sEventResult;

private:
    CButton m_btnNextMacro;
    CButton m_btnTestApi;
    CButton m_btnLoadMacro;
    CButton m_btnBrowse;
public:
    CEdit m_edtMacroText;
    afx_msg void OnBnClickedButton7();
    CButton m_btnTerminate;
    CButton m_btnAutoPlay;
    afx_msg void OnBnClickedButton8();
	CEdit m_edtButtonName;
	afx_msg void OnBnClickedButton9();
    CButton m_btnButtonPress;
    CButton m_btnSkip;
};
