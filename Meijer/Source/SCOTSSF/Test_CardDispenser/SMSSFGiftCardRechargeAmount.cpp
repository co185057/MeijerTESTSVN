#ifdef _TESTSSF
#include <stdafx.h> 				// MS MFC always needed first
//				// MS MFC template header

#include "Common.h" 				// Common includes

#include "SMState.h"				// Base state
#include "SMSSFGiftCardRechargeAmount.h"
#include "SMSSFSelectGiftCardActivity.h"
#include "SMSSFAcceptGiftCard.h"

#define COMP_ID ID_SM				// Base state component
#define T_ID	_T("SSFGiftCardRechargeAmount")

IMPLEMENT_DYNCREATE(SMSSFGiftCardRechargeAmount, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP


//////////////////////////////////////////
SMSSFGiftCardRechargeAmount::SMSSFGiftCardRechargeAmount()
{
	IMPLEMENT_TIMESTAMP

}

////////////////////////////////////
SMStateBase *SMSSFGiftCardRechargeAmount::Initialize(void)
{
	trace(L6, _T("+SMSSFGiftCardRechargeAmount::Initialize"));
	nTimeOut = 20;		// set 20sec timeout
	//+setup UI
	ps.SetCMFrame(_T("SelectPayment"));
	//show Leadthrutext2.  Leadthrutext2 is added in FastLane3.1.000
    m_pAppPSX->SetConfigProperty( _T("LeadthruText2"), _T("SelectPayment"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
	ps.SetLeadthruText(_T("Select Shopping Card Amount"), _T("Select the amount to add to the value of your shopping card."));
	ps.Button(_T("ButtonGoBack"), BTT_MEDIAEXIT, true);
	//display shopping card amount lists
	ps.ShowCardAmountList();
	ps.ShowFrame();
	//-setup UI
	trace(L6, _T("-SMSSFGiftCardRechargeAmount::Initialize"));
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    PSButtonGoBack()
// Description: Go to scan and bag
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSSFGiftCardRechargeAmount::PSButtonGoBack(void) 
{
  RETURNSTATE(SSFSelectGiftCardActivity);	
}

///////////////////////////////////////////////////////////////////////////////
// Function:    TimedOut
// Description: If timeout is reached, then go to scan and bag
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSSFGiftCardRechargeAmount::TimedOut(void)
{
  RETURNSTATE(SSFSelectGiftCardActivity);	
}


///////////////////////////////////////////////////////////////////////////////
// Function:    PSListLookup
// Description: When TenderImage control is clicked, this method is called by ScotApp
// Parameters : [in] csButtonIndex - button code. 
// Return Value: StateBase*
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSSFGiftCardRechargeAmount::PSListLookup(CString csButtonIndex)
{
	trace(L6, _T("+SMSSFGiftCardRechargeAmount::PSListLookup %s"), csButtonIndex);
	//save the selected code to SMState::m_SSFMoneyCenterGiftCardAmount
	m_SSFMoneyCenterGiftCardAmount = _ttoi(csButtonIndex);
	RETURNSTATE(SSFAcceptGiftCard);	
}

void SMSSFGiftCardRechargeAmount::UnInitialize()
{
  //hide LeadthruText2 since this control is not part of SelectPayment base.
  m_pAppPSX->SetConfigProperty( _T("LeadthruText2"), _T("Finish"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
}
#endif

