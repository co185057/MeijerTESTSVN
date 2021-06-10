#include "stdafx.h"
#include <winsock.h>                    // Winsock header file
#include <fcntl.h>
#include <stdio.h>
#include <math.h> // RFQ 7477
#include "RAProcedures.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"
#include "RAPKeyValue.h"
#include "LaneStateData.h"
#ifndef _CPPUNIT
#include "RAInterface.h"
#include "MPProcedures.h"
#include "ErrorObject.h"
#include "Common.h"
#include "qping.h"
#include "SCOTMsg.h"
#include "SCOTRAEventInfo.h"
#include "PSEvt.h"
#include "RAPKeyvalue.h"
#endif //CPPUNIT
// This is a big nono - we need a cleaner way to reference the TBCMD custom defines
#include "..\include\TransbrokerSSF.h" // RFQ 7477
#include "Base64EncodeDecode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMP_ID ID_RA               // base state component
#define T_ID    _T("RABase")

#ifndef _CPPUNIT
#include "MethodTrace.h"

#define csRAP _T("RAP")


DLLEXPORT extern MPProcedures mp;


  //included for TAR 381428
  struct RapOperator {
      CString csRapName;
      CString csRapOPID;
      bool bRapValidHeadCashier; //SR819
  };

  vector<RapOperator> csRapOperatorIDs;
  //end TAR 381428

#endif //CPPUNIt

long RAProcedures::m_lInterventionCount = 0;

#ifndef _CPPUNIT
RAProcedures::RAProcedures()
    : m_bDoRCMgrInit(true)
    , m_bRCMgrICreated(false)
    , m_bRCMgrPCreated(false)
    , m_nApprovedCount( 0 )
    , m_nRAPPreviousView(DATANEEDED_NOVIEW)
    , m_nRAPCurrentView(DATANEEDED_NOVIEW)
    , m_bExclusiveAccessGranted(false)
	, m_bIsRemoteApproval(false) // RFQ 7477
{
	m_csStateName = EMPTY_STRING;
}
#else
RAProcedures::RAProcedures()
{
}
#endif //_CPPUNIT


RAProcedures::~RAProcedures()
{
}

#ifndef _CPPUNIT
BEGIN_MESSAGE_MAP(RAProcedures, CWnd)
//{{AFX_MSG_MAP(RAProcedures)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


long RAProcedures::Initialize(CWnd* pWnd)
{
    LONG rc = 0;

    trace(L4, _T("+Initialize"));
    if ( m_bRCMgrICreated == false )
    {
        if( m_bRCMgrPCreated == false )
        {
            if(pWnd == NULL)
            {
                if( !(rc = Create( NULL, NULL, WS_DISABLED, CRect(0,0,0,0), AfxGetMainWnd(), IDW_RCMGRPWINDOW )))
                {
                    ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_RC_MGR_PROCEDURE_MAIN_WINDOW_FAILED);
                }
            }
            else
            {
                if( !(rc = Create( NULL, NULL, WS_DISABLED, CRect(0,0,0,0), pWnd, IDW_RCMGRPWINDOW )))
                {
                    ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_RC_MGR_PROCEDURE_WINDOW_FAILED);
                }
            }
            m_bRCMgrPCreated = true;
        }
        if( !(rc = m_rcmi.Create( NULL, NULL, WS_DISABLED, CRect(0,0,0,0), this, IDW_RCMGRIWINDOW )))
        {
            ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_RC_MGR_INTERFACE_WINDOW_FAILED );
        }
        m_bRCMgrICreated = true;
    }

    if( m_bDoRCMgrInit )
    {
        rc = m_rcmi.Initialize( ps.GetPSXObject()->GetPSXHandle() );
    }
    trace(L4, _T("-Initialize"));
    trace(L0, _T("-")_T(__TIMESTAMP__));

    if ((rc == S_OK) || (rc == S_FALSE))
    {
        (*CPSXInterface::instance()).Register(&EventHandler);

        approvalMap[KEY_APPROVAL_TYPE_OTHER]                    =  RA_CAT_OTHER;
        approvalMap[KEY_APPROVAL_TYPE_UNKNOWN]                  =  RA_CAT_UNKNOWN;
        //approvalMap[] =  RA_CAT_RECEIPT;
        approvalMap[KEY_APPROVAL_TYPE_DEVICE_ERROR]             =  RA_CAT_DEVERR;
        approvalMap[KEY_APPROVAL_TYPE_SECURITY]                 =  RA_CAT_SECURITY;
        approvalMap[KEY_APPROVAL_TYPE_VOID]                     =  RA_CAT_VOID;
        approvalMap[KEY_APPROVAL_TYPE_COLLECTION]               =  RA_CAT_COLLECTION;
        approvalMap[KEY_APPROVAL_TYPE_READY_TO_COLLECT]         =  RA_CAT_COLLECTION;
        approvalMap[KEY_APPROVAL_TYPE_AGE_RESTRICTION]          =  RA_CAT_AGERESTRICT;
        approvalMap[KEY_APPROVAL_TYPE_EFT]                      =  RA_CAT_EFT;
        //approvalMap[] =  RA_CAT_AUDACTION;
        approvalMap[KEY_APPROVAL_TYPE_OUTOFSERVICE]             =  RA_CAT_OUTOFSERVICE;
        //approvalMap[] =  RA_CAT_MEDIASHORTAGE;
        approvalMap[KEY_APPROVAL_TYPE_PRICE_REQUIRED]           =  RA_CAT_PRICENEEDED;
        approvalMap[KEY_APPROVAL_TYPE_UNKNOWN_ITEM]             =  RA_CAT_UNKNOWNITEM;
        approvalMap[KEY_APPROVAL_TYPE_RECALLED_ITEM]            =  RA_CAT_RECALLEDITEM;
        approvalMap[KEY_APPROVAL_TYPE_TIME_RESTRICTED_ITEM]     =  RA_CAT_TIMERESTRICT;
        approvalMap[KEY_APPROVAL_TYPE_SUSPEND]                  =  RA_CAT_SUSPEND;
        approvalMap[KEY_APPROVAL_TYPE_CALL_FOR_HELP]            =  RA_CAT_CALLFORHELP;
        //approvalMap[] =  RA_CAT_NEGATIVEDEPT;
        approvalMap[KEY_APPROVAL_TYPE_VISUAL_VALIDATION]        =  RA_CAT_VISUALVERIFY;
        approvalMap[KEY_APPROVAL_TYPE_SIG_CAP]                  =  RA_CAT_SIGCAP;
        approvalMap[KEY_APPROVAL_TYPE_OTHER_PAYMENT]            =  RA_CAT_OTHERPAYMENT;
        approvalMap[KEY_APPROVAL_TYPE_QUANTITY_RESTRICTED_ITEM] =  RA_CAT_QTYRESTRICT;
        approvalMap[KEY_APPROVAL_TYPE_DATANEEDED]               =  RA_CAT_DATANEEDED;
        approvalMap[KEY_APPROVAL_TYPE_UNDEACTIVATED_ITEM]       =  RA_CAT_EASITEMACTIVE;
        approvalMap[KEY_APPROVAL_TYPE_STALE]                    =  RA_CAT_STALE;
        approvalMap[KEY_APPROVAL_TYPE_DATA_NEEDED]              =  RA_CAT_DATANEEDED;
        approvalMap[KEY_APPROVAL_TYPE_RESTRICTED_NOT_ALLOWED]   =  RA_CAT_RESTRICTEDNOTALLOWED;
        approvalMap[KEY_APPROVAL_TYPE_COUPON]                   =  RA_CAT_COUPON;          // TAR 400905
        categoryMap[RA_CAT_OTHER]                   =   KEY_APPROVAL_TYPE_OTHER;
        categoryMap[RA_CAT_UNKNOWN]                 =   KEY_APPROVAL_TYPE_UNKNOWN;
        //categoryMap[] =  RA_CAT_RECEIPT;
        categoryMap[RA_CAT_DEVERR]                  =   KEY_APPROVAL_TYPE_DEVICE_ERROR;
        categoryMap[RA_CAT_SECURITY]                =   KEY_APPROVAL_TYPE_SECURITY;
        categoryMap[RA_CAT_VOID]                    =   KEY_APPROVAL_TYPE_VOID;
        categoryMap[RA_CAT_COLLECTION]              =   KEY_APPROVAL_TYPE_COLLECTION;
        categoryMap[RA_CAT_COLLECTION]              =   KEY_APPROVAL_TYPE_READY_TO_COLLECT;
        categoryMap[RA_CAT_AGERESTRICT]             =   KEY_APPROVAL_TYPE_AGE_RESTRICTION;
        categoryMap[RA_CAT_EFT]                     =   KEY_APPROVAL_TYPE_EFT;
        //categoryMap[] =  RA_CAT_AUDACTION;
        categoryMap[RA_CAT_OUTOFSERVICE]            =   KEY_APPROVAL_TYPE_OUTOFSERVICE;
        //categoryMap[] =  RA_CAT_MEDIASHORTAGE;
        categoryMap[RA_CAT_PRICENEEDED]             =   KEY_APPROVAL_TYPE_PRICE_REQUIRED;
        categoryMap[RA_CAT_UNKNOWNITEM]             =   KEY_APPROVAL_TYPE_UNKNOWN_ITEM;
        categoryMap[RA_CAT_RECALLEDITEM]            =   KEY_APPROVAL_TYPE_RECALLED_ITEM;
        categoryMap[RA_CAT_TIMERESTRICT]            =   KEY_APPROVAL_TYPE_TIME_RESTRICTED_ITEM;
        categoryMap[RA_CAT_SUSPEND]                 =   KEY_APPROVAL_TYPE_SUSPEND;
        categoryMap[RA_CAT_CALLFORHELP]             =   KEY_APPROVAL_TYPE_CALL_FOR_HELP;
        //categoryMap[] =  RA_CAT_NEGATIVEDEPT;
        categoryMap[RA_CAT_VISUALVERIFY]            =   KEY_APPROVAL_TYPE_VISUAL_VALIDATION;
        categoryMap[RA_CAT_SIGCAP]                  =   KEY_APPROVAL_TYPE_SIG_CAP;
        categoryMap[RA_CAT_OTHERPAYMENT]            =   KEY_APPROVAL_TYPE_OTHER_PAYMENT;
        categoryMap[RA_CAT_QTYRESTRICT]             =   KEY_APPROVAL_TYPE_QUANTITY_RESTRICTED_ITEM;
        categoryMap[RA_CAT_DATANEEDED]              =   KEY_APPROVAL_TYPE_DATANEEDED;
        categoryMap[RA_CAT_EASITEMACTIVE]           =   KEY_APPROVAL_TYPE_UNDEACTIVATED_ITEM;
        categoryMap[RA_CAT_STALE]                   =   KEY_APPROVAL_TYPE_STALE;
        categoryMap[RA_CAT_DATANEEDED]              =   KEY_APPROVAL_TYPE_DATA_NEEDED;
        categoryMap[RA_CAT_RESTRICTEDNOTALLOWED]    =   KEY_APPROVAL_TYPE_RESTRICTED_NOT_ALLOWED;
		approvalMap[KEY_APPROVAL_TYPE_HOPPER_SUBSTITUTION]        =  RA_CAT_HOPPERSUBSTITUTION;
		approvalMap[KEY_APPROVAL_TYPE_SUSPENSIONDECLINED]		=  RA_CAT_SUSPENSIONDECLINED;		// TAR 352409
		categoryMap[RA_CAT_SUSPENSIONDECLINED]      =	KEY_APPROVAL_TYPE_SUSPENSIONDECLINED;	// TAR 352409
        categoryMap[RA_CAT_COUPON]                  =   KEY_APPROVAL_TYPE_COUPON;               // TAR400905  
		approvalMap[KEY_APPROVAL_TYPE_NOSALE]       =   RA_CAT_NOSALE;  //sscoadk-321
		categoryMap[RA_CAT_NOSALE]                  =   KEY_APPROVAL_TYPE_NOSALE; //sscoadk-321 
        categoryMap[RA_CAT_GENERICDELAYED]          =   KEY_APPROVAL_TYPE_GENERIC_DELAYED;
        approvalMap[KEY_APPROVAL_TYPE_GENERIC_DELAYED] = RA_CAT_GENERICDELAYED;
        
    }
    
    //(+)SR969 - View CM at the RAP
    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_INITIALIZE );

    if (!co.fLocaleCashManagementScreen)
    {
        //if CashManagementScreen is disabled, don't allow viewing of CM at the RAP
        trace(L6,_T("CashManagementScreen = N. Don't allow viewing of CM Screen at the RAP, ignoring DisplayCashManagementAtRAP option."));         
        CKeyValue<bool>::Put(parmList, KEY_EV_VIEW_CM_AT_RAP, false ); 
    }
    else
    {
        //else check for the DisplayCashManagementAtRAP option
        trace(L6,_T("CashManagementScreen = Y. Viewing of CM Screen at the RAP depends on DisplayCashManagementAtRAP. DisplayCashManagementAtRAP = %d."),co.fLocaleDisplayCashManagementAtRAP);
        CKeyValue<bool>::Put(parmList, KEY_EV_VIEW_CM_AT_RAP, co.fLocaleDisplayCashManagementAtRAP ); 
    }
           
    _bstr_t bstrParmList((const _TCHAR *)parmList); 
    BSTR bstrResult = NULL;

    m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);
    //(-)SR969 - View CM at the RAP  

    return rc;
}


long RAProcedures::UnInitialize(void)
{
    bool rc;
    trace(L2, _T("+UnInitialize"));

    (*CPSXInterface::instance()).Unregister(&EventHandler);
    if( rc = (m_rcmi.Terminate() == S_OK) )
    {
        trace(L2, _T("After UnInitialize -- SecMgri:UnInitSecMgr"));
        ScotError(FATAL,SM_CAT_STOP_ABORT, SCOTAPP_RC_MGR_PROCEDURE_UNINIT);
    }
    if( !DestroyWindow() )                    // destroy this window
    {
        trace(L2, _T("After UnInitialize -- DestroyWindow"));
        ScotError(FATAL,SM_CAT_STOP_ABORT, SCOTAPP_RC_MGR_PROCEDURE_WINDOW_DESTROY);
    }

    m_bRCMgrPCreated = false;
    m_bRCMgrICreated = false;
    trace(L2, _T("-UnInitialize"));
    return rc;
}


long RAProcedures::ReloadOptions( )
{
    TRACE_METHOD(_T("RAProcedures::ReloadOptions"));

    if (!m_bRCMgrPCreated)
        return E_FAIL;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_RELOAD_OPTIONS );
    
    //(+)SR969 - View CM at the RAP 
    if (!co.fLocaleCashManagementScreen)
    {
        //if CashManagementScreen is disabled, don't allow viewing of CM at the RAP
        trace(L6,_T("CashManagementScreen = N. Don't allow viewing of CM Screen at the RAP, ignoring DisplayCashManagementAtRAP option."));         
        CKeyValue<bool>::Put(parmList, KEY_EV_VIEW_CM_AT_RAP, false ); //SR803
    }
    else
    {
        //else check for the DisplayCashManagementAtRAP option
        trace(L6,_T("CashManagementScreen = Y. Viewing of CM Screen at the RAP depends on DisplayCashManagementAtRAP. DisplayCashManagementAtRAP = %d."),co.fLocaleDisplayCashManagementAtRAP);
        CKeyValue<bool>::Put(parmList, KEY_EV_VIEW_CM_AT_RAP, co.fLocaleDisplayCashManagementAtRAP ); //SR803
    }          
    //(-)SR969 - View CM at the RAP
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    HRESULT hr = m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);

    return hr;
}


bool RAProcedures::EventHandler( CPSX *pPSX
                               , PPSXEVENT pEvent )
{
    TRACE_METHOD(_T("RAProcedures::EventHandler"));

    bool bHandled = false;

    switch (pEvent->nEvent)
    {
        case PS_SWITCHCONTEXTOTHER :
            return ra.SwitchContext(pEvent);
        case PS_RAPSIGNON :
            return ra.SignOn(pEvent);
        case PS_RAPSIGNOFF :
            return ra.SignOff(pEvent);
        case PS_RAPSCANDATA :
            return ra.ScannerData(pEvent);
        case PS_REMOTEDATA :
            return ra.RemoteData(pEvent);
        case PS_CONNECTREMOTE :
            return ra.ConnectToRemote(pEvent);
        case PS_REMOTE_DISCONNECTED :
            return ra.DisconnectFromRemote(pEvent);
        case PS_ADDOPERATOR :
            return ra.AddOperator(pEvent);
        case PS_ADDOPERATOR_AND_CONNECT :
            if ( ra.ConnectToRemote(pEvent))
            {
                return ra.AddOperator(pEvent);
            }
            else
            {
                return false;
            }
   // RFQ 7477
        case PS_RAPBIOMET :
            return ra.BiometricData(pEvent);
   
		case PS_UKEVT :
            return false;
		
        case PS_RAPCHECKIN:         //POS81634
            return ra.CheckIn(pEvent);               
        case PS_RAPCHECKOUT:        //POS81634
            return ra.CheckOut(pEvent); 
   // RFQ 7477
        default :
            return ra.ForwardInput(pEvent);
    }

    return bHandled;
}


