//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMEnterPinBase.CPP
//
// TITLE:    Class implementation for Enter PIN state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSelectCardType.h"       // Select card type state
#include "SMEnterPin.h"           
#include "SMSelectPayment.h"        // Select payment state
#include "SMCardMisRead.h"          // Bad card or data
#include "SMTakeCard.h"             // Take your card
#include "SMWaitForApproval.h"
#include "SMConfirmEBTAmount.h"      
#include "SMElectronicBenefitsTransfer.h"       
#include "SMEnterPin.h"
#include "SMConfirmDebitAmount.h"
#include "SMConfirmEBTAmount.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("EnterPinBase")

IMPLEMENT_DYNCREATE(SMEnterPinBase, CObject)// MFC Runtime class/object information

static  TBTENDERTYPE nTempTndrType = TB_TENDER_NONE;

//IMPLEMENT_STATE(EnterPin)

DEFINE_TIMESTAMP

////////////////////////
SMEnterPinBase::SMEnterPinBase(const TBTENDERTYPE nTyp, const bool fCshBck)
: nTndrType(nTyp), 
  fCashBackFlag(fCshBck),
  bPINCompleted(false)
{
  nTndrType = nTyp; 
  fCashBack = fCshBck;  // ..because of bad PIN

  IMPLEMENT_TIMESTAMP
  bMoveSAMsgDown = true;
}

////////////////////////////////////////
SMEnterPinBase::SMEnterPinBase()
: bPINCompleted(false)
{
  IMPLEMENT_TIMESTAMP
//  nTndrType = nTenderType;   // must initialize - TAR 142611.
  fCashBackFlag = fCashBack;
  nTndrType = nTempTndrType;
}

////////////////////////////////////////
SMStateBase  *SMEnterPinBase::Deliver(const TBTENDERTYPE nTyp, const bool fCshBck)
{
	nTndrType = nTyp; 
	fCashBackFlag = fCshBck;

	fCashBack = fCshBck;  // ..because of bad PIN
	bMoveSAMsgDown = true;

	return this;
}
////////////////////////////////////////
bool SMEnterPinBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, one dword and one bool (word)
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMEnterPinBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver((TBTENDERTYPE)DLParamDWORD(dlParameterList),    // nTyp
                (bool)DLParamWORD(dlParameterList));            // fCshBck
        return true;
    }
    else
    {
        trace(L2, _T("+SMEnterPinBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}

////////////////////////////////////
SMStateBase  *SMEnterPinBase::Initialize(void)
{
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("EnterPin") );

  // CANNOT enter Remote Assist Mode at this state
  //ra.OnSetTransactionState(RA_STATE_ASSISTNOTOK);
    ra.RequestEnableAssistMode(false);   //TAR 213249

  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_PINENTER, SCOT_LANGUAGE_PRIMARY));
  //Set text, if re-entering a PIN because previous attempt was
  //..wrong, then use different text
  if (nTndrType == TB_TENDER_NONE)
  {
	nTndrType = nTempTndrType;
  }
  PSTEXT MsgText;
  BMPS   bitmap;

  if (!cDMLastTrack)
	return setAndCreateAnchorState(BES_FINISH);
  if (nPINTries)
  {
    ps.SetCMFrame(_T("ReEnterPin"));
    MsgText = MSG_REENTER_PIN;
    // Needs to use CInfoFinalPopup bitmap 
	//in order for msg. to fit the screen.
    bitmap =  BMP_BENEFITTYPE; 
  }
  else 
  {
    ps.SetCMFrame(_T("EnterPin"));
    MsgText = MSG_PINENTER;
	//TAR194406+: display bmp based on the device
	if (gpSCOTEncryptor->GetProfileName().CompareNoCase(_T("NCR5992Pinpad.1")) == 0)
		bitmap = BMP_PINENTER_NOECHO_SIGDEV; //bmp with 5992
	else
		bitmap = BMP_PINENTER_NOECHO;   //bmp with 5945	TAR194406-
  }

  ps.CMButton(_T("ButtonGoBack"), BTT_CANCELPAY, true);

  ps.Message12(MsgText,true);
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);

  // allow the EchoInput() func to accept a PIN invisibly
  ps.AllowInput(MAX_DIGITS_PIN,true,false);
	
  // get account and working key from TB
  // TAR# 127873 - support changing PIN block format from ANSI to Diebold
  TBPINBLOCKFORMAT nPinBlockFormat   = TB_ANSI;         // default to ANSI before calling TB
  TBPINPADSYSTEM   nPinpadSystem     = TB_PINSYSTEM_MS; // default to Master/Session
  TBPINENCODING    nPinBlockEncoding = TB_PINENCODING_COMPATIBLE; // default to pre-2.0 behavior

  long rc = TBGetPINWorkingKey(
              csChargeAccount,		// Account number from MSR data
              csWorkingKey,			// Working key
              nTndrType,            // Tender type
              nPinBlockFormat,
              nPinpadSystem,
              nPinBlockEncoding);		    

  if (rc != TB_SUCCESS)
  {
	trace(L3,_T("Card rejected by TB, TBGetPINWorkingKey() returns:%d"),rc);
	// card data is not complete w.r.t TB or can be a TB problem to set the working key

	//TAR193038
	bBadCard = true;

	if (co.fTenderIsMotorizedMSR)
	{
		CREATE_AND_DISPENSE(CardMisRead)(MSG_MISREAD_MCRW, MSG_ENCRYPTORSETUPFAIL);
	}
	else
	{
		CREATE_AND_DISPENSE(CardMisRead)(MSG_MISREAD_SLIDE, MSG_ENCRYPTORSETUPFAIL);
	}
  }
  // TAR 320164
  trace(L6,_T("Encryptor working key <%s>  PinFormat %d, PinSystem %d, PinEncoding %d"),
        csWorkingKey,
        nPinBlockFormat,
        nPinpadSystem,
        nPinBlockEncoding);
	    
  // Store Encryptor Key and then put it into secure mode by calling
  // EncryptorGetPinBlock method.
  // Bringing it out of secure mode is accomplished 4 ways
  // so far 1.) disabling the encryptor 2.) depressing the
  // CANCEL or OK key on the pinpad or obtaining a full pin number.

  DMSayPhrase(ENTERYOURPIN);
  ps.ShowCMFrame();

  if(DMEncryptorSetup(csWorkingKey, nPinBlockFormat, nPinpadSystem, nPinBlockEncoding) != 0)
  {	  // encryptor setup failed, goto card misread and eject the card
	  bBadCard = true;
	  IsEncryptorSetupFail = true;
	  
	  if (co.fTenderIsMotorizedMSR)
	  {
		CREATE_AND_DISPENSE(CardMisRead)(MSG_MISREAD_MCRW, LTT_CARDMISREAD);
	  }
	  else
	  {
		CREATE_AND_DISPENSE(CardMisRead)(MSG_MISREAD_SLIDE, LTT_CARDMISREAD);	//TAR193038
	  }
  }	

  // Done with values after call to Encryptor setup. Clearing fields
  csChargeAccount = _T("");
  csChargeExpiry  = _T("");
  m_bNeedToDisplayAuxScreen = false;  // PIN Entry uses AuxDisp so don't overwrite screen

  return STATE_NULL;				  // stay in EnterPin state
}

