//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMFinishBase.CPP
//
// TITLE: Class implementation for Finish State
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>				 // MS MFC always needed first
#include <afxtempl.h>			 // MS MFC template header

#include "Common.h"				 // Common includes
#include "SMState.h"			 // Base state
#include "SMFinish.h"		
#include "SMContinueTrans.h"	 // Continue trans state
#include "SMWaitForApproval.h"	 // Wait for approval state
#include "SMTakeCard.h"			 // Take card
#include "SMThankYou.h"
#include "SMDepositCoupons.h"
#include "SMSystemMessage1.h"
#include "SMSystemMessage4.h"

#include "SMSelectCardType.h"	 
#include "SMCashPayment.h"		 // Cash payment state
#include "DMProcedures.h"		 // DM procedures
#include "SMGetFoodStampState.h"	
#include "SMSuspend.h"
#include "SMEnterCoupons.h"
#include "SMSwipeCard.h"
#include "SMSelectDebitType.h"
#include "SMEnterPin.h"
#include "SMConfirmDebitAmount.h"
#include "SMSelectPayment.h"        // Select payment state
#include "SMConfirmEBTAmount.h"
#include "SMSecUnExpectedIncrease.h"
#include "SMSecUnexpectedDecrease.h"
#include <mmsystem.h>               //Tar 198015: add this to support PlaySound //HDMerge-062502
#include "SMAssistedTender.h"
#include "SMVoucherPayment.h"
#include "SMScanCard.h"
#include "SMSavePreferences.h"
#include "SMTakeChange.h"		   // TAR 355864

#define COMP_ID ID_SM			   // base state component
#define T_ID	_T("FinishBase")

bool SMFinishBase::issuedCashAcceptorError = false;
static bool bIsOtherPayment = false;


