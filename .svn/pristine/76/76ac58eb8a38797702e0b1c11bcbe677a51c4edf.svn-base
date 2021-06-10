// RAInterface.cpp: implementation of the RAInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RAInterface.h"
#include "Common.h"
#include "RCMKeyValue.h"
#include "MPProcedures.h"
#include "RAPKeyvalue.h"



#define COMP_ID ID_RA

#define T_ID _T("RCMgri")

#include "MethodTrace.h"

static const _TCHAR *RI_NULL_STRING = _T("");

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define PROCESS_EXCEPTIONS(a)                                 \
    catch(COleDispatchException * e)                              \
{                                                             \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_RC_MGR_INTERFACE_OLEDISPATCH_EXCEPTION, \
  _T("%s|%s|%x"),   \
    msg,                                                      \
    e->m_strDescription,                                      \
    e->m_scError);                                            \
  e->Delete();                                               \
  return a;                                                   \
}                                                             \
    catch(COleException* e)                                       \
{                                                             \
  e->GetErrorMessage( cstr, 255 );                           \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_RC_MGR_INTERFACE_OLE_EXCEPTION,_T("%s|%s|%x"),        \
    msg,                                                      \
    cstr,                                                     \
    e->m_sc);                                                 \
  e->Delete();                                               \
  return a;                                                   \
}                                                             \
    catch(CException* e)                                          \
{                                                             \
  e->GetErrorMessage( cstr, 255 );                           \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_RC_MGR_INTERFACE_C_EXCEPTION,_T("%s|%s"),                   \
    msg,                                                      \
    cstr);                                                    \
  e->Delete();                                               \
  return a;                                                   \
}                                                             \
    catch(...)                                                    \
{                                                             \
  long le = GetLastError();                                   \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_RC_MGR_INTERFACE_EXCEPTION,_T("%s|%d"),msg,le);            \
  return a;                                                   \
}
DLLEXPORT extern MPProcedures mp;
extern RAProcedures ra;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RAInterface::RAInterface()
    : m_bRCMgrCreated(false)
    , m_eRemoteMode(DISCONNECTED)
    , m_nRAPDataPendingCount( 0 )
    , m_nRAPGenericDelayedPendingCount( 0 )
{

}

RAInterface::~RAInterface()
{

}


LONG RAInterface::Initialize(LONG lPSXHandle)
{
    trace(L2, _T("+InitRCMgr"));
    _TCHAR msg[] = T_ID _T("::InitRCMgr");

    LONG rc = -1;	// TAR 391593 - Uninitialized var. // 396244 
        
    if ( m_bRCMgrCreated == false )
    {
        if( !m_RCMgr.Create(NULL, NULL, CRect(0,0,0,0), this, IDC_RCMANAGERCTRL1 ))
        {
            ScotError(INFO,SM_CAT_STOP_ABORT, SCOTAPP_RC_MGR_INTERFACE_STOP);//TD 05/17/02 TAR 204873
        }
        else
        {
            rc = m_RCMgr.Initialize(lPSXHandle);
            switch (rc)
            {
                case S_OK :
                case S_FALSE :
                    m_bRCMgrCreated = true;
                    break;
            }
        }

    }
	else
    {
      rc = S_OK;  // TAR 397457
    }

    if ( !m_bRCMgrCreated )
        trace(L2, _T("RemoteConsoleManager control failed to initialize.  rc=%8.8x"), rc);

    trace(L2, _T("-InitRCMgr"));

    return rc;
}


LONG RAInterface::Terminate(void)
{
    trace(L2, _T("+UnInitRCMgr"));
    _TCHAR msg[] = T_ID _T("::UnInitRCMgr");
    LONG hr = S_OK;
    try
    {
     
        if (m_bRCMgrCreated )
        {       
            trace(L2, _T("UnInitRCMgr -- before Closing Window"));
            hr = m_RCMgr.Terminate();
            if (hr != S_OK)
                trace(L2, _T("Terminate() :Error text:%d"), hr);

            trace(L2, _T("Close Window"));
            try
            {
                hr = m_RCMgr.DestroyWindow();   // true - if succesful
            }
            catch(CException* e)
            {
                _TCHAR szBuff[300];
                e->GetErrorMessage(szBuff,300);
                trace(L2, _T("Got an exception in RCMgr.DestroyWindow() :Error text:%s"), szBuff);
                e->Delete();
            }
            trace(L2, _T("after DestroyWindow"));
            m_bRCMgrCreated = false;
        }
        trace(L2, _T("UnInitRCMgr -- before PROCESS_EXCEPTIONS"));
    }
    PROCESS_EXCEPTIONS(false);
    trace(L2, _T("-UnInitRCMgr"));

    return hr;
}