bool RAProcedures::SwitchContext( PPSXEVENT pEvent )
{
    TRACE_METHOD(_T("RAProcedures::SwitchContext"));

    if (!m_bRCMgrPCreated)
        return false;

    CString strRemoteConnectionName = pEvent->strRemoteConnectionName;

    if (strRemoteConnectionName.IsEmpty())
    {
        pEvent->wParam = false;  //RFQ 986
        return SwitchLaneContext( pEvent );
    }

    if (m_connections.find(strRemoteConnectionName) == m_connections.end())
        return false;

    trace(L6, _T("SwitchContext Event:  %s, %s, %s, %s  ")
            , pEvent->strRemoteConnectionName
            , pEvent->strControlName
            , pEvent->strContextName
            , pEvent->strEventName);

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_SWITCH_CONTEXT);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_CONTEXT, (const _TCHAR *)pEvent->strContextName);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_REMOTE_CONNECTION_NAME, (const _TCHAR *)pEvent->strRemoteConnectionName);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    m_rcmi.OnEvent(bstrParmList);

    //RFQ 986
    if (ra.IsRemoteMode() &&
        (strRemoteConnectionName == GetRemoteConnectionName()))
    {
        pEvent->wParam = true;
        return false;   //only pass on remote switch context if in remote mode
    }
    else
    {
        return true;
    }
    //RFQ 986
}

bool RAProcedures::SwitchLaneContext( PPSXEVENT pEvent )
{
    TRACE_METHOD(_T("RAProcedures::SwitchLaneContext"));

    trace(L6, _T("SwitchLaneContext Event:  %s, %s  ")
            , pEvent->strContextName
            , pEvent->strEventName);

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_SWITCH_LANE_CONTEXT);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_CONTEXT, (const _TCHAR *)pEvent->strContextName);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    m_rcmi.OnEvent(bstrParmList);

    // Handle the event, but we need to pass it on to let SCOTAPP also handle it.
    return false;
}


bool RAProcedures::ConnectToRemote( PPSXEVENT pEvent )
{
    TRACE_METHOD(_T("RAProcedures::ConnectToRemote"));

    if (!m_bRCMgrPCreated)
        return false;

    bool bHandled = true;

    CString strRemoteConnectionName = pEvent->strRemoteConnectionName;
    //if (m_connections.find(strRemoteConnectionName) != m_connections.end())
    //    return bHandled;

    CString vParam = (const _TCHAR *)(_bstr_t)pEvent->vParam;
    ConnectParms parms;

    parms.ParseKeys((const _TCHAR *)vParam);

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_CONNECT );
    CKeyValue<bool>::Put(parmList, KEY_IE_REMOTE_HANDHELD, (parms.GetHandheldRAP() != 0));
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_REMOTE_START_CONTEXT, parms.GetStartContext());
    CKeyValue<bool>::Put(parmList, KEY_IE_REMOTE_RAPLITE, (parms.GetRAPLite() != 0));
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_REMOTE_DEVICE_TYPE, parms.GetDeviceType());
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_REMOTE_CONNECTION_NAME, (const _TCHAR *)pEvent->strRemoteConnectionName);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    HRESULT hr = m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);

    bHandled = (hr == S_OK);

    if (bHandled)
        m_connections.insert(strRemoteConnectionName);

    return true;
}


BEGIN_KV_HANDLER_CLASS(RAProcedures::ConnectParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::ConnectParms, _T("HandheldRAP"), HandheldRAP)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::ConnectParms, _T("RapSingleSize"), StartContext)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::ConnectParms, _T("RAPLite"), RAPLite)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::ConnectParms, _T("DeviceType"), DeviceType)
END_KV_HANDLER_CLASS()
DEFINE_KV_LONG(RAProcedures::ConnectParms, HandheldRAP)
DEFINE_KV_CSTRING(RAProcedures::ConnectParms, StartContext)
DEFINE_KV_LONG(RAProcedures::ConnectParms, RAPLite)
DEFINE_KV_CSTRING(RAProcedures::ConnectParms, DeviceType)

void RAProcedures::ConnectParms::Clear()
{
    m_HandheldRAP = 0;
    m_StartContext.Empty();
    m_RAPLite = 0;
    m_DeviceType.Empty();
}


bool RAProcedures::DisconnectFromRemote( PPSXEVENT pEvent )
{
    TRACE_METHOD(_T("RAProcedures::DisconnectFromRemote"));

    if (!m_bRCMgrPCreated)
        return false;

    bool bHandled = true;

    CString strRemoteConnectionName = pEvent->strRemoteConnectionName;
    if (m_connections.find(strRemoteConnectionName) == m_connections.end())
        return bHandled;

    //update operator IDs
    addOperatorID(strRemoteConnectionName, _T(""));
    addRAPValidHeadCashier(strRemoteConnectionName, false);

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_DISCONNECT );
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_REMOTE_CONNECTION_NAME, (const _TCHAR *)pEvent->strRemoteConnectionName);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    HRESULT hr = m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);

    bHandled = (hr == S_OK);

    m_connections.erase(strRemoteConnectionName);

    return true;
}


void RAProcedures::DisconnectFromServer( )
{
    TRACE_METHOD(_T("RAProcedures::DisconnectFromServer"));

    if (!m_bRCMgrPCreated)
        return;

    m_rcmi.DisconnectFromServer( );

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_DISCONNECT_SERVER );
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    HRESULT hr = m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);
}


bool RAProcedures::AddOperator( PPSXEVENT pEvent )
{
    TRACE_METHOD(_T("RAProcedures::AddOperator"));
    if (!m_bRCMgrPCreated)
        return false;

    CString vParam = (const _TCHAR *)(_bstr_t)pEvent->vParam;
    SignonParms parms;
    parms.Clear();

    parms.ParseKeys((const _TCHAR *)vParam);

    CString csUserID = parms.GetUserId();


    CString strRemoteConnectionName = pEvent->strRemoteConnectionName;
    addOperatorID(strRemoteConnectionName, (const TCHAR *)csUserID);
    addRAPValidHeadCashier(strRemoteConnectionName, false);
    return true;

}

BOOL RAProcedures::SignOn( PPSXEVENT pEvent )
{
    TRACE_METHOD(_T("RAProcedures::SignOn"));
    if (!m_bRCMgrPCreated)
        return false;

    CString strRemoteConnectionName = pEvent->strRemoteConnectionName;
	int dLaunchpadPrefixLen = _tcslen(LAUNCHPAD_PREFIX);		// Fix Unicode problem

    if ((m_connections.find(strRemoteConnectionName) == m_connections.end()) &&
        ( pEvent->strRemoteConnectionName.Left(dLaunchpadPrefixLen) != LAUNCHPAD_PREFIX ) )
        return false;

    CString vParam = (const _TCHAR *)(_bstr_t)pEvent->vParam;
    SignonParms parms;
    parms.Clear();

    parms.ParseKeys((const _TCHAR *)vParam);

    if (!parms.GetScannerData().IsEmpty())
    {
        // +TAR453393
        CString csParam = (const _TCHAR*)(_bstr_t)pEvent->vParam;
        csParam += _TCHAR(';');
        
        int nSize = csParam.GetLength();
        bool bReplace = false;

        // Delimter (=) also has some problem, it can also be part of the barcode
        // so we should make sure that when we replace the ';' with '~',we take 
        // account of the '=' position. This position must be relative to the following
        // values:
        // KEY_CT_OPERATION_SCANDATA
        // KEY_CT_OPERATION_SCANTYPE
        // KEY_CT_OPERATION_SCANDATALABEL
        int nIndexScnData;
        int nIndexScnType;
        int nIndexScnDataLabel;
        
        CString csTemp = KEY_CT_OPERATION_SCANDATA;
        nIndexScnData = csParam.Find(KEY_CT_OPERATION_SCANDATA);
        nIndexScnData += csTemp.GetLength();
        
        csTemp = KEY_CT_OPERATION_SCANTYPE;
        nIndexScnType = csParam.Find(KEY_CT_OPERATION_SCANTYPE);
        nIndexScnType += csTemp.GetLength();
        
        csTemp = KEY_CT_OPERATION_SCANDATALABEL;
        nIndexScnDataLabel = csParam.Find(KEY_CT_OPERATION_SCANDATALABEL);
        nIndexScnDataLabel += csTemp.GetLength();
        
        for (int nCntr = nSize - 1; nCntr > 0; nCntr --)
        {
            switch(csParam[nCntr])
            {
            case _TCHAR('~'):
                {
                    trace(L4,_T("WARNING: Embedded Program separator<~> is found in index[%d]"),nCntr);
                    break;
                }
            case _TCHAR(';'):
                {
                    if(!bReplace)
                    {
                        bReplace = true;
                    }
                    else
                    {
                        csParam.SetAt(nCntr,_TCHAR('~'));  
                    }
                    break;
                }
            case _TCHAR('='):
                {
                    if(nCntr == nIndexScnData || nCntr == nIndexScnType || nCntr == nIndexScnDataLabel)
                    {
                        bReplace = false;
                    }
                    break;
                }
            default:
                break;
            }
        }
        csParam = csParam.Left(nSize-1);    // remove the added separator.
        trace(L6,_T("Remote edited data: %s"),csParam);
        // -TAR453393
        ScannerParms parms;
        parms.Clear();
        //parms.ParseKeys((const _TCHAR*)(_bstr_t)pEvent->vParam);
        parms.ParseKeys((LPCTSTR) csParam); // TAR453393

        CString szScannerData = (const TCHAR *)parms.GetScannerData();
        CString csLabelData = (const TCHAR *)parms.GetLabelData();
        //+SSCOP-914
		ConvertOposAsciiToBinary(csLabelData, OPOS_BC_DECIMAL);
        ConvertOposAsciiToBinary(szScannerData, OPOS_BC_DECIMAL);
        //-SSCOP-914
        for(nCntr = 0; nCntr < szScannerData.GetLength(); nCntr ++)
        {
            if(szScannerData[nCntr] == _TCHAR('~')) 
            {
                szScannerData.SetAt(nCntr,_TCHAR(';'));
            }
        }
    
        for(nCntr = 0; nCntr < csLabelData.GetLength(); nCntr ++)
        {
            if(csLabelData[nCntr] == _TCHAR('~')) 
            {
                csLabelData.SetAt(nCntr,_TCHAR(';'));
            }
        }
        //-TAR453393
        switch (parms.GetType())
        {
            case 104 :
                if (szScannerData[0] != _T('F'))
                    szScannerData = _T('F') + szScannerData;
                break;
            default :
                break;
        }

        return ScannerData( _T("cSignonScanData")
                         , szScannerData
                         //, (const TCHAR *)parms.GetLabelData()
                         , csLabelData	// TAR453393
                         , parms.GetType()
                         , strRemoteConnectionName);
    }

	BOOL bValidOperator;
    BOOL bValidHeadCashier; //SR819
	int  lReturn; //392717

    //+SR819
	bValidHeadCashier = tb.ValidateHeadCashier(parms.GetUserId(), parms.GetUserPwd()); 
	
	if(bValidHeadCashier != 1)
	{
		bValidHeadCashier = (BOOL)false; //Assigning 0 just in case the TB returned some none 0 or 1 value
		lReturn = (tb.ValidateOperator(parms.GetUserId(), parms.GetUserPwd()));  //392717
	}   
	else // 1
	{
		bValidHeadCashier = (BOOL)true;
		lReturn = 1;
	}
	//-SR819
	
    if(::TBIsAllowed(TB_FT_SIGNONRESULTS)){
		trace( L6, _T("RAProcedures::SignOn TBIsAllowed"));
		//392717+
		trace(L6, _T("%s"),strRemoteConnectionName); 
		if ((strRemoteConnectionName.Find(LAUNCHPAD_PREFIX) != -1) && (lReturn != 1))
    {
			trace( L6, _T("it is from LaunchPad"));
			lReturn = 0;
		}
		//bValidOperator = (tb.ValidateOperator(parms.GetUserId(), parms.GetUserPwd()));
		trace( L6, _T("lReturn is %d"), lReturn);
		bValidOperator = (BOOL)lReturn;
		//392717-
	
	}
	else{
		//392717+
		//bValidOperator = (BOOL)(bool)(tb.ValidateOperator(parms.GetUserId(), parms.GetUserPwd()));
		
		if (lReturn == 1 )
		{
			bValidOperator = (BOOL)true;
		}
		else {
			bValidOperator = (BOOL)false;
		}
		//392717-
		
	}

	// RFQ 7477
	//SOTF 6802
   CString csEmplid;
   if( TB_SUCCESS_IMMEDIATE ==  mp.sCurr->SendTBCommand(TBCMD_IS_MEIJER_BADGE, rp.GetOperator()) )      
   {
           if( TB_SUCCESS_IMMEDIATE ==  mp.sCurr->SendTBCommand(TBCMD_GET_EMPLID_FROM_BADGE, (const TCHAR *)parms.GetUserId(), &csEmplid) )           
           {
               return SignOn(strRemoteConnectionName, csEmplid, (const TCHAR *)parms.GetUserPwd(), bValidOperator, bValidHeadCashier);
           }
   }
	//SOTF    
	// RFQ 7477
    return SignOn(strRemoteConnectionName, (const TCHAR *)parms.GetUserId(), (const TCHAR *)parms.GetUserPwd(), bValidOperator, bValidHeadCashier); //SR819
}

BOOL RAProcedures::SignOn( const CString &sRemoteConnectionName
                         , const CString &sUserId
                         , const CString &sUserPwd
                         , BOOL bValid
						 , BOOL bRapValidHeadCashier) //SR819
{
    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_SIGNON );
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_REMOTE_CONNECTION_NAME, (LPCTSTR)sRemoteConnectionName);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OP_ID, (LPCTSTR)sUserId);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OP_PASSWORD, (LPCTSTR)sUserPwd, MASKED_ENCODING); // SSCOP-3253
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_AP_AUX_DATA, _T(""));   // POS81634  // RFQ 7477
    CKeyValue<BOOL>::Put(parmList, KEY_CT_OP_VALID, bValid);
	CKeyValue<BOOL>::Put(parmList, KEY_CT_OP_VALID_HEADCASHIER, bRapValidHeadCashier); //SR819
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    HRESULT hr = m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);

    return (hr == S_OK);
}


BEGIN_KV_HANDLER_CLASS(RAProcedures::SignonParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::SignonParms, _T("UserId"), UserId)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::SignonParms, _T("UserPwd"), UserPwd)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::SignonParms, KEY_CT_OPERATION_SCANDATA, ScannerData)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(RAProcedures::SignonParms, UserId)
DEFINE_KV_CSTRING(RAProcedures::SignonParms, UserPwd)
DEFINE_KV_CSTRING(RAProcedures::SignonParms, ScannerData)


void RAProcedures::SignonParms::Clear()
{
    m_UserId.Empty();
    m_UserPwd.Empty();
    m_ScannerData.Empty();
}


bool RAProcedures::SignOff( PPSXEVENT pEvent )
{
    TRACE_METHOD(_T("RAProcedures::SignOff"));

    if (!m_bRCMgrPCreated)
        return false;

    bool bHandled = false;

    CString strRemoteConnectionName = pEvent->strRemoteConnectionName;
    if (m_connections.find(strRemoteConnectionName) == m_connections.end())
        return bHandled;

    //update operator IDs
    addOperatorID(strRemoteConnectionName, _T(""));
    addRAPValidHeadCashier(strRemoteConnectionName, false);

    CString vParam = (const _TCHAR*)(_bstr_t)pEvent->vParam;
    SignonParms parms;

    parms.ParseKeys((const _TCHAR *)vParam);

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_SIGNOFF );
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OP_ID, (const _TCHAR *)parms.GetUserId());
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    HRESULT hr = m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);

    bHandled = (hr == S_OK);

    return bHandled;
}



BEGIN_KV_HANDLER_CLASS(RAProcedures::ScannerParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::ScannerParms, KEY_CT_OPERATION_SCANDATA, ScannerData)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::ScannerParms, KEY_CT_OPERATION_SCANDATALABEL, LabelData)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::ScannerParms, KEY_CT_OPERATION_SCANTYPE, Type)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(RAProcedures::ScannerParms, ScannerData)
DEFINE_KV_CSTRING(RAProcedures::ScannerParms, LabelData)
DEFINE_KV_LONG(RAProcedures::ScannerParms, Type)


void RAProcedures::ScannerParms::Clear()
{
    m_ScannerData.Empty();
    m_LabelData.Empty();
    m_Type = -1;
}


