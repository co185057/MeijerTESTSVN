//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMDepositGiftCardBase.cpp
//
// TITLE: Class implementation for DepositGiftCard state
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMDepositGiftCard.h"      

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("DepositGiftCardBase")

IMPLEMENT_DYNCREATE(SMDepositGiftCardBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMDepositGiftCardBase::SMDepositGiftCardBase()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMDepositGiftCardBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("DepositGiftCard") );	// RFC 330050
	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_DEPOSITGIFTCARD, SCOT_LANGUAGE_PRIMARY));

	nTimeOut = co.getTimeOutDepositGiftCardScreenTimer();

    ps.SetCMFrame(_T("DepositGiftCard"));
    ps.CMButtonFlash(_T("CMButton1Med"),true);  

	ps.ShowAmountDueAmountPaid(lBalanceDue - nDMCashInserted, lTotalPaid + nDMCashInserted);
	ps.ShowCMTBText(csTBMessage2Scot);

    DMSayPhrase(DEPOSITYOURCARD);	   //Only do this once at entry

	ps.Message12(MSG_DEPOSTITGIFTCARD,true);
	
	DMCouponLight(DM_MISC_STATE_ONE_HZ);
	ps.ShowCMFrame();
	return STATE_NULL;                //Stay in SMDepositGiftCard state
}

void SMDepositGiftCardBase::UnInitialize(void)
{
  DMCouponLight(DM_MISC_STATE_OFF);
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("DepositGiftCard") );	// RFC 330050
}

///////////////////////////////////////
SMStateBase *SMDepositGiftCardBase::TimedOut(void)
{
    return PSButton1();
}

SMStateBase  *SMDepositGiftCardBase::PSButton1(void) //Ok button
{
	return ReturnDelayedStateForGCTender();
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMDepositGiftCardBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering && !(lBalanceDue < 0.01))
		return FinalizationWtDecreaseNotAllowed(BES_DEPOSITGIFTCARD);
	else
		return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMDepositGiftCardBase::OnWtIncreaseNotAllowed()	 
{
  	return FinalizationWtIncrease(BES_DEPOSITGIFTCARD);  
}