CString RAInterface::MaskedParam(const CString& csParam)
{
	CString csVal(csParam);
	CString csReturn = _T("");
	CString csTemp = csVal;	
	int nLength = csVal.GetLength();	
	bool bFlag = true; 

	do{			
		int nStart = csVal.Find(cMASKED);
		if(nStart > -1)   
		{
			int nIndexEq = csVal.Find(cEQUAL, nStart);
			int nIndexCol = csVal.Find(cSEMICOLON, nIndexEq);
			if(nIndexEq > -1)
			{
				csTemp = csVal.Left(nIndexEq + 1);
				csTemp += cASTERISK;
			}

			if((nIndexCol > -1) && (nIndexCol < nLength))
	        {
				csVal = csVal.Mid(nIndexCol, nLength - nIndexCol);
		    }
			else
			{
				bFlag = false;
	        }

		}
		else
		{
			return csTemp;
		}		
		csReturn += csTemp;
	}while(bFlag);

	return csReturn;
}

LONG RAInterface::CanApprove(LPCTSTR bstrInParms, LONG* pResult)
{
    TRACE_METHOD_AT_LEVEL(_T("RAInterface::CanApprove"), 7);
    trace(L6, _T("RAInterface::CanApprove - bstrInParms: %s"), bstrInParms);
    return m_RCMgr.CanApprove(bstrInParms, pResult);
}


LONG RAInterface::GetApprovalInfo(LPCTSTR bstrInParms, BSTR* pbstrOutParms)
{
    TRACE_METHOD_AT_LEVEL(_T("RAInterface::GetApprovalInfo"), 7);
    trace(L6, _T("RAInterface::GetApprovalInfo - bstrInParms: %s"), bstrInParms);
    return m_RCMgr.GetApprovalInfo(bstrInParms, pbstrOutParms);
}


LONG RAInterface::OnApproval(LPCTSTR bstrInParms)
{
    TRACE_METHOD_AT_LEVEL(_T("RAInterface::OnApproval"), 7);
    trace(L6, _T("RAInterface::OnApproval - bstrInParms: %s"), bstrInParms);
    return m_RCMgr.OnApproval(bstrInParms);
}


LONG RAInterface::OnControl(LPCTSTR bstrInParms, BSTR* pbstrOutParms)
{
    TRACE_METHOD_AT_LEVEL(_T("RAInterface::OnControl"), 7);
	trace(L6, _T("RAInterface::OnControl - bstrInParms: %s"), MaskedParam( (CString)bstrInParms) );
    return m_RCMgr.OnControl(bstrInParms, pbstrOutParms);
}



LONG RAInterface::OnEvent(LPCTSTR bstrInParms, EEventFlags eFlags)
{
    TRACE_METHOD_AT_LEVEL(_T("RAInterface::OnEvent"), 7);

    switch (eFlags)
    {
        case NO_RAP_DATA_NEEDED :
            break;
        case RAP_DATA_NEEDED :
            m_nRAPDataPendingCount++;
            break;
        case RESET_RAP_DATA_NEEDED :
            m_nRAPDataPendingCount = 0;
            break;
        case RAP_GENERIC_DELAYED :
            m_nRAPGenericDelayedPendingCount++;
            break;
    }
    trace(L6, _T("RAInterface::OnEvent - bstrInParms: %s"), bstrInParms);
    return m_RCMgr.OnEvent(bstrInParms);
}


LONG RAInterface::Get(LPCTSTR bstrInParms, BSTR* pbstrOutParms)
{
    TRACE_METHOD_AT_LEVEL(_T("RAInterface::Get"), 7);
    trace(L6, _T("RAInterface::Get bstrInParms: %s"), bstrInParms);
    return m_RCMgr.Get(bstrInParms, pbstrOutParms);
}


LONG RAInterface::Set(LPCTSTR bstrInParms)
{
    TRACE_METHOD_AT_LEVEL(_T("RAInterface::Set"), 7);
    trace(L6, _T("RAInterface::Set - bstrInParms: %s"), bstrInParms);
    return m_RCMgr.Set(bstrInParms);
}