bool RAProcedures::ScannerData( PPSXEVENT pEvent )
{    
	CString strRemoteConnectionName = pEvent->strRemoteConnectionName;
    if (m_connections.find(strRemoteConnectionName) == m_connections.end())
        return false;
 
    ///+ 2D driver's license barcode handling
    //Currently, the PSC handheld scanner driver splits the data once it sees a carriage return character,
    //(because of 0x0D suffix in the registry) so lets assemble it back here.
    static CString cs2DBarcode = _T("");
    static int nBarcodeInstance = 0;
    static bool bIs2DBarcode = false;
  
    CString csVal = pEvent->vParam.bstrVal;
 
    trace( L7, _T("RAProcedures::ScannerData=%s"), csVal);
 
    CString csScannerData;
 
    int nStart = csVal.Find(_T("scanner-data"));
    trace( L6, _T("nStart=%d"), nStart);
    if(nStart > -1)   
    {
         int nIndexEq = csVal.Find(cEQUAL, nStart);
         int nIndexCol = csVal.Find(_T("scanner-data-label"), nIndexEq) - 1; //the semi-colon is -1 before the scanner-data-label
         trace( L6, _T("nIndexEq=%d"), nIndexEq);
         if(nIndexEq > -1)
         {
             csScannerData = csVal.Mid(nIndexEq + 1, (nIndexCol - (nIndexEq +1)));
         }                           
    }
 
    if(nBarcodeInstance > 0 && nBarcodeInstance < 4)
		nBarcodeInstance++;
 
	trace( L7, _T("nBarcodeInstance=%d"), nBarcodeInstance);
    trace( L7, _T("csScannerData=%s"), csScannerData);
    if((csScannerData.Find(_T("P@")) != -1) || (csScannerData.Find(_T("P @")) != -1) || (csScannerData.Find(_T("P")) == 0))
    {
		if(csScannerData.GetLength() > 10) //it means the data is sent in one chunk (the scanner suffix may not 0x0D)
        {
            cs2DBarcode = csScannerData.Right(csScannerData.GetLength() - 1); //remove the P
			trace( L6, _T("cs2DBarcode1=%s"), cs2DBarcode);
			return ScannerData( _T("cScanData"), (const TCHAR *)cs2DBarcode, (const TCHAR *)cs2DBarcode, 201);
        }
		
		bIs2DBarcode = true;
        nBarcodeInstance = 1;
        cs2DBarcode = _T("@\n\x001E\r");
        trace( L6, _T("cs2DBarcode1=%s"), cs2DBarcode);
        return false;
     }
	else if(csScannerData.Find(_T("ANSI")) != -1)
	{
		cs2DBarcode += csScannerData + _T("\x000D"); //restore the 0x0D (carriage return)
        trace( L6, _T("cs2DBarcode2=%s"), cs2DBarcode);
		if(csScannerData.Find(_T("636035")) != -1) //for IL
		{
			bIs2DBarcode = false;
			nBarcodeInstance = 0;
            return ScannerData( _T("cScanData"), (const TCHAR *)cs2DBarcode, (const TCHAR *)cs2DBarcode, 201);
        }
		return false;
     }
     else if(nBarcodeInstance == 3 && bIs2DBarcode) //KY, MI, and WI will fall into here
     {
        cs2DBarcode += csScannerData;
        bIs2DBarcode = false;
        nBarcodeInstance = 0;
		trace( L6, _T("cs2DBarcode3=%s"), cs2DBarcode);
		return ScannerData( _T("cScanData"), (const TCHAR *)cs2DBarcode, (const TCHAR *)cs2DBarcode, 201);
     }
 
	 bIs2DBarcode = false;
     nBarcodeInstance = 0;
	 ///- 2D driver's license barcode handling
	ScannerParms parms;
    parms.Clear();
	parms.ParseKeys((const _TCHAR*)(_bstr_t)pEvent->vParam);

	mp.sCurr->SendTBCommand(TBCMD_HHSCANNER_ACTIVE, 1);
 
    return ScannerData( _T("cScanData")
                      , (const TCHAR *)parms.GetScannerData()
                      , (const TCHAR *)parms.GetLabelData()
                      , parms.GetType());
}


bool RAProcedures::ScannerData( const CString &sDataType
                              , const CString &sScannerData
                              , const CString &sScannerLabelData
                              , long lScannerType
                              , const CString &sAuxData)
{
    bool bHandled = false;

    CString sFormattedScannerData;
    if (sAuxData.IsEmpty())
        sFormattedScannerData.Format(_T("%s:%s~%s~%ld")
                           , (LPCTSTR)sDataType
                           , (LPCTSTR)sScannerData
                           , (LPCTSTR)sScannerLabelData
                           , lScannerType);
    else
        sFormattedScannerData.Format(_T("%s~%s:%s~%s~%ld")
                           , (LPCTSTR)sDataType
                           , (LPCTSTR)sAuxData
                           , (LPCTSTR)sScannerData
                           , (LPCTSTR)sScannerLabelData
                           , lScannerType);

    CKeyValueParmList plist;
    CKeyValue<const _TCHAR*>::Put(plist
                                , KEY_AEV_AUX_DATA
                                , (const _TCHAR *)sFormattedScannerData );

    HRESULT hr = m_rcmi.ScannerData((const _TCHAR *)plist);
    bHandled = (hr == S_OK);

    return bHandled;
}
// RFQ 7477
BEGIN_KV_HANDLER_CLASS(RAProcedures::BiometParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::BiometParms, KEY_CT_OPERATION_BIOMET, BiometData)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::BiometParms, KEY_CT_OPERATION_BIOMETDATA, EncodedData)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(RAProcedures::BiometParms, BiometData)
DEFINE_KV_CSTRING(RAProcedures::BiometParms, EncodedData)

void RAProcedures::BiometParms::Clear()
{
    m_BiometData.Empty();
    m_EncodedData.Empty();
}

bool RAProcedures::BiometricData( PPSXEVENT pEvent )
{
    CString strRemoteConnectionName = pEvent->strRemoteConnectionName;
    if (m_connections.find(strRemoteConnectionName) == m_connections.end())
        return false;

    BiometParms parms;
    parms.Clear();
    parms.ParseKeys((const _TCHAR*)(_bstr_t)pEvent->vParam);

    return BiometricData( _T("cBiometData")
                      , (const TCHAR *)parms.GetBiometData()
                      , (const TCHAR *)parms.GetEncodedData());
}

bool RAProcedures::BiometricData( const CString &sDataType
                              , const CString &sBiometData
                              , const CString &sEncodedData
                              , const CString &sAuxData)
{
    bool bHandled = false;

    CString sFormattedBiometData;
    if (sAuxData.IsEmpty())
        sFormattedBiometData.Format(_T("%s:%s~%s")
                           , (LPCTSTR)sDataType
                           , (LPCTSTR)sBiometData
                           , (LPCTSTR)sEncodedData);
    else
        sFormattedBiometData.Format(_T("%s~%s:%s~%s")
                           , (LPCTSTR)sDataType
                           , (LPCTSTR)sAuxData
                           , (LPCTSTR)sBiometData
                           , (LPCTSTR)sEncodedData);

    CKeyValueParmList plist;
    CKeyValue<const _TCHAR*>::Put(plist
                                , KEY_AEV_AUX_DATA
                                , (const _TCHAR *)sFormattedBiometData );

    HRESULT hr = m_rcmi.BiometricData((const _TCHAR *)plist);
    bHandled = (hr == S_OK);

    return bHandled;
}
// RFQ 7477
bool RAProcedures::RemoteData( PPSXEVENT pEvent )
{
    bool bHandled = false;

    CString strRemoteConnectionName = pEvent->strRemoteConnectionName;
    if (m_connections.find(strRemoteConnectionName) == m_connections.end())
        return bHandled;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_REMOTE_DATA);
    CKeyValue<const _TCHAR*>::Put(parmList
                                , KEY_AEV_AUX_DATA
                                , (const _TCHAR*)(_bstr_t)pEvent->vParam );
    CKeyValue<const _TCHAR*>::Put(parmList
                                , KEY_CT_REMOTE_CONNECTION_NAME
                                , (const _TCHAR *)pEvent->strRemoteConnectionName);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    m_rcmi.OnEvent(bstrParmList);

    return true;
}


bool RAProcedures::ForwardInput( PPSXEVENT pEvent )
{
    if (!m_bRCMgrPCreated)
        return false;

    bool bHandled = false;

    CString strRemoteConnectionName = pEvent->strRemoteConnectionName;
 /* Temporarily comment out checking here till Dan fixes PSX
*/
    if (strRemoteConnectionName == _T(""))
        return bHandled;

    if (m_connections.find(strRemoteConnectionName) == m_connections.end())
        return bHandled;

    CKeyValueParmList parmList;
    CKeyValue<long>::Put(parmList, KEY_IE_LPARAM, pEvent->lParam);
    CKeyValue<long>::Put(parmList, KEY_IE_WPARAM, pEvent->wParam);
    CKeyValue<long>::Put(parmList, KEY_IE_EVENT_ID, pEvent->nEvent);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_CONTROL_NAME, (const _TCHAR *)pEvent->strControlName);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_CONTEXT_NAME, (const _TCHAR *)pEvent->strContextName);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_EVENT_NAME, (const _TCHAR *)pEvent->strEventName);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_REMOTE_CONNECTION_NAME, (const _TCHAR *)pEvent->strRemoteConnectionName);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    HRESULT hr = m_rcmi.OnInputEvent(bstrParmList);
    switch (hr)
    {
        case S_OK :
            bHandled = true;
            break;
        case S_FALSE :
            break;
        default :
            trace( L6, _T("RAProcedures::EventHandler failure to handle event.  hr=%d"), hr);
            bHandled = true;
            break;
    }

    return bHandled;
}


BOOL RAProcedures::CanApprove( LPCTSTR sType, LPCTSTR sOpId, LPCTSTR sPassword ) const
{
    if (!m_bRCMgrPCreated)
        return FALSE;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CA_TYPE, sType);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CA_OP_ID, sOpId);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CA_OP_PASSWORD, sPassword);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    LONG lResult;
    m_rcmi.CanApprove(bstrParmList, &lResult);

    return (BOOL)lResult;
}


BOOL RAProcedures::CanApproveSecurity( LPCTSTR sOpId, LPCTSTR sPassword ) const
{
    TRACE_METHOD(_T("RAProcedures::CanApproveSecurity"));
    return CanApprove( KEY_APPROVAL_TYPE_SECURITY, sOpId, sPassword );
}


BOOL RAProcedures::CanApproveCoupons( LPCTSTR sOpId, LPCTSTR sPassword ) const
{
    TRACE_METHOD(_T("RAProcedures::CanApproveCoupons"));
    return CanApprove( KEY_APPROVAL_TYPE_COUPON, sOpId, sPassword );
}


BOOL RAProcedures::CanApproveVoids( LPCTSTR sOpId, LPCTSTR sPassword ) const
{
    TRACE_METHOD(_T("RAProcedures::CanApproveVoids"));
    return CanApprove( KEY_APPROVAL_TYPE_VOID, sOpId, sPassword );
}


BOOL RAProcedures::CanApproveAgeRestrictions( LPCTSTR sOpId, LPCTSTR sPassword ) const
{
    TRACE_METHOD(_T("RAProcedures::CanApproveAgeRestrictions"));
    return CanApprove( KEY_APPROVAL_TYPE_AGE_RESTRICTION, sOpId, sPassword );
}


BOOL RAProcedures::CanApproveVisualValidation( LPCTSTR sOpId, LPCTSTR sPassword ) const
{
    TRACE_METHOD(_T("RAProcedures::CanApproveVisualValidation"));
    return CanApprove( KEY_APPROVAL_TYPE_VISUAL_VALIDATION, sOpId, sPassword );
}


BOOL RAProcedures::CanApproveCallForHelp( LPCTSTR sOpId, LPCTSTR sPassword ) const
{
    TRACE_METHOD(_T("RAProcedures::CanApproveCallForHelp"));
    return CanApprove( KEY_APPROVAL_TYPE_CALL_FOR_HELP, sOpId, sPassword );
}


BOOL RAProcedures::CanApproveOtherPayment( LPCTSTR sOpId, LPCTSTR sPassword ) const
{
    TRACE_METHOD(_T("RAProcedures::CanApproveOtherPayment"));
    return CanApprove( KEY_APPROVAL_TYPE_OTHER_PAYMENT, sOpId, sPassword );
}


BOOL RAProcedures::CanApproveSuspends( LPCTSTR sOpId, LPCTSTR sPassword ) const
{
    TRACE_METHOD(_T("RAProcedures::CanApproveSuspends"));
    return CanApprove( KEY_APPROVAL_TYPE_SUSPEND, sOpId, sPassword );
}


BOOL RAProcedures::CanApprovePriceRequireds( LPCTSTR sOpId, LPCTSTR sPassword ) const
{
    TRACE_METHOD(_T("RAProcedures::CanApprovePriceRequireds"));
    return CanApprove( KEY_APPROVAL_TYPE_PRICE_REQUIRED, sOpId, sPassword );
}


BOOL RAProcedures::CanApproveUnknownItems( LPCTSTR sOpId, LPCTSTR sPassword ) const
{
    TRACE_METHOD(_T("RAProcedures::CanApproveUnknownItems"));
    return CanApprove( KEY_APPROVAL_TYPE_UNKNOWN_ITEM, sOpId, sPassword );
}


BOOL RAProcedures::CanApproveRecalledItems( LPCTSTR sOpId, LPCTSTR sPassword ) const
{
    TRACE_METHOD(_T("RAProcedures::CanApproveRecalledItems"));
    return CanApprove( KEY_APPROVAL_TYPE_RECALLED_ITEM, sOpId, sPassword );
}


BOOL RAProcedures::CanApproveTimeRestrictedItems( LPCTSTR sOpId, LPCTSTR sPassword ) const
{
    TRACE_METHOD(_T("RAProcedures::CanApproveTimeRestrictedItems"));
    return CanApprove( KEY_APPROVAL_TYPE_TIME_RESTRICTED_ITEM, sOpId, sPassword );
}


BOOL RAProcedures::CanApproveSigCap( LPCTSTR sOpId, LPCTSTR sPassword ) const
{
    TRACE_METHOD(_T("RAProcedures::CanApproveSigCap"));
    return CanApprove( KEY_APPROVAL_TYPE_SIG_CAP, sOpId, sPassword );
}


BOOL RAProcedures::CanApproveApplicationErrors( LPCTSTR sOpId, LPCTSTR sPassword, DWORD nEventId ) const
{
    TRACE_METHOD(_T("RAProcedures::CanApproveApplicationErrors"));
    return CanApprove( KEY_APPROVAL_TYPE_APPLICATION_ERROR, sOpId, sPassword );
}


BOOL RAProcedures::CanApproveDeviceErrors( LPCTSTR sOpId, LPCTSTR sPassword ) const
{
    TRACE_METHOD(_T("RAProcedures::CanApproveDeviceErrors"));
    return CanApprove( KEY_APPROVAL_TYPE_DEVICE_ERROR, sOpId, sPassword );
}


BOOL RAProcedures::CanApproveQuantityRestrictedItems( LPCTSTR sOpId, LPCTSTR sPassword ) const
{
    TRACE_METHOD(_T("RAProcedures::CanApproveQuantityRestrictedItems"));
    return CanApprove( KEY_APPROVAL_TYPE_QUANTITY_RESTRICTED_ITEM, sOpId, sPassword );
}


BOOL RAProcedures::CanApproveUnDeActivatedItem ( LPCTSTR sOpId, LPCTSTR sPassword ) const
{
    TRACE_METHOD(_T("RAProcedures::CanApproveUnDeActivatedItem"));
    return CanApprove( KEY_APPROVAL_TYPE_UNDEACTIVATED_ITEM, sOpId, sPassword );
}


void RAProcedures::GetApprovalInfo( SCOTDO_APPROVAL_INFO* pInfo ) const
{
    TRACE_METHOD(_T("RAProcedures::GetApprovalInfo"));

    if (!m_bRCMgrPCreated)
        return;

    m_rcmi.GetApprovalInfo(pInfo);
}


BEGIN_KV_HANDLER_CLASS(RAProcedures::GetCurrentApprovalParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::GetCurrentApprovalParms, KEY_GET_VALUE, ApprovalType)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(RAProcedures::GetCurrentApprovalParms, ApprovalType)


DWORD RAProcedures::GetCurrentApprovalCode()
{
    if (!m_bRCMgrPCreated)
        return 0;

    CString csCategory = m_rcmi.GetCurrentApprovalCategory();

    return GetCategory((const _TCHAR *)csCategory);
}


DWORD RAProcedures::GetCategory( LPCTSTR pcszCategory )
{
    DWORD dwCurrentApprovalCode = RA_CAT_UNKNOWN;

    trace(L6, _T("csCategory:  %s"), pcszCategory);

    CApprovalTypeMap::iterator iter = approvalMap.find(pcszCategory);
    if (iter != approvalMap.end())
        dwCurrentApprovalCode = iter->second;

    return dwCurrentApprovalCode;
}


long RAProcedures::GetConnectionMode()
{
    TRACE_METHOD(_T("RAProcedures::GetConnectionMode"));
    return GetLongValue(KEY_GET_ID_CONNECTION_MODE, RA_CONNECTION_MODE_STORE);
}



BEGIN_KV_HANDLER_CLASS(RAProcedures::GetLongValueParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::GetLongValueParms, KEY_GET_VALUE, Value)
END_KV_HANDLER_CLASS()
DEFINE_KV_LONG(RAProcedures::GetLongValueParms, Value)


void RAProcedures::GetLongValueParms::Clear()
{
    m_Value = 0;
}

LONG RAProcedures::GetLongValue(const _TCHAR *pszId, long lDefaultValue)
{
    if (!m_bRCMgrPCreated)
        return -1;

    long lRetValue = lDefaultValue;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_GET_ID, pszId);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    m_rcmi.Get(bstrParmList, &bstrResult);

    GetLongValueParms parms;
    parms.SetValue(lDefaultValue);

    CString csResult = bstrResult;
    parms.ParseKeys(csResult);

    ::SysFreeString(bstrResult);

    return parms.GetValue();
}


bool RAProcedures::IsRemoteConnectionHandheld(const CString &sRemoteConnectionName)
{
    TRACE_METHOD(_T("RAProcedures::IsRemoteConnectionHandheld"));

    if (!m_bRCMgrPCreated)
        return false;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_GET_ID, KEY_GET_ID_CURRENT_REMOTE_CONNECTION_HANDHELD);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_GET_REMOTE_CONNECTION_NAME, (const _TCHAR *)sRemoteConnectionName);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    m_rcmi.Get(bstrParmList, &bstrResult);

    GetBoolValueParms parms;
    parms.SetValue(false);

    CString csResult = bstrResult;
    parms.ParseKeys(csResult);

    ::SysFreeString(bstrResult);

    return parms.IsValue();
}