IMPLEMENT_DYNCREATE(SMFinishBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(Finish)

DEFINE_TIMESTAMP
    
////////////////////
// Constructor
////////////////////
SMFinishBase::SMFinishBase()
: m_bCashTender(false), 
  m_bRetFoodStampState(false)
{
  IMPLEMENT_TIMESTAMP
  // saMsgFuncPtr = ps.SecurityMessageNonmodal;  //Tar 163689 Don't show security message text because there is no place to show
  // let us do above fix in a way we don't have to modify SecurityMessageNonmodal()  // TAR: 164195
  // on any screen, where we don't need to see security message,
  // just add the below line in the screen class's constructor
  saMsgFuncPtr = NULL;
}
 
///////////////////////////////////
// Initialize
///////////////////////////////////
SMStateBase  *SMFinishBase::Initialize(void)
{
  //Tar 206893.  Need to re-enable the AM button on RAP if we come back to this screen 
  //without finish other tender which goes through cardprocessing where the AM button is disabled
  //ra.OnSetTransactionState(RA_STATE_ASSISTOK);

  SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);

  m_bCancellingCashTender = false; //TAR323590
  EnableHandheldAssistMode(true);
  ra.RequestEnableAssistMode(true);   //TAR213249
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("Finish") );
  

  nDeviceErrorContext = 1;  //FZ20021010
  
  //411442+
  //we need to introduce another flag since io.reset() is called before io.nLoyaltyCardScanned is referenced.
  bool bLoyaltyCardAtFinish = false;
  if (io.nLoyaltyCardScanned == 1)
  {
	  bLoyaltyCardAtFinish = true;
  }
  //411442-

  
  io.Reset();	// 226678 must reset the item object before going into finalization because we're done with prev item's info.  LPM021503
  //+SR713
  if(bIsInPreTenderState)
  {
	return PreTenderState();
  }
  //-SR713

  bIsInFinalization = true; //tar 194770
  // tar 223051 once in F&P, allowed to remove items unless on WaitForapproval
  g_bAllowRemove = true;
  g_bMatchedWtWaitingOnEAS = false;
  m_bEASHappened = false;
  fBeyondFinishAndPayState = false;
  bool bIsAnyApprovalRequired = isAnyApprovalRequired(false);
  if (!bIsAnyApprovalRequired)
  {
     //Update RAP window with SCOT state, but not if approval being requested (TAR 256441)
     UpdateStateName(ps.GetPSText(TTL_SELECTPAYMENTTYPE, SCOT_LANGUAGE_PRIMARY));
  }

  setAnchorState(BES_FINISH);

  SMStateBase *sReturnState = CkStateStackAndReturn();
  if (sReturnState != STATE_NULL)
	return sReturnState;

  if (!co.IsAttendantModeOn() &&
      !co.IsInAssistMenus() &&
      NeedPreferenceUpdatePermission())
  {
      custInfo.SetPreference(_T("SavePreferences"), _T(""));
      RETURNSTATE(SavePreferences);
  }

  ps.SetCMFrame(_T("Finish")); //tar 177485
  ps.SetCMLeadthruText(TTL_SELECTPAYMENTTYPE);
  if ( co.fTenderEBTFSAllowed && bUpdateFoodStampTotal && (isAnySoldVoidedItems()))
  {
    // card inserted, items sold, no need of approvals, let me ask what type of card it is?
      m_bRetFoodStampState = true;
	  RETURNSTATE(GetFoodStampState)
  }

  //TAR#121479	LPM092199  moved check from attract screen to here
  SMStateBase * checkState = checkCashAcceptorCassette();
  if (checkState != STATE_NULL)		//if the cash acceptor cassette errors 
	return checkState;

  if (bIsAnyApprovalRequired)  // don't update SA - otherwise coupon ratio not okay show up in security log (not wanted according to Dusty)
  {
    dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                                DM_TRICOLORLIGHT_BLINK_1HZ,
                                DM_TRICOLORLIGHT_NORMALREQUEST);  //TAR 293966


	if (isOnlyHopperSubsApprovalRequired())
	{
			bool bEmpty = false;
			bool bChuteJam = false;
	
			
			CString csWork;
		    csWork=  _T("DeviceErrorCashChanger.xml:HOPPER_SUBSTITUTION");
			if( co.fLocaleCashManagementScreen && co.fLocaleCashManagementAPPTrackCashCount )
				csWork=  _T("DeviceErrorCashChanger.xml:HOPPER_SUBSTITUTION_CASH_MANAGEMENT");
			CString csTitle1, csTitle2, csMessage, csMovie;
			BMPS bitmap;
			PSTEXTWAVE nWave;
			ProcessDeviceErrorXMLFile( csWork, csTitle1, csTitle2, csMessage, bitmap, nWave, csMovie);		
		  
		    return  createLevel1Message(csWork,           // char *message
                              PS_NOTEXT,        // no canned message
                              PS_NOTEXT,        // no prompt
                              0,                // no value
                              DMCLASS_CASHCHANGER,// device reporting the error or -1
                              true);            // T/F need to issue CheckHealth after operator clears problem
		  
	}
    CREATE_AND_DISPENSE(WaitForApproval)(false,false);


  }

  // TAR 392460 - Moved TENDER_START message here so that it is sent after WaitForApproval is cleared.
  SASendSCOTInput( TENDER_START );

  trace(L7, _T("+ra.OnSetTransactionState, Tender State"));
  ra.OnSetTransactionState(RA_STATE_TENDER);
  trace(L7, _T("-ra.OnSetTransactionState"));

  TBEnterTenderState(true);   //Tar 138312 ,moved here to fix TAR:160950 by ` Koteswara

  if (bSuspendRequested && !bSuspendApproved)
     RETURNSTATE(Suspend)

  // TAR 180470, Crawford: 8/29/1: Moved DMMSREnable() before ConfigureTenderOptions() because
  // when the 5945 is enabled the display is replaced with "Slide you card" message
  // rather than the one left by ConfigureTenderOptions().
  DMMSREnable();					// enable the card reader, set track, turn on MEI

  //SR742+
  if(nDMCashInserted && dm.GetDegradedModeType() == DEGRADEDMODE_CARD_ONLY )
  {
	fCancelPayment = true;
	lChangeDue = nDMCashInserted;
	nDMCashInserted = 0;
	CREATE_AND_DISPENSE(TakeChange)(false);
  }
  //SR742-

  // set up the buttons based on then tender options in SCOTTend.xxx and SCOTOpts.xxx
  ConfigureTenderOptions();

  // before we pay, deposit coupons if necessary
  // don't show Deposit Coupons screen if coupon sensor is configured
  if (lCoupons && !IsUsingCouponSensor())
  {
	return DepositCouponsIfNecessary();
  }

  //TAR#125088 Wasn't refunding partial cash tender with cancelled items
  if ((lBalanceDue<0) ||                              //Change due
      (lBalanceDue==0 && isAnySoldVoidedItems()))	  //No Change due, but items sold and still inside a transaction 
  {
	if(g_bgVerifySig)
	{
		CREATE_AND_DISPENSE(WaitForApproval) (false,false, true) ; //(bVerifyTrans, bVoidTrans, bVerifySig)  
	}
    if(!fInSignedOff)
    {
	   return TBEnterTender(TB_TENDER_CASH, (nDMCashInserted) );	// may be zero or +ve
    }
    else
    {
       return TBFinish(); //tar 228763 
    }
  }

  if (!isAnySoldVoidedItems() || !(fInSignedOn || fInSignedOff)) 
  {
    // at the end of transaction, call TBFinish() to signoff
    if (fInSignedOn && !fInSignedOff)
    {
        fBeyondFinishAndPayState = true;
		//+TAR 318953
		if(nDMCashInserted>0)
		{			
			ps.ShowAmountDueAmountPaid(lBalanceDue, lTotalPaid + nDMCashInserted);
			return TBEnterTender(TB_TENDER_CASH, nDMCashInserted);			
		}		
		//-TAR 318953
		else
		{
			return TBFinish();   //Tar 126024
		}
    }
    else if (!fInSignedOn && !fInSignedOff) // no transaction was started yet or not inside any transaction
    {
        fBeyondFinishAndPayState = true;
		//+TAR 318953
		if(nDMCashInserted>0)
		{
			ps.ShowAmountDueAmountPaid(lBalanceDue, lTotalPaid + nDMCashInserted);
			return TBEnterTender(TB_TENDER_CASH, nDMCashInserted);			
		}
		else
		{
		//-TAR 318953
			CREATE_AND_DISPENSE(TakeCard)(true);
		}
    }
    else
    {
    	return STATE_NULL;
    }
  }
  
  //TAR193038+ If the card has been swiped & is a bad card, 
  //throw bad track data away, and Stay at FinishAndPay
  if (cDMLastTrack && bBadCard)
  {
	  delete cDMLastTrack;
	  cDMLastTrack = NULL;
	  nDMLastTrackLen = 0;
	  csDMEncryptedPin = EMPTY_STRING;
	  bBadCard = false;
  }  //TAR193038-

  // +TAR 355864
  // If dispense at cancel payment failed, try to dispense again.
  if(lChangeDue > 0)
  {
	  RETURNSTATE(TakeChange)
  }
  // -TAR 355864

  if (nDMCashInserted>0)	// if cash already inserted
  {
    if (nDMCashInserted<lBalanceDue)
    {
      RETURNSTATE(CashPayment)
    }
    ps.ShowAmountDueAmountPaid(lBalanceDue, lTotalPaid + nDMCashInserted);	// Tar 117991 
    return TBEnterTender(TB_TENDER_CASH, nDMCashInserted);			// may be zero or +ve
  }
  
  CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMFinishBase::Initialize"));
  // check for loyality reminder option and play a different wave file
  // TAR 195033 if Loyalty Card is not allowed, don't play the wave file
  // Tar 222575 - tar 201942 breaks the audio play in this state, added more checking to limit the old fix to only affect OtherPayment 
  if (!bFinish || (bFinish && !bIsOtherPayment)) //T.D 04/29/02 TAR 201942 shouldn't say "Insert Cash or select payment type"after "Confirm Suspend"
  {
	  if (nTenderType == TB_TENDER_GIFTCARD)
	  {
		  DMSayPhrase(GIFTCARDAPPLLIEDBALANCEREMAINING);
	  }
	  else if (co.fStateAllowLoyaltyCard && co.fStateLoyaltyCardReminder && !fLoyaltyCardTrx)
	  {
		  if(!co.fTenderCashAllowed)
			  DMSaySecurity( SEC_NONE, ps.GetSoundName(SELECTPAYMENTTYPEFREQSHOPPER));	// say with security so it can be interrupted
		  else if (!bFinish)
			  DMSaySecurity( SEC_NONE, ps.GetSoundName(INSERTCASHSELECTPAYMENTTYPEFREQSHOPPER));	// say with security so it can be interrupted
	  }

	  //Tar 207697 AH 06/25/02
	  //else if (co.fStateAllowLoyaltyCard &&  fLoyaltyCardTrx)  //tar 362671
      else if (co.fStateAllowLoyaltyCard && bLoyaltyCardAtFinish ) //411442
	  {
		  DMSaySecurity( SEC_NONE, ps.GetSoundName(FREQSHOPACCT));	// say with security so it can be interrupted
		  //io.nLoyaltyCardScanned = 0;
          bLoyaltyCardAtFinish  = true;
	  }


	  else if(!cDMLastTrack)    //card not inserted	
	  {
		  //TAR 410077
		  if(!co.fTenderCashAllowed || ( co.IsDegradedModeOn() && 
			  (dm.GetDegradedModeType()  == DEGRADEDMODE_CARD_ONLY)) )
		  {
			  DMSayPhrase(SELECTPAYMENTTYPE);
		  }
		  else
			  DMSayPhrase(INSERTCASHSELECTPAYMENTTYPE);
	  }
	  else
		  DMSayPhrase(CARDACKNOWLEDGEDSELECTCARDTYPE);
  }

  bIsOtherPayment = false;  //tar 222575
  dm.PinPadDisable(); //TAR 119728

  ps.LoadTenderList();	//this will show the entire list starting with the first item in the list

  //SR742+ if no payment button available on F&P.display call for help
  if ( !IsPaymentButtonAvailable() )
  {
	  return SMStateBase::PSButtonHelp();
  }
  //SR742-

  // if no payment yet, then leave scanner enabled 
  if (lTotalPaid <= 0)
  {
	trace(L2, _T("::Initialize - bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;
    SAWLDBScannerEnable();			// enable the scanner
  }

  // TAR 180470, Crawford: 8/29/1: Moved DMMSREnable() before ConfigureTenderOptions() because
  // when the 5945 is enabled the display is replaced with "Slide you card" message
  // rather than the one left by ConfigureTenderOptions().
  //DMMSREnable();					// enable the card reader, set track, turn on MEI

  DMMSRLight(DM_MISC_STATE_ONE_HZ); // blinking
  ps.ShowAmountDueAmountPaid(lBalanceDue, lTotalPaid + nDMCashInserted);
  ps.ShowCMFrame();
  return STATE_NULL;				// stay in Finish state
}

/////////////////////////////////
// UnInitialize
/////////////////////////////////
void SMFinishBase::UnInitialize(void)
{
  // BeginCodeWLDBSA11
    SAWLDBScannerDisable();			   // disable the scanner
  // EndCodeWLDBSA11
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("Finish") );

  DMCashAcceptorDisable();	// disable acceptor, turn light off	// Coin Acceptor Modifications
  //tar256688 -- only turn off CoinAcceptor in not going to CashPayment Screen
  //TAR 282926 
  if( isAnyApprovalRequired(false)  || (!m_bCashTender && !m_bRetFoodStampState && nDMCashInserted == 0) || ((nDMCashInserted > 0) && (nDMCashInserted >= lBalanceDue)) )
  {
	  /*  Tar 376685 -- take out fix for 313901
	  //
	  // TAR 314457 (SafePay):  Restrict 313901 fix to Glory recycler.
	  //                        313901 addressed by fix for 358311.  Fix for 313901 will
	  //						be removed in the next release.
	  if(dm.m_csCashRecyclerModel != _CASHDISPENSER_GLORY)	// TAR 313901
	  {				
		  DMCoinAcceptorDisable();	// disable acceptor, turn light off	// Coin Acceptor Modifications
	  }
	  else
	  {
		  CString csDeposit;
		  dm.CashRecyclerStopDeposit(csDeposit);			// TAR 313901
	  }*/

	  DMCoinAcceptorDisable();	// disable acceptor, turn light off	// Coin Acceptor Modifications
  }  

  if (!cDMLastTrack)				// Changed for PR 022598-8
  {
    DMMSRDisable();		// if no card inserted, disable
  }
  else if(!co.fTenderIsMotorizedMSR)      // we scanned in a card and we're using a swipe reader
  {
    DMMSRDisable();                // Disable reader to prevent "Can't swipe card now" msgs
  }
  DMMSRLight(DM_MISC_STATE_OFF);	// turn off MEI, card or no card
}

