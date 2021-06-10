#ifdef _TESTSSF
#include <stdafx.h> 				// MS MFC always needed first
//				// MS MFC template header

#include "Common.h" 				// Common includes

#include "SMState.h"				// Base state
#include "SMSSFCardRechargeDispenserTakeCard.h"
#include "SMScanAndBag.h"



#define COMP_ID ID_SM				// Base state component
#define T_ID	_T("SSFCardRechargeDispenserTakeCard")

IMPLEMENT_DYNCREATE(SMSSFCardRechargeDispenserTakeCard, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSSFCardRechargeDispenserTakeCard::SMSSFCardRechargeDispenserTakeCard()
{
	IMPLEMENT_TIMESTAMP

}

////////////////////////////////////
SMStateBase *SMSSFCardRechargeDispenserTakeCard::Initialize(void)
{
	trace(L6, _T("+SMSSFCardRechargeDispenserTakeCard::Initialize"));
	//+setup UI
	ps.SetCMFrame(_T("TakeCard")); 
	m_pAppPSX->SetConfigProperty( _T("ButtonHelp"), _T("TakeCard"), UI::PROPERTYSTATE, (long) UI::Disabled);	
	ps.ShowFrame();
	m_pAppPSX->SetControlProperty(_T("Display"), UI::PROPERTYREDRAW, VARIANT_TRUE );
	//-setup UI
	dm.CardDispenserMoveToGate();  //move the card to the gate to allow customer to get the card.
	trace(L6, _T("-SMSSFCardRechargeDispenserTakeCard::Initialize"));
	return STATE_NULL;
}

///////////
SMStateBase *SMSSFCardRechargeDispenserTakeCard::TimedOut(void)
{
	//timeout is reach. Capture the card and moved to reject bin.
	//go back to scan and bag
	dm.CardDispenserMoveToReject();  //move the card to the reject bin
 	RETURNSTATE(ScanAndBag);
	return STATE_NULL;
}
//************************************************************************ 
//
// Function:		DMDispenserPositionEvent
// Purpose:			This is called when an event from the card dispenser is 
//					detected.  Overriden from SMStateBase::DMDispenserPositionEvent
// Parameters:      long lStat - Card position reported by the event
// Returns:	        State
// Calls:
// Side Effects:
// Notes:           
//
//************************************************************************
SMStateBase* SMSSFCardRechargeDispenserTakeCard::DMDispenserPositionEvent( long lStat)
{
	trace(L6, _T("+SMSSFCardRechargeDispenserTakeCard::DMDispenserPositionEvent %d "), lStat);
	SMStateBase* retState = STATE_NULL;
	switch( lStat )
	{
	case DM_CD_CARD_REMOVED:	// card remove by the customer from the gate
		{
			csItemSent = _T("2222222222");
			retState = TBItemSale(false,0,false); //purchase the new "Shopping Card Recharge" item
			CString csAccountNo, csPrintLine; 
			//extract account number
			csAccountNo = GetAccountNumber();
			//Format csPrintLine: AcctNo: :mmddyyhhmmss0000  Amount: $25 
			csPrintLine.Format(_T("%s%s%s$%d"),	_T("AcctNo: "), csAccountNo , _T(" Amount: "), m_SSFMoneyCenterGiftCardAmount)  ;
			//send to the receipt the Account Number and amount
			TBSendTBCommand(TBCMD_CARDDISPENSER, (char *) (LPCTSTR)csPrintLine );  
		}
	}
	trace(L6, _T("-SMSSFCardRechargeDispenserTakeCard::DMDispenserPositionEvent"));
	return retState;
}
void SMSSFCardRechargeDispenserTakeCard::UnInitialize()
{
	m_pAppPSX->SetConfigProperty( _T("ButtonHelp"), _T("TakeCard"), UI::PROPERTYSTATE, (long) UI::Normal);	
}
#endif

