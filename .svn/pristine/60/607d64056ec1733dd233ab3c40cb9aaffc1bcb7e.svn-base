//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMPutBagOnScaleBase.cpp
//
// TITLE: Class implementation for Put Bag On Scale State - Done for SR085 by Shakti Kamal
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMPutBagOnScale.h"        // This state
#include "SMAttract.h"
#include "SMScanAndBag.h"
#include "SMSecUnExpectedIncrease.h"
#include "DMProcedures.h"
#include "MPProcedures.h"
#else
#include "SMPutBagOnScaleBase.h"
#endif


#ifndef _CPPUNIT

DLLEXPORT extern HWND hw; 
DLLEXPORT extern MPProcedures mp;

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("PutBagOnScaleBase")

IMPLEMENT_DYNCREATE(SMPutBagOnScaleBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMPutBagOnScaleBase::SMPutBagOnScaleBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////
SMStateBase *SMPutBagOnScaleBase::Initialize(void)
{
  bDoNotResetTimeOutOnDMEvent = true; 
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("PutBagOnScale") );
  nTimeOut = co.getTimeOutDisplayPutBagOnScaleScreenTimer();
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(RA_PUTBAGONSCALE, SCOT_LANGUAGE_PRIMARY));
  DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);
  ps.SetCMFrame(_T("PutBagOnScale"));
  setAnchorState(BES_PUTBAGONSCALE);

  trace(L2, _T("::Initialize - bEASReEnableScanner is set to false."));
  bEASReEnableScanner = false;
  SAWLDBScannerDisable();//disable the scanner
  // + TAR 411230
  if(nTimeOut == 0)
  {
	TimedOut();
  }
  // - TAR 411230
  return STATE_NULL;//stay in PutBagOnScale state
}

void SMPutBagOnScaleBase::UnInitialize(void)
{
  bDoNotResetTimeOutOnDMEvent = false; 
  bEASReEnableScanner = true;
  SAWLDBScannerEnable();//enable the scanner
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("PutBagOnScale") );
}

//////////////////////////////////////////
SMStateBase  *SMPutBagOnScaleBase::PSButtonHelp(void)
{
	return STATE(State)::PSButtonHelp();
}
#endif

/////////////////////////////////////
SMStateBase *SMPutBagOnScaleBase::PSButton2(void) //"Done" Button
{
#ifndef _CPPUNIT
	//+SSCOP-3886 - disable Done and Help button once Done button is clicked
	ps.ButtonEnable(_T("CMButton2Med"), false);
	ps.ButtonEnable(_T("ButtonHelp"), false);
	//-SSCOP-3886 

	trace(L6, _T("+SMPutBagOnScaleBase Done bttn pressed"));
	DMSaySecurity( SEC_NONE, _T("blank.wav") );//to interrupt the wave file 
	if (co.IsSecurityBypassOn() || co.IsSecurityDisableOn())
	{
	   RETURNSTATE(ScanAndBag);
	}
	else
	{
		CKeyValueParmList parmList;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_DONE_BTTN);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		BSTR bstrResult;
		bstrResult = NULL;
		SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
		SysFreeString(bstrResult);

		return STATE_NULL;
	}
#else
    return STATE_NULL;
#endif
}

#ifndef _CPPUNIT

////////////////////////////////////////
SMStateBase *SMPutBagOnScaleBase::PSEnterKey(void)
{
  DMSayPhrase(KEYPADVALID);
  return PSButton2();          
}

////////////////////////////////////////
SMStateBase *SMPutBagOnScaleBase::OnWtIncreaseNotAllowed()	 
{
//SSCOADK-2787 Fix migrated from 5.0.1
	ps.ButtonEnable(_T("ButtonHelp"), false);
	return STATE_NULL;//gets handled by the Sec Mgr
}

#endif

////////////////////////////////////////
SMStateBase *SMPutBagOnScaleBase::TareNone(void)
{
	//return setAndCreateAnchorState(BES_SECBAGBOXNOTVALID);
    if (g_pDelay_WaitForApprovalStateChange != NULL)//SSCOADK-3166
	{
		delete g_pDelay_WaitForApprovalStateChange;
		g_pDelay_WaitForApprovalStateChange = NULL;
    }
	return setAndCreateAnchorState(BES_SCANANDBAG);
}

////////////////////////////////////////
SMStateBase *SMPutBagOnScaleBase::TareOnlyAtAttract(void)
{
	ClearSecurityViolationLight();
	ra.OnNoRAPDataNeeded();//tell RAP the person fixed their own error
    if (g_pDelay_WaitForApprovalStateChange != NULL)//SSCOADK-3166
	{
		delete g_pDelay_WaitForApprovalStateChange;
		g_pDelay_WaitForApprovalStateChange = NULL;
    }

	return setAndCreateAnchorState(BES_SCANANDBAG);
}
#ifndef _CPPUNIT
////////////////////////////////////////
SMStateBase *SMPutBagOnScaleBase::TimedOut(void)
{
	//ps.SetCMFrame(_T("PutBagOnScale")); //transfer to initialize() tar 392689
	if(nTimeOut == co.getTimeOutDisplayPutBagOnScaleScreenTimer())
	{
		if(nTimeOut !=0)	// TAR 411230
		{
			nTimeOut = co.getTimeOutStateDefault();//SW 776 Need to have the timer in this state
		}
		ps.ShowCMTBText(csTBMessage2Scot);
		ps.SetCMLeadthruText(LTT_PUTBAGONSCALE);
		ps.Message12(MSG_PUTBAGONSCALE, true);
		ps.SetButtonText(_T("CMButton2Med"), BTT_DONE);
		ps.SetButtonText(_T("ButtonHelp"), BTT_HELP);

		//SSCOI-40789+ migrated from ADK 5.0.1
	        if(ImmediateInterventionStack.GetCount()>0)
        	{
	            ps.ButtonEnable(_T("ButtonHelp"), false);
	        }
		//SSCOI-40789-

		DMSayPhrase(PUTBAGONSCALE);
		ps.ShowCMFrame();
		return STATE_NULL;
	}
	else
	{
        return PSButton2();
	}
}
SMStateBase *SMPutBagOnScaleBase::OnBackToLGW(void)
{
    
    trace(L2, _T("SMPutBagOnScaleBase::OnBackToLGW, staying at the current state "));
    // + TAR 404971
    if(bQuietNeedSecApproval)  
    {
        bQuietNeedSecApproval = false;
    }
    // - TAR 404971 

    // TAB re-enable scanner and clear Wait For Approval State
    TABReEnableScannerAndClearWaitForApprovalState();

    // resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
    SAClearSecurityCondition();

    return STATE_NULL;
}
#endif
