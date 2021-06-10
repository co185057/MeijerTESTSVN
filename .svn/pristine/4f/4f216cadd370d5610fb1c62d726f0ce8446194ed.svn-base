//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeReceipt.cpp
//
// TITLE: Class impliementation for Take Receipt State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMTakeReceipt.h"          // MGV take receipt state
#include "SMThankYou.h"
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("TakeReceipt")

IMPLEMENT_DYNCREATE(SMTakeReceipt, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////
SMTakeReceipt::SMTakeReceipt()
: SMTakeReceiptBase()
{
  IMPLEMENT_TIMESTAMP
}

#ifndef _TRADESHOWDEMO
SMStateBase * SMTakeReceipt::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("TakeReceipt") );	// RFC 330050
    
	// start CRD 125377 don't display cancel order button on thank you 
	if 	(m_bShopAndDropEnabled)
    {
		m_pAppPSX->SetTransactionVariable(_T("ShopAndDropEnabledYesNoButtons"), VARIANT_FALSE); 
	}
	// end CRD 125377

    //+TAR 374645
    SMStateBase *sReturnState = CkStateStackAndReturn();
	if(sReturnState != STATE_NULL)
		return sReturnState;
    //-TAR 374645

	if (true == m_bCancellingCashTender) 
    {
		trace(L6, _T("SMTakeReceipt::Initialize canceling a tender. Returning to scan and bag"));
	
		nTenderType = TB_TENDER_NONE;
        m_bCancellingCashTender = false; // resets the global variable
		return createAnchorState();	
    } 
	//+TAR357192	
	CString csPropName = _T("ReleaseReceipt");
    CString csPropValue;
    SendTBCommand(TBCMD_GET_TBPROPERTY, &csPropName, &csPropValue);
	if (csPropValue == _T("1"))
	{
		csPropValue = _T("0");
		SendTBCommand(TBCMD_SET_TBPROPERTY, &csPropName, &csPropValue);

		CString csTBState;
		SendTBCommand(TBCMD_SET_TB_STATE,_T("TRXEND_DELAY_RECEIPT"));
	}
	//-TAR357192

	// CANNOT enter Remote Assist Mode at this state
	ra.RequestEnableAssistMode(false);   //TAR 213249

	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_TAKERECEIPT, SCOT_LANGUAGE_PRIMARY));

	if (((!nTrxReceiptLength && !co.fOperationsTBControlsPrinter) 
		&& !m_bEOTPrintPending) ||
		(co.fOperationsTBControlsPrinter && !AreThereAnySoldVoidedItems())  || //Fiscal Printer configured SCOTAPP does not get a receipt
		(ReceiptPrintSuccessful() && m_bHasSeenTakeReceipt && !m_bEOTPrintPending) || 
		(( !fInVoidTrans && lVoidedItems <= 0 && lItems <= 0 && m_bEOTPrintPending))) //TAR417129/TAR205127/239622 CHU if receipt has been printed, go to ThankYou.
	{
		m_bEOTPrintPending = false; //TAR417129
		m_bHasSeenTakeReceipt = false; //TAR239622 CHU 

		// there is no receipt for this transaction
        CString strPropName = _T("PrintGiftReceipt");
        CString strPrintGiftReceipt;
        SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strPropName, &strPrintGiftReceipt);

        strPropName = _T("RebateReceiptRequested");
        CString strPrintRebateReceipt;
        SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strPropName, &strPrintRebateReceipt);
        if( strPrintGiftReceipt == _T("0") && strPrintRebateReceipt == _T("0"))
        {
            RETURNSTATE(ThankYou)			// default time out will be used.
        }
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
                 	TimedOut();
		}
	}
    else
    {
        nTimeOut = 3;
    }
	//TAR 200294 CMal and NN 040502 Do not reset the timeout when DMEvent is fired 
	//For this TAR changes have been made to SMStateBase.cpp, SMAttractBase.cpp, SMTakeReceiptBase.cpp, and SCOTAPPDlg.cpp
	//if events are fired then do not reset the timeout, process it
	bDoNotResetTimeOutOnDMEvent = true;  // Don't let DMEvents affect timers

	// NewUI
	// +SR218 dont print on cancelled transaction
	//return to thank you state immediately if transaction cancelled and flag is false
 	if ( !co.fOperationsPrintReceiptOnCancelledTransaction && ( fInVoidTrans || (lVoidedItems > 0 && lItems <= 0)) )
			RETURNSTATE(ThankYou)
	else
	//show take receipt if flag is true, either transaction is cancelled or not
			ps.SetCMFrame(_T("TakeReceipt"));
	//-SR218
	// NewUI

	if (fInSuspendTrans)    //TAR#117305 If xact suspended, show Amount due
	  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	else
	  ps.ShowChangeDueAmountPaid(lChangeDue,lTotalPaid);

	//ps.Message12(MSG_TAKERECEIPT,true);  //TAR#285389
	// this button-enable is leaving to access to store mode 
	// which may throw SCOT into many other unknown problems
	//  ps.CMButton(IDC_BUTTONHELP,BTT_HELP,false); 

	// 228238 - this wave file shouldn't be interruptable LPM021803
	// RFC 233718 - Don't say receipt will print below payment device

	trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(MSG_RA_RECEIPTPRINTING, SCOT_LANGUAGE_PRIMARY));
	ra.OnNormalItem(_T("[highlight]")+
                  ps.GetPSText(MSG_RA_RECEIPTPRINTING, SCOT_LANGUAGE_PRIMARY));
	trace(L7, _T("-ra.OnNormalItem"));

	ps.ShowCMFrame(false);

    CheckGiftRebateReceipt();

	m_bHasSeenTakeReceipt = true; //TAR239622 CHU
	return STATE_NULL;
}
#else
#include "..\TradeShowDemo\SMTakeReceiptTSDemo.cpp"
#endif //_TRADESHOWDEMO