LONG RAInterface::OnInputEvent(LPCTSTR bstrInParms)
{
    TRACE_METHOD_AT_LEVEL(_T("RAInterface::OnInputEvent"), 7);
    trace(L6, _T("bstrInParms: %s"), bstrInParms);
    return m_RCMgr.OnInputEvent(bstrInParms);
}
/////////////////////////////////////////////////////////////////////////////
// Begin Event Sink
BEGIN_EVENTSINK_MAP(RAInterface, CWnd)
  //{{AFX_EVENTSINK_MAP(RAInterface)
    ON_EVENT(RAInterface, IDC_RCMANAGERCTRL1, 1, OnRequestApproval, VTS_BSTR)
    ON_EVENT(RAInterface, IDC_RCMANAGERCTRL1, 2, OnRequestSuspend, VTS_BSTR)
    ON_EVENT(RAInterface, IDC_RCMANAGERCTRL1, 3, OnCancelSuspend, VTS_BSTR)
    ON_EVENT(RAInterface, IDC_RCMANAGERCTRL1, 4, OnValidateOperator, VTS_BSTR)
    ON_EVENT(RAInterface, IDC_RCMANAGERCTRL1, 5, OnUnsolicitedData, VTS_BSTR)
    ON_EVENT(RAInterface, IDC_RCMANAGERCTRL1, 6, OnAssistMode, VTS_BSTR)
    ON_EVENT(RAInterface, IDC_RCMANAGERCTRL1, 7, OnRemoteConsoleEvent, VTS_BSTR)
    //}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// RCMgrInterface message handlers
void RAInterface::OnRequestApproval(LPCTSTR sParms) 
{
    trace(L6,_T("PostRCMgr OnRequestApproval:  %s"), sParms);

    m_RequestApprovalParms.Clear();
    m_RequestApprovalParms.ParseKeys(sParms);

    //Only set m_bIsInterventionApproved to true when the intervention is approved from the RAP and if it is not NEEDMOREDATA
    //Since for NEEDMOREDATA, ScotApp will always receive request Approval whether the intervention is approved or voided
    //and this is handled in SMSmDataEntryBase when the button is pressed.
    if (m_RequestApprovalParms.m_nCookie != -1 && m_RequestApprovalParms.m_nView != DATANEEDED_MOREDATA)
    {
        rp.m_bIsInterventionApproved = true;    //SR803: to report that the intervention was approved
    }
// +RFQ 7477
 //TAR383451+
	if (m_RequestApprovalParms.m_csCategory == _T("") )
	{
		m_RequestApprovalParms.m_csCategory = m_RequestApprovalParms.m_csType;
	}
	//TAR383451-
 // -RFQ 7477

    MessageElement * mePtr = new MessageElement; //create and new message element
    ZeroMemory(mePtr, sizeof(MessageElement));

    //fill in the message structure with the information pass in from the parameters of this function
    mePtr->id               = ID_RA;                //identifier, message comes from RA
    mePtr->raInfo.msg       = WMRA_REQUEST_APPROVAL;
    mePtr->raInfo.dwCookie  = m_RequestApprovalParms.m_nCookie;
//  mo.PostRCMgr( SmOnItemOK, csParms); 

    mePtr->raInfo.handle        = m_RequestApprovalParms.m_hApproval;
    mePtr->raInfo.approvalCode  = m_RequestApprovalParms.m_approvalCode;

	//TAR 381428 +
	trace(L6,_T("RAInterface::OnRequestApproval: using RAPName field"));
// +RFQ 7477 
    //SOTF 7183
    CString csOpID = ra.getOperatorID(m_RequestApprovalParms.m_csRemoteConnection);
    
    csOpID.TrimLeft();
    csOpID.TrimLeft();
    csOpID.MakeLower();

    if(csOpID == _T("notfound"))
    {
        _tcscpy(mePtr->raInfo.operatorID, m_RequestApprovalParms.m_csUserId);
    }else
    {
	_tcscpy(mePtr->raInfo.operatorID, ra.getOperatorID(m_RequestApprovalParms.m_csRemoteConnection));
    }
    //sotf	
// -RFQ 7477
	//TAR 381428 -

    mePtr->raInfo.nDataLength   = _tcslen(m_RequestApprovalParms.m_csData) + 1;
    mePtr->raInfo.pData = new TCHAR [mePtr->raInfo.nDataLength];
    memcpy((void*)mePtr->raInfo.pData, (void*)(const _TCHAR *) m_RequestApprovalParms.m_csData, mePtr->raInfo.nDataLength * sizeof(TCHAR));

    if (m_RequestApprovalParms.m_approvalInfo.bSigCapApprovalRequired)
    {
        m_RequestApprovalParms.m_approvalInfo.bSigCapRedoRequired 
            = (m_RequestApprovalParms.m_csData.Find(_T("cRedo")) != -1) ? TRUE : FALSE;
    }

    mo.PostIt(mePtr);
}

