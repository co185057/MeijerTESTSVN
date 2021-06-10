#ifdef _TESTSSF
#include <stdafx.h> 				// MS MFC always needed first
#include <afxtempl.h>				// MS MFC template header

#include "Common.h" 				// Common includes

#include "SMState.h"				// Base state
#include "SMSSFSelectGiftCardActivity.h"
#include "SMScanAndBag.h"
#include "SMSSFNewGiftCardAmount.h"
#include "SMSSFGiftCardRechargeAmount.h"
#include "SMSSFCardDispenserHealthCheckFailed.h"
#include "SMSSFGiftCardBalanceReadCard.h"

#define COMP_ID ID_SM				// Base state component
#define T_ID	_T("SSFSelectGiftCardActivity")

IMPLEMENT_DYNCREATE(SMSSFSelectGiftCardActivity, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

const int PURCHASE_NEW_SHOPPING_CARD = 1;
const int ADD_MONEY_TO_SHOPPING_CARD = 2;
const int SHOPPING_CARD_BALANCE = 3;

//////////////////////////////////////////
SMSSFSelectGiftCardActivity::SMSSFSelectGiftCardActivity()
{
	IMPLEMENT_TIMESTAMP

}

////////////////////////////////////
SMStateBase *SMSSFSelectGiftCardActivity::Initialize(void)
{
	trace(L6, _T("+SMSSFSelectGiftCardActivity::Initialize"));
	nTimeOut = 20;		// set 20sec timeout
	ps.SetCMFrame(_T("Finish")); 
	ps.SetLeadthruText(_T("Select Shopping Card Activity"));

	ps.ShowGiftCardActivityList();

	ps.ShowFrame();
	//-setup UI

	trace(L6, _T("-SMSSFSelectGiftCardActivity::Initialize"));
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    PSButtonGoBack()
// Description: Go to scan and bag
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSSFSelectGiftCardActivity::PSButtonGoBack(void) 
{
  RETURNSTATE(ScanAndBag);	
}

///////////////////////////////////////////////////////////////////////////////
// Function:    TimedOut
// Description: If timeout is reached, then go to scan and bag
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSSFSelectGiftCardActivity::TimedOut(void)
{
  RETURNSTATE(ScanAndBag);	
}


///////////////////////////////////////////////////////////////////////////////
// Function:    PSListLookup
// Description: When TenderImage control is clicked, this method is called by ScotApp
// Parameters : [in] csButtonIndex - button code. (1 - 3)
// Return Value: StateBase*
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSSFSelectGiftCardActivity::PSListLookup(CString csButtonIndex)
{
	trace(L6, _T("+SMSSFSelectGiftCardActivity::PSListLookup %s"), csButtonIndex);
	int nButtonIndex = _ttoi(csButtonIndex);
	switch(nButtonIndex)
	{
	case PURCHASE_NEW_SHOPPING_CARD:   //purchase new shopping card
		{
			if ( IsCardDispenserOK())  //check the healthe of card dispenser
			{
				RETURNSTATE(SSFNewGiftCardAmount);
			}else
			{
				RETURNSTATE(SSFCardDispenserHealthCheckFailed);
			}
			break;
		}
	case ADD_MONEY_TO_SHOPPING_CARD:   //add money to shopping card
		{
			RETURNSTATE(SSFGiftCardRechargeAmount);
			break;
		}
	case SHOPPING_CARD_BALANCE:			//shopping card balance
		{
			RETURNSTATE(SSFGiftCardBalanceReadCard);
			break;
		}

	}
	trace(L6, _T("-SMSSFSelectGiftCardActivity::PSListLookup %s"), csButtonIndex);
	return STATE_NULL;
}


#endif