//////////////////////////////////
// PSButtonGoBack
//////////////////////////////////
SMStateBase  *SMFinishBase::PSButtonGoBack(void)
{
  //+SR713
  if(co.fOperationsRepeatSellBags && co.fOperationsSellBagsAtFinalize)
  {
	bDoNotShowSellBags  = false;
  }
  //-SR713
  
  SASendSCOTInput( TRANSACTION_CONTINUE );
  bEASReEnableScanner = true;
  return setAndCreateAnchorState(BES_SCANANDBAG);
}
//////////////////////////////////
// PSButtonHelp
//////////////////////////////////
SMStateBase *SMFinishBase::PSButtonHelp()
{
  return SMStateBase::PSButtonHelp();
}

///////////////////////////////////
// DMAcceptor - cash inserted
///////////////////////////////////
SMStateBase  *SMFinishBase::DMAcceptor(void)
{
  ps.HideButtons();
  
  if (nDMCashInserted<lBalanceDue)
  {
    RETURNSTATE(CashPayment)
  }

  ps.ShowAmountDueAmountPaid(lBalanceDue, lTotalPaid + nDMCashInserted);
  return TBEnterTender(TB_TENDER_CASH, nDMCashInserted);		// may be zero or +ve
}

//////////////////////////////////
// DMScanner - something scanned
//////////////////////////////////
SMStateBase  *SMFinishBase::DMScanner(void)
{
  SASendSCOTInput( TRANSACTION_CONTINUE );
  csItemSent = csDMLastBarCode;
  return TBItemSale(false,0,true);
}