BEGIN_KV_HANDLER_CLASS(RAProcedures::GetBoolValueParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::GetBoolValueParms, KEY_GET_VALUE, Value)
END_KV_HANDLER_CLASS()
DEFINE_KV_BOOL(RAProcedures::GetBoolValueParms, Value)


void RAProcedures::GetBoolValueParms::Clear()
{
    m_Value = false;
}


bool RAProcedures::GetBoolValue(const _TCHAR *pszId, bool lDefaultValue)
{
    if (!m_bRCMgrPCreated)
        return false;

    bool lRetValue = lDefaultValue;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_GET_ID, pszId);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    m_rcmi.Get(bstrParmList, &bstrResult);

    GetBoolValueParms parms;
    parms.SetValue(lDefaultValue);

    CString csResult = bstrResult;
    parms.ParseKeys(csResult);

    ::SysFreeString(bstrResult);

    return parms.IsValue();
}


CString RAProcedures::GetRemoteConnectionName()
{
    TRACE_METHOD(_T("RAProcedures::GetRemoteConnectionName"));

    if (!m_bRCMgrPCreated)
        return _T("");

    return m_rcmi.GetRemoteConnectionName();
}


BEGIN_KV_HANDLER_CLASS(RAProcedures::GetStringValueParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::GetStringValueParms, KEY_GET_VALUE, Value)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(RAProcedures::GetStringValueParms, Value)

void RAProcedures::GetStringValueParms::Clear()
{
    m_Value.Empty();
}


CString RAProcedures::GetStringValue(const _TCHAR *pszId, const _TCHAR *lDefaultValue)
{
    if (!m_bRCMgrPCreated)
        return _T("");

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_GET_ID, pszId);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    m_rcmi.Get(bstrParmList, &bstrResult);

    GetStringValueParms parms;
    parms.SetValue(lDefaultValue);

    CString csResult = bstrResult;
    parms.ParseKeys(csResult);

    ::SysFreeString(bstrResult);

    return CString(parms.GetValue());
}


BSTR RAProcedures::GetApprovalInfo( LPCTSTR pDesc ) const
{
    BSTR bstrResult;
    bstrResult = NULL;
    if (m_bRCMgrPCreated)
    {
        CKeyValueParmList parmList;
        CKeyValue<const _TCHAR*>::Put(parmList, KEY_AI_TYPE, pDesc);
        _bstr_t bstrParmList((const _TCHAR *)parmList);

        m_rcmi.GetApprovalInfo(bstrParmList, &bstrResult);
    }

    return bstrResult;
}


void RAProcedures::OnNormalItem( LPCTSTR  sDesc )
{
    TRACE_METHOD(_T("RAProcedures::OnNormalItem"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_NORMAL_ITEM);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, sDesc);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.OnEvent(bstrParmList);
}


void RAProcedures::OnRemoteApproval(
   DWORD    dwHandle,
   LPCTSTR  sOpId,
   LPCTSTR  sPassword,
   int      nApprovalCode,
   LPCTSTR  sAuxData,
   DWORD    dwCookie )
{
    TRACE_METHOD(_T("RAProcedures::OnRemoteApproval"));

    if (!m_bRCMgrPCreated)
        return;
// RFQ 7477
    // required for TLOG to distinguish between Lane/RAP approvals
    // value must be reset false before end of function
    m_bIsRemoteApproval = true;

    CString csOpId = sOpId;
    CString csPassword = sPassword;
// RFQ 7477
    /////////////////////////////////////////////////////////////////
    //Begin FastLane 3.1 Reporting
	if (dwCookie != -1)  // TAR 418797
	{
		trace(L7, _T("+rp.SetInterventionTerminalNumber"));
		rp.SetInterventionTerminalNumber(csRAP);
		rp.SetOperator(csOpId); // RFQ 7477
		rp.SetInterventionEndInProgress(true);  // TAR 311902
        //rp.m_bIsInterventionApproved = true;    //SR803: report that the intervention was approved
		trace(L7, _T("-rp.SetInterventionTerminalNumber<%s>"),csRAP);
	}
    //End FastLane 3.1 Reporting
    ASSERT( nApprovalCode == RA_CODE_VERIFY_APPROVED
         || nApprovalCode == RA_CODE_APPROVED
         || nApprovalCode == RA_CODE_REDO );
    ASSERT( dwHandle != NULL );
    ASSERT( dwCookie != NULL );

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_AP_TYPE, KEY_APPROVAL_TYPE_REMOTE);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_AP_OP_ID, csOpId); // RFQ 7477
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_AP_OP_PASSWORD, csPassword); // RFQ 7477
    CKeyValue<int>::Put(parmList, KEY_AP_APPROVAL_CODE, nApprovalCode);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_AP_AUX_DATA, sAuxData);
    CKeyValue<long>::Put(parmList, KEY_AP_COOKIE, dwCookie);
    CKeyValue<long>::Put(parmList, KEY_AP_HANDLE, dwHandle);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.OnApproval(bstrParmList);

    CString csReportDesc, csCategory;
	DWORD TypeOfApproval;
    switch (nApprovalCode)
    {
        case RA_CODE_APPROVED :
		    TypeOfApproval = GetCurrentApprovalCode();
			csCategory = m_rcmi.GetCurrentApprovalCategory();

			//+tar 393092
			//save m_View if not yet -1 (dataneeded_noview)
			//like in the sample at id=2 even if OnNoRapDataNeeded already
			//change the view = DATANEEDED_NOVIEW, m_nInterventionView
			//will still be in UNEXPECTED_INCREASE
			if(SMStateBase::m_View != DATANEEDED_NOVIEW)
			{
		 		m_nInterventionView = SMStateBase::m_View; 
			}
			//-tar 393092

            if ((TypeOfApproval == RA_CAT_DATANEEDED) || (csCategory.Find(KEY_APPROVAL_TYPE_DATA_NEEDED) != -1))
                ReportInterventionEnd(dwCookie, SMStateBase::m_View);
            else
                ReportInterventionEnd(dwCookie, categoryMap[TypeOfApproval].c_str());

            m_nApprovedCount++;
            break;
        default :
            break;
    }
    m_bIsRemoteApproval = false; // RFQ 7477
}


void RAProcedures::OnSetTransactionState(int state)
{
    TRACE_METHOD(_T("RAProcedures::OnSetTransactionState"));

    if (!m_bRCMgrPCreated)
        return;

    //TAR213249
    if (state == RA_STATE_ASSISTOK)
    {
        RequestEnableAssistMode(true);
        trace(L6, _T("This method has been replaced by RequestEnableAssistMode(true)"));
    }
    else if (state == RA_STATE_ASSISTNOTOK)
    {
        RequestEnableAssistMode(false);
        trace(L6, _T("This method has been replaced by RequestEnableAssistMode(false)"));
    }

    //TAR213249
    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_SET_XACT_STATE);

    const _TCHAR *pszState = _T("");
    switch (state)
    {
        case RA_STATE_INACTIVE :
            pszState = KEY_CT_OPERATION_SET_XACT_STATE_INACTIVE; break;
        case RA_STATE_ITEMIZATION :
            pszState = KEY_CT_OPERATION_SET_XACT_STATE_ITEMIZATION; break;
        case RA_STATE_TENDER :
            pszState = KEY_CT_OPERATION_SET_XACT_STATE_TENDER; break;
        case RA_STATE_WAIT_FOR_APPROVAL :
            pszState = KEY_CT_OPERATION_SET_XACT_STATE_WAIT_FOR_APPROVAL; break;
        case RA_STATE_DATA_NEEDED :
            pszState = KEY_CT_OPERATION_SET_XACT_STATE_DATA_NEEDED; break;
        case RA_STATE_BAG_ITEM :
            pszState = KEY_CT_OPERATION_SET_XACT_STATE_BAG_ITEM; break;
        case RA_STATE_PICKING_UP_ITEMS :
            pszState = KEY_CT_OPERATION_SET_XACT_STATE_PICKING_UP_ITEMS; break;
        case RA_STATE_SECURITY_VIOLATION :
            pszState = KEY_CT_OPERATION_SET_XACT_STATE_SECURITY_VIOLATION; break;
			// RFQ 7477
		case RA_STATE_CASHDRAWER_OPEN:
			pszState = KEY_CT_OPERATION_SET_XACT_STATE_CASHDRAWER_OPEN; break;
		case RA_STATE_CASHDRAWER_CLOSE:
			pszState = KEY_CT_OPERATION_SET_XACT_STATE_CASHDRAWER_CLOSE; break;
			// RFQ 7477
    }
    CKeyValue<const _TCHAR *>::Put(parmList, KEY_CT_STATE, pszState);

    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);
}


void RAProcedures::OnTransactionEnd( void )
{
    TRACE_METHOD(_T("RAProcedures::OnTransactionEnd"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_XACT_END);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);

    m_rcmi.ResetRAPGenericDelayedPendingCount();
}


void RAProcedures::OnTransactionStart()
{
    TRACE_METHOD(_T("RAProcedures::OnTransactionStart"));

    if (!m_bRCMgrPCreated)
        return;

    OnNoRAPDataNeeded();

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_XACT_BEGIN);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);
}


void RAProcedures::ValidateOperator(
   LPCTSTR  sDesc,
   LPCTSTR  sOperatorID,
   LPCTSTR  sPassword,
   BOOL     bValid,
   DWORD dwCookie )
{
    TRACE_METHOD(_T("RAProcedures::ValidateOperator"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_VALIDATE_OPERATOR);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OP_ID, sOperatorID);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OP_PASSWORD, sPassword, MASKED_ENCODING); // SSCOP-3253
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_DESCRIPTION, sDesc);
    CKeyValue<long>::Put(parmList, KEY_AP_COOKIE, dwCookie);
    CKeyValue<bool>::Put(parmList, KEY_CT_VALID, (bValid!=0));
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);
}


void RAProcedures::OnCanSuspendNow( BOOL bCanSuspendNow )
{
    TRACE_METHOD(_T("RAProcedures::OnCanSuspendNow"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_CAN_SUSPEND_NOW);
    CKeyValue<bool>::Put(parmList, KEY_EV_VALUE, (bCanSuspendNow!=0 ? true : false) );
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.OnEvent(bstrParmList);
}

// +SR683
void RAProcedures::OnEnableSecurity( void )
{
    TRACE_METHOD(_T("RAProcedures::OnEnableSecurity"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_ENABLE_SECURITY);
    _bstr_t bstrParmList((const _TCHAR *)parmList); 
    m_rcmi.OnEvent(bstrParmList);
}

void RAProcedures::OnDisableSecurity( void )
{
    TRACE_METHOD(_T("RAProcedures::OnDisableSecurity"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_DISABLE_SECURITY);
    _bstr_t bstrParmList((const _TCHAR *)parmList); 
    m_rcmi.OnEvent(bstrParmList);
}
// -SR683

void RAProcedures::OnEnterStoreMode(
   LPCTSTR sOpId,
   LPCTSTR sPassword )
{
    TRACE_METHOD(_T("RAProcedures::OnEnterStoreMode"));

    if ( tb.IsTBInitialized() == TB_SUCCESS)    
    {
        CString csCommand = _T("Enter_StoreMode");
        mp.sCurr->SendTBCommand(TBCMD_CMDSTRING, csCommand);
    }

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_EV_TYPE_ENTER_STORE_MODE);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OP_ID, sOpId);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OP_PASSWORD, sPassword, MASKED_ENCODING);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);
}


void RAProcedures::OnExitStoreMode( void )
{
    TRACE_METHOD(_T("RAProcedures::OnExitStoreMode"));

    if ( tb.IsTBInitialized() == TB_SUCCESS)    
    {
        CString csCommand = _T("Exit_StoreMode");
        mp.sCurr->SendTBCommand(TBCMD_CMDSTRING, csCommand);
    }

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_EV_TYPE_EXIT_STORE_MODE);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);

    rp.ClearOperator();

    rp.SetInterventionEndInProgress(false);	// TAR 444331, 444337
}


void RAProcedures::OnAgeRestrictedItem(
   LPCTSTR  sDesc,
   int      nMinAge,
   long     lNumberOfUnapprovedRestrictedItems,  //SR803
   BOOL     bVoided,
   BOOL     bDOBNotNeededBtn   
   )
{
    TRACE_METHOD(_T("RAProcedures::OnAgeRestrictedItem"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, sDesc);
    CKeyValue<int>::Put(parmList, KEY_EV_MINIMUM_AGE, nMinAge );
    CKeyValue<long>::Put(parmList, KEY_EV_UNAPPROVED_RESTRICTED_ITEMS, lNumberOfUnapprovedRestrictedItems ); //SR803
    CKeyValue<bool>::Put(parmList, KEY_EV_VOIDED, (bVoided!=0) );
    CKeyValue<bool>::Put(parmList, KEY_EV_DOB_NOT_NEEDED, (bDOBNotNeededBtn!=0) );

    AddApproval( parmList, KEY_EV_TYPE_AGE_RESTRICTION );
}


void RAProcedures::AddApproval( const CKeyValueParmList &pList, LPCTSTR  sType, DWORD dwPriority )
{
    if (!m_bRCMgrPCreated)
        return;

    m_lInterventionCount++;
    long lCookie = m_lInterventionCount;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, sType);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_PRIORITY, GetPriority(dwPriority));
    CKeyValue<long>::Put(parmList, KEY_EV_COOKIE, m_lInterventionCount );

    parmList.Append(pList);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.OnEvent(bstrParmList);

    ReportInterventionStart( m_lInterventionCount, dwPriority, sType );
}


const _TCHAR *RAProcedures::GetPriority(DWORD dwPriority)
{
    switch (dwPriority)
    {
        case RA_PRIORITY_DEFER :
            return KEY_EV_PRIORITY_DELAYED;
        case RA_PRIORITY_URGENT :
            return KEY_EV_PRIORITY_URGENT;
        default :
            _ASSERTE(0);
    }

    return _T("");
}


BOOL RAProcedures::OnAgeApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnAgeApproval"));

    return OnApproval(KEY_APPROVAL_TYPE_AGE_RESTRICTION);
}
#endif //_CPPUNIT


BOOL RAProcedures::OnApproval( const _TCHAR *pszApprovalType, bool bGenericDelayed)
{
    if (!m_bRCMgrPCreated)
        return FALSE;

#ifndef _CPPUNIT
    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_AP_TYPE, pszApprovalType);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
#else
    _bstr_t bstrParmList(_T(""));
#endif //_CPPUNIT
 
    if(bGenericDelayed)
    {
        bstrParmList = pszApprovalType;
    }

    BOOL retValue = FALSE;
    switch (m_rcmi.OnApproval(bstrParmList))
    {
        case S_OK :
            retValue = TRUE;
            break;
        case S_FALSE :
        default :
            break;
    }
    return retValue;
}

#ifndef _CPPUNIT
void RAProcedures::GetAgeRestrictionInfo( SCOTDO_INFO*pInfo ) const
{
    TRACE_METHOD(_T("RAProcedures::GetAgeRestrictionInfo"));

    BSTR bstrResult = GetApprovalInfo( KEY_APPROVAL_TYPE_AGE_RESTRICTION );

    //TODO:  Add code to fill in the deferred override

    SysFreeString(bstrResult);
}


/*******************************************************************************
******************************************************************************
* Scot Deferred Override - Coupon Functions
*/

BOOL RAProcedures::OnCouponApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnCouponApproval"));

    return OnApproval(KEY_APPROVAL_TYPE_COUPON);
}


void RAProcedures::OnCouponItem(
    LPCTSTR  sDesc,
    long     nAmount,
    bool        bNeedApproval)
{
    TRACE_METHOD(_T("RAProcedures::OnCouponItem"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, sDesc);
    CKeyValue<long>::Put(parmList, KEY_EV_AMOUNT, nAmount );
    CKeyValue<bool>::Put(parmList, KEY_EV_NEED_APPROVAL, bNeedApproval );

    AddApproval( parmList, KEY_APPROVAL_TYPE_COUPON );
}


void RAProcedures::GetCouponInfo( SCOTDO_INFO*pInfo ) const
{
    TRACE_METHOD(_T("RAProcedures::GetCouponInfo"));

    BSTR bstrResult = GetApprovalInfo( KEY_APPROVAL_TYPE_COUPON );

    //TODO:  Add code to fill in the deferred override

    SysFreeString(bstrResult);
}


/*******************************************************************************
******************************************************************************
* Scot Deferred Override - Void Item Functions
*/


BOOL RAProcedures::OnVoidApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnVoidApproval"));

    return OnApproval(KEY_APPROVAL_TYPE_VOID);
}


void RAProcedures::OnVoidedItem(
    LPCTSTR  sDesc,
    long     nAmount,
    long     nGrossAmount )
{
    TRACE_METHOD(_T("RAProcedures::OnVoidedItem"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, sDesc);
    CKeyValue<long>::Put(parmList, KEY_EV_AMOUNT, nAmount );
    CKeyValue<long>::Put(parmList, KEY_EV_GROSS, nGrossAmount );

    AddApproval( parmList, KEY_APPROVAL_TYPE_VOID );
}

void RAProcedures::GetVoidInfo( SCOTDO_INFO*pInfo ) const
{
    TRACE_METHOD(_T("RAProcedures::GetVoidInfo"));

    BSTR bstrResult = GetApprovalInfo( KEY_APPROVAL_TYPE_VOID );

    //TODO:  Add code to fill in the deferred override

    SysFreeString(bstrResult);
}


/*******************************************************************************
******************************************************************************
* Scot Deferred Override - Security Functions
*/

BOOL RAProcedures::OnSecurityApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnSecurityApproval"));

    return OnApproval(KEY_APPROVAL_TYPE_SECURITY);
}

