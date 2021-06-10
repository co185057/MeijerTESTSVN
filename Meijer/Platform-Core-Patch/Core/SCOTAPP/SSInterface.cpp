// SSInterface.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include "Common.h"


#include "SSInterface.h"

#define COMP_ID ID_SASS

#define T_ID _T("ssi")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define PROCESS_EXCEPTIONS(a)                                 \
	catch(COleDispatchException * e)                              \
{                                                             \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SSINTERFACE_OLEDISPATCH_EXCEPTION, \
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
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SSINTERFACE_OLE_EXCEPTION,_T("%s|%s|%x"),        \
	msg,                                                      \
	cstr,                                                     \
	e->m_sc);                                                 \
  e->Delete();                                               \
  return a;                                                   \
}                                                             \
	catch(CException* e)                                          \
{                                                             \
  e->GetErrorMessage( cstr, 255 );                           \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SSINTERFACE_C_EXCEPTION,_T("%s|%s"),                   \
	msg,                                                      \
	cstr);                                                    \
  e->Delete();                                               \
  return a;                                                   \
}                                                             \
	catch(...)                                                    \
{                                                             \
  long le = GetLastError();                                   \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SSINTERFACE_EXCEPTION,_T("%s|%d"),msg,le);            \
  return a;                                                   \
}


/////////////////////////////////////////////////////////////////////////////
// SSInterface

SSInterface::SSInterface()
{
  SSOpened  = false;
  SSCreated = false;
}

SSInterface::~SSInterface()
{
  SSOpened  = false;
  SSCreated = false;
}
 /////////////////////////////////////////////////////////////////////////////
//
// Open the Smart Scale
//

bool SSInterface::InitSS()
{
  _TCHAR msg[] = T_ID _T("::InitSS");
	
  if ( SSCreated == false )
  {
    if( !ss.Create(NULL, NULL, CRect(0,0,0,0), this, IDC_SMARTSCALECTRL1 ))
    {
	  ScotError(INFO,SM_CAT_STOP_ABORT, SCOTAPP_SSINTERFACE_STOP);//TD 05/17/02 TAR 204873
	  return false;
    }
	else
	{
      SSCreated = true;
	}
	try 
	{

		if (SSOpened == false )
		{
			SSOpened = true;
		}
	}
    PROCESS_EXCEPTIONS(false);

  }// end - if ( SACreated == false )
  return true;
}

////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Close Smart Scale
//
bool SSInterface::UnInitSS()
{
  trace(L2, _T("+UnInitSA"));
  _TCHAR msg[] = T_ID _T("::UnInitSS");
  bool rc;
  try
  {
 
    if (SSOpened == true )
    {
	
      trace(L2, _T("UnInitSS -- before Closing Window"));
	  //Tar 214239,  Dr. Watson shutting down the app using UpCon
      //ss.CloseWindow();
	  //End Tar 214239
      trace(L2, _T("Close Window"));
	  try
	  {
      rc = !ss.DestroyWindow();	// true - if succesful
	  }
	  catch(CException* e)
	  {
        _TCHAR szBuff[300];
		e->GetErrorMessage(szBuff,300);
		trace(L2, _T("Got an exception in ss.DestroyWindow() :Error text:%s"), szBuff);
		e->Delete(); //TAR247967
	  }
      trace(L2, _T("after DestroyWindow"));
	  SSOpened = false;
      SSCreated = false;
	}
	trace(L2, _T("UnInitSS -- before PROCESS_EXCEPTIONS"));
  }
  PROCESS_EXCEPTIONS(false);
  trace(L2, _T("-UnInitSS"));
  return rc;
}


