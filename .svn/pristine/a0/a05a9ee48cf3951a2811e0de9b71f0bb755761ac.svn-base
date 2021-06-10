//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	 SMSelectCardTypeBase.CPP
//
// TITLE:	Class implementation for Select Card Type State
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>				 // MS MFC always needed first
#include <afxtempl.h>			 // MS MFC template header

#include "Common.h"				 // Common includes

#include "SMState.h"			 // Base state
#include "SMSelectCardType.h"
#include "SMCardMisRead.h"		 // Cannot read card state
#include "SMEnterPin.h"			 // Enter PIN state
#include "SMTakeCard.h"			 // Take card state
#include "SMSelectPayment.h"        // Select payment state
#include "SMSelectDebitType.h"	 // Select debit account type checking or savings
#include "SMWaitForApproval.h"	 // Wait for approval state
#include "SMElectronicBenefitsTransfer.h"	
#include "SMRequestSig.h"
#include "SMSignCharge.h"
#include "SMSwipeCard.h"


#define COMP_ID ID_SM			 // Base state component
#define T_ID	_T("SelectCardType")

IMPLEMENT_DYNCREATE(SMSelectCardTypeBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SelectCardType)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSelectCardTypeBase::SMSelectCardTypeBase()
{
  IMPLEMENT_TIMESTAMP
  saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
}

//////////////////////////////////////////
SMStateBase *SMSelectCardTypeBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SelectCardType") );	// RFC 330050
  DMSayPhrase(SELECTCARD); // Tar 142642
  DMEncryptorEnable();
 // fPIPShowCardProcessingScreen = false;

  // Validate that a master key has been stored inside the encryptor.
  bool bMasterKeyPresent = true;
  if (0 != DMEncryptorKeyTest())
  {
      bMasterKeyPresent = false;
  }
	
  //TAR 219971 & 220071 BZ  also tar 227543
    // if there's no card in, enable MSR and
  if (!cDMLastTrack && !co.fOperationsPartialIntegratedPINPad) // No card inserted, should not be select card type screen //PIP
  {
    //TAR 219971 & 220074 BZ: Enable the MSR only if it is not disconnected, otherwise will 
	//DMMSREnable() will take some time to finish without returning an error
	if (g_lDisplayFormRC == 0)
		DMMSREnable();	 // enable the MSR, turn light on
    return setAndCreateAnchorState(BES_FINISH);   
  }

  ResetAllTenderValues();
  
 
  PSTEXT nBtt3 = ((co.fTenderDebitCashBack && ( bMasterKeyPresent || co.fOperationsPartialIntegratedPINPad)) ? BTT_DEBITWITHCASHBACK : PS_NOTEXT);


  if((co.fTenderEBTFSAllowed && (lFoodStampsDue > 0))|| co. fTenderEBTCBAllowed)
	m_bAllowEBTScreen = true;
  else
	m_bAllowEBTScreen = false;
  PSTEXT nBtt4 = (m_bAllowEBTScreen ? BTT_EBT : PS_NOTEXT);
#ifndef NewUI
  ps.SetCMFrame(DEFAULT_FRAME_WITHOUTECHO);
  ps.SetCMBackground(BMP_CREDITDEBIT);
  ps.SetCMMascotImage(BMP_MASCOT_CREDITDEBIT);
  ps.SetCMLeadthruText(LTT_SELECTCARD);

  if (co.fTenderCreditAllowed)
    ps.CMButton(IDC_BUTTON1, BTT_CREDITCARD, true);  
  else
    ps.CMButton(IDC_BUTTON1, BTT_CREDITCARD, false);  

  if (co.fTenderDebitAllowed && ( bMasterKeyPresent || co.fOperationsPartialIntegratedPINPad))
    ps.CMButton(IDC_BUTTON2, BTT_DEBITCARD, true);  
  else
    ps.CMButton(IDC_BUTTON2, BTT_DEBITCARD, false);  
  
  ps.SetCMButton3Text(nBtt3);
  ps.SetCMButton4Text(nBtt4);
  ps.CMButton(IDC_BUTTONGOBACK, BTT_CANCELPAY, true);  
  ps.SetCMGobackButtonVisible(true);

  ps.ShowCMFrame();
  ps.Message12(LTT_SELPAYMENT, false);
  
  CString csWork;
  csWork.Format(
		ps.GetPSText(MSG_CHARGEAMT),
		ps.FmtDollar(lBalanceDue));
  ps.InfoMessage(csWork); 
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);

#endif

  // Display the Auxilary select card type screen
  SCOT_AUX_SCREEN eScreen;

  if (m_bAllowEBTScreen)
    if (co.fTenderCashAllowed)
      eScreen = AUX_SCREEN_CARD_TYPE;
    else
      eScreen = AUX_SCREEN_CARD_TYPE_NOCASH;
  else
    if (co.fTenderCashAllowed)
      eScreen = AUX_SCREEN_CARD_TYPE_NO_EBT;
    else
      eScreen = AUX_SCREEN_CARD_TYPE_NO_CASH_NO_EBT;

  DisplayAuxScreen(eScreen, true);

  
  return STATE_NULL;
}