/////////////////////////////////////////////////////////////////////////////
void RAInterface::OnRequestSuspend(LPCTSTR sParms) 
{
    trace(L6,_T("PostRCMgr OnRequestSuspend sParms=%s"), sParms);

    RAEventParms parms;

    parms.ParseKeys(sParms);

    MessageElement * mePtr = new MessageElement; //create and new message element
    ZeroMemory(mePtr, sizeof(MessageElement));

    //fill in the message structure with the information pass in from the parameters of this function
    mePtr->id               = ID_RA;                //identifier, message comes from RA
    mePtr->raInfo.msg       = WMRA_REQUEST_SUSPEND;
    mePtr->raInfo.dwCookie  = parms.m_nCookie;

    int nDataLength         = _tcslen(m_RequestApprovalParms.m_csData); 
    mePtr->raInfo.nDataLength   = nDataLength + 1;
    mePtr->raInfo.pData = new TCHAR [mePtr->raInfo.nDataLength];
    if (nDataLength != 0)
        memcpy((void*)mePtr->raInfo.pData
             , (void*)(const _TCHAR *) m_RequestApprovalParms.m_csData
             , mePtr->raInfo.nDataLength * sizeof(TCHAR));
    mePtr->raInfo.pData[nDataLength]=0;

    trace(L7, _T("+ra.OnCanSuspendNow, CanSuspend=false"));
    ra.OnCanSuspendNow(false) ;
    trace(L7, _T("-ra.OnCanSuspendNow"));

    mo.PostIt(mePtr);
}
/////////////////////////////////////////////////////////////////////////////
void RAInterface::OnCancelSuspend(LPCTSTR sParms) 
{
    trace(L6,_T("PostRCMgr OnCancelSuspend sParms=%s"), sParms);

    RAEventParms parms;

    parms.ParseKeys(sParms);
    
    MessageElement * mePtr = new MessageElement; //create and new message element
    ZeroMemory(mePtr, sizeof(MessageElement));

    //fill in the message structure with the information pass in from the parameters of this function
    mePtr->id               = ID_RA;                //identifier, message comes from RA
    mePtr->raInfo.msg       = WMRA_CANCEL_SUSPEND;
    mePtr->raInfo.dwCookie  = parms.m_nCookie;
    //case WMRA_CANCEL_SUSPEND:
 //     mePtr->raInfo.handle = lParam1;
 //   break;

    mo.PostIt(mePtr);
}
/////////////////////////////////////////////////////////////////////////////
void RAInterface::OnValidateOperator(LPCTSTR sParms) 
{
    trace(L6,_T("PostRCMgr OnValidateOperator sParms=%s"), sParms);

    RAEventParms parms;

    parms.ParseKeys(sParms);

    MessageElement * mePtr = new MessageElement; //create and new message element
    ZeroMemory(mePtr, sizeof(MessageElement));

    //fill in the message structure with the information pass in from the parameters of this function
    mePtr->id               = ID_RA;                //identifier, message comes from RA
    mePtr->raInfo.msg       = WMRA_VALIDATE_OPERATOR;
    mePtr->raInfo.dwCookie  = parms.m_nCookie;
 //   case WMRA_VALIDATE_OPERATOR:
 //     _tcsnccpy(mePtr->raInfo.operatorID, (LPCTSTR) lParam1, RAMSG_OPERLEN);
 //     _tcsnccpy(mePtr->raInfo.password, (LPCTSTR) lParam2, RAMSG_PASSLEN);
 //   break;

    mo.PostIt(mePtr);
}
/////////////////////////////////////////////////////////////////////////////
void RAInterface::OnUnsolicitedData(LPCTSTR sParms) 
{
    trace(L6,_T("PostRCMgr OnUnsolicitedData sParms=%s"), sParms);

    RAEventParms parms;

    parms.ParseKeys(sParms);
    
    MessageElement * mePtr = new MessageElement; //create and new message element
    ZeroMemory(mePtr, sizeof(MessageElement));

    //fill in the message structure with the information pass in from the parameters of this function
    mePtr->id               = ID_RA;                //identifier, message comes from RA
    mePtr->raInfo.msg       = WMRA_SEND_UNSOLICITED_DATA;
    mePtr->raInfo.dwCookie  = parms.m_nCookie;
    mePtr->raInfo.nDataLength   = _tcslen(parms.m_csData) + 1;
    mePtr->raInfo.pData = new TCHAR [mePtr->raInfo.nDataLength];
    memcpy((void*)mePtr->raInfo.pData, (void*)(const _TCHAR *) parms.m_csData, mePtr->raInfo.nDataLength * sizeof(TCHAR));

    mo.PostIt(mePtr);
}
/////////////////////////////////////////////////////////////////////////////
void RAInterface::OnAssistMode(LPCTSTR sParms) 
{
    trace(L6,_T("PostRCMgr OnAssistMode sParms=%s"), sParms);
    if (IsRemoteMode())
    {
       RAAMEventParms objEvtParms;
       objEvtParms.ParseKeys(sParms);
       if (objEvtParms.GetOperation() == KEY_AMEV_OPERATION_ENTER_AM)
	   {
   	      trace( L4, _T("OnAssistMode %s request from %s ignored; already connected to %s"), 
		             objEvtParms.GetOperation(),
		             objEvtParms.GetMachineName(),
		             m_AssistModeParms.GetRemoteConnectionName());
   	      return;
       }
    }
    m_AssistModeParms.ParseKeys(sParms);

	int operation;
	if ( (m_AssistModeParms.GetOperation() == KEY_AMEV_OPERATION_ENTER_AM)
	    || (m_AssistModeParms.GetOperation() == KEY_AMEV_OPERATION_ENTER_SM) 
        || (m_AssistModeParms.GetOperation() == KEY_AMEV_OPERATION_ENTER_REPORTING)
        || (m_AssistModeParms.GetOperation() == KEY_AMEV_OPERATION_ENTER_DATA_NEEDED))

	{
        m_eRemoteMode = CONNECTED;

        operation	= PS_CONNECTREMOTE;
		CString* pstr1 = new CString;	
		*pstr1 = m_AssistModeParms.GetMachineName();
		CString* pstr2 = new CString;	
		*pstr2 = m_AssistModeParms.GetContextName();
		LPARAM lParam = (LPARAM)pstr1;
		WPARAM wParam = (WPARAM)pstr2;
		trace( L6, _T("PostPS %d,%d,%d,%s"), operation, lParam, wParam, m_AssistModeParms.GetContextName() );
		if (ra.m_csStateName.Find(_T("ThankYou")) == -1)	 //TAR437218
		{
			mo.PostPS( operation, lParam, wParam, (const TCHAR *)m_AssistModeParms.GetContextName() );
		}
	}
	else
	{
        //m_eRemoteMode = DISCONNECTED;  //removed for TAR 299233

		operation	= PS_REMOTE_DISCONNECTED;
		trace( L6, _T("PostPS %d,%d,%d,"), operation, NULL, NULL );
		mo.PostPS( operation, NULL, NULL, _T("") );
	}

}


