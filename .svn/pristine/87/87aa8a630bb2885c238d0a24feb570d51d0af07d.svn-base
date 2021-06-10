//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoucherPaymentBase.cpp
//
// TITLE: Class implementation for Voucher Payment State
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"
#include "SMVoucherPayment.h"
#include "SMFinish.h"	

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("VoucherPaymentBase")

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMVoucherPaymentBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(VoucherPayment)


//////////////////////////////////////////
SMVoucherPaymentBase::SMVoucherPaymentBase()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMVoucherPaymentBase::Initialize(void)
{  
    fBeyondFinishAndPayState = true;

	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("VoucherPayment") );	// RFC 330050
    SMStateBase *sReturnState = CkStateStackAndReturn();
    if (sReturnState != STATE_NULL)
       return sReturnState;

    //Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(MSG_VOUCHER_PAYMENT, SCOT_LANGUAGE_PRIMARY));
	//TAR412506
	//When transferring from another screen (the help screen), and goes back to "Voucher Payment" screen the audio should be replayed.
	if (!GetISIPTimeOut() &&  ps.GetCurrentContext() != _T("VoucherPayment"))
	{
		DMSayPhrase(SCANVOUCHERS);	//TAR411040 SSCOADK-3432 Change back to scan voucher
	}
    setAnchorState(BES_VOUCHERPAYMENT);  //TAR 383736- 4.4G320- No audio for Voucher Payment
	ps.SetCMFrame(_T("VoucherPayment"));

    ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	bEASReEnableScanner = true;
    SAWLDBScannerEnable();			// enable the scanner

    ps.ShowCMFrame();
    return STATE_NULL;
}

void SMVoucherPaymentBase::UnInitialize(void)
{
    SAWLDBScannerDisable();			   // disable the scanner
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("VoucherPayment") );	// RFC 330050
}

SMStateBase  *SMVoucherPaymentBase::PSButton1(void) // ok done coupon/voucher tender
{
    SetTenderStarted(false); //SR797

	trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_ENDVOUCHERPAYMENT_BALANCEDUE, SCOT_LANGUAGE_PRIMARY));
	ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY +
                    ps.GetPSText(RA_ENDVOUCHERPAYMENT_BALANCEDUE, SCOT_LANGUAGE_PRIMARY));
	trace(L7, _T("-ra.OnNormalItem"));
    return setAndCreateAnchorState(BES_FINISH);
}

SMStateBase  *SMVoucherPaymentBase::PSButtonGoBack(void)
{
    return PSButton1();
}

SMStateBase  *SMVoucherPaymentBase::DMScanner(void)// scanned item
{
    csItemSent = csDMLastBarCode;
	csDMEncryptedPin.Empty();
	return TBEnterTender(TB_TENDER_VOUCHERPAYMENT, 0, 0 );	// may be zero or +ve
}

//////////////////////////////////
SMStateBase  *SMVoucherPaymentBase::SAEventOccurred(const long)
{
	return STATE_NULL;
}

////////////////////////////////////////
SMStateBase  *SMVoucherPaymentBase::PSCancelKey(void)/// override where needed
{
  DMSayPhrase(KEYPADVALID);
  return PSButton1();               // usually same as button 1
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMVoucherPaymentBase::OnWtIncreaseNotAllowed()	 
{
	return FinalizationWtIncrease(BES_VOUCHERPAYMENT);	
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMVoucherPaymentBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering)
		return FinalizationWtDecreaseNotAllowed(BES_VOUCHERPAYMENT);
	else
		return STATE_NULL;
}

SMStateBase  *SMVoucherPaymentBase::DMAcceptor(void)
{
	trace(L6, _T("SMVoucherPaymentBase::DMAcceptor()"));
	ps.HideButtons();
    
	return PSButton1();
}