/////////////////////////////////////////
SMStateBase *SMSelectCardTypeBase::PSButton1(void)// Credit card
{
  trace(L7, _T("+ra.OnCanSuspendNow, CanSuspend=false"));
  ra.OnCanSuspendNow(false) ;  // un-reversable tender mode. disable suspend
  trace(L7, _T("-ra.OnCanSuspendNow"));
  if( co.fOperationsPartialIntegratedPINPad )   //PIP
  {
 	 if(co.fOperationsCreditMSR) // Point user to Integrated MSR
    {
	   CREATE_AND_DISPENSE(SwipeCard)(TB_TENDER_CREDIT,lBalanceDue);
    }
	 else
	 	return TBEnterTender(TB_TENDER_PIP_CREDIT, lBalanceDue);
  }

  if (co.fTenderCreditNeedsPIN)
  {
	 CREATE_AND_DISPENSE(EnterPin)(TB_TENDER_CREDIT,co.fTenderCreditCashBack);
  }
  return TBEnterTender(TB_TENDER_CREDIT,lBalanceDue);
}

/////////////////////////////////////////////
//Debit without cash back --Feng2
SMStateBase *SMSelectCardTypeBase::PSButton2(void)
{
  trace(L7, _T("+ra.OnCanSuspendNow, CanSuspend=false"));
  ra.OnCanSuspendNow(false) ;  // un-reversable tender mode. disable suspend
  trace(L7, _T("-ra.OnCanSuspendNow"));

  if( co.fOperationsPartialIntegratedPINPad )   //PIP
  {
    if(co.fOperationsDebitMSR) // Point user to Integrated MSR
    {
      CREATE_AND_DISPENSE(SwipeCard)(TB_TENDER_DEBIT,lBalanceDue);
	 }
	 else
	   return TBEnterTender(TB_TENDER_PIP_DEBIT, lBalanceDue);
  }

  if (TBPromptForDebitAccountType())
  {
	 CREATE_AND_DISPENSE(SelectDebitType)(false);
  }

  if (co.fTenderDebitNeedsPIN)
  {
    CREATE_AND_DISPENSE(EnterPin)(TB_TENDER_DEBIT,false);
  }

  return TBEnterTender(TB_TENDER_DEBIT,lBalanceDue);
}

/////////////////////////////////////////
SMStateBase *SMSelectCardTypeBase::PSButton3(void)// ATM/Debit Checking //Feng2
{
  trace(L7, _T("+ra.OnCanSuspendNow, CanSuspend=false"));
  ra.OnCanSuspendNow(false) ;  // un-reversable tender mode. disable suspend
  trace(L7, _T("-ra.OnCanSuspendNow"));

  if( co.fOperationsPartialIntegratedPINPad )   //PIP
  {
    if( co.fOperationsDebitMSR || co.fOperationsPIPSCOTPromptCashBack )
	 {
      CREATE_AND_DISPENSE(SelectPayment)(TB_TENDER_DEBIT); 
	 }
    else 
	   return TBEnterTender(TB_TENDER_PIP_DEBIT_CASHBACK, lBalanceDue);
  }

  if (TBPromptForDebitAccountType())
  {
    CREATE_AND_DISPENSE(SelectDebitType)(true);
  }

  if(!co.fTenderDebitNeedsPIN)
  {
    CREATE_AND_DISPENSE(SelectPayment)(TB_TENDER_DEBIT); //With cash back
  }

  CREATE_AND_DISPENSE(EnterPin)(TB_TENDER_DEBIT,true);
}

/////////////////////////////////////////
SMStateBase *SMSelectCardTypeBase::PSButton4(void)// EBT or Cancel
{
  trace(L7, _T("+ra.OnCanSuspendNow, CanSuspend=false"));
	ra.OnCanSuspendNow(false) ;  // un-reversable tender mode. disable suspend
  trace(L7, _T("-ra.OnCanSuspendNow"));
  if(m_bAllowEBTScreen)
  {
	CREATE_AND_DISPENSE(ElectronicBenefitsTransfer)(false);
  }
  else
  {
	CREATE_AND_DISPENSE(TakeCard)(false);
  }
}

/////////////////////////////////////////
SMStateBase  *SMSelectCardTypeBase::PSButtonGoBack(void)// cancel
{
  trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_ENDCARDTENDER_BALANCEDUE, SCOT_LANGUAGE_PRIMARY));
  ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+
                  ps.GetPSText(RA_ENDCARDTENDER_BALANCEDUE, SCOT_LANGUAGE_PRIMARY));
  trace(L7, _T("-ra.OnNormalItem"));
  setAnchorState(BES_FINISH);
  CREATE_AND_DISPENSE(TakeCard)(false);
}

//////////////////////////////////
SMStateBase *SMSelectCardTypeBase::SAEventOccurred(const long)
{
  /******cm150000: Modified in Merge 2******/
  //Security Log button is gone, this should be gone also  if (isSecurityApprovalRequired())
  //{
	//CREATE_AND_DISPENSE(WaitForApproval)(false,false);
  //}
  /******cm150000: End Merge 2******/
  return STATE_NULL;
}

/////////////////////////////////////////
void SMSelectCardTypeBase::UnInitialize(void)
{
  //DMEncryptorDisable();
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SelectCardType") );	// RFC 330050
}

/////////////////////////////////////////
SMStateBase  *SMSelectCardTypeBase::PSCancelKey(void)/// override where needed
{
  DMSayPhrase(KEYPADVALID);
  setAnchorState(BES_FINISH);
  CREATE_AND_DISPENSE(TakeCard)(false);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSelectCardTypeBase::OnWtDecrease()
{
	FinalizationWtDecrease();
	return STATE_NULL;
}