CString RAInterface::GetRemoteConnectionName() const
{
    return m_AssistModeParms.GetRemoteConnectionName();
}
/////////////////////////////////////////////////////////////////////////////


BEGIN_KV_HANDLER_CLASS(RAInterface::RCEventParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RCEventParms, KEY_RCEV_OPERATION, Operation)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RCEventParms, KEY_RCEV_CLIENT_MACHINE_NAME, ClientName)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RCEventParms, KEY_RCEV_COOKIE, Cookie)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RCEventParms, KEY_RCEV_VIEW, View)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RCEventParms, KEY_RCEV_TYPE, Type)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(RAInterface::RCEventParms, Operation)
DEFINE_KV_CSTRING(RAInterface::RCEventParms, ClientName)
DEFINE_KV_CSTRING(RAInterface::RCEventParms, Type)
DEFINE_KV_INT(RAInterface::RCEventParms, Cookie)
DEFINE_KV_INT(RAInterface::RCEventParms, View)

BEGIN_KV_HANDLER_CLASS_CLEAR(RAInterface::RCEventParms)
    CLEAR_KV_CSTRING(Operation)
    CLEAR_KV_CSTRING(ClientName)
    CLEAR_KV_CSTRING(Type)
    CLEAR_KV_INT(Cookie, 0)
    CLEAR_KV_INT(View, -1)