void RAProcedures::OnSecurityEvent(
    LPCTSTR  sDesc,
    DWORD    nConclusion,
    int      nLevel )
{
    TRACE_METHOD(_T("RAProcedures::OnSecurityEvent"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, sDesc);
    CKeyValue<long>::Put(parmList, KEY_EV_CONCLUSION, nConclusion );
    CKeyValue<int>::Put(parmList, KEY_EV_LEVEL, nLevel );

    AddApproval( parmList, KEY_APPROVAL_TYPE_SECURITY, nLevel == 2 ? RA_PRIORITY_URGENT : RA_PRIORITY_DEFER );
}


void RAProcedures::GetSecurityInfo( SCOTDO_INFO* pInfo ) const
{
    TRACE_METHOD(_T("RAProcedures::GetSecurityInfo"));

    BSTR bstrResult = GetApprovalInfo( KEY_APPROVAL_TYPE_SECURITY );

    //TODO:  Add code to fill in the deferred override

    SysFreeString(bstrResult);
}

/*******************************************************************************
******************************************************************************
* Scot Deferred Override - Visual Validation Functions
*/

BOOL RAProcedures::OnVisualValidationApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnVisualValidationApproval"));

    return OnApproval(KEY_APPROVAL_TYPE_VISUAL_VALIDATION);
}

BOOL RAProcedures::OnHopperSubstitutionApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnHopperSubstitutionApproval"));

    return OnApproval(KEY_APPROVAL_TYPE_HOPPER_SUBSTITUTION);
}

void RAProcedures::OnHopperSubstitution(
    LPCTSTR  sDesc)
{
    TRACE_METHOD(_T("RAProcedures::OnHopperSubstitution"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, sDesc);

    AddApproval( parmList, KEY_APPROVAL_TYPE_HOPPER_SUBSTITUTION );
}

void RAProcedures::OnVisualValidationItem(
    LPCTSTR  sDesc,
    BOOL     bVoided )
{
    TRACE_METHOD(_T("RAProcedures::OnVisualValidationItem"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, sDesc);
    CKeyValue<bool>::Put(parmList, KEY_EV_VOIDED, (bVoided!=0) );

    AddApproval( parmList, KEY_APPROVAL_TYPE_VISUAL_VALIDATION );
}


void RAProcedures::GetVisualValidationInfo( SCOTDO_INFO* pInfo ) const
{
    TRACE_METHOD(_T("RAProcedures::GetVisualValidationInfo"));

    BSTR bstrResult = GetApprovalInfo( KEY_APPROVAL_TYPE_VISUAL_VALIDATION );

    //TODO:  Add code to fill in the deferred override

    SysFreeString(bstrResult);
}


/*******************************************************************************
******************************************************************************
* Scot Deferred Override - Other Payment */

BOOL RAProcedures::OnOtherPaymentApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnOtherPaymentApproval"));

    return OnApproval(KEY_APPROVAL_TYPE_OTHER_PAYMENT);
}


void RAProcedures::OnOtherPaymentEvent(
    LPCTSTR  sDesc,
    BOOL     bVoided )
{
    TRACE_METHOD(_T("RAProcedures::OnOtherPaymentEvent"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, sDesc);
    CKeyValue<bool>::Put(parmList, KEY_EV_VOIDED, (bVoided!=0) );

    AddApproval( parmList, KEY_APPROVAL_TYPE_OTHER_PAYMENT );
}

void RAProcedures::GetOtherPaymentInfo( SCOTDO_INFO* pInfo ) const
{
    TRACE_METHOD(_T("RAProcedures::GetOtherPaymentInfo"));

    BSTR bstrResult = GetApprovalInfo( KEY_APPROVAL_TYPE_OTHER_PAYMENT );

    //TODO:  Add code to fill in the deferred override

    SysFreeString(bstrResult);
}

/*******************************************************************************
******************************************************************************
* Scot Deferred Override - Suspend Functions
*/



void RAProcedures::OnRequestSuspend( LPCTSTR sReason )
{
    TRACE_METHOD(_T("RAProcedures::OnRequestSuspend"));

    m_sSuspendReason = sReason;
}


BOOL RAProcedures::OnSuspendApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnSuspendApproval"));
    //tar 371932 & tar 371918+
	ReportInterventionEnd(m_lInterventionCount, KEY_APPROVAL_TYPE_SUSPEND);
	//tar 371932 & tar 371918-

    return OnApproval(KEY_APPROVAL_TYPE_SUSPEND);
}


void RAProcedures::OnReadyToSuspend( void )
{
    TRACE_METHOD(_T("RAProcedures::OnReadyToSuspend"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, m_sSuspendReason);

    AddApproval( parmList, KEY_APPROVAL_TYPE_SUSPEND, RA_PRIORITY_URGENT );

    m_sSuspendReason.Empty();
}

/*******************************************************************************
******************************************************************************
* Scot Deferred Override - Unknown Price Functions
*/

BOOL RAProcedures::OnUnknownPriceApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnUnknownPriceApproval"));

    return OnApproval(KEY_APPROVAL_TYPE_UNKNOWN_PRICE);
}

void RAProcedures::OnUnknownPrice( LPCTSTR sDesc )
{
    TRACE_METHOD(_T("RAProcedures::OnUnknownPrice"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, ps.ConvertToEscapeSequence(sDesc, _T("\\n"), _T("\n")));

    AddApproval( parmList, KEY_APPROVAL_TYPE_UNKNOWN_PRICE );
}


void RAProcedures::GetUnknownPriceInfo( SCOTDO_INFO* pInfo ) const
{
    TRACE_METHOD(_T("RAProcedures::GetUnknownPriceInfo"));

    BSTR bstrResult = GetApprovalInfo( KEY_APPROVAL_TYPE_UNKNOWN_PRICE );

    //TODO:  Add code to fill in the deferred override

    SysFreeString(bstrResult);
}


/*******************************************************************************
******************************************************************************
* Scot Deferred Override - Recalled Item Functions
*/


BOOL RAProcedures::OnRecalledItemApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnRecalledItemApproval"));

    return OnApproval(KEY_APPROVAL_TYPE_RECALLED_ITEM);
}

void RAProcedures::OnRecalledItem( LPCTSTR sDesc )
{
    TRACE_METHOD(_T("RAProcedures::OnRecalledItem"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, ps.ConvertToEscapeSequence(sDesc, _T("\\n"), _T("\n")));

    AddApproval( parmList, KEY_APPROVAL_TYPE_RECALLED_ITEM );
}


void RAProcedures::GetRecalledItemInfo( SCOTDO_INFO* pInfo ) const
{
    TRACE_METHOD(_T("RAProcedures::GetRecalledItemInfo"));

    BSTR bstrResult = GetApprovalInfo( KEY_APPROVAL_TYPE_RECALLED_ITEM );

    //TODO:  Add code to fill in the deferred override

    SysFreeString(bstrResult);
}





/*******************************************************************************
******************************************************************************
* Scot Deferred Override - Signature Capture Functions
*/

BOOL RAProcedures::OnSigCapApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnSigCapApproval"));

    return OnApproval(KEY_APPROVAL_TYPE_SIG_CAP);
}


void RAProcedures::OnSigCapData(
    LPCTSTR sDesc,
    LPCTSTR cSigData,
    DWORD nLength )
{
    TRACE_METHOD(_T("RAProcedures::OnSigCapData"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, sDesc);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_SIG_CAP_DATA, cSigData);
    CKeyValue<long>::Put(parmList, KEY_EV_LENGTH, nLength);

    AddApproval( parmList, KEY_APPROVAL_TYPE_SIG_CAP, RA_PRIORITY_URGENT );
}


void RAProcedures::GetSigCapInfo( SCOTDO_INFO * pInfo ) const
{
    TRACE_METHOD(_T("RAProcedures::GetSigCapInfo"));

    BSTR bstrResult = GetApprovalInfo( KEY_APPROVAL_TYPE_SIG_CAP );

    //TODO:  Add code to fill in the deferred override

    SysFreeString(bstrResult);
}

/*******************************************************************************
******************************************************************************
* Scot Deferred Override - Time Restricted Item Functions
*/


BOOL RAProcedures::OnTimeRestrictedItemApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnTimeRestrictedItemApproval"));

    return OnApproval(KEY_APPROVAL_TYPE_TIME_RESTRICTED_ITEM);
}

void RAProcedures::OnTimeRestrictedItem( LPCTSTR sDesc )
{
    TRACE_METHOD(_T("RAProcedures::OnTimeRestrictedItem"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, ps.ConvertToEscapeSequence(sDesc, _T("\\n"), _T("\n")));

    AddApproval( parmList, KEY_APPROVAL_TYPE_TIME_RESTRICTED_ITEM );
}


void RAProcedures::GetTimeRestrictedItemInfo( SCOTDO_INFO* pInfo ) const
{
    TRACE_METHOD(_T("RAProcedures::GetTimeRestrictedItemInfo"));

    BSTR bstrResult = GetApprovalInfo( KEY_APPROVAL_TYPE_TIME_RESTRICTED_ITEM );

    //TODO:  Add code to fill in the deferred override

    SysFreeString(bstrResult);
}

/*******************************************************************************
******************************************************************************
* Scot Deferred Override - Unknown Item Functions
*/

BOOL RAProcedures::OnUnknownItemApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnUnknownItemApproval"));

    return OnApproval(KEY_APPROVAL_TYPE_UNKNOWN_ITEM);
}

BOOL RAProcedures::OnUnknownItem(
    LPCTSTR  sDesc,
    BOOL     bKeyed )
{
    TRACE_METHOD(_T("RAProcedures::OnUnknownItem"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, sDesc);
    CKeyValue<bool>::Put(parmList, KEY_EV_KEYED, (bKeyed!=0) );

    AddApproval( parmList, KEY_APPROVAL_TYPE_UNKNOWN_ITEM );

    return TRUE;
}

void RAProcedures::GetUnknownItemInfo( SCOTDO_INFO* pInfo ) const
{
    TRACE_METHOD(_T("RAProcedures::GetUnknownItemInfo"));

    BSTR bstrResult = GetApprovalInfo( KEY_APPROVAL_TYPE_UNKNOWN_ITEM );

    //TODO:  Add code to fill in the deferred override

    SysFreeString(bstrResult);
}

/*******************************************************************************
******************************************************************************
* Scot Deferred Override - Device Error Functions
*/

/*
// Begin TAR 192405, 192461
//These two methods are NOT used anymore

// In the place of call to OnDeviceError(), OnRAPDataNeeded() is called now
// since RAP is notified of any Device errors as an DataNeeded Intervention.
void RAProcedures::OnDeviceError(
LPCTSTR  sDesc,
int      nDeviceClass,
int      nDeviceId  )
{
m_DeviceErrorData.AddEvent( this, sDesc, nDeviceClass, nDeviceId  );
return;
}

// In the place of call to ClearDeviceError(), OnNoRAPDataNeeded() is called now
// since RAP is notified of any Device errors as an DataNeeded Intervention.
BOOL RAProcedures::ClearDeviceError( void )
{
return Approve( &m_DeviceErrorData, m_sOpId, m_sPassword );
}
// End TAR 192405, 192461
*/

void RAProcedures::GetDeviceErrorInfo( SCOTDO_INFO* pInfo ) const
{
    TRACE_METHOD(_T("RAProcedures::GetDeviceErrorInfo"));

    BSTR bstrResult = GetApprovalInfo( KEY_APPROVAL_TYPE_DEVICE_ERROR );

    //TODO:  Add code to fill in the deferred override

    SysFreeString(bstrResult);
}



/*******************************************************************************
******************************************************************************
* Scot Deferred Override - Application Error Functions
*/

void RAProcedures::OnApplicationErrorEvent(
    LPCTSTR  sDesc,
    int      nRaCat,
    int      nEventId )
{
    TRACE_METHOD(_T("RAProcedures::OnApplicationErrorEvent"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, sDesc);
    CKeyValue<int>::Put(parmList, KEY_EV_CATEGORY, nRaCat );
    CKeyValue<int>::Put(parmList, KEY_EV_EVENT_ID, nEventId );

    AddApproval( parmList, KEY_APPROVAL_TYPE_APPLICATION_ERROR, RA_PRIORITY_URGENT );
}

void RAProcedures::OnCancelApplicationError( void )
{
    TRACE_METHOD(_T("RAProcedures::OnCancelApplicationError"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_CANCEL_APPLICATION_ERROR);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.OnEvent(bstrParmList);
}


BOOL RAProcedures::OnApproveApplicationError( void )
{
    TRACE_METHOD(_T("RAProcedures::OnApproveApplicationError"));

    return OnApproval(KEY_APPROVAL_TYPE_APPLICATION_ERROR);
}


void RAProcedures::GetApplicationErrorInfo( SCOTDO_INFO * pInfo ) const
{
    TRACE_METHOD(_T("RAProcedures::GetApplicationErrorInfo"));

    BSTR bstrResult = GetApprovalInfo( KEY_APPROVAL_TYPE_APPLICATION_ERROR );

    //TODO:  Add code to fill in the deferred override

    SysFreeString(bstrResult);
}

/*******************************************************************************
******************************************************************************
* Scot Deferred Override - EAS Item Active Functions
*/

BOOL RAProcedures::OnUnDeActivatedItemApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnUnDeActivatedItemApproval"));

    return OnApproval(KEY_APPROVAL_TYPE_UNDEACTIVATED_ITEM);
}

void RAProcedures::OnUnDeActivatedItem(LPCTSTR  sDesc)
{
    TRACE_METHOD(_T("RAProcedures::OnUnDeActivatedItem"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, sDesc);

    AddApproval( parmList, KEY_APPROVAL_TYPE_UNDEACTIVATED_ITEM );
}

void RAProcedures::GetUnDeActivatedItemInfo( SCOTDO_INFO*pInfo ) const
{
    TRACE_METHOD(_T("RAProcedures::GetUnDeActivatedItemInfo"));

    BSTR bstrResult = GetApprovalInfo( KEY_APPROVAL_TYPE_UNDEACTIVATED_ITEM );

    //TODO:  Add code to fill in the deferred override

    SysFreeString(bstrResult);
}

/*******************************************************************************
******************************************************************************
* Scot Deferred Override - Data Needed Functions
*/

static long lMisMatchId = 0;
static long lUnexIncreaseId = 0;
static long lUnexDecreaseId = 0;
static long lDontBagId = 0; //TAR433737

// Adds event and approval to SCOTRA
void RAProcedures::OnRAPDataNeeded(
    LPCTSTR  sDesc,
    LPCTSTR  sView,
    int iView,
    LPCTSTR  sReportCategoryName)
{
    TRACE_METHOD(_T("RAProcedures::OnRAPDataNeeded"));
	bool bSkipReporting = false;

    if (!m_bRCMgrPCreated)
        return;

    //TAR433737
    if(iView ==  DATANEEDED_MISMATCHWEIGHT)
	{
		if( SMStateBase::m_bCurrentIntervention_MisMatch == false )
		{
			SMStateBase::m_bCurrentIntervention_MisMatch = true;
			lMisMatchId = m_lInterventionCount+1;
		}
		else
		{
			trace ( L6, _T("RAProcedures::OnRAPDataNeeded: no need to log this intervention "));
			bSkipReporting = true;
		}
	}
    if(iView == DATANEEDED_UNEXPECTEDINCREASE)
	{
		if( SMStateBase::m_bCurrentIntervention_UnexIncrease == false )
		{
			SMStateBase::m_bCurrentIntervention_UnexIncrease = true;
			lUnexIncreaseId = m_lInterventionCount+1;
		}
		else
		{
			trace ( L6, _T("RAProcedures::OnRAPDataNeeded: no need to log this intervention "));
			bSkipReporting = true;
		}
	}
    if(iView == DATANEEDED_UNEXPECTEDDECREASE)
	{
		if( SMStateBase::m_bCurrentIntervention_UnexDecrease == false )
		{
			SMStateBase::m_bCurrentIntervention_UnexDecrease = true;
			lUnexDecreaseId = m_lInterventionCount+1;
		}

		else
		{
			trace ( L6, _T("RAProcedures::OnRAPDataNeeded: no need to log this intervention "));
			bSkipReporting = true;
		}
	}
    if(iView == DATANEEDED_DONTBAGTHRESHOLDEXCEEDED)
	{
		if( SMStateBase::m_bCurrentIntervention_DontBagThreshold == false )
		{
			SMStateBase::m_bCurrentIntervention_DontBagThreshold = true;
			lDontBagId = m_lInterventionCount+1;
		}

		else
		{
			trace ( L6, _T("RAProcedures::OnRAPDataNeeded: no need to log this intervention "));
			//return; //Curretly, a same intervention has been outstanding, no need to log the newer interventions to report server
			bSkipReporting = true;
		}
	}

	if(!bSkipReporting)
		m_lInterventionCount++;
    long lCookie = m_lInterventionCount;

    m_nRAPPreviousView  = m_nRAPCurrentView;
    m_nRAPCurrentView   = iView;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_RAP_DATA_NEEDED);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_PRIORITY, GetPriority(RA_PRIORITY_URGENT));
    CKeyValue<long>::Put(parmList, KEY_EV_COOKIE, m_lInterventionCount );
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_VIEW_TEXT, sView);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, sDesc);
    CKeyValue<int>::Put(parmList, KEY_EV_VIEW, iView);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.OnEvent(bstrParmList, RAP_DATA_NEEDED);

	if (!bSkipReporting)
	{
		ReportInterventionStart( m_lInterventionCount, RA_PRIORITY_URGENT, iView, sReportCategoryName );
	}
}


