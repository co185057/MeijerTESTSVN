#ifdef _TESTSSF
#include <stdafx.h> 				// MS MFC always needed first
#include <afxtempl.h>				// MS MFC template header

#include "Common.h" 				// Common includes

#include "SMState.h"				// Base state
#include "SMSSFGiftCardBalanceReadCard.h"
#include "SMSSFCardDispenserReadFailed.h"
#include "SMSSFSelectGiftCardActivity.h"
#include "SMSSFGiftCardDisplayBalance.h"

#define COMP_ID ID_SM				// Base state component
#define T_ID	_T("SSFGiftCardBalanceReadCard")

IMPLEMENT_DYNCREATE(SMSSFGiftCardBalanceReadCard, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSSFGiftCardBalanceReadCard::SMSSFGiftCardBalanceReadCard()
{
	IMPLEMENT_TIMESTAMP

}

////////////////////////////////////
SMStateBase *SMSSFGiftCardBalanceReadCard::Initialize(void)
{
	trace(L6, _T("+SMSSFGiftCardBalanceReadCard::Initialize"));
	//+setup UI
    ps.SetCMFrame(_T("SystemMessage")); 
	m_pAppPSX->SetConfigProperty( _T("CMButton1Med"), _T("SystemMessage"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	ps.Button(_T("ButtonGoBack"), BTT_GOBACK, true);
	ps.Message12(_T("\n\nInsert your card into the card dispenser"), true);
	ps.ShowFrame();
	//-setup UI
	dm.CardDispenserEnableInsertion(true);   //enable card insertion 
	trace(L6, _T("-SMSSFGiftCardBalanceReadCard::Initialize"));
	return STATE_NULL;
}

///////////////////////////
SMStateBase *SMSSFGiftCardBalanceReadCard::TimedOut(void)
{
	RETURNSTATE(SSFCardDispenserReadFailed);	
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
SMStateBase* SMSSFGiftCardBalanceReadCard::DMDispenserPositionEvent( long lStat)
{
	trace(L6, _T("+SMSSFGiftCardBalanceReadCard::DMDispenserPositionEvent %d"), lStat );
	switch( lStat )
	{
	case DM_CD_CARD_IN_TRANSPORT:					  // the card is in TRANSPORT position
		if (OPOS_SUCCESS != dm.CardDispenserRead() )  // read the card
		{											  // if unsuccessfull,  	
			RETURNSTATE(SSFCardDispenserReadFailed);  
		}
		break;
	}
	trace(L6, _T("-SMSSFGiftCardBalanceReadCard::DMDispenserPositionEvent"));
	return STATE_NULL;
}

//************************************************************************ 
//
// Function:		DMCardReader
// Purpose:			Once a card is successfully read, the event from the 
//					MSR will be received through the DMCardReader override.  
// Parameters:      0
// Returns:	        StateBase*
// Calls:
// Side Effects:
// Notes:           
//
//************************************************************************
SMStateBase* SMSSFGiftCardBalanceReadCard::DMCardReader( void )
{
	trace(L6, _T("+SMSSFGiftCardBalanceReadCard::DMCardReader "));
	CString csCardData;
	if ( IsValidShoppingCard(csCardData) )
	{
		m_csGiftCardTrackData = csCardData; //save card data
		RETURNSTATE(SSFGiftCardDisplayBalance);   
	}else
	{
		RETURNSTATE(SSFCardDispenserReadFailed);   
	}
	trace(L6, _T("-SMSSFGiftCardBalanceReadCard::DMCardReader"));
	return STATE_NULL;
}
SMStateBase  *SMSSFGiftCardBalanceReadCard::PSButtonGoBack(void)
{
	RETURNSTATE(SSFSelectGiftCardActivity);	
}
void SMSSFGiftCardBalanceReadCard::UnInitialize(void)
{
	dm.CardDispenserEnableInsertion(false);		//disable card insertion
	m_pAppPSX->SetConfigProperty( _T("CMButton1Med"), _T("SystemMessage"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
}

#endif

