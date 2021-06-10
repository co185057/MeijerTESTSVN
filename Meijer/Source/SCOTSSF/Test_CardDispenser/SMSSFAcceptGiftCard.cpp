#ifdef _TESTSSF
#include <stdafx.h> 				// MS MFC always needed first
//				// MS MFC template header

#include "Common.h" 				// Common includes

#include "SMState.h"				// Base state
#include "SMSSFAcceptGiftCard.h"
#include "SMSSFCardDispenserReadFailed.h"
#include "SMSSFGiftCardRechargeAmount.h"
#include "SMSSFIssueRecharge.h"

#define COMP_ID ID_SM				// Base state component
#define T_ID	_T("SSFAcceptGiftCard")

IMPLEMENT_DYNCREATE(SMSSFAcceptGiftCard, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

CString SMState::m_csGiftCardTrackData;
//////////////////////////////////////////
SMSSFAcceptGiftCard::SMSSFAcceptGiftCard()
{
	IMPLEMENT_TIMESTAMP

}

////////////////////////////////////
SMStateBase *SMSSFAcceptGiftCard::Initialize(void)
{
	trace(L6, _T("+SMSSFAcceptGiftCard::Initialize"));
	nTimeOut = 10;		//set 10sec timeout for this state
	//+setup UI
    ps.SetCMFrame(_T("SystemMessage")); 
	m_pAppPSX->SetConfigProperty( _T("CMButton1Med"), _T("SystemMessage"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	ps.Message12(_T("\n\nInsert your card into the card dispenser"), true);
	ps.Button(_T("ButtonGoBack"), BTT_GOBACK, true);
	ps.ShowFrame();
	//-setup UI
	dm.CardDispenserEnableInsertion(true);   //enable card insertion 
	trace(L6, _T("-SMSSFAcceptGiftCard::Initialize"));
	return STATE_NULL;
}

///////////////////////////
SMStateBase *SMSSFAcceptGiftCard::TimedOut(void)
{
	RETURNSTATE(SSFCardDispenserReadFailed);	// return to scan and bag once timeout is reached
}

//************************************************************************ 
//
// Function:		DMDispenserPositionEvent
// Purpose:			This is called when an event from the card dispenser is 
//					detected.  Overriden from SMStateBase::DMDispenserPositionEvent
// Parameters:      long lStat - Card position reported by the event
// Returns:	        SMStateBase*
// Calls:
// Side Effects:
// Notes:           
//
//************************************************************************
SMStateBase* SMSSFAcceptGiftCard::DMDispenserPositionEvent( long lStat)
{
	trace(L6, _T("+SMSSFAcceptGiftCard::DMDispenserPositionEvent %d"), lStat );
	switch( lStat )
	{
	case DM_CD_CARD_IN_TRANSPORT:					  // the card is in TRANSPORT position
		if (OPOS_SUCCESS != dm.CardDispenserRead() )  // read the card
		{											  // if unsuccessfull,  	
			RETURNSTATE(SSFCardDispenserReadFailed);  
		}
		break;
	}
	trace(L6, _T("-SMSSFAcceptGiftCard::DMDispenserPositionEvent"));
	return STATE_NULL;
}

//************************************************************************ 
//
// Function:		DMCardReader
// Purpose:			Once a card is successfully read, the event from the 
//					MSR will be received through the DMCardReader override.  
// Parameters:      0
// Returns:	        SMStateBase*
// Calls:
// Side Effects:
// Notes:           
//
//************************************************************************
SMStateBase* SMSSFAcceptGiftCard::DMCardReader( void )
{
	trace(L6, _T("+SMSSFAcceptGiftCard::DMCardReader "));
	CString csCardData ;
	if ( IsValidShoppingCard(csCardData) )
	{
		m_csGiftCardTrackData = csCardData; //save card data.
		RETURNSTATE(SSFIssueRecharge);   
	}else
	{
		RETURNSTATE(SSFCardDispenserReadFailed);   
	}
	trace(L6, _T("-SMSSFAcceptGiftCard::DMCardReader"));
	return STATE_NULL;
}
SMStateBase  *SMSSFAcceptGiftCard::PSButtonGoBack(void)
{
	RETURNSTATE(SSFGiftCardRechargeAmount);	
}
void SMSSFAcceptGiftCard::UnInitialize(void)
{
	dm.CardDispenserEnableInsertion(false);		//disable card insertion
	m_pAppPSX->SetConfigProperty( _T("CMButton1Med"), _T("SystemMessage"), UI::PROPERTYVISIBLE, VARIANT_TRUE);

}

#endif

