// RAInterface.h: interface for the RAInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAINTERFACE_H__77CA0A2A_B941_4904_86C0_DD33507F556D__INCLUDED_)
#define AFX_RAINTERFACE_H__77CA0A2A_B941_4904_86C0_DD33507F556D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RCMgrWrapper.h"
#include "KeyValueHandler.h"

#define IDC_RCMANAGERCTRL1    2069


typedef enum EEventFlags 
{
    NO_RAP_DATA_NEEDED = 0,
    RAP_DATA_NEEDED,
    RESET_RAP_DATA_NEEDED,
    RAP_GENERIC_DELAYED
};


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
RAInterface : public CWnd  
{
public:


    enum EConnectionState
    {
        DISCONNECTED = 0,
        CONNECTED,
        ACCEPTED
    };


    class RAEventParms
          : public CKeyValueHandler<RAEventParms>
    {
          DECLARE_KV_HANDLER_MAP();

          DECLARE_KV_HANDLER(UserId);
          DECLARE_KV_HANDLER(UserPwd);
          DECLARE_KV_HANDLER(RemoteConnection);
          DECLARE_KV_HANDLER(ApprovalCode);
          DECLARE_KV_HANDLER(Cookie);
          DECLARE_KV_HANDLER(AuxData);
          DECLARE_KV_HANDLER(Category);
          DECLARE_KV_HANDLER(Type);
          DECLARE_KV_HANDLER(View);
          DECLARE_KV_HANDLER(PendingApprovals);


    public:

        void Clear();

        void TraceThis(const _TCHAR *pPairs) throw();
        CString m_csUserId;
        CString m_csRemoteConnection;
        CString m_csUserPwd;
        CString m_csData;
        CString m_csCategory;
        CString m_csType;

        DWORD m_hApproval;
        int m_approvalCode;
        int m_nCookie;
        int m_nView;
        int m_nPendingApprovals;

        bool m_bDataAvailable;

        SCOTDO_APPROVAL_INFO m_approvalInfo;

    };

    DECLARE_KV_HANDLER_CLASS_BEGIN(RAAMEventParms)
        DECLARE_KV_CSTRING(Operation)
        DECLARE_KV_CSTRING(MachineName)
        DECLARE_KV_CSTRING(ContextName)
        DECLARE_KV_CSTRING(RemoteConnectionName)
    DECLARE_KV_HANDLER_CLASS_END

    DECLARE_KV_HANDLER_CLASS_BEGIN(RCEventParms)
        DECLARE_KV_CSTRING(Operation)
        DECLARE_KV_CSTRING(ClientName)
        DECLARE_KV_INT(Cookie)
        DECLARE_KV_INT(View)
        DECLARE_KV_CSTRING(Type)
    DECLARE_KV_HANDLER_CLASS_END


public:
    RAInterface();
    virtual ~RAInterface();

    int  SecMgrCreated;
   
    friend class CMainFrame;
    _TCHAR cstr[255];


// Attributes
public:

// OCX Operations
public:

    virtual LONG Initialize(LONG lPSXHandle);
    virtual LONG Terminate(void);
    virtual LONG CanApprove(LPCTSTR bstrInParms, LONG* pResult);
    virtual LONG GetApprovalInfo(LPCTSTR bstrInParms, BSTR* pbstrOutParms);
    virtual LONG OnApproval(LPCTSTR bstrInParms);
    virtual LONG OnControl(LPCTSTR bstrInParms, BSTR* pbstrOutParms);
    virtual LONG OnEvent(LPCTSTR bstrInParms, EEventFlags eFlags = NO_RAP_DATA_NEEDED);
    virtual LONG Get(LPCTSTR bstrInParms, BSTR* pbstrOutParms);
    virtual LONG Set(LPCTSTR bstrInParms);
    virtual LONG OnInputEvent(LPCTSTR bstrInParms);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(SecMgrInterface)
    //}}AFX_VIRTUAL

// Operations handled internally
public:
    
    bool IsRAPDataAvailable() const;
    CString GetRAPData( void ) const;
    CString GetCurrentApprovalCategory() const;
    void GetApprovalInfo( SCOTDO_APPROVAL_INFO* pInfo ) const;
    bool IsRemoteMode() const;

    void AcceptConnection(const CString &, const CString &);
    HRESULT ScannerData(LPCTSTR sParms);
    HRESULT BiometricData(LPCTSTR sParms); // RFQ 7477
    void DisconnectFromServer();

    LONG GetRAPDataNeededPendingCount() const;

    LONG GetRAPGenericDelayedPendingCount() const;
    void ResetRAPGenericDelayedPendingCount();
    void DecrementRAPGenericDelayedPendingCount();

    CString GetRemoteConnectionName() const;

  // Generated message map functions
protected:
    //{{AFX_MSG(RAInterface)
    afx_msg void OnRequestApproval(LPCTSTR sParms);
    afx_msg void OnRequestSuspend(LPCTSTR sParms);
    afx_msg void OnCancelSuspend(LPCTSTR sParms);
    afx_msg void OnValidateOperator(LPCTSTR sParms);
    afx_msg void OnUnsolicitedData(LPCTSTR sParms);
    afx_msg void OnAssistMode(LPCTSTR sParms);
    afx_msg void OnRemoteConsoleEvent(LPCTSTR sParms);


    DECLARE_EVENTSINK_MAP()
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:

    CRemoteConsoleMgrWrapper m_RCMgr;

    bool m_bRCMgrCreated;

    RAEventParms m_RequestApprovalParms;

    RAAMEventParms m_AssistModeParms;

    EConnectionState m_eRemoteMode;

    long m_nRAPDataPendingCount;
    long m_nRAPGenericDelayedPendingCount;
	CString MaskedParam(const CString& csParam);
};

#endif // !defined(AFX_RAINTERFACE_H__77CA0A2A_B941_4904_86C0_DD33507F556D__INCLUDED_)
