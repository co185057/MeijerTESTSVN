//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMDegradedContinueTransBase.cpp
//
// TITLE: Class implementation for Degraded Continue Transaction State
//
// AUTHOR:    Allen Jay Cagaanan
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMDegradedContinueTrans.h"      // This state
#include "DMProcedures.h"
#include "SMScanAndBag.h"
#include "SMSecUnexpectedIncrease.h"
#else
#include "SMDegradedContinueTransBase.h"     
#endif

#ifndef _CPPUNIT
DLLEXPORT extern DMProcedures dm;   // Global DM object

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("DegradedContinueTransBase")

IMPLEMENT_DYNCREATE(SMDegradedContinueTransBase, CObject)// MFC Runtime class/object information

//pIMPLEMENT_STATE(DegradedContinueTrans)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMDegradedContinueTransBase::SMDegradedContinueTransBase(void)
{
   IMPLEMENT_TIMESTAMP
}


///////////////////////////////////////
SMStateBase *SMDegradedContinueTransBase::Initialize(void)
{
   TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("DegradedContinueTrans") );
   UpdateStateName(ps.GetPSText(LTT_CONTRANS, SCOT_LANGUAGE_PRIMARY));
   //nTimeOut = co.getTimeOutContinueTransaction();
	
   DMSayPhrase(DOYOUWISHTOCONTINUE);
   SAWLDBScannerDisable();   // Disable the scanner
   
   if( getAnchorState() != BES_PUTBAGONSCALE )
   {
      setAnchorState(BES_DEGRADEDCONTINUETRANS);
   }
   ra.RequestEnableAssistMode(false);
   // NewUI  
   ps.SetCMFrame(_T("ContinueTrans"));
   ps.CMButtonFlash(_T("CMButton1Med"),true);   //  Flash the Yes button.
   ps.ShowAmountDueAmountPaid(lBalanceDue - nDMCashInserted, 
                             lTotalPaid  + nDMCashInserted);
   ps.ShowCMTBText(csTBMessage2Scot);
   // End NewUI
   
   ps.ButtonState(_T("ButtonHelp"), false, false); //TAR418792

   if ( dm.GetDegradedModeType()  == DEGRADEDMODE_CARD_ONLY)
   {
      ps.SetLeadthruText(LTT_CREDIT_DEBIT_ONLY, PS_BLANK);
      ps.Message12(MSG_CREDITDEBITCARDMODE, true);
   }
   else if ( dm.GetDegradedModeType()  == DEGRADEDMODE_CASH_ONLY)
   {
      ps.SetLeadthruText(LTT_CASHONLYMODE, PS_BLANK);
      ps.Message12(MSG_CASHONLYMODE, true);
   }

   ps.ShowCMFrame();
   return STATE_NULL;                // stay in DegradedContinueTrans state

}
#endif

SMStateBase *SMDegradedContinueTransBase::PSButton1(void)	//Yes Button
{
   // if item is scanned from attract, wait to go to proper sale screen
   if (fItemIsWaiting)
   {
       if(m_bScanOnAttractAtQuietMode && !fStateAssist)
       {
           return setAndCreateAnchorState(BES_SCANANDBAG); //wait here for SM to fire exception
       }
#ifndef _CPPUNIT
       setAnchorState(BES_SCANANDBAG);
       SMStateBase* retState = TBItemSale(false,0,true);
       // go to inprogress
       return retState;
#endif
   }
   else
   {
#ifndef _CPPUNIT
      if( getAnchorState() == BES_PUTBAGONSCALE  ) 
      {
          CKeyValueParmList parmList;
          CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_CUST_HAS_BAG);
          _bstr_t bstrParmList((const _TCHAR *)parmList);
          BSTR bstrResult;
          bstrResult = NULL;
          SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
          SysFreeString(bstrResult);
          
          return setAndCreateAnchorState(BES_PUTBAGONSCALE);
      }
#endif
      RETURNSTATE(ScanAndBag);
   }
}

#ifndef _CPPUNIT


SMStateBase *SMDegradedContinueTransBase::PSButton2(void)	//No Button
{
   if (fRequestForHelp)
   {
      fRequestForHelp = false;
      trace(L7, _T("+ra.OnCallForHelpApproval"));
      ra.OnCallForHelpApproval();
      trace(L7, _T("-ra.OnCallForHelpApproval"));
   } 
   return setAndCreateAnchorState(BES_FINISH);		// default time out will be used.
}


SMStateBase *SMDegradedContinueTransBase::TimedOut(void)
{
   return PSButton2();
}


SMStateBase  *SMDegradedContinueTransBase::DMCardReader(void)
{
   return STATE_RETURN;
}


void SMDegradedContinueTransBase::UnInitialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("DegradedContinueTrans") );
  if (io.bLoyaltyCard)
  {	  
     CKeyValueParmList parmList;
     CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_RESUME);
     _bstr_t bstrParmList((const _TCHAR *)parmList);
     BSTR bstrResult;
     bstrResult = NULL;
     SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
     SysFreeString(bstrResult);
  }

}


SMStateBase *SMDegradedContinueTransBase::OnWtIncreaseNotAllowed()	 
{
    if(SMStateBase::g_pDelay_WaitForApprovalStateChange == NULL)
       SMStateBase::g_pDelay_WaitForApprovalStateChange = SMStateBase::createState(BES_SECUNEXPECTEDINCREASE);

    trace(L6,_T("We are at SMDegradedContinueTransBase::OnWtIncreaseNotAllowed(), Saving next state BES_SECUNEXPECTEDINCREASE in g_pDelay_WaitForApprovalStateChange."));

    return STATE_NULL;
}
#endif