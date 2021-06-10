//////////////////////////////////////////////////////////////////////////////
//
// FILE:    SSProcedures.cpp
//
// TITLE:   Smart Scale  (NCR API) Interface
//
//
// AUTHOR:  
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "stdafx.h"
#include "Common.h"
#include "smartscale.h"
#include "SSInterface.h"
#include "SSProcedures.h"
#include "devmgr.h"
#include "ncrdevmgr.h"
#include "DMTriColorLight.h"
//#include "DMInterface.h"
#include "DMProcedures.h"

#define COMP_ID ID_SASS               // used by the trace() calls to identify this component
#define T_ID        _T("SSp")
/////////////////////////////////////////////////////////////////////////////
// SSProcedures

////////////////////////////
SSProcedures::SSProcedures()
{
  doSSInit   = true;
  SSICreated = false;
  SSPCreated = false;
}

/////////////////////////////
SSProcedures::~SSProcedures()
{
  doSSInit   = false;
  SSICreated = false;
  SSPCreated = false;
}

BEGIN_MESSAGE_MAP(SSProcedures, CWnd)
//{{AFX_MSG_MAP(SSProcedures)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////
long SSProcedures::Initialize(CWnd * pWnd)
{
  long rc;
  trace(L0, _T("+")_T(__TIMESTAMP__));
  trace(L4, _T("+Initialize"));
  if ( SSICreated == false )
  {
    if( SSPCreated == false )
    {
      if(pWnd == NULL)
      {
        if( !(rc = Create( NULL, NULL, WS_DISABLED, CRect(0,0,0,0), AfxGetMainWnd(), IDW_SSPWINDOW )))
        {
          ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_SSPROCEDURE_MAIN_WINDOW_FAILED);

        }
      }
      else
      {
        if( !(rc = Create( NULL, NULL, WS_DISABLED, CRect(0,0,0,0), pWnd, IDW_SSPWINDOW )))
        {
          ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_SSPROCEDURE_WINDOW_FAILED);
        }
      }
      SSPCreated = true;
    }
    if( !(rc = ssi.Create( NULL, NULL, WS_DISABLED, CRect(0,0,0,0), this, IDW_SSIWINDOW )))
    {
      ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_SSINTERFACE_WINDOW_FAILED );

    }
    SSICreated = true;
  }
  // caller of this routine
	if( doSSInit )
	{
		rc = ssi.InitSS();
		if(rc <= 0)
		{
			// anything special to be done if control creation or initSA fails
		}
	}
	trace(L4, _T("-Initialize"));
	trace(L0, _T("-")_T(__TIMESTAMP__));
	return rc;
}
/////////////////////////////////
long SSProcedures::UnInitialize()
{
  long rc;
  trace(L2, _T("+UnInitialize"));
		
  if( rc = ssi.UnInitSS() )
  {
  trace(L2, _T("After UnInitialize -- ssi:UnIniSS"));
    ScotError(FATAL,SM_CAT_STOP_ABORT, SCOTAPP_SSPROCEDURE_UNINIT);
  }
  if( !DestroyWindow() )					  // destroy this window
  {
  trace(L2, _T("After UnInitialize -- DestroyWindow"));
    ScotError(FATAL,SM_CAT_STOP_ABORT, SCOTAPP_SSPROCEDURE_WINDOW_DESTROY);
  }
  SSPCreated = false;
  SSICreated = false;
  trace(L2, _T("-UnInitialize"));
  return rc;
}
/////////////////////////////////
long SSProcedures::GetSlowConclusionTimeout()
{
  long r;
  trace(L2,_T("+GetSlowConclusionTimeout"));
  r = ssi.GetSlowConclusionTimeout();
  trace(L2,_T("-GetSlowConclusionTimeout return: %d"), r);
  return r;
}

////////////////////////////////////////////////////////////////////////////

void SSProcedures::SetSlowConclusionTimeout(long lTimeOut)
{
  trace(L2,_T("+SetSlowConclusionTimeout TimeOut: %d"), lTimeOut);
  ssi.SetSlowConclusionTimeout(lTimeOut);
  trace(L2,_T("-SetSlowConclusionTimeout"));
  
}
/////////////////////////////////
long SSProcedures::GetSlowConclusionTimeoutInAttract()
{
  long r;
  trace(L2,_T("+GetSlowConclusionTimeoutInAttract"));
  r = ssi.GetSlowConclusionTimeoutInAttract();
  trace(L2,_T("-GetSlowConclusionTimeoutInAttract return: %d"), r);
  return r;
}

////////////////////////////////////////////////////////////////////////////

