#pragma once

// RemoteConsoleMgrCtrl.h : Declaration of the CRemoteConsoleMgrCtrl ActiveX Control class.


namespace rap 
{
    class CRemoteAPMgr;
}

// CRemoteConsoleMgrCtrl : See RemoteConsoleMgrCtrl.cpp for implementation.

class CRemoteConsoleMgrCtrl 
    : public COleControl
    , public CBaseObject
{
	DECLARE_DYNCREATE(CRemoteConsoleMgrCtrl)

// Constructor
public:
	CRemoteConsoleMgrCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();

// Implementation
protected:
	~CRemoteConsoleMgrCtrl();

    DECLARE_OLECREATE_EX(CRemoteConsoleMgrCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CRemoteConsoleMgrCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CRemoteConsoleMgrCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CRemoteConsoleMgrCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
        eventidOnRemoteConsoleEvent = 7L,        eventidOnAssistMode = 6L,        eventidOnUnsolicitedData = 5L,        eventidOnValidateOperator = 4L,        eventidOnCancelSuspend = 3L,        eventidOnRequestSuspend = 2L,        eventidOnRequestApproval = 1L,        dispidOnInputEvent = 10L,        dispidOnApproval = 9L,        dispidSet = 8L,        dispidGet = 7L,        dispidOnEvent = 6L,        dispidOnControl = 5L,        dispidGetApprovalInfo = 4L,        dispidCanApprove = 3L,        dispidTerminate = 2L,        dispidInitialize = 1L
    };
protected:

    LONG Initialize(LONG lPSXHandle);
    LONG Terminate(void);
    LONG CanApprove(LPCTSTR bstrInParms, LONG* pResult);
    LONG GetApprovalInfo(LPCTSTR bstrInParms, BSTR* pbstrOutParms);
    LONG OnControl(LPCTSTR bstrInParms, BSTR* pbstrOutParms);
    LONG OnEvent(LPCTSTR bstrInParms);
    LONG Get(LPCTSTR bstrInParms, BSTR* pbstrOutParms);
    LONG Set(LPCTSTR bstrInParms);
    LONG OnApproval(LPCTSTR bstrParms);
    LONG OnInputEvent(LPCTSTR bstrInParms);

private:

    rap::CRemoteAPMgr *m_pRCManager;

public:

    void OnRequestApproval(LPCTSTR bstrParms)
    {
        FireEvent(eventidOnRequestApproval, EVENT_PARAM(VTS_BSTR), bstrParms);
    }

    void OnRequestSuspend(LPCTSTR bstrParms)
    {
        FireEvent(eventidOnRequestSuspend, EVENT_PARAM(VTS_BSTR), bstrParms);
    }

    void OnCancelSuspend(LPCTSTR bstrParms)
    {
        FireEvent(eventidOnCancelSuspend, EVENT_PARAM(VTS_BSTR), bstrParms);
    }

    void OnValidateOperator(LPCTSTR bstrParms)
    {
        FireEvent(eventidOnValidateOperator, EVENT_PARAM(VTS_BSTR), bstrParms);
    }

    void OnUnsolicitedData(LPCTSTR bstrParms)
    {
        FireEvent(eventidOnUnsolicitedData, EVENT_PARAM(VTS_BSTR), bstrParms);
    }

    void OnAssistMode(LPCTSTR bstrParms)
    {
        FireEvent(eventidOnAssistMode, EVENT_PARAM(VTS_BSTR), bstrParms);
    }

    void OnRemoteConsoleEvent(LPCTSTR bstrParms)
    {
        FireEvent(eventidOnRemoteConsoleEvent, EVENT_PARAM(VTS_BSTR), bstrParms);
    }
};