/////////////////////////////////////
// DMCardReader - card inserted
/////////////////////////////////////
SMStateBase  *SMFinishBase::DMCardReader(void)
{
//  lEOT_LGW = g_lLGW; //tar 194770
  DMSaySecurity(SEC_NONE, ps.GetSoundName(CARDACKNOWLEDGEDSELECTCARDTYPE));	//TAR 202910
  return STATE_NULL;
}

//////////////////////////////////////////////
// SAEventOccurred
//////////////////////////////////////////////
SMStateBase  *SMFinishBase::SAEventOccurred(const long)
{
	/******cm150000: Modified in Merge 2******/
	//Security Log button is gone, this should be gone also
	//if (isSecurityApprovalRequired())
	//{
   // CREATE_AND_DISPENSE(WaitForApproval)(false,false);
	//}
	/******cm150000: End Modified In Merge 2******/
    return STATE_NULL;
}

///////////////////////////////////////////////
// checkCashAcceptorCassette
///////////////////////////////////////////////
SMStateBase  *SMFinishBase::checkCashAcceptorCassette()
{

   	CString csTitle1, csTitle2, csMessage, csMovie, csWork;
	BMPS bitmap;
	PSTEXTWAVE nWave;

  if (co.fTenderCashAllowed)
  {
    long lCode = dm.GetCashAcceptorDeviceStatus();
    // Sure, it SAYS it's OK, but it's not to be trusted!
    // (i.e. not all errors reported by above function are correct at this time.)
    if (lCode == DM_CASHACCEPTOR_STATUS_OK)
      lCode = lastDMCashAcceptorCode;

    if ((lCode == DM_CASHACCEPTOR_STATUS_TAMPER_OUT) ||
		(lCode == DM_CASHACCEPTOR_STATUS_FAILURE)    ||
		(lCode == DM_CASHACCEPTOR_STATUS_JAM)
		/* Base issue: after successful check health, device always gives DM_CASHACCEPTOR_STATUS_RESET.
        That means reset status is not a error and do not check for it here. TAR# 208758
        || (lCode == DM_CASHACCEPTOR_STATUS_RESET)
        */
		) 
    {
		// +TAR 357015:  Remove hardcoded error tags.
		csWork = GetDeviceErrorMnemonic(DMCLASS_CASHACCEPTOR, 
			                            dm.GetDeviceName(DMCLASS_CASHACCEPTOR, 0), 
			                            lCode, 
			                            dm.GetResultCodeExtended(DMCLASS_CASHACCEPTOR, 0), 
			                            nDeviceErrorContext);
		// -TAR 357015

      // Normally only createLevelxMessage is issued - bypass that so we
      // can control if we ask for operator/password (after the first time,
      // don't ask again if this condition continues to occur).
	  if (issuedCashAcceptorError)
	  {			
		CREATE_AND_DISPENSE(SystemMessage1)(csWork,      // char *message
				PS_NOTEXT,
				PS_NOTEXT,	// no prompt
				0,					// no value
				DMCLASS_CASHACCEPTOR, // DM device
				true);     // DM check health after
	  }
      else
      {
        issuedCashAcceptorError = true;
        // Begining of TAR 197957
        //Notify RAP
		CString csSecInfo, csErrorMessage;
		//Tar215999 Send RAP the correct message based on the error.

        ProcessDeviceErrorXMLFile( csWork, csTitle1, csTitle2, csMessage, bitmap, nWave, csMovie);

        trace(L7, _T("+ra.OnRAPDataNeeded, ErrorMessage=%s, DeviceClass=%d"), 
		      csWork, DMCLASS_CASHACCEPTOR);

        // write the error description on the receipt
	    ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csTitle1);
        // send a DataNeeded Approval
	    cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("DeviceFailure"));
	    m_View = DATANEEDED_DEVICEERROR;
	    csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), 
			             csTitle1 + _T("\n") + csTitle2 ,
						 ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), 
						 _T(""), _T(""), _T(""), _T(""));
	    ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
	    trace(L7, _T("-ra.OnRAPDataNeeded "));
		// End of TAR 197957


	  CREATE_AND_DISPENSE(SystemMessage4)(csWork,      // char *message
                                    PS_NOTEXT,
                                    PS_NOTEXT,	// no prompt
                                    0,					// no value
                                    DMCLASS_CASHACCEPTOR, // DM device
                                    true);     // DM check health after

	

      } 
    }
    else
	  issuedCashAcceptorError = false;
      return STATE_NULL;
  }
  else
    return STATE_NULL;
}

