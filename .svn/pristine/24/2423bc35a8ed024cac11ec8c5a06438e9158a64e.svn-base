//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	 SMSwipeCardBase.CPP
//
// TITLE:	Class implementation for Select Card Type State
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>				 // MS MFC always needed first
//			 // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"				 // Common includes

#include "SMState.h"			 // Base state
#include "SMSwipeCard.h"
#include "SMTakeCard.h"			 // Take card state
#include "SMSelectPayment.h"        // Select payment state
#include "SMSelectDebitType.h"	 // Select debit account type checking or savings
#include "SMWaitForApproval.h"	 // Wait for approval state
#include "SMElectronicBenefitsTransfer.h"	
#include "SMRequestSig.h"
#include "SMSignCharge.h"
#include "SMSelectCardType.h"
#include "SMEnterPin.h"
#include "SMConfirmDebitAmount.h"
#include "SMConfirmEBTAmount.h"
#else
#include "SMSwipeCardBase.h"

#endif // _CPPUNIT
#define COMP_ID ID_SM			 // Base state component
#define T_ID	_T("SwipeCard")

IMPLEMENT_DYNCREATE(SMSwipeCardBase, CObject)// MFC Runtime class/object information

//PIMPLEMENT_STATE(SwipeCard)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSwipeCardBase::SMSwipeCardBase(const TBTENDERTYPE nTyp,const long lAmt)
: nTndrType(nTyp),
  lTenderAmt(lAmt)
{
  IMPLEMENT_TIMESTAMP
#ifndef _CPPUNIT
  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
#endif // _CPPUNIT
}

//////////////////////////////////////////
SMSwipeCardBase::SMSwipeCardBase()
{
  IMPLEMENT_TIMESTAMP
#ifndef _CPPUNIT
  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
#endif // _CPPUNIT
}

////////////////////////////////////////
SMStateBase  *SMSwipeCardBase::Deliver(const TBTENDERTYPE nTyp,const long lAmt)
{
  nTndrType = nTyp;
  lTenderAmt = lAmt;

  return this;
}