void SMTakeReceipt::UnInitialize(void)
{
    SMTakeReceiptBase::UnInitialize();
}

SMStateBase * SMTakeReceipt::OnWtIncreaseNotAllowed(void)
{
    return SMTakeReceiptBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMTakeReceipt::OnWtDecrease(void)
{
    return SMTakeReceiptBase::OnWtDecrease();
}

SMStateBase * SMTakeReceipt::TimedOut(void)
{
	if ( (ReceiptPrintSuccessful() && !m_bEOTPrintPending) || (retryReceiptAttempts > 20) )
	{
        bDoNotResetTimeOutOnDMEvent = false; // TAR 200294 NN 041602 - leaving TakeReceipt, back to normal
		
        CString strPropName = _T("PrintGiftReceipt");
        CString strPrintGiftReceipt;
        SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strPropName, &strPrintGiftReceipt);
		
        if ( ((hReceiptFile == NULL) && (strPrintGiftReceipt == _T("0"))) ||
			(retryReceiptAttempts > 20) )  
        {
            RETURNSTATE(ThankYou)					// default time out will be used.
        }
        else
        {
            trace(L6, _T("!! Still Printing.   PrintGiftReceipt=%s"), strPrintGiftReceipt );
            retryReceiptAttempts++;
            return STATE_NULL;
        }
    } else if(m_bEOTPrintPending )
	{
		nTimeOut = 3; //TAR417129
		m_bForceReceiptPrint = true; //flag to force receipt printing
		EndOfTransactionPrint();	
		return STATE_NULL;
	}
    else
    {
		// wait until receipt finishes correctly, try again in a second
		nTimeOut = 1;
		retryReceiptAttempts++;
		trace(L6, _T("!!! Receipt Print Error: retryReceiptAttempts:%d"), retryReceiptAttempts);
		return STATE_NULL;
    }
    //return SMTakeReceiptBase::TimedOut();
}

SMStateBase *SMTakeReceipt::CheckGiftRebateReceipt(void)
{
	CString strPropName = _T("PrintGiftReceipt");
	CString strPrintGiftReceipt;
	SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strPropName, &strPrintGiftReceipt);
	
	strPropName = _T("RebateReceiptRequested");
	CString strPrintRebateReceipt;
	SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strPropName, &strPrintRebateReceipt);
	
	if( strPrintGiftReceipt != _T("0") || strPrintRebateReceipt == _T("1"))
	{
		DMSayPhrase(_T("TakeReceipt_WaitForRebate.wma"));
	}
	else DMSayPhrase(_T("TakeReceipt.wma"));
	
	//if( strPrintRebateReceipt == _T("1")) 
	//{
	//  DMSayPhrase(_T("TakeRebateReceipt.wma"));
	//}
	
	//RETURNSTATE(PrintGiftReceipt);		// continue with gift receipt printing
	
	return STATE_NULL;
}
