// SMInterface.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include "Common.h"


#include "SecMgrInterface.h"

#define COMP_ID ID_SECMGR

#define T_ID _T("SecMgri")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define PROCESS_EXCEPTIONS(a)                                 \
	catch(COleDispatchException * e)                              \
{                                                             \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SEC_MGR_INTERFACE_OLEDISPATCH_EXCEPTION, \
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
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SEC_MGR_INTERFACE_OLE_EXCEPTION,_T("%s|%s|%x"),        \
	msg,                                                      \
	cstr,                                                     \
	e->m_sc);                                                 \
  e->Delete();                                               \
  return a;                                                   \
}                                                             \
	catch(CException* e)                                          \
{                                                             \
  e->GetErrorMessage( cstr, 255 );                           \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SEC_MGR_INTERFACE_C_EXCEPTION,_T("%s|%s"),                   \
	msg,                                                      \
	cstr);                                                    \
  e->Delete();                                               \
  return a;                                                   \
}                                                             \
	catch(...)                                                    \
{                                                             \
  long le = GetLastError();                                   \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SEC_MGR_INTERFACE_EXCEPTION,_T("%s|%d"),msg,le);            \
  return a;                                                   \
}


/////////////////////////////////////////////////////////////////////////////
// SSInterface

SecMgrInterface::SecMgrInterface()
{
  SecMgrCreated = false;
}

SecMgrInterface::~SecMgrInterface()
{
  SecMgrCreated = false;
}
 /////////////////////////////////////////////////////////////////////////////
//
// Open the Security Manager

bool SecMgrInterface::InitSecMgr()
{
  _TCHAR msg[] = T_ID _T("::InitSecMgr");
	
  if ( SecMgrCreated == false )
  {
    if( !SecMgr.Create(NULL, NULL, CRect(0,0,0,0), this, IDC_SECURITYMANAGERCTRL1 ))
    {
	  ScotError(INFO,SM_CAT_STOP_ABORT, SCOTAPP_SEC_MGR_INTERFACE_STOP);//TD 05/17/02 TAR 204873
	  SecMgrCreated = false;
    }
	else
	{
      SecMgrCreated = true;
	}

  }
  return SecMgrCreated;
}

////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Close Security Manager
//
bool SecMgrInterface::UnInitSecMgr()
{
  trace(L2, _T("+UnInitSecMgr"));
  _TCHAR msg[] = T_ID _T("::UnInitSecMgr");
  bool rc;
  try
  {
 
    if (SecMgrCreated )
    {
	
      trace(L2, _T("UnInitSecMgr -- before Closing Window"));
	  //Tar 214239,  Dr. Watson shutting down the app using UpCon
      //ss.CloseWindow();
	  //End Tar 214239
      trace(L2, _T("Close Window"));
	  try
	  {
      rc = !SecMgr.DestroyWindow();	// true - if succesful
	  }
	  catch(CException* e)
	  {
        _TCHAR szBuff[300];
		e->GetErrorMessage(szBuff,300);
		trace(L2, _T("Got an exception in SecMgr.DestroyWindow() :Error text:%s"), szBuff);
		e->Delete(); //TAR247967
	  }
      trace(L2, _T("after DestroyWindow"));
      SecMgrCreated = false;
	}
	trace(L2, _T("UnInitSecMgr -- before PROCESS_EXCEPTIONS"));
  }
  PROCESS_EXCEPTIONS(false);
  trace(L2, _T("-UnInitSecMgr"));
  return rc;
}


////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Security Manager Functions 
//
/////////////////////////////////
CString SecMgrInterface::GetOutstandingInterventionList()
{
	_TCHAR msg[] = T_ID _T("::GetOutstandingInterventionList");
	CString strResult;
	try
	{
		strResult = SecMgr.GetOutstandingInterventionList();
		return strResult;
	}
  PROCESS_EXCEPTIONS(strResult);
  return strResult;
}

////////////////////////////////////////////////////////////////////////////
CString SecMgrInterface::GetDevices()
{
	_TCHAR msg[] = T_ID _T("::GetDevices");
	CString strResult;
	try
	{
		strResult = SecMgr.GetDevices();
		return strResult;
	}
  PROCESS_EXCEPTIONS(strResult);
  return strResult;
}