SMStateBase *SMFinishBase::PSListLookup(CString cItemCode) // list or lookup selection
{
// NewUI
	long lItemCode = 0;
	lItemCode = _ttoi(cItemCode);
	return ButtonPressed(lItemCode);
// NewUI
}

SMStateBase *SMFinishBase::ButtonPressed(int buttonindex)  //buttonindex start from 1 to the max button number
{
    trace(L6,_T("Tender button number %d was pressed on Finish and Pay screen."), buttonindex);
	SetTenderButton(buttonindex);
    int nTenderType = GetTenderButton().nTenderType;

//USSF START
	CString csTempstr;
	csTempstr.Format(_T("%d"), (long)nTenderType);
	USSFDATA(_T("TENDERTYPE"), csTempstr);
//USSF END

    //++Tar 266220 
    EnableHandheldAssistMode(false);
    if(TB_TENDER_OTHERPAYMENT == nTenderType)
    {
        //If tranx partially tendered, but config option not set to allow suspend if tranx is partially tendered
        if( (short(lTotalPaid) != nDMCashInDrawer) /*RFC267820 Not approved for 4.0 yet --> && !co.fOperationsAllowSuspendIfPartiallyTendered*/ ) 
        {
            //change tender type so that suspend can't happen at RAP either
            nTenderType = TB_TENDER_ASSISTTENDER;
        }
    }
    //--Tar 266220
    if (GetTenderButton().nAssistedTender >= 1)
    {
      CString sDesc;
      CString sTenderDesc = GetTenderButton().csTenderText;
      ASSERT(!sTenderDesc.IsEmpty());
      if (!sTenderDesc.IsEmpty())
         {
         sDesc.Format(ps.GetPSText(RA_ASSISTED_TENDER,SCOT_LANGUAGE_PRIMARY), 
            (LPCTSTR)co.TenderList[SCOT_LANGUAGE_PRIMARY][buttonindex-1].csTenderText);  //TAR 270923  
         CREATE_AND_DISPENSE(AssistedTender)(sDesc);
         }
      else
         return STATE_ERROR;
    }
	switch(nTenderType)
	{
	case TB_TENDER_CASH:  //cash
		{
		  m_bCashTender = true;
		  RETURNSTATE(CashPayment)
		}
   	//case TB_TENDER_OTHERPAYMENT: //OtherPayment //Tar 206110 //Undo this change --Feng
    case TB_TENDER_ASSISTTENDER:  //Tar 207043 Feng 20020611
	case TB_TENDER_CHECK: //check
        {
			CString sDesc, sTenderDesc = GetTenderButton().csTenderText;
			ASSERT(!sTenderDesc.IsEmpty());
			if (!sTenderDesc.IsEmpty())
			{
                EnableHandheldAssistMode(true);
				sDesc.Format(ps.GetPSText(RA_ASSISTED_TENDER,SCOT_LANGUAGE_PRIMARY), 
					(LPCTSTR)co.TenderList[SCOT_LANGUAGE_PRIMARY][buttonindex-1].csTenderText);   //TAR 270923 
				trace(L6, _T("TB_TENDER AssistedTender %s"), sDesc);
				CREATE_AND_DISPENSE(AssistedTender)(sDesc);
			}
			else
				return STATE_ERROR;
        }
	//Begin RFC 356020: Support Check payment on lane using printer 7167
	case TB_TENDER_CHECK_LANE: //check on lane only
        {
			CString sDesc, sTenderDesc = GetTenderButton().csTenderText;
			ASSERT(!sTenderDesc.IsEmpty());
			if (!sTenderDesc.IsEmpty())
			{
				if (dm.IsCheckPaymentAtLaneAvailable())
				{
					SMStateBase::nTenderType = TB_TENDER_CHECK_LANE;
					dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                                DM_TRICOLORLIGHT_BLINK_1HZ,
                                DM_TRICOLORLIGHT_NORMALREQUEST);  
					CREATE_AND_DISPENSE(WaitForApproval)(false,false);
				}
				else
				{
					trace (L6, _T(" SMFinishBase::ButtonPressed() with TB_TENDER_CHECK_LANE -- Local check device is not available"));
					CREATE_AND_DISPENSE(SystemMessage1)(	NULL,           // no char *message
									            MSG_CHECK_DEVICE_NOT_AVAILABLE,
									            PS_NOTEXT,		// no prompt
									            0,				// no prompt value
									            -1,				// no device
									            false);
				}
				
			}
			else
				return STATE_NULL;
        }
		break;
	case TB_TENDER_CHECK_LANE_RAP: //check on lane and rap
        {
			CString sDesc, sTenderDesc = GetTenderButton().csTenderText;
			ASSERT(!sTenderDesc.IsEmpty());

			SMStateBase::nTenderType = TB_TENDER_CHECK_LANE_RAP;
			if (!sTenderDesc.IsEmpty())
			{
					dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                                DM_TRICOLORLIGHT_BLINK_1HZ,
                                DM_TRICOLORLIGHT_NORMALREQUEST);  
					CREATE_AND_DISPENSE(WaitForApproval)(false,false);
			}
			else
				return STATE_NULL;
        }
	//End RFC 356020: Support Check payment on lane using printer 7167
	case TB_TENDER_OTHERPAYMENT: //OtherPayment
		{
          EnableHandheldAssistMode(true);
		  SASendSCOTInput( ITEMIZATION );
		  fOtherPayment = true;
		  bIsOtherPayment = true; //tar 222575
		  CREATE_AND_DISPENSE(WaitForApproval)(false,false);
		}

	case TB_TENDER_COUPON: // Coupon
		{
		  //TAR#102019  Coupons allowed in Assist Mode ONLY (at end of purchase)
          EnableHandheldAssistMode(true);
		  if (co.csTenderAllowCoupons == _T("O"))
		  {
			lCouponsInAssist = true;
			CREATE_AND_DISPENSE(WaitForApproval)(false, false);			
		  }
		  else 
		  {
			SASendSCOTInput(TRANSACTION_CONTINUE);
			RETURNSTATE(EnterCoupons)
		  }
		}
	case TB_TENDER_VOUCHERPAYMENT: // Coupon
		{
          EnableHandheldAssistMode(true);
		  trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_BEGINVOUCHERPAYMENT,SCOT_LANGUAGE_PRIMARY));
		  ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ps.GetPSText(RA_BEGINVOUCHERPAYMENT,SCOT_LANGUAGE_PRIMARY));
		  trace(L7, _T("-ra.OnNormalItem"));
		  //TAR#102019  Coupons allowed in Assist Mode ONLY (at end of purchase)
          //TAR 291672 - Vouchers should not be controlled by AllowCoupons setting
		  //if (co.csTenderAllowCoupons == _T("O"))
		  //{
		  //	lCouponsInAssist = true;
		  //	CREATE_AND_DISPENSE(WaitForApproval)(false, false);			
		  //}
		  //else 
		  //{
			RETURNSTATE(VoucherPayment)
		  //}
		}
	case TB_TENDER_GIFTCARD: // Gift Card
	case TB_TENDER_PIP_GIFTCARD: //PIP gift card
		{
			if (co.nOperationsGiftCardRedemption == 1)
			{
				CREATE_AND_DISPENSE(ScanCard)(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType),lBalanceDue);
			}
			else if (co.nOperationsGiftCardRedemption == 2)
			{
				return StandardCardProcessing(buttonindex);
			}
		}
	default:
		return StandardCardProcessing(buttonindex);

	}
}