#ifndef _CPPUNIT
bool SMSwipeCardBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, both dwords
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMSwipeCardBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver((TBTENDERTYPE)DLParamDWORD(dlParameterList),    // nTyp
                DLParamDWORD(dlParameterList));                 // lAmt
        return true;
    }
    else
    {
        trace(L2, _T("+SMSwipeCardBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}


//////////////////////////////////////////
SMStateBase *SMSwipeCardBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SwipeCard") );	// RFC 330050
  SMStateBase *sReturnState = CkStateStackAndReturn();
  if (sReturnState != STATE_NULL)
     return sReturnState;
  if (cDMLastTrack)   //Card already swiped
  {
    return DMCardReader();   // Don't need stay on this screen
  }


  DMEncryptorEnable();

  // Validate that a master key has been stored inside the encryptor.
  bool bMasterKeyPresent = true;
  if (0 != DMEncryptorKeyTest())
  {
    bMasterKeyPresent = false;
  }
	
  // if there's no card in, enable MSR and
  if (!cDMLastTrack) // No card inserted, should not be select card type screen //PIP
  {
    DMMSREnable();   // enable the MSR, turn light on
  }
// NewUI
  ps.SetCMFrame(_T("SwipeCard"));
// NewUI

  if(!co.fOperationsPartialIntegratedPINPad)
  { //Not-PIP
	 if (co.fTenderIsMotorizedMSR)
	 {
		ps.SetCMBackground(BMP_CARDINSERT);
		DMSaySecurity(SEC_NONE, ps.GetSoundName(INSERTCARD));	//TAR 202910
		ps.Message12(TXT_FINISHANDPAY_CARD_ONLY, true);
	 } 
	 else
	 {
	  
		if (gpSCOTEncryptor->GetProfileName().CompareNoCase(_T("NCR5992Pinpad.1")) == 0)	//TAR193732+ CHU: display differenct backgroud based on the device
			ps.SetCMBackground(BMP_CARDSWIPE_SIGDEV);  
		else 
			ps.SetCMBackground(BMP_CARDSWIPE);//Integrated MSR (background bitmap from Registry)

		DMSaySecurity(SEC_NONE, ps.GetSoundName(SLIDEYOURCARD));	//TAR 202910
		ps.Message12(MSG_SLIDECARDCHECKBASKET, true);
		DisplayAuxScreen(AUX_SCREEN_FINISH_PAY_CARD, true);	//TAR193732-
	 }
  }
  else if (co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bIntegratedMSR)
  {// PIP with Integrated MSR
	 if (co.fOperationsUse7401MSR)  //7401
	 {
		 DMSaySecurity(SEC_NONE, ps.GetSoundName(SLIDECARDONSIDEOFMONITOR));  //TAR 202910
		 ps.SetCMBackground(BMP_SLIDECARD7401);   //slide your card on the side of Monitor
		 ps.Message12(LTT_SLIDEONSIDEOFMONITOR, true);
	 }
	 else                           //Other Integrated MSRs (background bitmap from Registry)
	 {
		 //TAR193732+ CHU: display differenct backgroud based on the device
		 if (gpSCOTEncryptor->GetProfileName().CompareNoCase(_T("NCR5992Pinpad.1")) == 0)
			ps.SetCMBackground(BMP_CARDSWIPE_SIGDEV);  
         else 
			ps.SetCMBackground(BMP_CARDSWIPE);//Integrated MSR (background bitmap from Registry)
		 //TAR193732-
		 DMSaySecurity(SEC_NONE, ps.GetSoundName(SLIDEYOURCARD));	//TAR 202910
		 ps.Message12(MSG_SLIDECARDCHECKBASKET, true);	 
         DisplayAuxScreen(AUX_SCREEN_FINISH_PAY_CARD, true);
	 }
  }
  else 
  {
	//TAR193732+ CHU: display differenct backgroud based on the device
	if (gpSCOTEncryptor->GetProfileName().CompareNoCase(_T("NCR5992Pinpad.1")) == 0)
		ps.SetCMBackground(BMP_CARDSWIPE_SIGDEV);  
    else 
	    DMSaySecurity(SEC_NONE, ps.GetSoundName(SLIDECARDONPAYMENTDEVICE));	//TAR 202910
	//TAR193732-
    ps.SetCMBackground(BMP_SLIDECARDPAYMENTDEVICE);    //slide your card on the payment device
    ps.Message12(LTT_SLIDEONPAYMENTDEVICE, true);
  }

    
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMFrame();
  // SR875
  DisplayVerifoneForm(_T("SLIDECRD"));
  
  return STATE_NULL;
}


/////////////////////////////////////////
SMStateBase  *SMSwipeCardBase::PSButtonGoBack(void)// cancel
{
  trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_ENDCARDTENDER_BALANCEDUE, SCOT_LANGUAGE_PRIMARY));
  //tar 248815 - we shouldn't send this message to RAP at this point
  //ra.OnNormalItem(_T("[highlight]")+
  //               ps.GetPSText(RA_ENDCARDTENDER_BALANCEDUE, SCOT_LANGUAGE_PRIMARY));
  trace(L7, _T("-ra.OnNormalItem"));
  
  
  SetTenderStarted(false); //SR797

  return setAndCreateAnchorState(BES_FINISH);
}

//////////////////////////////////
SMStateBase *SMSwipeCardBase::SAEventOccurred(const long)
{
	//cm150000: Added back in for Merge 2
//Security Log button is gone, this should be gone also
 /* if (isSecurityApprovalRequired())
  {
	CREATE_AND_DISPENSE(WaitForApproval)(false,false);
  }*/
	return STATE_NULL;
}