void RAProcedures::OnRAPDataNeeded(
    LPCTSTR  sDesc,
    int iView)
{
    TRACE_METHOD(_T("RAProcedures::OnRAPDataNeeded"));

    if (!m_bRCMgrPCreated)
        return;

    m_lInterventionCount++;
    long lCookie = m_lInterventionCount;

    m_nRAPPreviousView  = DATANEEDED_NOVIEW;
    m_nRAPCurrentView   = DATANEEDED_NOVIEW;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_RAP_DATA_NEEDED);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_PRIORITY, GetPriority(RA_PRIORITY_URGENT));
    CKeyValue<long>::Put(parmList, KEY_EV_COOKIE, m_lInterventionCount );
    CKeyValue<int>::Put(parmList, KEY_EV_VIEW, iView);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.OnEvent(bstrParmList, RAP_DATA_NEEDED);

    ReportInterventionStart( m_lInterventionCount
                           , RA_PRIORITY_URGENT
                           , KEY_EV_TYPE_RAP_DATA_NEEDED );
}


BOOL RAProcedures::IsRAPDataAvailable(void) const
{
    TRACE_METHOD(_T("RAProcedures::IsRAPDataAvailable"));

    if (!m_bRCMgrPCreated)
        return FALSE;

    return m_rcmi.IsRAPDataAvailable() ? TRUE : FALSE;
}


CString RAProcedures::GetRAPData( void ) const
{
    TRACE_METHOD(_T("RAProcedures::GetRAPData"));

    if (!m_bRCMgrPCreated)
        return _T("");

    return m_rcmi.GetRAPData();
}


int RAProcedures::GetRAPPreviousView( void ) const
{
    TRACE_METHOD(_T("RAProcedures::GetRAPPreviousView"));

    return m_nRAPPreviousView;
}


int RAProcedures::GetRAPView( void ) const
{
    TRACE_METHOD(_T("RAProcedures::GetRAPView"));

    return m_nRAPCurrentView;
}


int RAProcedures::GetRAPDataPendingCount(void) const
{
    if (!m_bRCMgrPCreated)
        return -1;

    return m_rcmi.GetRAPDataNeededPendingCount();
}


// Actually update the approval to RA_APPROVED
void RAProcedures::OnNoRAPDataNeeded( void )
{
    TRACE_METHOD(_T("RAProcedures::OnNoRAPDataNeeded"));

    if (!m_bRCMgrPCreated)
	{
		//TAR 371932 & tar 371918 added trace
		TRACE_METHOD(_T("RAProcedures::OnNoRAPDataNeeded,m_bRCMgrPCreated is false"));
  
		return;
	}

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_NO_RAP_DATA_NEEDED);
    CKeyValue<bool>::Put(parmList, KEY_APPROVAL_TYPE_REMOTE, SMStateBase::g_bFromRAP);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.OnEvent(bstrParmList, RESET_RAP_DATA_NEEDED);

    if(SMStateBase::m_PrevView != DATANEEDED_NOVIEW && !mp.sCurr->GetStatePopped()) //tar 252987
    {
        SMStateBase::m_View = SMStateBase::m_PrevView;
        SMStateBase::m_PrevView = DATANEEDED_NOVIEW;
    }
    else if(!mp.sCurr->GetStatePopped())
    {
        //tar 252987 changed reset value from 0 to -1 (DATANEEDED_NOVIEW)
        SMStateBase::m_View = DATANEEDED_NOVIEW;    //Reset the view value when approve
    }

	rp.ClearOperator(); //SSCOP-3775
}


void RAProcedures::OnMediaLow( LPCTSTR sDesc , bool bMediaLow )
{
    TRACE_METHOD(_T("RAProcedures::OnMediaLow"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_MEDIA_LOW);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, ps.ConvertToEscapeSequence(sDesc, _T("\\n"), _T("\n")));
    CKeyValue<bool>::Put(parmList, KEY_EV_VALUE, bMediaLow);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.OnEvent(bstrParmList);
}


void RAProcedures::OnOutOfService( LPCTSTR sDesc )
{
    TRACE_METHOD(_T("RAProcedures::OnOutOfService"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_OUT_OF_SERVICE);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, ps.ConvertToEscapeSequence(sDesc, _T("\\n"), _T("\n")));
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.OnEvent(bstrParmList);
}


void RAProcedures::OnReturnToService( void )
{
    TRACE_METHOD(_T("RAProcedures::OnReturnToService"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_RETURN_TO_SERVICE);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.OnEvent(bstrParmList);
}


void RAProcedures::OnCallForHelp( LPCTSTR sDesc )
{
    TRACE_METHOD(_T("RAProcedures::OnCallForHelp"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, ps.ConvertToEscapeSequence(sDesc, _T("\\n"), _T("\n")));

    AddApproval( parmList, KEY_EV_TYPE_CALL_FOR_HELP );
}


BOOL RAProcedures::OnCallForHelpApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnCallForHelpApproval"));

    return OnApproval(KEY_APPROVAL_TYPE_CALL_FOR_HELP);
}


void RAProcedures::SetTransactionTotal( LPCTSTR sAmount )
{
    TRACE_METHOD(_T("RAProcedures::SetTransactionTotal"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_SET_ID, KEY_SET_ID_XACT_TOTAL);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_VALUE, sAmount);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.Set(bstrParmList);
}


void RAProcedures::OnReadyToCollect( LPCTSTR  sDesc )
{
    TRACE_METHOD(_T("RAProcedures::OnReadyToCollect"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, sDesc);

    AddApproval( parmList, KEY_EV_TYPE_READY_TO_COLLECT, RA_PRIORITY_URGENT );
}


void RAProcedures::SetLightState( int greenState,
                                    int yellowState,
                                    int redState)
{
    TRACE_METHOD(_T("RAProcedures::SetLightState"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_SET_LIGHT_STATE);
    CKeyValue<int>::Put(parmList, KEY_CT_OPERATION_SET_LIGHT_STATE_GREEN, greenState);
    CKeyValue<int>::Put(parmList, KEY_CT_OPERATION_SET_LIGHT_STATE_YELLOW, yellowState);
    CKeyValue<int>::Put(parmList, KEY_CT_OPERATION_SET_LIGHT_STATE_RED, redState);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);
}


void RAProcedures::OnAddOtherEvent( LPCTSTR sDesc )
{
    TRACE_METHOD(_T("RAProcedures::OnAddOtherEvent"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_ADD_OTHER_EVENT);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, ps.ConvertToEscapeSequence(sDesc, _T("\\n"), _T("\n")));
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.OnEvent(bstrParmList);
}

/*******************************************************************************
******************************************************************************
* Scot Deferred Override - Quantity Restricted Item Functions
*/


BOOL RAProcedures::OnQuantityRestrictedItemApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnQuantityRestrictedItemApproval"));

    return OnApproval(KEY_APPROVAL_TYPE_QUANTITY_RESTRICTED_ITEM);
}


void RAProcedures::OnQuantityRestrictedItem( LPCTSTR sDesc )
{
    TRACE_METHOD(_T("RAProcedures::OnQuantityRestrictedItem"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, ps.ConvertToEscapeSequence(sDesc, _T("\\n"), _T("\n")));

    AddApproval( parmList, KEY_APPROVAL_TYPE_QUANTITY_RESTRICTED_ITEM );
}


void RAProcedures::GetQuantityRestrictedItemInfo( SCOTDO_INFO* pInfo ) const
{
    TRACE_METHOD(_T("RAProcedures::GetQuantityRestrictedItemInfo"));

    BSTR bstrResult = GetApprovalInfo( KEY_APPROVAL_TYPE_QUANTITY_RESTRICTED_ITEM );

    //TODO:  Add code to fill in the deferred override

    SysFreeString(bstrResult);
}

/*******************************************************************************
******************************************************************************
* Scot Deferred Override - Restricted Not Allowed Functions
*/


BOOL RAProcedures::OnRestrictedNotAllowedApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnRestrictedNotAllowedApproval"));

    return OnApproval(KEY_APPROVAL_TYPE_RESTRICTED_NOT_ALLOWED);
}

void RAProcedures::OnRestrictedNotAllowed( LPCTSTR sDesc )
{
    TRACE_METHOD(_T("RAProcedures::OnRestrictedNotAllowed"));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, ps.ConvertToEscapeSequence(sDesc, _T("\\n"), _T("\n")));

    AddApproval( parmList, KEY_APPROVAL_TYPE_RESTRICTED_NOT_ALLOWED );
}


void RAProcedures::GetRestrictedNotAllowedInfo( SCOTDO_INFO* pInfo ) const
{
    TRACE_METHOD(_T("RAProcedures::GetRestrictedNotAllowedInfo"));

    BSTR bstrResult = GetApprovalInfo( KEY_APPROVAL_TYPE_RESTRICTED_NOT_ALLOWED );

    //TODO:  Add code to fill in the deferred override

    SysFreeString(bstrResult);
}
#endif //_CPPUNIT


void RAProcedures::RequestEnableAssistMode(bool status)
{
#ifndef _CPPUNIT
    TRACE_METHOD(_T("RAProcedures::RequestEnableAssistMode"));

    if (!m_bRCMgrPCreated)
        return;

    status = status && mp.sCurr->RAPAssistModeAllowed();

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_SET_ID, KEY_SET_ID_REQUEST_ENABLE_ASSIST_MODE);
    CKeyValue<bool>::Put(parmList, KEY_EV_VALUE, status);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.Set(bstrParmList);
#endif
}

#ifndef _CPPUNIT
BEGIN_KV_HANDLER_CLASS(RAProcedures::GetRequestEnableAssistModeParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::GetRequestEnableAssistModeParms, KEY_GET_VALUE, RequestEnableAssistMode)
END_KV_HANDLER_CLASS()
DEFINE_KV_BOOL(RAProcedures::GetRequestEnableAssistModeParms, RequestEnableAssistMode)


void RAProcedures::GetRequestEnableAssistModeParms::Clear()
{
    m_RequestEnableAssistMode = false;
}

bool RAProcedures::GetRequestEnableAssistMode(void)
{
    TRACE_METHOD(_T("RAProcedures::GetRequestEnableAssistMode"));

    if (!m_bRCMgrPCreated)
        return false;

    bool bRequestEnableAssistMode = false;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_GET_ID, KEY_SET_ID_REQUEST_ENABLE_ASSIST_MODE);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    m_rcmi.Get(bstrParmList, &bstrResult);


    //TODO:  New to do something with value
    GetRequestEnableAssistModeParms parms;

    CString csResult = bstrResult;
    parms.ParseKeys(csResult);

    ::SysFreeString(bstrResult);

    return parms.IsRequestEnableAssistMode();
}



void RAProcedures::RequestEnableHandheldAssistMode(bool status)
{
    TRACE_METHOD(_T("RAProcedures::RequestEnableHandheldAssistMode"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_SET_ID, KEY_SET_ID_REQUEST_ENABLE_HH_ASSIST_MODE);
    CKeyValue<bool>::Put(parmList, KEY_EV_VALUE, status);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.Set(bstrParmList);
}


BEGIN_KV_HANDLER_CLASS(RAProcedures::GetRequestEnableHandheldAssistModeParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAProcedures::GetRequestEnableHandheldAssistModeParms, KEY_GET_VALUE, RequestEnableHandheldAssistMode)
END_KV_HANDLER_CLASS()
DEFINE_KV_BOOL(RAProcedures::GetRequestEnableHandheldAssistModeParms, RequestEnableHandheldAssistMode)


void RAProcedures::GetRequestEnableHandheldAssistModeParms::Clear()
{
    m_RequestEnableHandheldAssistMode = false;
}

bool RAProcedures::GetRequestEnableHandheldAssistMode(void)
{
    TRACE_METHOD(_T("RAProcedures::GetRequestEnableHandheldAssistMode"));

    if (!m_bRCMgrPCreated)
        return false;

    bool bRequestEnableHandheldAssistMode = false;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_GET_ID, KEY_SET_ID_REQUEST_ENABLE_HH_ASSIST_MODE);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    m_rcmi.Get(bstrParmList, &bstrResult);


    //TODO:  New to do something with value
    GetRequestEnableHandheldAssistModeParms parms;

    CString csResult = bstrResult;
    parms.ParseKeys(csResult);

    ::SysFreeString(bstrResult);

    return parms.IsRequestEnableHandheldAssistMode();
}


void RAProcedures::AcceptConnection(const CString &sMachineName, const CString &sRemoteContextName)
{
    if (!m_bRCMgrPCreated)
        return;

    m_rcmi.AcceptConnection(sMachineName, sRemoteContextName);
}


bool RAProcedures::IsRemoteMode() const
{
    if (!m_bRCMgrPCreated)
        return false;

    return m_rcmi.IsRemoteMode();
}


void RAProcedures::AcquireExclusiveAccess(const CString &sRemoteConnectionName)
{
    TRACE_METHOD(_T("RAProcedures::AcquireExclusiveAccess"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_ACQUIRE_EXCLUSIVE_ACCESS );
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_REMOTE_CONNECTION_NAME, (const _TCHAR *)sRemoteConnectionName);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    HRESULT hr = m_rcmi.OnControl(bstrParmList, &bstrResult);

    m_bExclusiveAccessGranted = (hr == S_OK);
	if(m_bExclusiveAccessGranted){
		trace( L6, _T("RAProcedures::AcquireExclusiveAccess setting report operator."));
		rp.SetOperator(ra.getOperatorID(sRemoteConnectionName));  //TAR381428
	}
    ::SysFreeString(bstrResult);

    return;
}


void RAProcedures::ReleaseExclusiveAccess()
{
    TRACE_METHOD(_T("RAProcedures::ReleaseExclusiveAccess"));

    if (!m_bRCMgrPCreated)
        return;

    if (!m_bExclusiveAccessGranted)
        return;

    m_bExclusiveAccessGranted = false;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_RELEASE_EXCLUSIVE_ACCESS );
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    HRESULT hr = m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);

    return;
}


void RAProcedures::ReportInterventionStart( long lId, DWORD dwPriority, LPCTSTR pcszCategoryName )
{
    trace( L6, _T("RAProcedures::ReportInterventionStart id=%d  priority=%d  category=%s")
             , lId, dwPriority, pcszCategoryName);

    long lCategory = GetCategory(pcszCategoryName);
    CString csReportDesc = mp.sCurr->GetCategory(lCategory);
	rp.InterventionStart(csReportDesc, lId, dwPriority);
// RFQ 7477
    // Convert the category bitmap to a numeric value within the range 
    // supported by the short int Override type.  To distinguish between
    // the View based and Category based overrides, we will add 1000 (decimal)
    // to the value
    long lOverride = RA_CAT_UNBITMAP(lCategory) + 1000;

    SaveInterventionForTLOG(lId, lOverride);
// RFQ 7477
}