SMStateBase *SMFinishBase::StandardCardProcessing(int buttonindex)  
{
	fBeyondFinishAndPayState = true;
	if(!(co.fOperationsPartialIntegratedPINPad && !co.TenderList[ps.m_languageUsed][buttonindex-1].bIntegratedMSR))
	{
		//Non- PIP
		if(!cDMLastTrack)   // Card is not swiped
		{
			//ra.OnNormalItem("[highlight]"+ps.GetPSText(RA_BEGINCARDTENDER, SCOT_LANGUAGE_PRIMARY));
			CREATE_AND_DISPENSE(SwipeCard)(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType),lBalanceDue);
		}
		else  // Card is swiped
		{
			DMEncryptorEnable();
			 // fPIPShowCardProcessingScreen = false;

			  // Validate that a master key has been stored inside the encryptor.
			  bool bMasterKeyPresent = true;
			  if (0 != DMEncryptorKeyTest())
			  {
				  bMasterKeyPresent = false;
			  }
			  ResetAllTenderValues(); //new Finalization

            trace(L7, _T("+ra.OnCanSuspendNow, CanSuspend=false"));
			ra.OnCanSuspendNow(false) ;  // un-reversable tender mode. disable suspend
			trace(L7, _T("-ra.OnCanSuspendNow"));
			trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_BEGINCARDTENDER, SCOT_LANGUAGE_PRIMARY));
			ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ps.GetPSText(RA_BEGINCARDTENDER, SCOT_LANGUAGE_PRIMARY));
			trace(L7, _T("-ra.OnNormalItem"));
			//if debit
			if((co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType == TB_TENDER_DEBIT ||
				co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType == TB_TENDER_PIP_DEBIT ||
				co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType == TB_TENDER_PIP_DEBIT_CASHBACK) && TBPromptForDebitAccountType() )
			{
				CREATE_AND_DISPENSE(SelectDebitType)(true);
			}


		  if(co.TenderList[ps.m_languageUsed][buttonindex-1].bNeedPin)
		  {
			CREATE_AND_DISPENSE(EnterPin)(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType),co.TenderList[ps.m_languageUsed][buttonindex-1].bCashBack);
		  }
		
		  if(co.TenderList[ps.m_languageUsed][buttonindex-1].bCashBack)
		  {
				CREATE_AND_DISPENSE(SelectPayment)(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType)); //With cash back
		  }
		  if(co.TenderList[ps.m_languageUsed][buttonindex-1].bConfirmTotal)
		  {
			if( co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType == TB_TENDER_FOODSTAMPS ||
				co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType == TB_TENDER_PIP_FOODSTAMPS )     //FoodStamp
			{
				CREATE_AND_DISPENSE(ConfirmEBTAmount)(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType),lFoodStampsDue,0 );
			}
			else if ( co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType == TB_TENDER_EBT ||
				co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType == TB_TENDER_PIP_EBT )           //Cash Benefit
			{
				CREATE_AND_DISPENSE(ConfirmEBTAmount)(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType),lBalanceDue,0);
			}
			else  //Other cards
			{
				CREATE_AND_DISPENSE(ConfirmDebitAmount)(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType),lBalanceDue);
			}
 		  }

		  return TBEnterTender(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType),lBalanceDue);	
		  
		}
	}
	else
	{
		//PIP
		trace(L7, _T("+ra.OnCanSuspendNow, CanSuspend=false"));
		ra.OnCanSuspendNow(false) ;  // un-reversable tender mode. disable suspend
		trace(L7, _T("-ra.OnCanSuspendNow"));
		trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_BEGINCARDTENDER, SCOT_LANGUAGE_PRIMARY));
		ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ps.GetPSText(RA_BEGINCARDTENDER, SCOT_LANGUAGE_PRIMARY));
		trace(L7, _T("-ra.OnNormalItem"));

		if(co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType == TB_TENDER_PIP_GENERIC)   //PIP_Generic
				return TBEnterTender(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType),lBalanceDue);	
		
		//There is no pinneed option for PIP. The customer need enter PIP at PINPad device

		// cashback
		  	
		  //if(co.TenderList[ps.m_languageUsed][buttonindex-1].bCashBack || co.fOperationsPIPSCOTPromptCashBack )   //need cashback
		if( (co.bReadPIPSCOTPromptCashBack && co.fOperationsPIPSCOTPromptCashBack) || (!co.bReadPIPSCOTPromptCashBack && co.TenderList[ps.m_languageUsed][buttonindex-1].bCashBack ))  
		{
				CREATE_AND_DISPENSE(SelectPayment)(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType)); //With cash back
		  }
		
		  if((co.bReadPIPSCOTPromptConfirmTotal && co.fOperationsPIPSCOTPromptConfirmTotal ) || (!co.bReadPIPSCOTPromptConfirmTotal && co.TenderList[ps.m_languageUsed][buttonindex-1].bConfirmTotal ))   //confirm total
		  {		  
			if( co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType == TB_TENDER_FOODSTAMPS ||
				co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType == TB_TENDER_PIP_FOODSTAMPS )     //FoodStamp
			{
				CREATE_AND_DISPENSE(ConfirmEBTAmount)(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType),lFoodStampsDue,0 );
			}
			else if ( co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType == TB_TENDER_EBT ||
				co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType == TB_TENDER_PIP_EBT )           //Cash Benefit
			{
				CREATE_AND_DISPENSE(ConfirmEBTAmount)(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType),lBalanceDue,0);
			}
			else  //Other cards
			{
				CREATE_AND_DISPENSE(ConfirmDebitAmount)(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType),lBalanceDue);
			}
 		  }
		  else
		  {    // Call PIP EnterTender
 			  if( co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType == TB_TENDER_FOODSTAMPS ||
				co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType == TB_TENDER_PIP_FOODSTAMPS )     //FoodStamp
			  {
					// submit food stamp total instead of balancedue to let customer
					// get change for his/her coupons
					long nSubmitTotal = lFoodStampsDue;
					if( lBalanceDue < lFoodStampsDue) // customer has already paid part of balance off  //Tar 154631
					{
						if (!co.fTenderSubmitFSTtotalIfMorethanTrxTotal)
						{
							nSubmitTotal = lBalanceDue;
						}
					}
					return TBEnterTender(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType),nSubmitTotal);	
			  }
			  else
				  return TBEnterTender(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][buttonindex-1].nTenderType),lBalanceDue);	
		  }
	}

}

