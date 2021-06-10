//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCardProcessingBase.cpp
//
// TITLE: Class impliementation for check signature state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMCardProcessing.h"   
#include "SMInProgress.h"           // In progress state
#include "SMSystemMessage1.h"		// SSCOADK-1110
#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("CardProcessingBase")

const long NCR_DO_NOT_INSERT = 1007;

IMPLEMENT_DYNCREATE(SMCardProcessingBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(CardProcessing)


DEFINE_TIMESTAMP


////////////////////////////////////////
SMCardProcessingBase::SMCardProcessingBase()
{
  IMPLEMENT_TIMESTAMP
}

/////////////////////////////////////////////
SMStateBase  *SMCardProcessingBase::Initialize(void)
{
    // Tell security that we entered this state
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("CardProcessing") );
    // the below line has to be the first thing in this function
	// otherwise, there will be a race condition where RAP operator will manage to get into assistmode at RAP
	// CANNOT enter Remote Assist Mode at this state
    ra.RequestEnableAssistMode(false);  // TAR246532

	//TAR245485
	setAnchorState(BES_CARDPROCESSING);
	//TAR241756+ CHU 070203
	//If we are in this state and we have to go to SystemMessage, when we 
	//come back from SystemMessage, TB could have already finished its tasks
	//and told ScotApp to go to a new state, we need to go to that state instead of 
	//sitting here which makes ScotApp hung
    SMStateBase *sReturnState = CkStateStackAndReturn();
    if (sReturnState != STATE_NULL)
	  return sReturnState;
	//TAR241756-

	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_CARDPROCESSING, SCOT_LANGUAGE_PRIMARY));
   //@@@INFOPOINT

	ps.SetCMFrame(_T("CardProcessing"));

	CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMCardProcessingBase::Initialize"));

    if(     co.fOperationsPartialIntegratedPINPad 
		&& !co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bIntegratedMSR  //PIP 
		&& !fInLoyaltyCardProcessing) // merged TAR233032 to 3.1.1 - fixed by Chen
	{
		DMSayPhrase(USEPINPAD);
		ps.SetCMLeadthruText(LTT_PINPADENTRY);
		ps.Message12(MSG_PINPADENTRY, true);
	}
	else
	{
		ps.SetCMLeadthruText(LTT_CARDPROCESSING);
		ps.Message12(MSG_CARDPROCESSING, true);
	}



    ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	//@@@INFOPOINT
	
  // Display the Auxilary card processing screen
  DisplayAuxScreen(AUX_SCREEN_CARD_PROCESSING, TRUE);
	ps.ShowCMFrame(false);

  CREATE_AND_DISPENSE(InProgress)(storeMode(),
                            	helpMode(),
		                        co.getTimeOutTender());
}
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMCardProcessingBase::OnWtIncreaseNotAllowed()	 
{
	return FinalizationWtIncrease(BES_CARDPROCESSING);	

}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMCardProcessingBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering)
		return FinalizationWtDecreaseNotAllowed(BES_CARDPROCESSING);
	else
		return STATE_NULL;
}

//////////////////////////////////////////////////////////////////////////
void SMCardProcessingBase::UnInitialize()
{
    // Tell security that we exiting this state
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("CardProcessing") );

  // CAN enter Remote Assist Mode after leaving this state
 // ra.OnSetTransactionState(RA_STATE_ASSISTOK);	//TAR 209868
 // ra.RequestEnableAssistMode(true);   //TAR213249



}

//+TAR252694
SMStateBase  *SMCardProcessingBase::DMScanner(void) // scanned an item

{

  trace(L3, _T("Probable race condition, scanned code: %s is going to be ignored"),  csDMLastBarCode); 

  return STATE_NULL;

}
//-TAR252694

//+SSCOADK-1110
//If the bill/coin is accepted in this state, we will reject it
////////////////////////////////
SMStateBase *SMCardProcessingBase::DMAcceptor(void)
{
    long rc;
    trace(L6,_T("SMCardProcessingBase::DMAcceptor; can't accept coin/bill in this state."));
    long amountDispensed = 0; 
    rc = DMCashAcceptorReject();  // Coin Acceptor Modifications
    if (rc < 0)			//  most likely out of change
    {
        return DMCashChangerError(nDMLastBillEvent);
    }
    else if (nDMLastBillEvent > 0)
    {
        amountDispensed = nDMLastBillEvent;
        trace(L6, _T("Reject bill amount: %d"), nDMLastBillEvent);
        nDMCashInserted -= nDMLastBillEvent;
        nDMLastBillEvent = 0;
    }

    rc = DMCoinAcceptorReject();  // Coin Acceptor Modifications

    if (rc < 0)			
    {
        return DMCashChangerError(nDMLastCoinEvent);
    }
    else if (nDMLastCoinEvent > 0)
    {
        amountDispensed = nDMLastCoinEvent;
        trace(L6, _T("Reject coin amount: %d"), nDMLastCoinEvent);
        nDMCashInserted -= nDMLastCoinEvent;
        nDMLastCoinEvent = 0;
    }
 
    CString csTemp = EMPTY_STRING;
    csTemp.Format(ps.GetPSText(MSG_RA_CHANGEDISPENSED, SCOT_LANGUAGE_PRIMARY), ps.FmtDollar(amountDispensed));
    amountDispensed = 0;
    trace(L7, _T("+ra.OnNormalItem, Message=%s"), csTemp);
    ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ csTemp);
    trace(L7, _T("-ra.OnNormalItem"));

    if (!Is5XHardware())  
    {
        bool fCheckHealth = false;
        CString sModelName=dm.GetDeviceModelName(DMCLASS_CASHCHANGER, 0);
        CString csWork = DMGetDeviceErrorHTML(DMCLASS_CASHCHANGER, sModelName, CString(_T("")), NCR_DO_NOT_INSERT, -99, 0, NULL, &fCheckHealth, 0, co.csPrimaryLanguageCode );

        CREATE_AND_DISPENSE(SystemMessage1)(csWork,	// no char *message	
		     		PS_NOTEXT,	// canned message
			    	PS_NOTEXT,  // no prompt
				    0,          // no prompt value
				    -1,         // no device
				    false, true);     // no DM check health needed 

    }

    return STATE_NULL;    
}
//-SSCOADK-1110