void RAProcedures::ReportInterventionStart( long lId, DWORD dwPriority, int nView, LPCTSTR pcszReportCategoryName )
{
    CString csReportDesc = pcszReportCategoryName;
    if ( csReportDesc.IsEmpty())
    {
        csReportDesc = mp.sCurr->GetDescFromView(nView);
    }


    trace( L6, _T("RAProcedures::ReportInterventionStart id=%d  priority=%d  category=%s")
             , lId, dwPriority, (const _TCHAR *)csReportDesc);

    //TAR340654-
	//+TAR408991
    if ( nView == DATANEEDED_CLOSELANE)
	{
		trace ( L6, _T("RAProcedures::ReportInterventionStart: no need to log this intervention "));
		return;  // Ignore closed lane intervention for reporting purpose
	}
	//-TAR408991
	rp.InterventionStart(csReportDesc, lId, dwPriority);
    SaveInterventionForTLOG(lId, nView); // RFQ 7477
}
void RAProcedures::ReportInterventionEnd( long lId, int nView )
{

	//+tar 393092: transaction xml file error one sided entry
	// if nView is already in DATANEEDED_NOVIEW, it will use the interventionView that
	//	was save earlier (from OnRemoteApproval)
    int iView;		
	if(nView == DATANEEDED_NOVIEW) 
    {
		iView = m_nInterventionView;
    }
	else
	{
		iView = nView;
	}
	CString csReportDesc = mp.sCurr->GetDescFromView(iView);  
	//-tar 393092: transaction xml file error one sided entry

    trace( L6, _T("RAProcedures::ReportInterventionEnd id=%d category=%s")
             , lId, (const _TCHAR *)csReportDesc);
	//TAR340654+
	//trace ( L6, _T("RAProcedures::ReportInterventionEnd exception type = %d"), secEventParms.m_csExceptionType);
	if ( iView ==  DATANEEDED_MISMATCHWEIGHT) //tar 393092
	{
		if ( !(SMStateBase::m_bCurrentIntervention_MisMatch) || (lMisMatchId != lId) )
                       {
			trace ( L6, _T("RAProcedures::ReportInterventionEnd: no need to log InterventionEnd "));
			rp.SetInterventionEndInProgress(false); //429111
			return;
		}
		else
		{
			SMStateBase::m_bCurrentIntervention_MisMatch = false;
			lMisMatchId = 0;

            //+SR814.5 SR815.6 SR851 Personalization and Security           
            if( (rp.GetInterventionTerminalNumber() == _T("RAP")) || (rp.GetOperator() != EMPTY_STRING) )
            {
                custInfo.IncrementMetricLong( _T("WeightMismatchCountOperatorCleared"), 1);
                trace ( L6, _T("Incremented Value of WeightMismatchCountOperatorCleared: %d"),custInfo.GetMetricLong(_T("WeightMismatchCountOperatorCleared")));
            }
            else
            {
                custInfo.IncrementMetricLong( _T("WeightMismatchCountUserCleared"), 1); 
                trace ( L6, _T("Incremented Value of WeightMismatchCountUserCleared: %d"),custInfo.GetMetricLong(_T("WeightMismatchCountUserCleared")));
            }
            //-SR814.5 SR815.6 SR851 Personalization and Security
		}
	}

    if ( iView == DATANEEDED_UNEXPECTEDINCREASE)//tar 393092
	{
		if ( !(SMStateBase::m_bCurrentIntervention_UnexIncrease)|| (lUnexIncreaseId != lId))
                        {
			trace ( L6, _T("RAProcedures::ReportInterventionEnd: no need to log InterventionEnd "));
			rp.SetInterventionEndInProgress(false); //429111
			return;
		}
		else
		{
			SMStateBase::m_bCurrentIntervention_UnexIncrease = false;
			lUnexIncreaseId = 0;

            //+SR814.5 SR815.6 SR851 Personalization and Security
            if( (rp.GetInterventionTerminalNumber() == _T("RAP")) || (rp.GetOperator() != EMPTY_STRING) )
            {
                custInfo.IncrementMetricLong( _T("UnexpectedIncreaseCountOperatorCleared"), 1); 
                trace ( L6, _T("Incremented Value of UnexpectedIncreaseCountOperatorCleared: %d"),custInfo.GetMetricLong(_T("UnexpectedIncreaseCountOperatorCleared")));
            }
            else
            {
                custInfo.IncrementMetricLong( _T("UnexpectedIncreaseCountUserCleared"), 1); 
                trace ( L6, _T("Incremented Value of UnexpectedIncreaseCountUserCleared: %d"),custInfo.GetMetricLong(_T("UnexpectedIncreaseCountUserCleared")));
            }
            //-SR814.5 SR815.6 SR851 Personalization and Security
		}
	}
    if ( iView == DATANEEDED_UNEXPECTEDDECREASE )//tar 393092
	{

		if ( !(SMStateBase::m_bCurrentIntervention_UnexDecrease) || (lUnexDecreaseId != lId))
        {
			trace ( L6, _T("RAProcedures::ReportInterventionEnd: no need to log InterventionEnd "));
			rp.SetInterventionEndInProgress(false); //429111
			return;
		}
		else
		{
			SMStateBase::m_bCurrentIntervention_UnexDecrease = false;
			rp.SetInterventionEndInProgress(false); //429111
			lUnexDecreaseId = 0;

            //+SR814.5 SR815.6 SR851 Personalization and Security
            if( (rp.GetInterventionTerminalNumber() == _T("RAP")) || (rp.GetOperator() != EMPTY_STRING) )
            {
                custInfo.IncrementMetricLong( _T("UnexpectedDecreaseCountOperatorCleared"), 1); 
                trace ( L6, _T("Incremented Value of UnexpectedDecreaseCountOperatorCleared: %d"),custInfo.GetMetricLong(_T("UnexpectedDecreaseCountOperatorCleared")));
            }
            else
            {
                custInfo.IncrementMetricLong( _T("UnexpectedDecreaseCountUserCleared"), 1); 
                trace ( L6, _T("Incremented Value of UnexpectedDecreaseCountUserCleared: %d"),custInfo.GetMetricLong(_T("UnexpectedDecreaseCountUserCleared")));
            }
            //-SR814.5 SR815.6 SR851 Personalization and Security
		}
	}
    //TAR340654-
    if ( iView == DATANEEDED_DONTBAGTHRESHOLDEXCEEDED )//TAR433737
	{

		if ( !(SMStateBase::m_bCurrentIntervention_DontBagThreshold) || (lDontBagId != lId))
        {
			trace ( L6, _T("RAProcedures::ReportInterventionEnd: no need to log InterventionEnd "));
			rp.SetInterventionEndInProgress(false); //SSCOADK-5975
			return;
		}
		else
		{
			SMStateBase::m_bCurrentIntervention_DontBagThreshold = false;
			lDontBagId = 0;
		}
	}
	//+TAR408991
	if ( iView == DATANEEDED_CLOSELANE)
	{
		trace ( L6, _T("RAProcedures::ReportInterventionEnd: no need to log InterventionEnd "));
		rp.SetInterventionEndInProgress(false); //429111
		return;  // Ignore closed lane intervention for reporting purpose
	}
	//-TAR408991

    mp.sCurr->g_bInterventionApproval=false; //TAR380602/379317

    //TAR 418827+
    CString csCurrentStateName = mp.sCurr->GetRuntimeClass()->m_lpszClassName;
    if(csCurrentStateName == _T("SMSmAuthorization"))
    {
        mp.sCurr->UpdateStateName("");
    }
    //TAR 418827-
	rp.InterventionEnd(csReportDesc, lId);
	SendInterventionToTLOG(lId, nView); // RFQ 7477
}

void RAProcedures::ReportInterventionEnd( long lId, LPCTSTR pcszCategoryName )
{
    long lCategory = GetCategory(pcszCategoryName);
    CString csReportDesc = mp.sCurr->GetCategory(lCategory);

    trace( L6, _T("RAProcedures::ReportInterventionEnd id=%d category=%s")
             , lId, (const _TCHAR *)csReportDesc);

	rp.InterventionEnd(csReportDesc, lId);
    //TAR 418827+
    CString csCurrentStateName = mp.sCurr->GetRuntimeClass()->m_lpszClassName;
    if(csCurrentStateName == _T("SMSmAuthorization"))
    {
        mp.sCurr->UpdateStateName("");
    }
    //TAR 418827-
// RFQ 7477
    // Convert the category bitmap to a numeric value within the range 
    // supported by the short int Override type.  To distinguish between
    // the View based and Category based overrides, we will add 1000 (decimal)
    // to the value
    long lOverride = RA_CAT_UNBITMAP(lCategory) + 1000;

    SendInterventionToTLOG(lId, lOverride);
// RFQ 7477
}

//TAR 364905 Begin
void RAProcedures::ForceApprovalBeforeSuspend(bool status)
{
    TRACE_METHOD(_T("RAProcedures::ForceApprovalBeforeSuspend"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_SET_ID, KEY_SET_ID_FORCE_APPROVAL_BEFORE_SUSPEND);
    CKeyValue<bool>::Put(parmList, KEY_EV_VALUE, status);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.Set(bstrParmList);
}
//TAR 364905 End

//+SR831
void RAProcedures::OnDisableHandlingAtLocation(CString desc )
{
    TRACE_METHOD(_T("RAProcedures::OnDisableHandlingAtLocation"));
    trace( L6, _T("RAProcedures::OnDisableHandlingAtLocation desc=%s"), desc);

    if (!m_bRCMgrPCreated)
        return;

    const _TCHAR *pszState = _T("");

    if(desc == _T("SMDataEntryGoToLane"))
        pszState = KEY_EV_TYPE_DISABLE_RAP_AND_HHRAP_HANDLING;
    else if(desc == _T("SMDataEntryGoToLaneOrRap"))
        pszState = KEY_EV_TYPE_DISABLE_HHRAP_HANDLING;
    else if(desc == _T("SMDataEntryGoToLaneOrHHRap"))
        pszState = KEY_EV_TYPE_DISABLE_RAP_HANDLING;
    else
        pszState = KEY_EV_TYPE_RESET_DISABLE_HANDLING;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, pszState);
    _bstr_t bstrParmList((const _TCHAR *)parmList); 
    m_rcmi.OnEvent(bstrParmList);
}

void RAProcedures::OnEnableHandlingAtLocation( void )
{
    TRACE_METHOD(_T("RAProcedures::OnEnableHandlingAtLocation"));

    if (!m_bRCMgrPCreated)
        return;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_RESET_DISABLE_HANDLING);
    _bstr_t bstrParmList((const _TCHAR *)parmList); 
    m_rcmi.OnEvent(bstrParmList);
}
//-SR831


CString RAProcedures::getOperatorID(CString csRapName)
{
    for(int i=0;i < csRapOperatorIDs.size();i++)
    {
        
        if(csRapOperatorIDs[i].csRapName == csRapName)
        {
            trace( L7, _T("RAProcedures::getOperatorID() returns RAP %s - %s") , csRapName, csRapOperatorIDs[i].csRapOPID);
            return csRapOperatorIDs[i].csRapOPID;
        }
    }
    return _T("NotFound");
}

void RAProcedures::addOperatorID(CString csRapName, CString csOpID)
{
    trace( L7, _T("RAProcedures::addOperatorID() for RAP %s - %s") , csRapName,csOpID);
        
    if(getOperatorID(csRapName) != _T("NotFound"))
    {
        
        for(int i=0;i < csRapOperatorIDs.size();i++)
        {
        
            if(csRapOperatorIDs[i].csRapName == csRapName) 
            {
                csRapOperatorIDs[i].csRapOPID = csOpID;
            }
        }
    }
    else{
        RapOperator newOperator;
        newOperator.csRapName = csRapName;
        newOperator.csRapOPID = csOpID;
        csRapOperatorIDs.push_back(newOperator);
    }

}

//+SR819
bool RAProcedures::getRAPValidHeadCashier(CString csRapName)
{
    for(int i=0;i < csRapOperatorIDs.size();i++)
    {
        
        if(csRapOperatorIDs[i].csRapName == csRapName)
        {
            trace( L7, _T("RAProcedures::getRAPValidHeadCashier() returns RAP %s - %d") , csRapName, csRapOperatorIDs[i].bRapValidHeadCashier);
            return csRapOperatorIDs[i].bRapValidHeadCashier;
        }
    }
    return false;
}

void RAProcedures::addRAPValidHeadCashier(CString csRapName, bool csOpType)
{
    trace( L7, _T("RAProcedures::addRAPValidHeadCashier() for RAP %s - %d") , csRapName,csOpType);
        
    if(getOperatorID(csRapName) != _T("NotFound"))
    {
        
        for(int i=0;i < csRapOperatorIDs.size();i++)
        {
        
            if(csRapOperatorIDs[i].csRapName == csRapName) 
            {
                csRapOperatorIDs[i].bRapValidHeadCashier = csOpType;
            }
        }
    }
    else{
        RapOperator newOperator;
        newOperator.csRapName = csRapName;
        newOperator.bRapValidHeadCashier = csOpType;
        csRapOperatorIDs.push_back(newOperator);
    }

}
//-SR819

//sscoadk-321+
BOOL RAProcedures::OnNoSaleApproval( void )
{
    TRACE_METHOD(_T("RAProcedures::OnAgeApproval"));

    return OnApproval(KEY_APPROVAL_TYPE_NOSALE);
}
//sscoadk-321-

//sscoadk-321+
void RAProcedures::OnNoSale( LPCTSTR sDesc )
{
    TRACE_METHOD(_T("RAProcedures::OnNoSale"));

    CKeyValueParmList parmList;
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, sDesc);
  

    AddApproval( parmList, KEY_APPROVAL_TYPE_NOSALE );
}
//sscoadk-321-

#endif //_CPPUNIT

void RAProcedures::OnGenericDelayedIntervention(
   LPCTSTR  sDesc,
   LPCTSTR  sID,
   LPCTSTR  sInterventionViewString,
   LPCTSTR  sInterventionSource,
   CString  sInterventionHandler,
   LPCTSTR  sReportCategoryName,
   BOOL     bClearImmediate,
   long     lRptID
   )
{
    TRACE_METHOD(_T("RAProcedures::OnGenericDelayedIntervention"));
    m_lInterventionCount++;

    const _TCHAR *pszState = _T("");

    long lInterventionHandle = 1;

    if(sInterventionHandler == _T("SMDataEntryGoToLane"))
        lInterventionHandle = 2;
    else if(sInterventionHandler == _T("SMDataEntryGoToLaneOrRap"))
        lInterventionHandle = 3;
    else if(sInterventionHandler == _T("SMDataEntryGoToLaneOrHHRap"))
        lInterventionHandle = 4;
    else
        lInterventionHandle = 1;

    long lReportID = lRptID;
    if (!lReportID)
    {
        lReportID = m_lInterventionCount;
    }



    CKEYVALUEPARMLIST parmList;
    CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_EV_PRIORITY, KEY_EV_PRIORITY_DELAYED);
    CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_EV_DESCRIPTION, sDesc);
    CKEYVALUE<long>::Put(parmList, KEY_EV_COOKIE, lReportID);
    CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_EV_STRING_ID, sID);
    CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_EV_VIEW_TEXT, sInterventionViewString);
    CKEYVALUE<int>::Put(parmList, KEY_EV_VIEW, DATANEEDED_GENERICDELAYED);
    CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_GENERIC_DELAYED);
    CKEYVALUE<long>::Put(parmList, KEY_EV_HANDLE_TYPE, lInterventionHandle);


    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.OnEvent(bstrParmList, RAP_GENERIC_DELAYED);
#ifndef _CPPUNIT
    if (_tcslen(sReportCategoryName) == 0)
    {
        ReportInterventionStart( lReportID, RA_PRIORITY_DEFER, KEY_EV_TYPE_GENERIC_DELAYED );
    }
    else
    {
        ReportInterventionStart( lReportID, RA_PRIORITY_DEFER, DATANEEDED_GENERICDELAYED, sReportCategoryName );
    }
#endif
}

int RAProcedures::GetRAPGenericDelayedPendingCount(void) const
{
    if (!m_bRCMgrPCreated)
        return -1;

    return m_rcmi.GetRAPGenericDelayedPendingCount();
}

void RAProcedures::OnGenericDelayedApproval(LPCTSTR  sID, int nRptID, bool bRemote)
{
    TRACE_METHOD(_T("RAProcedures::OnGenericDelayedApproval"));

    if (!m_bRCMgrPCreated)
	{
		//TAR 371932 & tar 371918 added trace
		TRACE_METHOD(_T("RAProcedures::OnGenericDelayedApproval,m_bRCMgrPCreated is false"));
  
		return;
	}

    CKEYVALUEPARMLIST parmList;
    CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_EV_TYPE, KEY_EV_TYPE_GENERIC_DELAYED);
    CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_EV_STRING_ID, sID);
    CKEYVALUE<int>::Put(parmList, KEY_EV_COOKIE, nRptID);
    CKEYVALUE<bool>::Put(parmList, KEY_APPROVAL_TYPE_REMOTE, bRemote);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    m_rcmi.DecrementRAPGenericDelayedPendingCount();
    OnApproval(bstrParmList, true);

}

void RAProcedures::OnAssistModeAvailable()
{
    TRACE_METHOD(_T("RAProcedures::OnAssistModeAvailable"));

    if (!m_bRCMgrPCreated)
        return;


    CKEYVALUEPARMLIST parmList;
    CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_XACT_ASSISTMODEAVAILABLE);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);
}

void RAProcedures::OnAssistModeNotAvailable()
{
    TRACE_METHOD(_T("RAProcedures::OnAssistModeNotAvailable"));

    if (!m_bRCMgrPCreated)
        return;


    CKEYVALUEPARMLIST parmList;
    CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_XACT_ASSISTMODENOTAVAILABLE);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);
}
// RFQ 7477
void RAProcedures::SaveInterventionForTLOG(long lId, long lOverride)
{
    // Get the in-memory record
    CSavedInterventionMap::iterator iter = m_savedIntMap.find(lId);
    if( iter == m_savedIntMap.end() )
    {
        // +SOTF 6312
		SavedIntervention si;
        si.csManager = _T("");
        si.lLocation = si.lOverride = si.lStartTime = si.lTerminal = si.lWaitTime = 0;
		// -SOTF 6312

        m_savedIntMap.insert(std::make_pair(lId, si));
        iter = m_savedIntMap.find(lId);
    }  

    iter->second.lOverride = lOverride;
    iter->second.lStartTime = GetTickCount();
}

