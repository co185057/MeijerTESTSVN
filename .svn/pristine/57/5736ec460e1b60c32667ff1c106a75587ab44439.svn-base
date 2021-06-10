#ifdef _TESTSSF
#include <stdafx.h> 				// MS MFC always needed first
//				// MS MFC template header

#include "Common.h" 				// Common includes

#include "SMState.h"				// Base state
#include "SMSSFIssueNewGiftCard.h"
#include "SMSSFCardDispenserHealthCheckFailed.h"
#include "SMSSFCardDispenserTakeCard.h"
#include "SMSSFSelectGiftCardActivity.h"

#define COMP_ID ID_SM				// Base state component
#define T_ID	_T("SSFIssueNewGiftCard")

IMPLEMENT_DYNCREATE(SMSSFIssueNewGiftCard, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

int SMState::m_SSFMoneyCenterGiftCardAmount;

//////////////////////////////////////////
SMSSFIssueNewGiftCard::SMSSFIssueNewGiftCard()
{
	IMPLEMENT_TIMESTAMP

}

////////////////////////////////////
SMStateBase *SMSSFIssueNewGiftCard::Initialize(void)
{
	trace(L6, _T("+SMSSFIssueNewGiftCard::Initialize"));
	
	nTimeOut = 20; //set 20sec timeout
	m_pAppPSX->SetConfigProperty( _T("CMButton1Med"), _T("SystemMessage"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	m_pAppPSX->SetConfigProperty( _T("ButtonGoBack"), _T("SystemMessage"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	//check if carddispenser is ok
	if ( IsCardDispenserOK() )
	{
		ps.SetCMFrame(_T("SystemMessage")); 
		ps.Message12Modal(_T("\n\nShopping Card Dispenser Processing"));
		ps.ShowFrame();
	}else
	{
		RETURNSTATE(SSFCardDispenserHealthCheckFailed);		
	}
	dm.CardDispenserDispense();	//move card from the bin to the transport 
	trace(L6, _T("-SMSSFIssueNewGiftCard::Initialize"));
	return STATE_NULL;
}

SMStateBase *SMSSFIssueNewGiftCard::TimedOut(void)
{
	RETURNSTATE(SSFSelectGiftCardActivity);
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
SMStateBase* SMSSFIssueNewGiftCard::DMDispenserPositionEvent( long lStat)
{
	trace(L6, _T("+SMSSFIssueNewGiftCard::DMDispenserPositionEvent %d"), lStat);
	switch( lStat )
	{
	case DM_CD_HOPPER_EMPTY:	//bin is empty
		{
			RETURNSTATE(SSFCardDispenserHealthCheckFailed);
			break;
		}
	case DM_CD_HOPPER_DOOR_OPEN:	//door hopper is open
		{
			RETURNSTATE(SSFCardDispenserHealthCheckFailed);
			break;
		}
	case DM_CD_CARD_IN_TRANSPORT:  //card is in the transport ready for writing
		{
			if (WriteGiftCard(true))
			{
				RETURNSTATE(SSFCardDispenserTakeCard);
			}else
			{
				dm.CardDispenserMoveToReject();	
			}
			break;
		}
	case DM_CD_CARD_REMOVED:	//card is move to reject bin from transport. 
		{						
			if (IsCardDispenserOK())
			{
				dm.CardDispenserDispense();	//move card from the bin to the transport

			}else
			{
				RETURNSTATE(SSFCardDispenserHealthCheckFailed);
			}
			break;
		}
	}
	trace(L6, _T("-SMSSFIssueNewGiftCard::DMDispenserPositionEvent"));
	return STATE_NULL;
}
void SMSSFIssueNewGiftCard::UnInitialize(void)
{
	m_pAppPSX->SetConfigProperty( _T("CMButton1Med"), _T("SystemMessage"), UI::PROPERTYVISIBLE, VARIANT_TRUE);	
	m_pAppPSX->SetConfigProperty( _T("ButtonGoBack"), _T("SystemMessage"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
}



#endif