/////////////////////////////////////////
void SMSwipeCardBase::UnInitialize(void)
{
  if (!cDMLastTrack && !waitingForCardRemoval)
  {
	DMMSRDisable();	// if no card inserted, disable, no problems
  }
  else if(!co.fTenderIsMotorizedMSR)		// we scanned in a card and we're using a swipe reader
  {
	DMMSRDisable();					// Disable reader to prevent "Can't swipe card now" msgs
  }
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SwipeCard") );	// RFC 330050
}

/////////////////////////////////////////
SMStateBase  *SMSwipeCardBase::PSCancelKey(void)/// override where needed
{
  DMSayPhrase(KEYPADVALID);
  return PSButtonGoBack();
}

/////////////////////////////////////
// DMCardReader - card inserted
/////////////////////////////////////
SMStateBase  *SMSwipeCardBase::DMCardReader(void)
{
	trace(L7, _T("+ra.OnCanSuspend, CanSuspend=false"));
	ra.OnCanSuspendNow(false) ;  // un-reversable tender mode. disable suspend
	trace(L7, _T("-ra.OnCanSuspend"));
	trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_BEGINCARDTENDER, SCOT_LANGUAGE_PRIMARY));
	ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ps.GetPSText(RA_BEGINCARDTENDER, SCOT_LANGUAGE_PRIMARY));
	trace(L7, _T("-ra.OnNormalItem"));

	//DMEncryptorEnable();
	// fPIPShowCardProcessingScreen = false;

	// Validate that a master key has been stored inside the encryptor.
	bool bMasterKeyPresent = true;
	
	if (0 != DMEncryptorKeyTest())
	{
		bMasterKeyPresent = false;
	}
	ResetAllTenderValues(); //new Finalization
	//if debit
	if((co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nTenderType == TB_TENDER_DEBIT ||
		co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nTenderType == TB_TENDER_PIP_DEBIT ||
		co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nTenderType == TB_TENDER_PIP_DEBIT_CASHBACK) && TBPromptForDebitAccountType() )
	{
		CREATE_AND_DISPENSE(SelectDebitType)(true);
	}


  if(co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bNeedPin)
  {
	CREATE_AND_DISPENSE(EnterPin)(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nTenderType), co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bCashBack);
  }

  if(co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bCashBack)
  {
  	CREATE_AND_DISPENSE(SelectPayment)(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nTenderType)); //With cash back
  }

  if(co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bConfirmTotal)
  {		  
	if( co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nTenderType == TB_TENDER_FOODSTAMPS ||
		co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nTenderType == TB_TENDER_PIP_FOODSTAMPS )     //FoodStamp
	{
		CREATE_AND_DISPENSE(ConfirmEBTAmount)(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nTenderType),lFoodStampsDue,0 );
	}
	else if ( co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nTenderType == TB_TENDER_EBT ||
		co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nTenderType == TB_TENDER_PIP_EBT )           //Cash Benefit
	{
		CREATE_AND_DISPENSE(ConfirmEBTAmount)(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nTenderType),lBalanceDue,0);
	}
	else  //Other cards
	{
		CREATE_AND_DISPENSE(ConfirmDebitAmount)(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nTenderType),lBalanceDue);
	}
  }
	// allow customer to get coupon or discounts if foodstamps total is more than trx total, TAR:165687
	// submit food stamp total instead of balancedue to let customer
	// get change for his/her coupons
	long nSubmitTotal = lBalanceDue;   //Tar 170386 lFoodStampsDue;
	if( co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nTenderType == TB_TENDER_FOODSTAMPS ||
		co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nTenderType == TB_TENDER_PIP_FOODSTAMPS )     //FoodStamp
	{
		if( lBalanceDue < lFoodStampsDue) // customer has already paid part of balance off  //Tar 154631
		{
			// safeway fix, lFoodStampsDue = lBalanceDue; //gets passed in as the available EBT balance
			if (co.fTenderSubmitFSTtotalIfMorethanTrxTotal)
			{
				nSubmitTotal = lFoodStampsDue;
			}
		
		}
	}
	return TBEnterTender(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nTenderType),nSubmitTotal);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSwipeCardBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering)
		return FinalizationWtDecreaseNotAllowed(BES_FINISH);
	else
		return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSwipeCardBase::OnWtIncreaseNotAllowed()	 //tar 194770
{
	return FinalizationWtIncrease(BES_FINISH);	

}