END_KV_HANDLER_CLASS_CLEAR()


void RAInterface::OnRemoteConsoleEvent(LPCTSTR sParms) 
{
    trace(L6,_T("PostRCMgr OnRemoteConsoleEvent sParms=%s"), sParms);

    RCEventParms parms;

    parms.ParseKeys(sParms);

	if (parms.GetOperation() == KEY_RCEV_OPERATION_SEND_CLIENT_MACHINE_NAME)
	{    
		MessageElement * mePtr = new MessageElement; //create and new message element
		ZeroMemory(mePtr, sizeof(MessageElement));

		//fill in the message structure with the information pass in from the parameters of this function
		mePtr->id               = ID_RA;                //identifier, message comes from RA
		mePtr->raInfo.msg       = WMRA_SEND_CLIENT_MACHINE_NAME;
		mePtr->raInfo.nDataLength   = _tcslen(parms.GetClientName()) + 1;
		mePtr->raInfo.pData = new TCHAR [mePtr->raInfo.nDataLength];
		memcpy((void*)mePtr->raInfo.pData, (void*)(const _TCHAR *) parms.GetClientName(), mePtr->raInfo.nDataLength * sizeof(TCHAR));

		mo.PostIt(mePtr);
	}
	else if (parms.GetOperation() == KEY_RCEV_OPERATION_INTERVENTION_APPROVED)
	{   
        //It means that a RAPDataNeededIntervention has been approved if GetClientName is not empty.
        //So no need to call ReportInterventionEnd because it has already been called in OnRemoteApproval. 
        if(parms.GetClientName().IsEmpty() || parms.GetType() == _T("generic-delayed"))
        {
            if (ps.RemoteMode())
            {
                rp.SetOperator(ra.getOperatorID(ra.GetRemoteConnectionName()));
                rp.SetInterventionTerminalNumber(_T("RAP")); 
            }
            if (parms.GetView() != -1)
                ra.ReportInterventionEnd(parms.GetCookie(), parms.GetView());
            else 
                ra.ReportInterventionEnd(parms.GetCookie(), parms.GetType());
        }

		//tar 371932 & tar 371918+ 
		MessageElement * mePtr = new MessageElement; //create and new message element
		ZeroMemory(mePtr, sizeof(MessageElement));

		//fill in the message structure with the information pass in from the parameters of this function
		mePtr->id               = ID_RA;                //identifier, message comes from RA
		mePtr->raInfo.msg       = WMRA_SEND_INTERVENTION_APPROVED;
		mePtr->raInfo.nDataLength   = 0;
		mePtr->raInfo.pData = NULL;
		mo.PostIt(mePtr);
		//tar 371932 & tar 371918-

	}
	else
	{
	}

}


bool RAInterface::IsRemoteMode() const
{
    trace(L6,_T("RAInterface::IsRemoteMode() returns=%s"), (m_eRemoteMode == ACCEPTED ? _T("true") : _T("false")));
    return (m_eRemoteMode == ACCEPTED);
}


void RAInterface::AcceptConnection(const CString &sMachineName, const CString &sRemoteContextName)
{
    trace(L6, _T("RAInterface::AcceptConnection"));
    m_eRemoteMode = ACCEPTED;
}


HRESULT RAInterface::ScannerData(LPCTSTR lpszScannerData)
{
    TRACE_METHOD(_T("RAInterface::ScannerData"));

    OnUnsolicitedData(lpszScannerData);

    return S_OK;
}
// +RFQ 7477
HRESULT RAInterface::BiometricData(LPCTSTR lpszBiometricData)
{
    TRACE_METHOD(_T("RAInterface::BiometricData"));

    OnUnsolicitedData(lpszBiometricData);

    return S_OK;
}
// -RFQ 7477

void RAInterface::DisconnectFromServer()
{
    trace(L6, _T("RAInterface::DisconnectFromServer"));
    m_eRemoteMode = DISCONNECTED;
}