/////////////////////////////////
// ConfigureTenderOptions
/////////////////////////////////
void SMFinishBase::ConfigureTenderOptions(void)
{
  //   if Cash allowed set up the aux screen differently
  SCOT_AUX_SCREEN eScreen;

  if (co.fTenderCashAllowed && !m_bCashRecyclerStopDeposit && 
      lBalanceDue > 0)  // TAR 417345:  Only re-enable if a balance is due.
  {
    // pass type of cash in dispenser ";100,500,1000,2000,5000,10000"
    DMCashAcceptorEnable(co.csTenderDenominations);	// Coin Acceptor Modifications
    DMCoinAcceptorEnable();							// Coin Acceptor Modifications
    DMAcceptorLight(DM_MISC_STATE_ONE_HZ);	// blinking light
	DMCoinAcceptorLight(DM_MISC_STATE_ONE_HZ);      // Tar 139796 Coin Acceptor Light
  }


  //start: tar180436
  //On 5994, "Select Payment Type" is displayed even though cash is allowed,
  //because "Insert Cash and Select Payment Type" is too long to fit on aux screen
    eScreen = AUX_SCREEN_SELECT_PAYMENT_TYPE;
  //end: tar180436

  DisplayAuxScreen(eScreen, true);

}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMFinishBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering)
		return FinalizationWtDecreaseNotAllowed(BES_FINISH);
	else
		return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMFinishBase::OnWtIncreaseNotAllowed()	 
{
    return FinalizationWtIncrease(BES_FINISH);	
}

