// RemoteConsoleMgrCtrl.cpp : Implementation of the CRemoteConsoleMgrCtrl ActiveX Control class.

#include "stdafx.h"
#include "RemoteAPMgr.h"
#include "RemoteConsoleMgr.h"
#include "RemoteConsoleMgrCtrl.h"
#include "RemoteConsoleMgrPpg.h"
#include ".\remoteconsolemgrctrl.h"

using namespace fastlane;
using namespace rap;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CRemoteConsoleMgrCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CRemoteConsoleMgrCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CRemoteConsoleMgrCtrl, COleControl)
    DISP_FUNCTION_ID(CRemoteConsoleMgrCtrl, "Initialize", dispidInitialize, Initialize, VT_I4, VTS_I4)
    DISP_FUNCTION_ID(CRemoteConsoleMgrCtrl, "Terminate", dispidTerminate, Terminate, VT_I4, VTS_NONE)
    DISP_FUNCTION_ID(CRemoteConsoleMgrCtrl, "CanApprove", dispidCanApprove, CanApprove, VT_I4, VTS_BSTR VTS_PI4)
    DISP_FUNCTION_ID(CRemoteConsoleMgrCtrl, "GetApprovalInfo", dispidGetApprovalInfo, GetApprovalInfo, VT_I4, VTS_BSTR VTS_PBSTR)
    DISP_FUNCTION_ID(CRemoteConsoleMgrCtrl, "OnControl", dispidOnControl, OnControl, VT_I4, VTS_BSTR VTS_PBSTR)
    DISP_FUNCTION_ID(CRemoteConsoleMgrCtrl, "OnEvent", dispidOnEvent, OnEvent, VT_I4, VTS_BSTR)
    DISP_FUNCTION_ID(CRemoteConsoleMgrCtrl, "Get", dispidGet, Get, VT_I4, VTS_BSTR VTS_PBSTR)
    DISP_FUNCTION_ID(CRemoteConsoleMgrCtrl, "Set", dispidSet, Set, VT_I4, VTS_BSTR)
    DISP_FUNCTION_ID(CRemoteConsoleMgrCtrl, "OnApproval", dispidOnApproval, OnApproval, VT_I4, VTS_BSTR)
    DISP_FUNCTION_ID(CRemoteConsoleMgrCtrl, "OnInputEvent", dispidOnInputEvent, OnInputEvent, VT_I4, VTS_BSTR)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CRemoteConsoleMgrCtrl, COleControl)
    EVENT_CUSTOM_ID("OnRequestApproval", eventidOnRequestApproval, OnRequestApproval, VTS_BSTR)
    EVENT_CUSTOM_ID("OnRequestSuspend", eventidOnRequestSuspend, OnRequestSuspend, VTS_BSTR)
    EVENT_CUSTOM_ID("OnCancelSuspend", eventidOnCancelSuspend, OnCancelSuspend, VTS_BSTR)
    EVENT_CUSTOM_ID("OnValidateOperator", eventidOnValidateOperator, OnValidateOperator, VTS_BSTR)
    EVENT_CUSTOM_ID("OnUnsolicitedData", eventidOnUnsolicitedData, OnUnsolicitedData, VTS_BSTR)
    EVENT_CUSTOM_ID("OnAssistMode", eventidOnAssistMode, OnAssistMode, VTS_BSTR)
    EVENT_CUSTOM_ID("OnRemoteConsoleEvent", eventidOnRemoteConsoleEvent, OnRemoteConsoleEvent, VTS_BSTR)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CRemoteConsoleMgrCtrl, 1)
	PROPPAGEID(CRemoteConsoleMgrPropPage::guid)
END_PROPPAGEIDS(CRemoteConsoleMgrCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CRemoteConsoleMgrCtrl, "REMOTECONSOLEMGR.RemoteConsoleMgrCtrl.1",
	0xb9897549, 0xf1b, 0x4f36, 0xb3, 0x74, 0x64, 0x58, 0x43, 0xc0, 0xb5, 0x83)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CRemoteConsoleMgrCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DRemoteConsoleMgr =
		{ 0xCB25E7ED, 0x88C9, 0x41DF, { 0x9F, 0x5C, 0x3D, 0x76, 0x27, 0xC5, 0x20, 0x4 } };
const IID BASED_CODE IID_DRemoteConsoleMgrEvents =
		{ 0x956F1022, 0x3060, 0x485E, { 0x80, 0xD9, 0x1D, 0x40, 0xDB, 0x44, 0x48, 0x1 } };



// Control type information