BEGIN_KV_HANDLER_CLASS(RAInterface::RAAMEventParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RAAMEventParms, KEY_AMEV_OPERATION, Operation)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RAAMEventParms, KEY_AMEV_MACHINE_NAME, MachineName)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RAAMEventParms, KEY_AMEV_CONTEXT_NAME, ContextName)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RAAMEventParms, KEY_AMEV_REMOTE_CONNECTION_NAME, RemoteConnectionName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(RAInterface::RAAMEventParms, Operation)
DEFINE_KV_CSTRING(RAInterface::RAAMEventParms, MachineName)
DEFINE_KV_CSTRING(RAInterface::RAAMEventParms, ContextName)
DEFINE_KV_CSTRING(RAInterface::RAAMEventParms, RemoteConnectionName)

void RAInterface::RAAMEventParms::Clear()
{
    m_Operation.Empty();
    m_MachineName.Empty();
    m_ContextName.Empty();
    m_RemoteConnectionName.Empty();
}


bool RAInterface::IsRAPDataAvailable() const
{
    return (m_RequestApprovalParms.m_nView != -1);
}


CString RAInterface::GetRAPData( void ) const
{
    return m_RequestApprovalParms.m_csData;
}


LONG RAInterface::GetRAPDataNeededPendingCount() const
{
    return m_nRAPDataPendingCount;
}

LONG RAInterface::GetRAPGenericDelayedPendingCount() const
{
    return m_nRAPGenericDelayedPendingCount;
}

void RAInterface::ResetRAPGenericDelayedPendingCount()
{
    m_nRAPGenericDelayedPendingCount = 0;
}

void RAInterface::DecrementRAPGenericDelayedPendingCount()
{
    m_nRAPGenericDelayedPendingCount--;
}

CString RAInterface::GetCurrentApprovalCategory() const
{
    return m_RequestApprovalParms.m_csCategory;
}


void RAInterface::GetApprovalInfo( SCOTDO_APPROVAL_INFO* pInfo ) const
{
    memcpy(pInfo, &m_RequestApprovalParms.m_approvalInfo, sizeof(SCOTDO_APPROVAL_INFO));
}


BEGIN_KV_HANDLER_MAP(RAInterface::RAEventParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RAEventParms, KEY_AEV_OP_ID, UserId)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RAEventParms, KEY_AEV_OP_PASSWORD, UserPwd)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RAEventParms, KEY_AEV_REMOTE_CONNECTION_NAME, RemoteConnection)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RAEventParms, KEY_AEV_APPROVAL_CODE, ApprovalCode)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RAEventParms, KEY_AEV_COOKIE, Cookie)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RAEventParms, KEY_AEV_AUX_DATA, AuxData)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RAEventParms, KEY_AEV_TYPE, Type)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RAEventParms, KEY_AEV_CATEGORY, Category)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RAEventParms, KEY_AEV_VIEW, View)
    DEFINE_KV_HANDLER_MAP_ENTRY(RAInterface::RAEventParms, KEY_AEV_PENDING_APPROVALS, PendingApprovals)
END_KV_HANDLER_MAP()