long SecMgrInterface::GetOutstandingInterventionCount()
{
	_TCHAR msg[] = T_ID _T("::GetOutstandingInterventionCount");
	long lResult = 0;
	try
	{
		lResult = SecMgr.GetOutstandingInterventionCount();
		return lResult;
	}
  PROCESS_EXCEPTIONS(lResult);
  return lResult;
}
void SecMgrInterface::SecurityUnadvise(long dwCookie)
{
	SecMgr.SecurityUnadvise(dwCookie);
}


long SecMgrInterface::SecurityAdvise(long lFlags)
{
	return SecMgr.SecurityAdvise(lFlags);
}

void SecMgrInterface::ItemSold(long lCookie, LPCTSTR bstrParms)
{
	SecMgr.ItemSold(lCookie, bstrParms);
}

void SecMgrInterface::ItemDetails(long lCookie, LPCTSTR bstrParms)
{
	SecMgr.ItemDetails(lCookie, bstrParms);
}

void SecMgrInterface::VoidItem(long lCookie, LPCTSTR bstrParms)
{
	SecMgr.VoidItem(lCookie, bstrParms);
}

void SecMgrInterface::TransactionControl(long lCookie, LPCTSTR bstrParms)
{
	SecMgr.TransactionControl(lCookie, bstrParms);
}

long SecMgrInterface::AppControl(long lCookie, LPCTSTR bstrParms,BSTR* bstrOutParms)
{
	_TCHAR msg[] = T_ID _T("::AppControl");
	long lResult = 0;
	try
	{
		lResult = SecMgr.AppControl(lCookie, bstrParms, bstrOutParms);
		return lResult;
	}
    PROCESS_EXCEPTIONS(lResult);
    return lResult;
}

void SecMgrInterface::OverrideException(long lCookie, LPCTSTR bstrParms)
{
	SecMgr.OverrideException(lCookie, bstrParms);
}

long SecMgrInterface::SecurityMaintenance(long lCookie, LPCTSTR bstrParms)
{
	_TCHAR msg[] = T_ID _T("::SecurityMaintenance");
	long lResult = 0;
	try
	{
		lResult = SecMgr.SecurityMaintenance(lCookie, bstrParms);
		return lResult;
	}
    PROCESS_EXCEPTIONS(lResult);
    return lResult;
}

void SecMgrInterface::SecurityControl(long lCookie, LPCTSTR bstrInParms, BSTR* bstrOutParms)
{
	SecMgr.SecurityControl(lCookie, bstrInParms, bstrOutParms);
}

long SecMgrInterface::Initialize(long lCookie, LPCTSTR bstrInParms, BSTR* pbstrValues)
{
	return SecMgr.Initialize(lCookie, bstrInParms, pbstrValues);
}
/////////////////////////////////////////////////////////////////////////////
// Begin Event Sink
BEGIN_EVENTSINK_MAP(SecMgrInterface, CWnd)
  //{{AFX_EVENTSINK_MAP(CSmartScale)
	ON_EVENT(SecMgrInterface, IDC_SECURITYMANAGERCTRL1, 1 /* OnItemOK */, OnItemOK, VTS_BSTR)
	ON_EVENT(SecMgrInterface, IDC_SECURITYMANAGERCTRL1, 2 /* OnTransactionComplete */, OnTransactionComplete, VTS_NONE)
	ON_EVENT(SecMgrInterface, IDC_SECURITYMANAGERCTRL1, 3 /* OnSecurityException */, OnSecurityException, VTS_BSTR)
	ON_EVENT(SecMgrInterface, IDC_SECURITYMANAGERCTRL1, 4 /* OnSecurityExceptionCleared */, OnSecurityExceptionCleared, VTS_BSTR)
	ON_EVENT(SecMgrInterface, IDC_SECURITYMANAGERCTRL1, 5 /* OnSecurityMaintenance */, OnSecurityMaintenance, VTS_BSTR)
	ON_EVENT(SecMgrInterface, IDC_SECURITYMANAGERCTRL1, 6 /* OnDeviceFailure */, OnDeviceFailure, VTS_BSTR)
	ON_EVENT(SecMgrInterface, IDC_SECURITYMANAGERCTRL1, 7 /* OnInitializationComplete */, OnInitializationComplete, VTS_BSTR)
	ON_EVENT(SecMgrInterface, IDC_SECURITYMANAGERCTRL1, 8 /* OnAppControl */, OnAppControl, VTS_BSTR)
	ON_EVENT(SecMgrInterface, IDC_SECURITYMANAGERCTRL1, 9 /* OnSecurityControl */, OnSecurityControl, VTS_BSTR)
	ON_EVENT(SecMgrInterface, IDC_SECURITYMANAGERCTRL1, 10 /* OnItemPickFinish */, OnItemPickFinish, VTS_NONE)


	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// SecMgrInterface message handlers
