//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeReceiptBase.cpp
//
// TITLE: Class implementation for Take Receipt State
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMTakeReceipt.h"      // Take receipt state
#include "SMThankYou.h"             // Thank you state
#include "SMSecUnExpectedIncrease.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("TakeReceiptBase")
#else
#include "FakeKVParmList.h"
#define CKeyValueParmList CFakeKeyValueParmList
#define CKeyValue CFakeKeyValue
#include "SMTakeReceiptBase.h"
#endif

IMPLEMENT_DYNCREATE(SMTakeReceiptBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(TakeReceipt)


DEFINE_TIMESTAMP

//////////////////////////////
SMTakeReceiptBase::SMTakeReceiptBase()
: retryReceiptAttempts(0)
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////
SMStateBase  *SMTakeReceiptBase::Initialize(void)
{
       
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("TakeReceipt") );	// RFC 330050
        
        //+TAR 374645
        SMStateBase *sReturnState = CkStateStackAndReturn();
	if(sReturnState != STATE_NULL)
		return sReturnState;
        //-TAR 374645
        
	// CANNOT enter Remote Assist Mode at this state
	ra.RequestEnableAssistMode(false);   //TAR 213249

	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_TAKERECEIPT, SCOT_LANGUAGE_PRIMARY));

	if (((!nTrxReceiptLength && !co.fOperationsTBControlsPrinter) 
		&& !m_bEOTPrintPending) ||
		(co.fOperationsTBControlsPrinter && !AreThereAnySoldVoidedItems())  || //Fiscal Printer configured SCOTAPP does not get a receipt
		(ReceiptPrintSuccessful() && m_bHasSeenTakeReceipt && !m_bEOTPrintPending) || 
		(( !fInVoidTrans && lVoidedItems <= 0 && lItems <= 0 && m_bEOTPrintPending)) || //TAR417129/TAR205127/239622 CHU if receipt has been printed, go to ThankYou.
		( !SMStateBase::bPrintReceipt )) //SR832, skip Take Receipt screen.
	{
	  m_bEOTPrintPending = false; //TAR417129
	  m_bHasSeenTakeReceipt = false; //TAR239622 CHU 
      // there is no receipt for this transaction
      RETURNSTATE(ThankYou)			// default time out will be used.
	}
    
    // any event, like a line print. will reset the time out
    // allow 3 secoinds of idle time after any event before
    // proceding to the ThankYou state
    
	   //SR697 gc185038
    if(m_bEOTPrintPending)
    {
        if (co.csOperationsEOTReceiptPrintingState == _T("NonInterventionState")) 
        {  	//There might be an intervention waiting, wait for a second
            nTimeOut=1;
        }
        else
        {  	//Call TimedOut to start the receipt print immediately 
            // TAR 398436 release object if not used
            SMStateBase *tempSMReturn = TimedOut();
            if (!((tempSMReturn >= STATE_NULL) && (tempSMReturn <= STATE_ERROR)))
            { 
                // If not a special code, delete it
                delete tempSMReturn;
            }
        }
    }
    else
    {
        nTimeOut = co.getTimeOutTakeReceipt();
    }

	//TAR 200294 CMal and NN 040502 Do not reset the timeout when DMEvent is fired 
	//For this TAR changes have been made to SMStateBase.cpp, SMAttractBase.cpp, SMTakeReceiptBase.cpp, and SCOTAPPDlg.cpp
	//if events are fired then do not reset the timeout, process it
	bDoNotResetTimeOutOnDMEvent = true;  // Don't let DMEvents affect timers

	// NewUI
	// +SR218 dont print on cancelled transaction
	//return to thank you state immediately if transaction cancelled and flag is false
 	if ( !co.GetfOperationsPrintReceiptOnCancelledTransaction() && fInVoidTrans )
			RETURNSTATE(ThankYou)
	else
	//show take receipt if flag is true, either transaction is cancelled or not
			ps.SetCMFrame(_T("TakeReceipt"));
	//-SR218
	// NewUI

	if (fInSuspendTrans)    //TAR#117305 If xact suspended, show Amount due
	  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	else
	  ps.ShowChangeDueAmountPaid((lTotalPaid-lTotalSale),lTotalPaid, false);

	//ps.Message12(MSG_TAKERECEIPT,true);  //TAR#285389
	// this button-enable is leaving to access to store mode 
	// which may throw SCOT into many other unknown problems
	//  ps.CMButton(IDC_BUTTONHELP,BTT_HELP,false); 

	// 228238 - this wave file shouldn't be interruptable LPM021803
	// RFC 233718 - Don't say receipt will print below payment device

	trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(MSG_RA_RECEIPTPRINTING, SCOT_LANGUAGE_PRIMARY));
	ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+
                  ps.GetPSText(MSG_RA_RECEIPTPRINTING, SCOT_LANGUAGE_PRIMARY));
	trace(L7, _T("-ra.OnNormalItem"));

	ps.ShowCMFrame(false);
	m_bHasSeenTakeReceipt = true; //TAR239622 CHU
	return STATE_NULL;
}

/////////////////////////////////////
SMStateBase  *SMTakeReceiptBase::TimedOut(void)
{
  if ( (ReceiptPrintSuccessful() && !m_bEOTPrintPending) || (retryReceiptAttempts > 50) )
  {
      DMSaySecurity(SEC_NONE, ps.GetSoundName(TAKERECEIPT));
      bDoNotResetTimeOutOnDMEvent = false; // TAR 200294 NN 041602 - leaving TakeReceipt, back to normal
      RETURNSTATE(ThankYou)					  // default time out will be used.
  }
 
  else if(m_bEOTPrintPending )
  {

	nTimeOut = co.getTimeOutTakeReceipt(); //TAR417129
	m_bForceReceiptPrint = true; //flag to force receipt printing
	EndOfTransactionPrint();	
	return STATE_NULL;
  }

  else
  {
    // wait until receipt finishes correctly, try again in a second
    nTimeOut = 1;
    retryReceiptAttempts++;
	trace(L6, _T("Waiting for print completion: retryReceiptAttempts:%d"), retryReceiptAttempts);
    return STATE_NULL;
  }
}

#ifndef _CPPUNIT
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMTakeReceiptBase::OnWtDecrease()
{
	FinalizationWtDecrease();
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMTakeReceiptBase::OnWtIncreaseNotAllowed()	 
{
  if ((ReceiptPrintSuccessful() && !m_bEOTPrintPending) || (retryReceiptAttempts > 50) )
	return FinalizationWtIncrease(BES_THANKYOU);	//TAR201863 wave file playing twice even tho receipt already printed LPM041702
  else
  	return FinalizationWtIncrease(BES_TAKERECEIPT);//TD 04/05/02 TAR 200394 

}


//////////////////////////////////////////////////////////////////////////
void SMTakeReceiptBase::UnInitialize()
{
  // CAN enter Remote Assist Mode after leaving this state
  //ra.OnSetTransactionState(RA_STATE_ASSISTOK);	//TAR 204027
   CKeyValueParmList parmList;
   CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_RESUME);
   _bstr_t bstrParmList((const _TCHAR *)parmList);
   BSTR bstrResult;
   bstrResult = NULL;
   SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
   SysFreeString(bstrResult);
   TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("TakeReceipt") );	// RFC 330050
}
#endif //_CPPUNIT