////////////////////////////////////////////////////////////////////////////////////////////
void RAInterface::RAEventParms::ApprovalCode(CKeyValueParmList::iterator &keys)
{
    CString sApprovalCode = CKeyValue<const _TCHAR *>::Get(keys, NULL);
    if (sApprovalCode == KEY_AEV_APPROVAL_CODE_APPROVED)
        m_approvalCode = RA_CODE_APPROVED;
    else if (sApprovalCode == KEY_AEV_APPROVAL_CODE_VERIFY_APPROVED)
    {
        m_approvalCode = RA_CODE_VERIFY_APPROVED;
        m_approvalInfo.bDropoffRequired = TRUE;
    }
    else if (sApprovalCode == KEY_AEV_APPROVAL_CODE_REDO)
        m_approvalCode = RA_CODE_REDO;
    else
        ASSERT(0);
}
////////////////////////////////////////////////////////////////////////////////////////////
void RAInterface::RAEventParms::Cookie(CKeyValueParmList::iterator &keys)
{
    m_nCookie       = CKeyValue<int>::Get(keys, -1);
    m_hApproval     = m_nCookie;
}
////////////////////////////////////////////////////////////////////////////////////////////
void RAInterface::RAEventParms::View(CKeyValueParmList::iterator &keys)
{
    m_nView         = CKeyValue<int>::Get(keys, -1);
}
////////////////////////////////////////////////////////////////////////////////////////////
void RAInterface::RAEventParms::PendingApprovals(CKeyValueParmList::iterator &keys)
{
    m_nPendingApprovals         = CKeyValue<int>::Get(keys, 0);
    m_approvalInfo.bAnyApprovalRequired = (m_nPendingApprovals != 0 ? TRUE : FALSE);
}
////////////////////////////////////////////////////////////////////////////////////////////
void RAInterface::RAEventParms::AuxData(CKeyValueParmList::iterator &keys)
{
    m_csData = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void RAInterface::RAEventParms::Type(CKeyValueParmList::iterator &keys)
{
    m_csType = CKeyValue<const _TCHAR *>::Get(keys, NULL);
    if (m_csType == KEY_APPROVAL_TYPE_COUPON)                       m_approvalInfo.bCouponApprovalRequired = TRUE;
    else if (m_csType == KEY_APPROVAL_TYPE_VOID)                    m_approvalInfo.bVoidApprovalRequired = TRUE;
    else if (m_csType == KEY_APPROVAL_TYPE_AGE_RESTRICTION)         m_approvalInfo.bAgeApprovalRequired = TRUE;
    else if (m_csType == KEY_APPROVAL_TYPE_VISUAL_VALIDATION)       m_approvalInfo.bVisualVerifyApprovalRequired = TRUE;
    else if (m_csType == KEY_APPROVAL_TYPE_CALL_FOR_HELP)           m_approvalInfo.bCallForHelpApprovalRequired = TRUE;
    else if (m_csType == KEY_APPROVAL_TYPE_OTHER_PAYMENT)           m_approvalInfo.bOtherPaymentApprovalRequired = TRUE;
    else if (m_csType == KEY_APPROVAL_TYPE_SECURITY)                m_approvalInfo.bSecurityApprovalRequired = TRUE;
    else if (m_csType == KEY_APPROVAL_TYPE_SIG_CAP)                 m_approvalInfo.bSigCapApprovalRequired = TRUE;
    else if (m_csType == KEY_APPROVAL_TYPE_UNKNOWN_ITEM)            m_approvalInfo.bUnknownItem = TRUE;
    else if (m_csType == KEY_APPROVAL_TYPE_RECALLED_ITEM)           m_approvalInfo.bRecalledItem = TRUE;
    else if (m_csType == KEY_APPROVAL_TYPE_TIME_RESTRICTED_ITEM)    m_approvalInfo.bTimeRestrictedItem = TRUE;
    else if (m_csType == KEY_APPROVAL_TYPE_APPLICATION_ERROR)       m_approvalInfo.bApplicationError = TRUE;
    else if (m_csType == KEY_APPROVAL_TYPE_DEVICE_ERROR)            m_approvalInfo.bDeviceError = TRUE;
    else if (m_csType == KEY_APPROVAL_TYPE_QUANTITY_RESTRICTED_ITEM)m_approvalInfo.bQuantityRestrictedItem = TRUE;
    else if (m_csType == KEY_APPROVAL_TYPE_UNDEACTIVATED_ITEM)      m_approvalInfo.bUnDeActivatedItemApprovalRequired = TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////
void RAInterface::RAEventParms::UserId(CKeyValueParmList::iterator &keys)
{
    m_csUserId = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void RAInterface::RAEventParms::RemoteConnection(CKeyValueParmList::iterator &keys)
{
    m_csRemoteConnection = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void RAInterface::RAEventParms::UserPwd(CKeyValueParmList::iterator &keys)
{
    m_csUserPwd = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void RAInterface::RAEventParms::Category(CKeyValueParmList::iterator &keys)
{
    m_csCategory = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void RAInterface::RAEventParms::Clear()
{
    m_csData            = RI_NULL_STRING;
    m_csUserId          = RI_NULL_STRING;
    m_csRemoteConnection = RI_NULL_STRING;
    m_csUserPwd         = RI_NULL_STRING;
    m_csCategory        = RI_NULL_STRING;
    m_approvalCode      = -1;
    m_nCookie           = -1;
    m_nView             = -1;
    m_nPendingApprovals = 0;
    memset(&m_approvalInfo, 0, sizeof(m_approvalInfo));
}
////////////////////////////////////////////////////////////////////////////////////////////
void RAInterface::RAEventParms::TraceThis(const _TCHAR *pPairs) throw()
{
    
    trace(L6, CString (pPairs) );
}
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(RAInterface, CWnd)
    //{{AFX_MSG_MAP(RAInterface)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
END_MESSAGE_MAP()