#endif // _CPPUNIT
///////////////////////////////////////////////////////////////////////////
//TAR212390+ CHU/Dale 08/20/02
//If We are going to AssistMode from RAP at this state, we need to reset
//button index in order for the SMStateBase::AdjustTenderButtonIndex logic to work
//If we don't do this, there is no receipt printed.
SMStateBase *SMSwipeCardBase::OnRAPUnsolicitedDataReceived(LPCTSTR sData) 
{
    trace (L6, _T("+SMSwipeCardBase::OnRAPUnsolicitedDataReceived(%s)."), sData);

    CString csData = sData;
    if (GetUnsolictedDataBarcodeType(csData) == SCAN_SDT_QRCODE)
    {
        DMSayPhrase(KEYPADINVALID);
        return STATE_NULL;
    }
    nTenderButtonIndex = -1;
    trace (L6, _T("-SMSwipeCardBase::OnRAPUnsolicitedDataReceived() -- Calling base..."));   
    return STATE(State)::OnRAPUnsolicitedDataReceived(sData);
}
//TAR212390-

#ifndef _CPPUNIT
SMStateBase *SMSwipeCardBase::DMAcceptor(void)
{
  if(co.fLocaleLeaveCashAcceptorsEnabledDuringTransaction) //SSCOADK-836
  {
	trace(L6,_T("SMSwipeCardBase::DMAcceptor() (nDMCashInserted=%d) (m_nDMCashPaid=%d) (lTotalPaid=%d) (lBalanceDue=%d)"),nDMCashInserted,m_nDMCashPaid,lTotalPaid,lBalanceDue);
	ps.HideButtons();
  
	return PSButtonGoBack();  
  }
   
  long rc;
  trace(L6,_T("SMSwipeCardBase::DMAcceptor; can't accept coin/bill in this state."));
  
  rc = DMCashAcceptorReject();  // Coin Acceptor Modifications
  if (rc < 0)			//  most likely out of change
  {
	return DMCashChangerError(nDMLastBillEvent);
  }
  else if (nDMLastBillEvent > 0)
  {
    trace(L6, _T("Reject bill amount: %d"), nDMCashInserted);
    nDMCashInserted -= nDMLastBillEvent;
    nDMLastBillEvent = 0;
	
  }

  //+SSCOADK-836
  if(nDMLastCoinEvent)
  {
	  rc = DMCoinAcceptorReject();  
	  if (rc <= 0)	
	  {
		  return DMCashChangerError(nDMLastCoinEvent);
	  }
	  else 
	  {
		  trace(L6, _T("Rejected coin amount: %d"), rc);
		  nDMCashInserted -= rc;
		  if(nDMLastCoinEvent == rc)
		  {
			  nDMLastCoinEvent = 0;
		  }
		  else
		  {
			  nDMLastCoinEvent -= rc;
			  return DMCashChangerError(nDMLastCoinEvent);
		  }
	  }
  }
  //-SSCOADK-836

  if (!Is5XHardware() && !Is6XHardware())
  {
	  //+rfc330995
	  ps.SetCMFrame(_T("TakeYourReturnedCash"));
	  ps.ShowAmountDueAmountPaid(lBalanceDue, lTotalPaid);
	  ps.ShowCMFrame(true);
	  //-rfc330995
  }

  return STATE_NULL;    
}


SMStateBase *SMSwipeCardBase::PSButton1(void)    //rfc330995
{
	trace(L6, _T("SMSwipeCardBase::PSButton1"));
	return STATE_RESUME;		//return to previous screen/context
}

#endif // _CPPUNIT