////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Smart Scale Functions 
//
/////////////////////////////////
long SSInterface::GetSlowConclusionTimeout()
{
	_TCHAR msg[] = T_ID _T("::GetSlowConclusionTimeout");
	long r = 0;
	try
	{
		r = ss.GetSlowConclusionTimeout();
		return r;
	}
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
void SSInterface::SetSlowConclusionTimeout(long lTimeOut)
{
	ss.SetSlowConclusionTimeout(lTimeOut);
}
////////////////////////////////////////////////////////////////////////////
long SSInterface::GetSlowConclusionTimeoutInAttract()
{
	_TCHAR msg[] = T_ID _T("::GetSlowConclusionTimeoutInAttract");
	long r = 0;
	try
	{
		r = ss.GetSlowConclusionTimeoutInAttract();
		return r;
	}
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
void SSInterface::SetSlowConclusionTimeoutInAttract(long lTimeOut)
{
	ss.SetSlowConclusionTimeoutInAttract(lTimeOut);
}
////////////////////////////////////////////////////////////////////////////
long SSInterface::GetSlowConclTimeoutInNotExpecting()
{
	_TCHAR msg[] = T_ID _T("::GetSlowConclTimeoutInNotExpecting");
	long r = 0;
	try
	{
		r = ss.GetSlowConclTimeoutInNotExpecting();
		return r;
	}
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
void SSInterface::SetSlowConclTimeoutInNotExpecting(long lTimeOut)
{
	ss.SetSlowConclTimeoutInNotExpecting(lTimeOut);
}
////////////////////////////////////////////////////////////////////////////
long SSInterface::GetSCWatchdogTimeout()
{
  _TCHAR msg[] = T_ID _T("::GetSCWatchdogTimeout");
  long r = 0;
  try
  {
    r = ss.GetSCWatchdogTimeout();
  }
	PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
	
void SSInterface::SetSCWatchdogTimeout(long lTimeOut)
{
	ss.SetSCWatchdogTimeout(lTimeOut);
}
////////////////////////////////////////////////////////////////////////////	
long SSInterface::GetZeroWeightTimeout()
{
  _TCHAR msg[] = T_ID _T("::GetZeroWeightTimeout");
  long r = 0;
  try
  {
    r = ss.GetZeroWeightTimeout();
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////

void SSInterface::SetZeroWeightTimeout(long lTimeOut)
{
    ss.SetZeroWeightTimeout(lTimeOut);

}
////////////////////////////////////////////////////////////////////////////
	
long SSInterface::GetLastGoodWeight()
{
  _TCHAR msg[] = T_ID _T("::GetLastGoodWeight");
  long r = 0;
  try
  {
    r = ss.GetLastGoodWeight();
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
	
void SSInterface::SetLastGoodWeight(long lLGW)
{
	ss.SetLastGoodWeight(lLGW);
}
////////////////////////////////////////////////////////////////////////////
	
BOOL SSInterface::GetHoldWeight()
{
  _TCHAR msg[] = T_ID _T("::GetHoldWeight");
  bool r = false;
  try
  {
    r = (ss.GetHoldWeight() ? true:false);
  }
  PROCESS_EXCEPTIONS(false);
   return r;
}
////////////////////////////////////////////////////////////////////////////
	
void SSInterface::SetHoldWeight(BOOL bHold)
{
	ss.SetHoldWeight(bHold);
}
////////////////////////////////////////////////////////////////////////////
BOOL SSInterface::GetSkipBagging()
{
  _TCHAR msg[] = T_ID _T("::GetSkipBagging");
  bool r = false;	
  try
  {
    r = (ss.GetSkipBagging() ? true:false);
  }
  PROCESS_EXCEPTIONS(false);
  return r;
 }


////////////////////////////////////////////////////////////////////////////
void SSInterface::SetSkipBagging(BOOL bSkipbagging)
{
	 	ss.SetSkipBagging(bSkipbagging);
}
////////////////////////////////////////////////////////////////////////////
long SSInterface::GetHeavyWeightLimit()
{
  _TCHAR msg[] = T_ID _T("::GetHeavyWeightLimit");
  long r = 0;
  try
  {
    r = ss.GetHeavyWeightLimit();
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}

////////////////////////////////////////////////////////////////////////////
void SSInterface::SetHeavyWeightLimit(long lHWL)
{
	ss.SetHeavyWeightLimit(lHWL);
}
////////////////////////////////////////////////////////////////////////////
long SSInterface::GetMeduimWeightLimit()
{
  _TCHAR msg[] = T_ID _T("::GetMeduimWeightLimit");
  long r = 0;
	
  try
  {
    r = ss.GetMeduimWeightLimit();
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
void SSInterface::SetMeduimWeightLimit(long lSMWL)
{
	ss.SetMeduimWeightLimit(lSMWL);
}
////////////////////////////////////////////////////////////////////////////
long SSInterface::GetHeavyWeightTol()
{
  _TCHAR msg[] = T_ID _T("::GetHeavyWeightTol");
  long r = 0;
	
  try
  {
    r = ss.GetHeavyWeightTol();
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
void SSInterface::SetHeavyWeightTol(long lSHWT)
{	
	ss.SetHeavyWeightTol(lSHWT);
}
////////////////////////////////////////////////////////////////////////////
long SSInterface::GetMediumWeightTol()
{
  _TCHAR msg[] = T_ID _T("::GetMediumWeightTol");
  long r = 0;
	
  try
  {
    r = ss.GetMediumWeightTol();
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
void SSInterface::SetMediumWeightTol(long lSMWT)
{
	ss.SetMediumWeightTol(lSMWT);
}
////////////////////////////////////////////////////////////////////////////
long SSInterface::GetLightWeightTol()
{
  _TCHAR msg[] = T_ID _T("::GetLightWeightTol");
  long r = 0;
  try
  {
    r = ss.GetLightWeightTol();
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
void SSInterface::SetLightWeightTol(long lLWT)
{		
	ss.SetLightWeightTol(lLWT);
}
////////////////////////////////////////////////////////////////////////////

void SSInterface::GetStatus(BSTR* bstrDeviceName, BSTR* bstrCheckHealthText, long* lStatus)
{
	 ss.GetStatus(bstrDeviceName,bstrCheckHealthText,lStatus);
}
//////////////////////////////////////////////////////////////////////////////	
void SSInterface::ResetTimers(long nTimer)
{
	ss.ResetTimers(nTimer);
}
//////////////////////////////////////////////////////////////////////////////	
void SSInterface::SetSoldItemInfo(LPDISPATCH lpdispWeightInfo)
{
	ss.SetSoldItemInfo(lpdispWeightInfo);
}
//////////////////////////////////////////////////////////////////////////////
long SSInterface::GetCurrentTotalWeight()
{
  _TCHAR msg[] = T_ID _T("::GetCurrentTotalWeight");
  long r = 0;
  try
  {
    r = ss.GetCurrentTotalWeight();
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}
//////////////////////////////////////////////////////////////////////////////	
void SSInterface::SetCurrentTotalWeight(long lSCTW)
{
	ss.SetCurrentTotalWeight(lSCTW);
}
//////////////////////////////////////////////////////////////////////////////
long SSInterface::GetNewItemZeroWeightTimeout()
{
  _TCHAR msg[] = T_ID _T("::GetNewItemZeroWeightTimeout");
  long r = 0;
  try
  {
    r = ss.GetNewItemZeroWeightTimeout();
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}
//////////////////////////////////////////////////////////////////////////////	
void SSInterface::SetNewItemZeroWeightTimeout(long lNZWIT)
{
	ss.SetNewItemZeroWeightTimeout(lNZWIT);
}
////////////////////////////////////////////////////////////////////////////
BOOL SSInterface::GetInAttract()
{
  _TCHAR msg[] = T_ID _T("::GetInAttract");
  bool r = false;	
  try
  {
    r = (ss.GetInAttract() ? true:false);
  }
  PROCESS_EXCEPTIONS(false);
  return r;
 }


////////////////////////////////////////////////////////////////////////////
void SSInterface::SetInAttract(BOOL bInAttract)
{
	 	ss.SetInAttract(bInAttract);
}

/////////////////////////////////////////////////////////////////////////////
// Begin Event Sink
BEGIN_EVENTSINK_MAP(SSInterface, CWnd)
  //{{AFX_EVENTSINK_MAP(CSmartScale)
	ON_EVENT(SSInterface, IDC_SMARTSCALECTRL1, 4 /* BackToLGW */, OnBackToLGW, VTS_NONE)
	ON_EVENT(SSInterface, IDC_SMARTSCALECTRL1, 3 /* DeviceFailure */, OnDeviceFailure, VTS_I4)
	ON_EVENT(SSInterface, IDC_SMARTSCALECTRL1, 5 /* LateMatchedWeight */, OnLateMatchedWeight, VTS_I4 VTS_I4)
	ON_EVENT(SSInterface, IDC_SMARTSCALECTRL1, 1 /* MatchedWeight */, OnMatchedWeight, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(SSInterface, IDC_SMARTSCALECTRL1, 2 /* UnmatchedWeight */, OnUnmatchedWeight, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// SSInterface message handlers
void SSInterface::OnBackToLGW() 
{
	trace(L6,_T("PostSS SS_BACKTOLGW"));
	mo.PostSS( SS_DATA, -1, 0, 0, SS_BACKTOLGW, -1);	
}
/////////////////////////////////////////////////////////////////////////////
void SSInterface::OnDeviceFailure(long nFailType) 
{
	trace(L6,_T("PostSS SS_FAILURE   %d"),nFailType);
	mo.PostSS( SS_DATA, -1, -1, -1, SS_FAILURE, nFailType);	
}

/////////////////////////////////////////////////////////////////////////////
void SSInterface::OnLateMatchedWeight(long lLastGoodWeight, long lCurrentTotalWeight, long lIndex, long lItemWt) 
{
	// TODO: Add your control notification handler code here
	
}
/////////////////////////////////////////////////////////////////////////////
void SSInterface::OnMatchedWeight(long lID, long lLastGoodWeight, long lCurrentTotalWeight) 
{
	trace(L6,_T("PostSS SS_MATCHEDWEIGHT lID: %d, lLastGoodWeight: %d, lCurrentTotalWeight: %d"),lID,lLastGoodWeight,lCurrentTotalWeight);
	mo.PostSS( SS_DATA, lID, lLastGoodWeight, lCurrentTotalWeight, SS_MATCHEDWEIGHT, -1 );
}
/////////////////////////////////////////////////////////////////////////////
void SSInterface::OnUnmatchedWeight(long lLastGoodWeight, long lCurrentTotalWeight) 
{
	trace(L6,_T("PostSS SS_UNMATCHEDWEIGHT lLastGoodWeight: %d, lCurrentTotalWeight %d"),lLastGoodWeight,lCurrentTotalWeight);
	mo.PostSS( SS_DATA, -1, lLastGoodWeight, lCurrentTotalWeight, SS_UNMATCHEDWEIGHT, -1);	
}

BEGIN_MESSAGE_MAP(SSInterface, CWnd)
	//{{AFX_MSG_MAP(SSInterface)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()


