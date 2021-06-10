//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMContinueTransBase.cpp
//
// TITLE: Class implementation for contionue transaction state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMContinueTrans.h"    // This state
#include "SMConfirmAbort.h"         // Confirm abort state
#include "SMWaitForApproval.h"      // Wait for approval
#include "SMFinish.h"               // Finish up
#include "SMRequestSig.h"			// TAR126645 
#include "SMScanAndBag.h"			// SR742

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("ContinueTransBase")

IMPLEMENT_DYNCREATE(SMContinueTransBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ContinueTrans)


DEFINE_TIMESTAMP

//////////////////////////////////////
SMContinueTransBase::SMContinueTransBase(void)
{
  IMPLEMENT_TIMESTAMP
}

/////////////////////////////////////////
SMStateBase *SMContinueTransBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ContinueTrans") );	// RFC 330050
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_CONTRANS, SCOT_LANGUAGE_PRIMARY));
  nTimeOut = co.getTimeOutContinueTransaction();
	
  DMSayPhrase(DOYOUWISHTOCONTINUE);
  SAWLDBScannerDisable();   // Disable the scanner
  // NewUI  
  ps.SetCMFrame(_T("ContinueTrans"));
  ps.CMButtonFlash(_T("CMButton1Med"),true);   //  Flash the Yes button.
  ps.ShowAmountDueAmountPaid(lBalanceDue - nDMCashInserted, 
                             lTotalPaid  + nDMCashInserted);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  // End NewUI

  ps.ButtonState(_T("ButtonHelp"), true, true); //TAR418792

  ps.SetLeadthruText(LTT_CONTRANS, PS_BLANK); //SR742
  ps.Message12(MSG_CONTRANS, true);
  
  	// BeginCodeWLDBSA11
	// In case of time-out, we set to FALSE this flag
	// That is a small security in case of problem with SA.
	fInWLDBProcess = FALSE;
	// EndCodeWLDBSA11
  ps.ShowCMFrame();
  return STATE_NULL;                // stay in ContinueTransBase state
}

///////////////////////////////////////
SMStateBase *SMContinueTransBase::TimedOut(void)
{
  ps.DisableButtons();
  // if an item was in the middle of being sold, like at the qty needed screen then it needs to be canceled.  will not hurt anything if it isn't needed
  //+ TAR 384891 and TAR 386017
  if(!fInMultiSelectPickList)
  {
      SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
  }
  //- TAR 384891 and TAR 386017
  if ( isAnySoldVoidedItems() )
  {
    CREATE_AND_DISPENSE(WaitForApproval)(false,true);
  }
  else
  {
    return PSButton2();
  }
}

////////////////////////////////////////
SMStateBase *SMContinueTransBase::PSButton1(void) // Yes, continue trans
{
  /*if ((anchorState == BES_SELECTCARDTYPE) && co.m_bOperationsVerifySignature) //TAR126645  needs to return to RequestSig, not ConfirmSig LPM111999
  //This only works from timing out from the signature confirm screen
  //if the timeout is from the select card type the below is wrong
  //if the timeout is from after the approval needed and store confirm sig the below is wrong again
  //a state needs to be added for request signature so this can return to there from here
  //delete these comments when tar 126645 is fixed. I will be on vacation therefore this comment...
  {
	RETURNSTATE(RequestSig);
  }
  else
  {}*/
    if (bIsFromVoidApproval)  //sscoadk-5198
	   return STATE_RETURN;
    else
       return createAnchorState();
}

////////////////////////////////////////
SMStateBase *SMContinueTransBase::PSButton2(void) // no, abort trans
{
	//SR879
	if	(g_pRememberStateBeforeHelp != NULL)
	{	
		CString csStateName = g_pRememberStateBeforeHelp->GetRuntimeClass()->m_lpszClassName;
		delete g_pRememberStateBeforeHelp;
		g_pRememberStateBeforeHelp = NULL;
		trace(L6,_T("Delete the state %s saved in g_pRememberStateBeforeHelp"),csStateName);
	}
	
	if (fSAInItemSale && !fInMultiSelectPickList) //+tar 389947
	{
		SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
		setAnchorState(BES_SCANANDBAG);
	}//-tar 389947

  if ( isAnySoldVoidedItems() )
    RETURNSTATE(ConfirmAbort);
	
   // if an item was in the middle of being sold, like at the qty needed screen then it needs to be canceled.  will not hurt anything if it isn't needed
   	//+ SSCOP-3552
    if(getAnchorState() != BES_START) 
    {
        SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
    }
    //else
    //{
        //if(!fSAInTransaction)
            //SASendSCOTInput( ITEMIZATION );
    //}
    //- SSCOP-3552
	
  //return new SMThankYou;   // default time out will be used.

	if (fRequestForHelp)
	{
        //! turn off the tricolor light
        SMStateBase Obj;
        Obj.DMTurnOffTriColorLight();    
		fRequestForHelp = false;
		trace(L7, _T("+ra.OnCallForHelpApproval"));
		ra.OnCallForHelpApproval();
		trace(L7, _T("-ra.OnCallForHelpApproval"));
	}

   	if(getAnchorState() == BES_START || getAnchorState() == BES_LANGUAGESELECT) //SSCOP-3552
	{
		return setAndCreateAnchorState(BES_START);
	}

  return setAndCreateAnchorState(BES_FINISH);						  // default time out will be used.
}

/////////////////////////////////////////
//SMStateBase *SMContinueTransBase::PSEnterKey(void)// Yes
//{
//  DMSayPhrase("KeyPadValid");
//  return PSButton2();
//}

//////////////////////////////////////////
//SMStateBase *SMContinueTransBase::PSCancelKey(void) /// override where needed
//{
//  DMSayPhrase("KeyPadValid");
//  return PSButton3();             // usually same as button 3
//}


//TAR323145
//We only get here because of a racing condition: MSR should be disabled at uninitialization of 
//the previous state but if the card is swipped at leaving the previous state...
/////////////////////////////////////
// DMCardReader - card inserted
/////////////////////////////////////
SMStateBase  *SMContinueTransBase::DMCardReader(void)
{
	return STATE_RETURN;

}
//TAR323145-

// + RFC 330050
void SMContinueTransBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ContinueTrans") );	
}
// - RFC 330050