//////////////////////////////////////
void SMEnterPinBase::UnInitialize()
{
  if (!bPINCompleted)
  {
      DMEncryptorCancelSecureMode();
  }
  nTempTndrType = nTndrType;
  // CAN enter Remote Assist Mode after leaving this state
  //ra.OnSetTransactionState(RA_STATE_ASSISTOK);
  ra.RequestEnableAssistMode(true);   //TAR213249
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("EnterPin") );

}

///////////////////////////////
SMStateBase  *SMEnterPinBase::TimedOut()
{
  //DMEncryptorCancelSecureMode();
  // Back to our regularly scheduled timeout programming.
  return SMStateBase ::TimedOut();
}

/////////////////////////////////// 
SMStateBase  *SMEnterPinBase::PSButtonHelp(void)// help
{
  setAnchorState(BES_ENTERPIN); //TAR137419 return to enter pin screen after help
  //DMEncryptorCancelSecureMode();
  return SMStateBase ::PSButtonHelp();
}

///////////////////////////////////
SMStateBase  *SMEnterPinBase::PSButtonGoBack(void)// cancel
{
  // The response the application produces when the UI Cancel button is depressed
  // is now to quickly disable and re-enable the encryptor.  This causes a Cancel event
  // to be fired by the encryptor and it also causes the encryptor to exit secure mode
  // which is what we needed to do to correct the PR#65 error.
  // The applications general state is with the encryptor enabled, done in the out-of-service
  // state so to follow protocal we quickly re-enable it.
  //DMEncryptorCancelSecureMode();
  setAnchorState(BES_FINISH);
  // goto take card, to eject the card and return to the base state
  CREATE_AND_DISPENSE(TakeCard)(false);		  // not end of transaction
}

/////////////////////////////////////////////////
SMStateBase  *SMEnterPinBase::PSNumericKey(const long lKey)
{
  // be sure never to show the PIN number
  // this is handled 3 way (over kill)
  // 1) the Encryptor returns lKey = some odd character
  // 2) we pass the Echo Input a '0"
  // 3) the EchoInput routine will show a '*'
  // when the encryptor get the right number of
  // PIN digits it will signal a DM_ENCRYTOR event
  // that will be handled by SMStateDM
	
  PSEchoInput(_T('0'));
  return STATE_NULL;
}