void SSProcedures::SetSlowConclusionTimeoutInAttract(long lTimeOut)
{
  trace(L2,_T("+SetSlowConclusionTimeoutInAttract TimeOut: %d"), lTimeOut);
  ssi.SetSlowConclusionTimeoutInAttract(lTimeOut);
  trace(L2,_T("-SetSlowConclusionTimeoutInAttract"));
  
}
/////////////////////////////////
long SSProcedures::GetSlowConclTimeoutInNotExpecting()
{
  long r;
  trace(L2,_T("+GetSlowConclTimeoutInNotExpecting"));
  r = ssi.GetSlowConclTimeoutInNotExpecting();
  trace(L2,_T("-GetSlowConclTimeoutInNotExpecting return: %d"), r);
  return r;
}

////////////////////////////////////////////////////////////////////////////

void SSProcedures::SetSlowConclTimeoutInNotExpecting(long lTimeOut)
{
  trace(L2,_T("+SetSlowConclTimeoutInNotExpecting TimeOut: %d"), lTimeOut);
  ssi.SetSlowConclTimeoutInNotExpecting(lTimeOut);
  trace(L2,_T("-SetSlowConclTimeoutInNotExpecting"));
  
}
////////////////////////////////////////////////////////////////////////////
	
long SSProcedures::GetSCWatchdogTimeout()
{
long r;
  trace(L2, _T("+GetSCWatchdogTimeout"));
  r = (long)ssi.GetSCWatchdogTimeout();
  trace(L2, _T("-SetSlowConclusionTimeout: return <%d>"), r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
	
void SSProcedures::SetSCWatchdogTimeout(long lTimeOut)
{
	trace(L2,_T("+SetSCWatchdogTimeout TimeOut: %d"), lTimeOut);
	ssi.SetSCWatchdogTimeout(lTimeOut);
	trace(L2,_T("-SetSCWatchdogTimeout"));
}
////////////////////////////////////////////////////////////////////////////
	
long SSProcedures::GetZeroWeightTimeout()
{
long r;
  trace(L2, _T("+GetZeroWeightTimeout"));
  r = (long)ssi.GetZeroWeightTimeout();
  trace(L2, _T("-GetZeroWeightTimeout: return <%d>"), r);
  return r;
}
////////////////////////////////////////////////////////////////////////////

void SSProcedures::SetZeroWeightTimeout(long lTimeOut)
{
	trace(L2,_T("+SetZeroWeightTimeout TimeOut: %d"), lTimeOut);
	ssi.SetZeroWeightTimeout(lTimeOut);
	trace(L2,_T("-SetZeroWeightTimeout"));
}
////////////////////////////////////////////////////////////////////////////
	
long SSProcedures::GetLastGoodWeight()
{
long r;
  trace(L2, _T("+GetLastGoodWeight"));
  r = (long)ssi.GetLastGoodWeight();
  trace(L2, _T("-GetLastGoodWeight: return <%d>"), r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
	
void SSProcedures::SetLastGoodWeight(long lLGW)
{
	//tar 202079
	if (lLGW == 134217728)
	{
		trace(L2,_T("+SetLastGoodWeight LGW: 0x%x, reset LGW to current total weight, clear weight expectation, all SmartScale timers stopped."), lLGW);
	}
	else if (lLGW == 136314880)  //TAR228860 022403: 136314880=0x8200000
	{
		trace(L2,_T("+SetLastGoodWeight LGW: 0x%x, reset LGW and clear previous weight structure saved in SmartScale for possible late bagging of light items."), lLGW);
	}
    else
	{
		trace(L2,_T("+SetLastGoodWeight LGW: %d"), lLGW);
	}
	ssi.SetLastGoodWeight(lLGW);
	//TAR 216262 When we tell smart scale to set the LGW, we also should update 
	//global variable g_lLGW to the current LGW
	SMStateBase::g_lLGW = GetLastGoodWeight();
	trace(L2,_T("-SetLastGoodWeight"));
}
////////////////////////////////////////////////////////////////////////////
	
BOOL SSProcedures::GetHoldWeight()
{
  BOOL r;
  trace(L2,_T("+GetHoldWeight"));
  if (ssi.SSOpened)  //Prevent asserts if done before Scale opened
  {
      r = ssi.GetHoldWeight();
  }
  trace(L2,_T("-GetHoldWeight"));
  return (long)r;
}
////////////////////////////////////////////////////////////////////////////
	
void SSProcedures::SetHoldWeight(BOOL bHold)
{
    trace(L2,_T("+SetHoldWeight bHold: %d"), bHold);
    if (ssi.SSOpened) //Prevent asserts if done before Scale opened
    {
        ssi.SetHoldWeight(bHold);
    }
	trace(L2,_T("-SetHoldWeight"));
}
////////////////////////////////////////////////////////////////////////////
BOOL SSProcedures::GetSkipBagging()
{
  BOOL r;
  trace(L2,_T("+GetSkipBagging"));
  r = ssi.GetSkipBagging();
  trace(L2,_T("-GetSkipBagging return <%d>"), r);
  return (long)r;
}
////////////////////////////////////////////////////////////////////////////
void SSProcedures::SetSkipBagging(BOOL bSkipbagging)
{
	    trace(L2,_T("+SetSkipBagging bSkipBagging: %d"), bSkipbagging);
	 	ssi.SetSkipBagging(bSkipbagging);
		trace(L2,_T("-SetSkipBagging"));
}
////////////////////////////////////////////////////////////////////////////
long SSProcedures::GetHeavyWeightLimit()
{
long r;
  trace(L2, _T("+GetHeavyWeightLimit"));
  r = (long)ssi.GetHeavyWeightLimit();
  trace(L2, _T("-GetHeavyWeightLimit: return <%d>"), r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
void SSProcedures::SetHeavyWeightLimit(long lHWL)
{
	trace(L2,_T("+SetHeavyWeightLimit HWL: %d"), lHWL);
	ssi.SetHeavyWeightLimit(lHWL);
	trace(L2,_T("-SetHeavyWeightLimit"));
}
////////////////////////////////////////////////////////////////////////////
long SSProcedures::GetMeduimWeightLimit()
{
long r;
  trace(L2, _T("+GetMeduimWeightLimit"));
  r = (long)ssi.GetMeduimWeightLimit();
  trace(L2, _T("-GetMeduimWeightLimit: return <%d>"), r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
void SSProcedures::SetMeduimWeightLimit(long lSMWL)
{
	trace(L2,_T("+SetMeduimWeightLimit SMWL: %d"), lSMWL);
	ssi.SetMeduimWeightLimit(lSMWL);
	trace(L2,_T("-SetMeduimWeightLimit"));
}
////////////////////////////////////////////////////////////////////////////
long SSProcedures::GetHeavyWeightTol()
{
long r;
  trace(L2, _T("+GetHeavyWeightTol"));
  r = (long)ssi.GetHeavyWeightTol();
  trace(L2, _T("-GetHeavyWeightTol: return <%d>"), r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
void SSProcedures::SetHeavyWeightTol(long lSHWT)
{	
	trace(L2,_T("+SetHeavyWeightTol SHWT: %d"), lSHWT);
	ssi.SetHeavyWeightTol(lSHWT);
	trace(L2,_T("-SetHeavyWeightTol"));
}
////////////////////////////////////////////////////////////////////////////
long SSProcedures::GetMediumWeightTol()
{	
long r;
  trace(L2, _T("+GetMediumWeightTol"));
  r = (long)ssi.GetMediumWeightTol();
  trace(L2, _T("-GetMediumWeightTol: return <%d>"), r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
void SSProcedures::SetMediumWeightTol(long lSMWT)
{
	trace(L2,_T("+SetMediumWeightTol SMWT: %d"), lSMWT);
	ssi.SetMediumWeightTol(lSMWT);
	trace(L2,_T("-SetMediumWeightTol"));
}
////////////////////////////////////////////////////////////////////////////
long SSProcedures::GetLightWeightTol()
{	
long r;
  trace(L2, _T("+GetLightWeightTol"));
  r = (long)ssi.GetLightWeightTol();
  trace(L2, _T("-GetLightWeightTol: return <%d>"), r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
void SSProcedures::SetLightWeightTol(long lLWT)
{		
	trace(L2,_T("+SetLightWeightTol LWT: %d"), lLWT);
	ssi.SetLightWeightTol(lLWT);
	trace(L2,_T("-SetLightWeightTol"));
}

////////////////////////////////////////////////////////////////////////////
BOOL SSProcedures::GetInAttract()
{
  BOOL r;
  trace(L2,_T("+GetInAttract"));
  r = ssi.GetInAttract();
  trace(L2,_T("-GetInAttract return <%d>"), r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
void SSProcedures::SetInAttract(BOOL bInAttract)
{
	    trace(L2,_T("+SetInAttract bInAttract: %d"), bInAttract);
	 	ssi.SetInAttract(bInAttract);
		trace(L2,_T("-SetInAttract"));
}
////////////////////////////////////////////////////////////////////////////

void SSProcedures::GetStatus(BSTR* bstrDeviceName, BSTR* bstrCheckHealthText, long* lStatus)
{
	trace(L2,_T("+GetStatus"));
	ssi.GetStatus(bstrDeviceName,bstrCheckHealthText,lStatus);
	//tar 202079
	CString csDeviceName = *bstrDeviceName;
	CString csCheckHealthText = *bstrCheckHealthText;
	long lTmpStatus = *lStatus;	 
	trace(L2,_T("-GetStatus DeviceName: %s, CheckHealthText: %s, Status: %d"), csDeviceName, csCheckHealthText, lTmpStatus);		
}
////////////////////////////////////////////////////////////////////////////	
void SSProcedures::ResetTimers(long nTimer)
{
	trace(L2,_T("+ResetTimers Timer: %d"), nTimer);
	ssi.ResetTimers(nTimer);
	trace(L2,_T("-ResetTimers"));
}
////////////////////////////////////////////////////////////////////////////
void SSProcedures::SetSoldItemInfo(LPDISPATCH lpdispWeightInfo)
{ 
	  //tar 193806
	  IWTInfoPtr pTempWTInfoObj = lpdispWeightInfo;
	  CString csUPC;
	  //long lItemWt;
	  //pTempWTInfoObj->get_lItemWT(&lItemWt);
	  csUPC = io.csOrigCode;// csItemCode; 
	  //trace(L2,"+SetSoldItemInfo Raw UPC: %s, Wt: %d", csUPC, lItemWt);

      
	  //CHU+ modified the trace 030702
	  trace(L2,_T("+SetSoldItemInfo Raw UPC: %s"), csUPC);
	  VARIANT varWLDBArray;
	  WLDBEntry* m_pWLDBEntryArray;
	   // Get the WLDB Entries for the scanned item
      if (pTempWTInfoObj)
	  {

		if(!(pTempWTInfoObj->get_WLDBEntries(&varWLDBArray) == E_FAIL))
		{
		
		  // Increment the lock count of an array, and retrieve a pointer to the array data    	
		  HRESULT hr = SafeArrayAccessData(varWLDBArray.parray,(void**)&m_pWLDBEntryArray);
		  trace(L7, _T("hr: %s"), hr);
		  if (SUCCEEDED(hr))
		  {
			for(int i=0; i < pTempWTInfoObj->NumWLDBEntries; i++)
			{
				trace(L2,_T("SetSoldItemInfo: Weight Entry #%d: %d. Non-WLDB Weight = %d (If non-zero, this item is either Price Embedded OR Weight Needed - using ScannerScale, %d, or Calculated wt, %d, from TB Extended Price/Unit Price.  Did not come from WLDB)."), 
						i+1,m_pWLDBEntryArray[i].lexpectedWt, io.lTotalWeightEntered, io.l_gPriceEmbeddedWt, io.l_gPriceEmbeddedWt);
			}
			SafeArrayUnaccessData(varWLDBArray.parray);
		  }
		  else
		  {
			trace(L2, _T("                          no WLDB entry"));
		  }
		  //CHU-
		}
		else
		{
			trace(L2, _T("                          no WLDB entry"));
		}
	  
	        ssi.SetSoldItemInfo(lpdispWeightInfo);
	  }		

	  else
		  trace(L6,_T("not valid"));

   trace(L2,_T("-SetSoldItemInfo"));
	  
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
long SSProcedures::GetCurrentTotalWeight()
{	
long r;
  trace(L2, _T("+GetCurrentTotalWeight"));
  r = (long)ssi.GetCurrentTotalWeight();
  trace(L2, _T("-GetCurrentTotalWeight: return <%d>"), r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
void SSProcedures::SetCurrentTotalWeight(long lCTW)
{		
	trace(L2, _T("+SetCurrentTotalWeight CTW: %d"), lCTW);
	ssi.SetLightWeightTol(lCTW);
	trace(L2, _T("-SetCurrentTotalWeight"));
}
////////////////////////////////////////////////////////////////////////////
	
long SSProcedures::GetNewItemZeroWeightTimeout()
{
long r;
  trace(L2, _T("+GetNewItemZeroWeightTimeout"));
  r = (long)ssi.GetNewItemZeroWeightTimeout();
  trace(L2, _T("-GetNewItemZeroWeightTimeout: return <%d>"), r);
  return r;
}
////////////////////////////////////////////////////////////////////////////

void SSProcedures::SetNewItemZeroWeightTimeout(long lTimeOut)
{
	trace(L2,_T("+SetNewItemZeroWeightTimeout: %d"), lTimeOut);
	ssi.SetNewItemZeroWeightTimeout(lTimeOut);
	trace(L2,_T("-SetNewItemZeroWeightTimeout"));
}
////////////////////////////////////////////////////////////////////////////