SMStateBase *SMFinishBase::PSConnectRemote( CString& strMachineName, CString& strRemoteContext )
{
    SMStateBase *retState = SMStateBase::PSConnectRemote(strMachineName, strRemoteContext);

    if (retState != STATE_NULL)
    {
        SASendSCOTInput( TRANSACTION_CONTINUE );
        bEASReEnableScanner = true;
        setAnchorState(BES_SCANANDBAG);
    }
    return retState;
}

///////////////////////////////////////////////////////////////////////
// Method    : IsPaymentButtonAvailable
// Parameters: void
// Returns   : bool (true, false)
// Purpose   : A method to check if there is payment button is 
//             display in FinishAndPay screen. 
///////////////////////////////////////////////////////////////////////
//SR742
bool SMFinishBase::IsPaymentButtonAvailable()
{
	CString csCtlName = _T("TenderImage");
	bool bReturnValue = false;
	COleVariant vReturn,v1, v2;

    for (long i = 0; i < MAXBUTTONNUM; i++)
    {	
	   v1 = i;
       m_pAppPSX->SendCommand(csCtlName, UI::COMMANDGETBUTTONVISIBLE, vReturn, 1, v1, v2 );
	   if (vReturn.boolVal == VARIANT_TRUE)
	   {
			return true;
	   }
    }
 return bReturnValue;
}
//SR742-