/////////////////////////////////////////
SMStateBase  *SMEnterPinBase::PSClearKey(void)
{
  PSClearInput();
	
  return STATE_NULL;
}


/////////////////////////////////////////
SMStateBase  *SMEnterPinBase::DMEncryptorSecured(void)// now in secure mode
{
  return STATE_NULL;
}

/////////////////////////////////////////
SMStateBase  *SMEnterPinBase::DMEncryptorCanceled(void)// encryptor canceled
{
  DMSayPhrase(KEYPADVALID);
  return PSButtonGoBack();
}

/////////////////////////////////////////
SMStateBase  *SMEnterPinBase::DMEncryptorCompleted(void)// encryptor got PIN
{
  csDMEncryptedPin = DMEncryptorGetPIN();
  to.HexTrace(L6,_T("Pin Block"),csDMEncryptedPin,csDMEncryptedPin.GetLength());
  //return new SMSelectPayment;

  trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(MSG_RA_PINENTERED, SCOT_LANGUAGE_PRIMARY));
  ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+
                  ps.GetPSText(MSG_RA_PINENTERED, SCOT_LANGUAGE_PRIMARY));
  trace(L7, _T("-ra.OnNormalItem"));
  bPINCompleted = true;

  DMSayPhrase(KEYPADVALID);

	if (nTndrType==TB_TENDER_FOODSTAMPS || nTndrType==TB_TENDER_PIP_FOODSTAMPS || nTndrType==TB_TENDER_EBT || nTndrType==TB_TENDER_PIP_EBT)
	{
		if(fCashBackFlag)					 
		{
			CREATE_AND_DISPENSE(SelectPayment)(nTndrType); //With cash back
		}
		long lAmtDue = lBalanceDue;

		if (nTndrType==TB_TENDER_FOODSTAMPS || nTndrType ==TB_TENDER_PIP_FOODSTAMPS)
			lAmtDue = lFoodStampsDue; // ConfirmEBTAmount will adjust the total if needed for foodstamps tender

		if(co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bConfirmTotal)
		{
			CREATE_AND_DISPENSE(ConfirmEBTAmount)(nTndrType,lAmtDue,0);
		}
		//else
		if (nTndrType==TB_TENDER_FOODSTAMPS || nTndrType ==TB_TENDER_PIP_FOODSTAMPS)
		{
			// submit food stamp total instead of balancedue to let customer
			// get change for his/her coupons
			lAmtDue = lFoodStampsDue;
			if( lBalanceDue < lFoodStampsDue) // customer has already paid part of balance off  //Tar 154631
			{
				if (!co.fTenderSubmitFSTtotalIfMorethanTrxTotal)
				{
					lAmtDue = lBalanceDue;
				}
			}
		}
		return TBEnterTender(
				 nTndrType,
				lAmtDue);
	}
    //Other cards
	if(co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bCashBack)					 
	{
		CREATE_AND_DISPENSE(SelectPayment)(nTndrType);		  //With cash back
	}
	  
	if(co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bConfirmTotal)
	{
	    CREATE_AND_DISPENSE(ConfirmDebitAmount)(nTndrType,lBalanceDue);
	}
	
  // the TS BALANCE/CHANGE message will change us to either
  // STCashPayment or STTakeChange or STTakeReceipt

  return TBEnterTender(
	  nTndrType,
		lBalanceDue);
}

/////////////////////////////////////////////////
SMStateBase  *SMEnterPinBase::DMEncryptorPinIsTooShort(void) // The returned pin was to short.
{
  // The Customer did not enter a full sized pin
  DMSayPhrase(KEYPADINVALID);
	
  PSClearAllInput();     // since ATM pad thinks all data input is cleared
	
  // reset the encryptor
  //DMEncryptorCancelSecureMode();
	
  nPINTries++;
  if(nPINTries < 3)
  {
    CREATE_AND_DISPENSE(EnterPin)(nTndrType, fCashBackFlag); //try again
  }
  else
  {  // 3 wrong PIN attempts.. must do something different
    if (co.fTenderIsMotorizedMSR)
    {
      CREATE_AND_DISPENSE(CardMisRead)(MSG_INSERT_DIFF_CARD, TTL_WRONG_PIN);
    }
    else
    {
	  CREATE_AND_DISPENSE(CardMisRead)(MSG_SLIDE_DIFF_CARD, TTL_WRONG_PIN);
    }
  } 
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMEnterPinBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering)
		return FinalizationWtDecreaseNotAllowed(BES_STATERETURN);
	else
		return STATE_NULL;
}
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMEnterPinBase::OnWtIncreaseNotAllowed()	 
{
	return FinalizationWtIncrease(BES_STATERETURN);	
}