static const DWORD BASED_CODE _dwRemoteConsoleMgrOleMisc =
	OLEMISC_INVISIBLEATRUNTIME |
    OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CRemoteConsoleMgrCtrl, IDS_REMOTECONSOLEMGR, _dwRemoteConsoleMgrOleMisc)



// CRemoteConsoleMgrCtrl::CRemoteConsoleMgrCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CRemoteConsoleMgrCtrl

BOOL CRemoteConsoleMgrCtrl::CRemoteConsoleMgrCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_REMOTECONSOLEMGR,
			IDB_REMOTECONSOLEMGR,
			afxRegApartmentThreading,
			_dwRemoteConsoleMgrOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CRemoteConsoleMgrCtrl::CRemoteConsoleMgrCtrl - Constructor

CRemoteConsoleMgrCtrl::CRemoteConsoleMgrCtrl()
    : m_pRCManager(CSingleton<CRemoteAPMgr>::instance())
{
	InitializeIIDs(&IID_DRemoteConsoleMgr, &IID_DRemoteConsoleMgrEvents);
	// TODO: Initialize your control's instance data here.
    SetTraceObject(g_hTrace);

    m_pRCManager->SetTraceObject(g_hTrace);
    m_pRCManager->SetControl(this);
}



// CRemoteConsoleMgrCtrl::~CRemoteConsoleMgrCtrl - Destructor

CRemoteConsoleMgrCtrl::~CRemoteConsoleMgrCtrl()
{
	// TODO: Cleanup your control's instance data here.
    IMODTRACE_DEVELOP();

	// TODO: Cleanup your control's instance data here.
    try
    {
        CSingleton<CRemoteAPMgr>::Release();
    }
    catch (...)
        { ITRACE_ERROR("Exception caught in " __FUNCTION__); }

    m_pRCManager = NULL;
}



// CRemoteConsoleMgrCtrl::OnDraw - Drawing function

void CRemoteConsoleMgrCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& /* rcInvalid */)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CRemoteConsoleMgrCtrl::DoPropExchange - Persistence support

void CRemoteConsoleMgrCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CRemoteConsoleMgrCtrl::GetControlFlags -
// Flags to customize MFC's implementation of ActiveX controls.
//
// For information on using these flags, please see MFC technical note
// #nnn, "Optimizing an ActiveX Control".
DWORD CRemoteConsoleMgrCtrl::GetControlFlags()
{
	DWORD dwFlags = COleControl::GetControlFlags();


	// The control can activate without creating a window.
	// TODO: when writing the control's message handlers, avoid using
	//		the m_hWnd member variable without first checking that its
	//		value is non-NULL.
	dwFlags |= windowlessActivate;
	return dwFlags;
}



// CRemoteConsoleMgrCtrl::OnResetState - Reset control to default state

void CRemoteConsoleMgrCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CRemoteConsoleMgrCtrl message handlers

LONG CRemoteConsoleMgrCtrl::Initialize(LONG lPSXHandle)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_pRCManager->Initialize(lPSXHandle, g_hTrace);
}

LONG CRemoteConsoleMgrCtrl::Terminate(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_pRCManager->Terminate();
}

LONG CRemoteConsoleMgrCtrl::CanApprove(LPCTSTR bstrInParms, LONG* pResult)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_pRCManager->CanApprove(bstrInParms, pResult);
}

LONG CRemoteConsoleMgrCtrl::GetApprovalInfo(LPCTSTR bstrInParms, BSTR* pbstrOutParms)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_pRCManager->GetApprovalInfo(bstrInParms, pbstrOutParms);
}

LONG CRemoteConsoleMgrCtrl::OnControl(LPCTSTR bstrInParms, BSTR* pbstrOutParms)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_pRCManager->OnControl(bstrInParms, pbstrOutParms);
}

LONG CRemoteConsoleMgrCtrl::OnEvent(LPCTSTR bstrInParms)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_pRCManager->OnEvent(bstrInParms);
}

LONG CRemoteConsoleMgrCtrl::Get(LPCTSTR bstrInParms, BSTR* pbstrOutParms)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_pRCManager->Get(bstrInParms, pbstrOutParms);
}


LONG CRemoteConsoleMgrCtrl::Set(LPCTSTR bstrInParms)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_pRCManager->Set(bstrInParms);
}


LONG CRemoteConsoleMgrCtrl::OnApproval(LPCTSTR bstrInParms)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_pRCManager->OnApproval(bstrInParms);
}

LONG CRemoteConsoleMgrCtrl::OnInputEvent(LPCTSTR bstrInParms)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return m_pRCManager->OnInputEvent(bstrInParms);
}