void RAProcedures::SendInterventionToTLOG(long lId, long lOverride)
{	
    // Get the in-memory record
    CSavedInterventionMap::iterator iter = m_savedIntMap.find(lId);
    if( iter != m_savedIntMap.end() )
    {
        if( rp.GetOperator().GetLength() > 0 )
        {
            if( mp.sCurr->bIsTransactionStarted )
            {
				// +SOTF 6312
                CString csEmplid;
                if( TB_SUCCESS_IMMEDIATE ==  mp.sCurr->SendTBCommand(TBCMD_IS_MEIJER_BADGE, rp.GetOperator()) )
                {
                    if( TB_SUCCESS_IMMEDIATE ==  mp.sCurr->SendTBCommand(TBCMD_GET_EMPLID_FROM_BADGE, rp.GetOperator(), &csEmplid) )
                    {
                        trace(L0, _T("SendInterventionToTLOG: Retrieved Emplid from badge id and storing ")
                                  _T("as manager in TLOG override record."));
                        iter->second.csManager = csEmplid;
                    }
                    else
                    {
                        trace(L0, _T("SendInterventionToTLOG: Failed to convert badge id to Emplid. ")
                                  _T("Storing badge value as manager in TLOG override record."));
                        iter->second.csManager = rp.GetOperator();
                    }
                }
                else
                {
                    iter->second.csManager = rp.GetOperator();
                }

				// -SOTF 6312
                iter->second.lOverride = lOverride;

                //SOTF 7242 
                //keep the total time in milliseconds and send to ACS
//                 iter->second.lWaitTimeMs = (GetTickCount() - iter->second.lStartTime);
//                 double dWaitTime = (iter->second.lWaitTimeMs / 1000.0);
//                 
//                 double dTemp;
//                 modf(dWaitTime, &dTemp);
//                 iter->second.lWaitTime = (long) dTemp;
                //sotf                 
        
                // Determine location:1 = FastLane, 2 = HHRAP, 3 = RAP
                if( m_bIsRemoteApproval )
                {
                    if( ra.IsRemoteConnectionHandheld(ra.GetRemoteConnectionName()) )
                    {
                        iter->second.lLocation = 2;
                    }
                    else
                    {
                        iter->second.lLocation = 3;
                    }
                }
                else
                {
                    iter->second.lLocation = 1;
                }    
                iter->second.lTerminal = _ttol(rp.GetTerminalNumber());

				if(iter->second.lWaitTime < 600)
				{
					trace(L0, _T("RAProcedures::SendInterventionToTLOG(%s,%d,%d,%d,%d, %d)"),//SOTF 6312 7242
						iter->second.csManager,    //SOTF 6312
						iter->second.lOverride,
						iter->second.lLocation,
						iter->second.lWaitTime,
						iter->second.lTerminal,
						iter->second.lWaitTimeMs); //SOTF 7242
					mp.sCurr->SendTBCommand(TBCMD_TLOG_OVERRIDE,
						iter->second.csManager,    //SOTF 6312
						iter->second.lOverride,
						iter->second.lLocation,
						iter->second.lWaitTime,
						iter->second.lTerminal,
							iter->second.lWaitTimeMs); //SOTF 7242
				}
				else
				{
					trace(L0, _T("Intervention took more than 10 mins. Sending wait time as 0."));
					mp.sCurr->SendTBCommand(TBCMD_TLOG_OVERRIDE,
						iter->second.csManager,
						iter->second.lOverride,
						iter->second.lLocation,
						0,
						iter->second.lTerminal,
						0);
				}
            }
            else
            {
                trace(L0, 
                    _T("RAProcedures::SendInterventionToTLOG id=%d Override=%d.")
                    _T("Not sending to TLOG; Intervention occurred outside transaction."),
                    lId, lOverride);
            }

        }
        else
        {
            trace(L0, 
            _T("RAProcedures::SendInterventionToTLOG id=%d Override=%d.")
            _T("Not sending to TLOG; Intervention has no operator."),
            lId, lOverride);
        }

        // Intervention Start/End has been processed. Remove it from the list.
        m_savedIntMap.erase(iter);
    }
    else
    {
        trace(L0, 
            _T("RAProcedures::SendInterventionToTLOG id=%d failed to")
            _T(" send TLOG record. InterventionStart not found. (It may have already been sent.)"),
            lId);
    }

	m_bIsRemoteApproval = false;
}

CString RAProcedures::GetRegString(LPCTSTR szKey, LPCTSTR szName)
{
    long lRetVal;
    HKEY hKey;
    CString csRet = _T("");
    
    lRetVal = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, KEY_QUERY_VALUE, &hKey);

    if( lRetVal == ERROR_SUCCESS )
    {
        TCHAR szData[500] = {0};
        DWORD dwSize = sizeof(szData);
        lRetVal = ::RegQueryValueEx(hKey, szName, NULL, NULL, (LPBYTE)szData, &dwSize);
        if( lRetVal == ERROR_SUCCESS )
        {
            csRet = szData;
        }
        else
        {
            trace(L4, _T("Failed to read the registry value %s\\%s"), szKey, szName);
        }
        RegCloseKey(hKey);
    }
    else
    {
        trace(L4, _T("Failed to open the HKLM registry key for reading."));
    }
    
    return csRet;
}

void RAProcedures::SetRegString(LPCTSTR szKey, LPCTSTR szName, LPCTSTR szValue)
{
    long lRetVal;
    HKEY hKey;
    
    lRetVal = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, KEY_WRITE, &hKey);

    if( lRetVal == ERROR_SUCCESS )
    {
        DWORD dwSize = (_tcslen(szValue)+1)*sizeof(szValue[0]);
        lRetVal = ::RegSetValueEx(hKey, szName, 0, REG_SZ, (const LPBYTE)szValue, dwSize);
        if( lRetVal != ERROR_SUCCESS )
        {
            trace(L6, _T("Failed to write the registry value %s\\%s=%s"), szKey, szName, szValue);
        }
        RegCloseKey(hKey);
    }
    else
    {
        trace(L4, _T("Failed to open the HKLM registry key for writing."));
    }
}

void RAProcedures::SetIsRemoteApprovalFlag(bool bValue)
{
  m_bIsRemoteApproval = bValue;  
}

//POS81634
// Handling check-in context coming from the RAP station
BOOL RAProcedures:: CheckIn( PPSXEVENT pEvent )
{
    if (!m_bRCMgrPCreated)
        return false;

    CString strRemoteConnectionName = pEvent->strRemoteConnectionName;
	int dLaunchpadPrefixLen = _tcslen(LAUNCHPAD_PREFIX);		// Fix Unicode problem

    if ((m_connections.find(strRemoteConnectionName) == m_connections.end()) &&
        ( pEvent->strRemoteConnectionName.Left(dLaunchpadPrefixLen) != LAUNCHPAD_PREFIX ) )
        return false;

    CString vParam = (const _TCHAR *)(_bstr_t)pEvent->vParam;
    SignonParms parms;
    parms.Clear();

    parms.ParseKeys((const _TCHAR *)vParam);

    if (!parms.GetScannerData().IsEmpty())
    {
        ScannerParms parms;
        parms.Clear();
        parms.ParseKeys((const _TCHAR*)(_bstr_t)pEvent->vParam);

        CString szScannerData = parms.GetScannerData();
        switch (parms.GetType())
        {
            case 104 :
                if (szScannerData[0] != _T('F'))
                    szScannerData = _T('F') + szScannerData;
                break;
            default :
                break;
        }

        return ScannerData( _T("cSignonScanData")
                         , szScannerData
                         , (const TCHAR *)parms.GetLabelData()
                         , parms.GetType()
                         , strRemoteConnectionName);
    }

	BOOL bValidOperator;
	
    int nRet =  -99;
    int currentRAPid =  -99;
    int offlineStatus = -99;


		//Start CRD 345698
		CString strGrocery, strGM, strScanPortal;
		strGrocery = GetHookProperty(_T("RAPAttendantTrackingGrocery"));
		strGM = GetHookProperty(_T("RAPAttendantTrackingGM"));
		strScanPortal = GetHookProperty(_T("RAPAttendantTrackingScanPortal"));
		//End CRD 345698

		CString csRAPOrigin = strRemoteConnectionName.Mid(16); 
		int nRAPOrigin = _ttol(csRAPOrigin);
		trace(L6, _T("Store Section RAP, Grocery=%s, GM=%s, ScanPortal=%s, Origin=%s, %d"), strGrocery, strGM, strScanPortal, csRAPOrigin, nRAPOrigin);

		nRet = mp.sCurr->SendTBCommand(TBCMD_CHECKINOUT_OPERATOR, 1, (const TCHAR *)parms.GetUserId(), (const TCHAR *)parms.GetUserPwd(), nRAPOrigin, &currentRAPid, &offlineStatus);
		trace(L6, _T("Checkin on RAP%d: nRet=%d; currentRAPid=%d; offlineStatus=%d"),nRAPOrigin, nRet,currentRAPid,offlineStatus);

		CString csRAPOriginID, csRAPCurrentID;
		csRAPOriginID.Format(_T("RAP%d"), nRAPOrigin);		//CRD 345698
		csRAPCurrentID.Format(_T("RAP%d"), currentRAPid);	//CRD 345698
    
		// check in is successfull, write to TLOG
		if( nRet == 0 )
		{
			bValidOperator = (BOOL)true;
        
			CString csTimeStamp;
			SYSTEMTIME st;
			GetLocalTime(&st);
			csTimeStamp.Format(_T("%02d/%02d/%02d %02d:%02d:%02d"), st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute, st.wSecond);

			CString csCurrentVirtualRAPID;
			//Start CRD 345698        
			csCurrentVirtualRAPID = GetHookProperty(csRAPCurrentID);
			csCurrentVirtualRAPID = csCurrentVirtualRAPID.Mid(0,csCurrentVirtualRAPID.Find(_T(":")));
			//End CRD 345698

			trace(L6, _T("Valid operator, Checked in to %s, Virtual RAPID(%s), TimeStamp(%s)"),csRAPCurrentID,csCurrentVirtualRAPID,csTimeStamp);
        
			int nVirtualRAPID = _ttol(csCurrentVirtualRAPID);

			//CRD 392418
			bool bAttendantTrackingEnabled = GetHookProperty(_T("RAPAttendantTrackingEnabled")) == _T("1") ? true: false;
			if(bAttendantTrackingEnabled)
			{
				mp.sCurr->SendTBCommand(TBCMD_NOTIFY_CHECKINOUT, (const TCHAR *)parms.GetUserId(), (const TCHAR *)csTimeStamp, 1, currentRAPid, nVirtualRAPID, offlineStatus);
			}
		}
		// attendant has been checked in to different store section
		else if( nRet == 2 )
		{
			trace(L6, _T("RAPOrigin(%s), RAPCurrent(%s)"), csRAPOriginID, csRAPCurrentID);
			// if the attendant has checked in to the different RAP but in the same section, then just pass it through and no need to check out
			// !!! This part is needed to be commented out for testing, uncomment it when checking in !!!!
			if( (strGrocery.Find(csRAPOriginID) != -1 && strGrocery.Find(csRAPCurrentID) != -1 ) ||
				(strGM.Find(csRAPOriginID) != -1 && strGM.Find(csRAPCurrentID) != -1 ) ||
				(strScanPortal.Find(csRAPOriginID) != -1 && strScanPortal.Find(csRAPCurrentID) != -1 ))
			{
				nRet = 0;
				bValidOperator = (BOOL)true;
				trace(L6, _T("RAP Attendant has been checked in to the same store section area, check in successfull"));
			}
			else
			{           
				// send the invalid flag to the RAP with the return code=2 and aux data=current RAP ID so that RAP could display the Check out confirmation
				bValidOperator = (BOOL)false;
				trace(L6, _T("RAP Attendant has been checked in to the different store area, check in not successfull"));
			}   
		}
		else
		{
			bValidOperator = (BOOL)false;
			trace(L6, _T("InValid operator, NOT Checked in"));
		}
       
	CString csEmplid;
    if( TB_SUCCESS_IMMEDIATE ==  mp.sCurr->SendTBCommand(TBCMD_IS_MEIJER_BADGE, rp.GetOperator()) )      
    {
        if( TB_SUCCESS_IMMEDIATE ==  mp.sCurr->SendTBCommand(TBCMD_GET_EMPLID_FROM_BADGE, (const TCHAR *)parms.GetUserId(), &csEmplid) )           
        {
            return CheckIn(strRemoteConnectionName, csEmplid, (const TCHAR *)parms.GetUserPwd(), bValidOperator, nRet, (const TCHAR *)csRAPCurrentID );
        }
    }

    return CheckIn(strRemoteConnectionName, (const TCHAR *)parms.GetUserId(), (const TCHAR *)parms.GetUserPwd(), bValidOperator, nRet, (const TCHAR *)csRAPCurrentID);
}

BOOL RAProcedures::CheckIn( const CString &sRemoteConnectionName
                         , const CString &sUserId
                         , const CString &sUserPwd
                         , BOOL bValid
                         , int iValid
                         , const CString &sReason)
{
    trace(L6, _T("Checkin with RemoteConnName(%s), UserID(%s), Valid(%d), ResponseCode(%d), RAPID(%s)"), sRemoteConnectionName,sUserId,bValid,iValid, sReason);
    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_SIGNON );
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_REMOTE_CONNECTION_NAME, (LPCTSTR)sRemoteConnectionName);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OP_ID, (LPCTSTR)sUserId);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OP_PASSWORD, (LPCTSTR)sUserPwd);
    CKeyValue<BOOL>::Put(parmList, KEY_CT_OP_VALID, bValid);
    CKeyValue<int>::Put(parmList, KEY_AP_APPROVAL_CODE, iValid);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_AP_AUX_DATA, (LPCTSTR)sReason);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    HRESULT hr = m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);

    return (hr == S_OK);
}

bool RAProcedures::CheckOut( PPSXEVENT pEvent )
{
    if (!m_bRCMgrPCreated)
        return false;    

    CString vParam = (const _TCHAR *)(_bstr_t)pEvent->vParam;
    SignonParms parms;
    parms.Clear();

    parms.ParseKeys((const _TCHAR *)vParam);

    int nRet =  -99;
    int currentRAPid =  -99;
    int offlineStatus = -99;    

    CString strRemoteConnectionName = pEvent->strRemoteConnectionName;
    CString csRAPOrigin = strRemoteConnectionName.Mid(16); 
    int nRAPOrigin = _ttol(csRAPOrigin);
    
    CString csRAPCurrent = pEvent->strControlName;
    trace(L6, _T("Checkout from RAP%d, RAP Current ID=%s"), nRAPOrigin, csRAPCurrent);
    if( csRAPCurrent != _T("") )
    {
        nRAPOrigin = _ttol(csRAPCurrent.Mid(3));
    }

    nRet = mp.sCurr->SendTBCommand(TBCMD_CHECKINOUT_OPERATOR, 0, (const TCHAR *)parms.GetUserId(), (const TCHAR *)parms.GetUserPwd(), nRAPOrigin, &currentRAPid, &offlineStatus);
    trace(L6, _T("Checkout on RAP nRet=%d; currentRAPid=%d; offlineStatus=%d"),nRet,currentRAPid,offlineStatus);

    CString csRAPCurrentID;
    csRAPCurrentID.Format(_T("RAP%d"), currentRAPid);	//CRD 345698

    BOOL bCheckoutSuccess;
    if( nRet == 0 || nRet == 2)
    {
        bCheckoutSuccess = (BOOL)true;

        // if checkout is success then send TLOG information back to POS
        if( nRet == 0 )
        {
            CString strSectionName = _T("RAPAttendantTracking");

            CString csTimeStamp;
            SYSTEMTIME st;
            GetLocalTime(&st);
            csTimeStamp.Format(_T("%02d/%02d/%02d %02d:%02d:%02d"), st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute, st.wSecond);

			//Start CRD 345698
            CString csCurrentVirtualRAPID;
            csCurrentVirtualRAPID = GetHookProperty(csRAPCurrentID);
			csCurrentVirtualRAPID = csCurrentVirtualRAPID.Mid(0,csCurrentVirtualRAPID.Find(_T(":")));
			//End CRD 345698

            trace(L6, _T("Valid operator, Checked out from %s, Virtual RAPID(%s), TimeStamp(%s)"),csRAPCurrentID,csCurrentVirtualRAPID,csTimeStamp);
        
            int nVirtualRAPID = _ttol(csCurrentVirtualRAPID);

			//CRD 392418
			bool bAttendantTrackingEnabled = GetHookProperty(_T("RAPAttendantTrackingEnabled")) == _T("1") ? true: false;
			if(bAttendantTrackingEnabled)
			{
				mp.sCurr->SendTBCommand(TBCMD_NOTIFY_CHECKINOUT, (const TCHAR *)parms.GetUserId(), (const TCHAR *)csTimeStamp, 0, currentRAPid, nVirtualRAPID, offlineStatus);
			}
        }

        trace(L6, _T("Checkout success"));
    }
    else
    {
        bCheckoutSuccess = (BOOL)false;
        trace(L6, _T("Checkout Not success"));
    }

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_SIGNON );
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OP_ID, (const _TCHAR *)parms.GetUserId());
    CKeyValue<BOOL>::Put(parmList, KEY_CT_OP_VALID, bCheckoutSuccess);
    CKeyValue<int>::Put(parmList, KEY_AP_APPROVAL_CODE, nRet);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_AP_AUX_DATA, _T("Checkout"));
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    HRESULT hr = m_rcmi.OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);

    return (hr == S_OK);
}
//e POS81634
// RFQ 7477

CString RAProcedures::GetHookProperty(CString csProp)
{
    CString csValue = _T("");
    mp.sCurr->SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csProp, &csValue);
    return csValue;
}

void RAProcedures::UpdateInterventionWaitTimes( long totalWaitTimeSec )
{	
	CSavedInterventionMap::iterator iter = m_savedIntMap.begin();
	trace(L6, _T("The operator response time for current outstanding inverntion(s) is  = %d"), totalWaitTimeSec);
	//iterate through saved intervention list and update the attendant response time aka customer wait time
	while (iter != m_savedIntMap.end())
	{
		trace(L6, _T("Setting the wait time %d sec for the intervention code: %d"), totalWaitTimeSec, iter->second.lOverride);
		iter->second.lWaitTime = totalWaitTimeSec;
		iter->second.lWaitTimeMs = totalWaitTimeSec * 1000;
		iter++;
	}
}