void SecMgrInterface::OnItemOK(LPCTSTR sParms) 
{
	trace(L6,_T("PostSecMgr SmOnItemOK"));
    CString csParms( sParms);
	mo.PostSecMgr( SmOnItemOK, csParms);	
}
/////////////////////////////////////////////////////////////////////////////
void SecMgrInterface::OnTransactionComplete() 
{
	trace(L6,_T("PostSecMgr SmOnTransactionComplete"));
   	mo.PostSecMgr( SmOnTransactionComplete, _T(""));	
}

/////////////////////////////////////////////////////////////////////////////
void SecMgrInterface::OnSecurityException(LPCTSTR sParms) 
{
	trace(L6,_T("PostSecMgr SmOnSecurityException sParms=%s"), sParms);
    CString csParms(sParms);
	mo.PostSecMgr( SmOnSecurityException, csParms);	
	
}
/////////////////////////////////////////////////////////////////////////////
void SecMgrInterface::OnSecurityExceptionCleared(LPCTSTR sParms) 
{
	trace(L6,_T("PostSecMgr SmOnSecurityExceptionCleared sParms=%s"), sParms);
    CString csParms(sParms);
	mo.PostSecMgr( SmOnSecurityExceptionCleared, csParms);	
}
/////////////////////////////////////////////////////////////////////////////
void SecMgrInterface::OnSecurityMaintenance(LPCTSTR sParms) 
{
	trace(L6,_T("PostSecMgr SmOnSecurityMaintenance sParms=%s"), sParms);
    CString csParms(sParms);
	mo.PostSecMgr( SmOnSecurityMaintenance, csParms);	

}
/////////////////////////////////////////////////////////////////////////////
void SecMgrInterface::OnDeviceFailure(LPCTSTR sParms) 
{
	trace(L6,_T("PostSecMgr SmOnDeviceFailure sParms=%s"), sParms);
    CString csParms(sParms);
	mo.PostSecMgr( SmOnDeviceFailure, csParms);	
}
/////////////////////////////////////////////////////////////////////////////
void SecMgrInterface::OnInitializationComplete(LPCTSTR sParms) 
{
	trace(L6,_T("PostSecMgr SmOnInitializationComplete sParms=%s"), sParms);
    CString csParms(sParms);
	mo.PostSecMgr( SmOnInitializationComplete, csParms);	
}

void SecMgrInterface::OnAppControl(LPCTSTR sParms) 
{
	trace(L6,_T("PostSecMgr OnAppControl sParms=%s"), sParms);
    CString csParms(sParms);
	mo.PostSecMgr( SmOnAppControl, csParms);	
}

void SecMgrInterface::OnSecurityControl(LPCTSTR sParms) 
{
	trace(L6,_T("PostSecMgr OnSecurityControl sParms=%s"), sParms);
    CString csParms(sParms);
	mo.PostSecMgr( SmOnSecurityControl, csParms);	
}

//SR679
void SecMgrInterface::OnItemPickFinish() 
{
	trace(L6,_T("PostSecMgr SmItemPickFinish"));
   	mo.PostSecMgr( SmItemPickFinish, _T(""));	
}


BEGIN_MESSAGE_MAP(SecMgrInterface, CWnd)
	//{{AFX_MSG_MAP(SecMgrInterface)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()


