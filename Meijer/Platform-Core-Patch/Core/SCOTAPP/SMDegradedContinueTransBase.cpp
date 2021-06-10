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
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMDegradedContinueTrans.h"      // This state
#include "DMProcedures.h"
#include "SMScanAndBag.h"
#include "SMSecUnexpectedIncrease.h"

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


SMStateBase *SMDegradedContinueTransBase::PSButton1(void)	//Yes Button
{
   // if item is scanned from attract, wait to go to proper sale screen
   if (fItemIsWaiting)
   {
      setAnchorState(BES_SCANANDBAG);
      SMStateBase* retState = TBItemSale(false,0,true);
      // go to inprogress
      return retState;
   }
   else
   {
      if( getAnchorState() == BES_PUTBAGONSCALE  ) 
      {
         return setAndCreateAnchorState(BES_PUTBAGONSCALE);
      }
         RETURNSTATE(ScanAndBag);
   }
}


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
}