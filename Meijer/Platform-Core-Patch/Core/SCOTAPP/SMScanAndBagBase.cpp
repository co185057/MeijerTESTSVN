//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMScanAndBagBase.CPP
//
// TITLE: Class header for Scan and Bag state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMScanAndBag.h"       // This state
#include "SMFinish.h"               // Finish state
#include "SMVoidItem.h"             // Void Items state
#include "SMCheckBasketReminder.h"  // Check Basket Reminder State
#include "SMKeyInCode.h" 
#include "SMGetFoodStampState.h"
#include "SMPickingUpItems.h"
#include "SMSecBagAreaBackup.h"
#include "SMSecUnExpectedIncrease.h"
#include "SMWaitForApproval.h"
#include "MPProcedures.h"			// RFC 276235
#include "SMSmAssistMenu.h"
#include "VolumeControl.h"
#include "SMEnterQuantity.h"
#include "SMProduceFavorites.h"

//begin TAR#118659	LPM092899
#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;   // Global DM object
// end 118659
CString SMScanAndBagBase::csCurContext = EMPTY_STRING;
bool SMScanAndBagBase::bXmlBtnVisibility = false; //TAR 279197

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("ScanAndBag")
DLLEXPORT extern HWND hw;				// RFC 276235
DLLEXPORT extern MPProcedures mp;   // RFC 276235

IMPLEMENT_DYNCREATE(SMScanAndBagBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ScanAndBag)


DEFINE_TIMESTAMP

int SMScanAndBagBase::ms_nNumberOfItems = 0;

////////////////////////////
SMScanAndBagBase::SMScanAndBagBase()
{
  IMPLEMENT_TIMESTAMP
  saMsgFuncPtr =  PSProceduresBase::SecurityMessageNonmodal;
}

//////////////////////////////////////
SMStateBase *SMScanAndBagBase::Initialize(void)
{ 
  g_bMultiPickSkipBaggingAllowed = false;// TAR 398289
  //+SR713
  if(bDoNotShowSellBags && bIsInPreTenderState)
  {	
		return setAndCreateAnchorState(BES_FINISH); //TAR405523
  }
  //-SR713
  
  //TAR 377492 - make sure if we get to Scan and Bag that TBStart has been called
  if (!bIsTransactionStarted)
  {
	  return TBStart();
  }
  //TAR 364905 Begin
  if (co.fOperationsForceDelayedApprovalBeforeSuspend && IsForceDelayedApprovalBeforeSuspend() && !co.IsInAssistMenus())
  {
	  trace(L7, _T("+ra.ForceApprovalBeforeSuspend, ForceApprovalBeforeSuspend=true"));
	  ra.ForceApprovalBeforeSuspend(true);
	  trace(L7, _T("-ra.ForceApprovalBeforeSuspend"));
  }//TAR 364905 End


  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ScanAndBag") );
  g_bReadyForItem = false;//TAR 363023

  bIsMaxCouponLimitInterventionDone = false;	//SR678

  appBusy.SetBusyStatus(co.getTimeOutBusyLeaseTimer());
  EnableHandheldAssistMode(true);

  // TAR 217306 - Cash inserted shouldn't be ignored after security violation.
  if (nDMCashInserted>0 && !bIsInPreTenderState)	// if cash already inserted
  {
      RETURNSTATE(Finish)
  }
  // TAR 251195
  if (bQuietNeedSecApproval)
  {
	  CREATE_AND_DISPENSE(SecUnExpectedIncrease)(BES_SCANANDBAG, BES_SCANANDBAG);
  }

  m_TimerResponse = 0;			//RFC 276235

  if (co.IsImmedInterventionOn() &&
      isAnyApprovalRequired(false))
  {
    CREATE_AND_DISPENSE(WaitForApproval)(false,false);
  }
  // End TAR 251195
  // Rel 3.2 Requirement 4.5.1 - Single Mode RAP 
  // ra.RequestEnableAssistMode(true); // need to move this line down
  
	//SR81 begin
	if (!co.IsInAssistMenus())
	{
		if(co.fOperationsImmediateInterventionForRestrictedItems && isRestrictedItemApprovalRequired())
		{
			CREATE_AND_DISPENSE(WaitForApproval)(false,false);		
		}

		if(co.fOperationsImmediateInterventionForVisualVerifyItems && isVisualItemApprovalRequired())
		{
			CREATE_AND_DISPENSE(WaitForApproval)(false,false);		
		}
	}
	//SR81 end

  setAnchorState(BES_SCANANDBAG);//Tar 202095
  nDeviceErrorContext = 0;  //FZ20021010
  fBeyondFinishAndPayState = false;
  g_bAllowRemove = false;			//tar 194770
  
  //TAR 412492 if TB want to go back to Scan and Bag right after coming from Finish and Pay, send transaction_continue message to Security Manager to make them in sync
  if(bIsInFinalization)
  {
	  SASendSCOTInput( TRANSACTION_CONTINUE );
	  bIsInFinalization = false;		//tar 194770
  }

  g_bMatchedWtWaitingOnEAS = false;
  m_bEASHappened = false;
  
  //+tar 399428
  if(dm.fStateTakeawayBelt)
  {
	  trace(L6, _T("Setting TABstateBeforeTransport= %d in Scan&Bag"),BES_EMPTY_STATE);
	  TABstateBeforeTransport = BES_EMPTY_STATE;
  }
  //-tar 399428
  
  if(bSecMaintMode)					// Tar 214898 RJF 092702 ensure that flag gets turned off while in S&B
      bSecMaintMode = false;

  //Update RAP window with SCOT state. LTT_BELT_WELCOME = "Scan Items" which is generic for TAB or not LPM032802 TAR198779
  UpdateStateName(ps.GetPSText(LTT_BELT_WELCOME, SCOT_LANGUAGE_PRIMARY));
  nSellState = getAnchorState();		  // save current sell entry state

  // Check for bag area backup condition on Takeaway Belt.  If already backed
  // up then go to the BagAreaBackup screen immediately.
  if (dm.fTakeawayBeltBackup && !co.IsSecurityBypassOn())
  {
      RETURNSTATE(SecBagAreaBackup);
  }

  //TAR195100+ CMal/Chu021402
  g_bGLOBALScanAndBagInitializeDone = false;
  
  SMStateBase *sReturnState = CkStateStackAndReturn();
  if (sReturnState != STATE_NULL)
	  return sReturnState;
 
  //TAR195100 CMal/Chu021402 
  if ( g_bGLOBALScanAndBagInitializeDone == true )
	  return STATE_NULL;

  // BeginCodeWLDBSA11
  if(!co.IsInAssistMenus())
  {
	bEASReEnableScanner = true;
	SAWLDBScannerEnable();
  }// enable the scanner
  // EndCodeWLDBSA11

  TBEnterTenderState(false);   //Tar 138312

  //fix screen blinking
  CString csPrevMessage12 = ps.GetCurrentMessageBoxText();
  CString csNewMessage12 = EMPTY_STRING;
  long lMessage12Id = 0;
  trace(L7, _T("lMessageId = %d"),lMessage12Id );


  if ((custInfo.GetOption(_T("PersonalizeLanguage")) == "Y"))
  {
      if ((custInfo.GetPreference(_T("LangSelectionMethod")) != _T("Button")) &&
          (GetPreferredLanguage() != m_customerLanguage))
      {
		  //USSF START
		  USSFHOOK(_T("ChangeLanguage"), ChangeLanguage(GetPreferredLanguage());, ;);
          //ChangeLanguage(GetPreferredLanguage());
		  //USSF END
          PreferencesReceived();
          CString csMessage12;
          CString csScanorKeyin = ps.GetPSText(MSG_KEYBARCODE);
          
          
          CString csDesc = ps.GetPSText(MSG_PERSONALIZATION_IN_EFFECT);
          csMessage12 = csDesc + _T("\n\n\n") + csScanorKeyin;
          
          ps.SetPSText(PS_WORK2, csMessage12);
          
          lMessage12Id = PS_WORK2;
      }
  }

  //fix screen blinking end

  // TAR#126398 save this sell state when from attract LPM111699
  if (nSellState == BES_START) { nSellState = getAnchorState();	}

  ResetAllTenderValues();
  //Tar 112723
  bFinish = false;
  bUpdateFoodStampTotal = true;
  
  trace(L7, _T("+ra.OnSetTransactionState, Itemization State"));
  ra.OnSetTransactionState(RA_STATE_ITEMIZATION);
  trace(L7, _T("-ra.OnSetTransactionState"));


  PSTEXT nBtt3;
  if (bPartialTenderWithFoodStamps && (co.fTenderAllowCancelWithPartialEBTTender == false))
    nBtt3 = PS_NOTEXT;
  else
    nBtt3 = BTT_CANCELITEMS;

  dm.PinPadEnable(); //TAR 121779

  // present either the S&B screen or the Welcome screen


  //Tar 200013 BZ/TK 03/26/02 
  //Scanning loyalty card before any item sold/voided could cause S&B screen lock-up (all buttons were disabled).  
  //To fix this problem, we need to add a check for loyalty card scanned. Therefore, when isAnySoldVoidedItems() 
  //returns false and fItemIsWaiting are false, but io.bLoyaltyCard is true, ps.ShowCMFrame() will still gets called
  //to enable buttons on the screen.  
  if (isAnySoldVoidedItems() || fItemIsWaiting || io.bLoyaltyCard)
  {   
	  //fix screen blinking
	  //Tar 200013 BZ/TK 3/26/02: The original line of code below, "if" statement without brackets, could cause screen
	  //frozen, so we commented it out and moved the check for bIsSysMsgScreenState to the next "if" statement. 
	  //if ((bAlreadyInSnB == false) || (bIsSysMsgScreenState == true)) //tar 185972
	  // actual fix for 195100, as need to make sure that the Window exist,
	  // so must perform this code ONCE per showing this state.


	  // TAR 205214 TK - Fix Screen blinking again
	  // Took out the check for g_bSnBScreenIsShown = true from the if statement below
	  // Since bAlreadyInSnB is what is controlling the blinking 
	  if (bAlreadyInSnB == false || bIsSysMsgScreenState == true) 
	  {
		  InitializeBasedOnOption();	// This hides the frame!!
          if (dm.fStateTakeawayBelt)
          {
		      ps.SetCMLeadthruText(LTT_BELT_WELCOME);
          }
          else
          {
		      ps.SetCMLeadthruText(LTT_WELCOME);
          }
		  //ps.SetButtonText(_T("CMButton3Global"), nBtt3);
	
	//TAR 279197+
		  if (!bXmlBtnVisibility)
		  {
			  COleVariant v;
			  m_pAppPSX->GetConfigProperty(_T("CMButton3Global"), csCurContext, UI::PROPERTYVISIBLE, v);
			  if ((COleVariant)VARIANT_TRUE == v)
			  {
				  bXmlBtnVisibility = true;
				  SetButton(_T("CMButton3Global"), ps.GetPSText(nBtt3), true); // TAR 218915
			  }
			  
		  }
		  else
		  {
			  SetButton(_T("CMButton3Global"), ps.GetPSText(nBtt3), true); // TAR 218915
		  }
	 //TAR 279197-


		  g_bSnBScreenIsShown = true; //TAR195100 CMal/Chu021402 
	  } 
	  	  
	  if (fDMScaleZeroed)
	  {
			// Make Finish button blink unless there's something on the scale
			if (lDMScaleWeight<=0)
			{
				ps.CMButtonFlash(_T("CMButton4LgFinish"),true);
			}
	  }

      //ps.Prompt(TXT_ITEMCODE);  //tar 264140
      // allow the EchoInput() func to accept up to max digits
      ps.AllowInput(MAX_DIGITS_ITEMCODE,false,false);
		
      if (!ps.ShowDelayedMessages())  // show delayed msg or...
	  {
        if (io.d.fDetails)            // if we have details to show, show them
		{
          if (io.nLoyaltyCardScanned)
		  {
			CString csMessage12, csScannedLoyalty;
			CString csScanorKeyin = ps.GetPSText(MSG_SCANORKEYIN);
			   csScanorKeyin = ps.GetPSText(MSG_KEYBARCODE);

			if (io.nLoyaltyCardScanned == 2)
			{
				// loyalty scanned but this is not the first time
				csScannedLoyalty = ps.GetPSText(MSG_SCANNEDLOYALTY_NTHTIME);
                if (co.IsInAssistMenus())
                {
                    SMSmAssistMenu::SetAssistMessage(MSG_RA_LOYALTYCARDINFO2, PS_NOSOUND);
                }
			}
			else
			{
				// loyalty scanned, this is the first time
				csScannedLoyalty = ps.GetPSText(MSG_SCANNEDLOYALTY_FIRSTTIME);
				DMSaySecurity(SEC_NONE, ps.GetSoundName(FREQSHOPACCT));	//TAR 202910
				bAlreadyPlayedLCReminder = true;	//Tar 200719 BZ 04/02/02
                if (co.IsInAssistMenus())
                {
                    SMSmAssistMenu::SetAssistMessage(MSG_RA_LOYALTYCARDINFO2, FREQSHOPACCT);
                }
			}
			io.nLoyaltyCardScanned = 0; // do not play this wave file again
			io.Reset();	//228018 since item object not reset, item code still contains loyalty barcode # LPM021703

            csMessage12 = csScannedLoyalty + _T("\n\n\n") + csScanorKeyin;

            ps.SetPSText(PS_WORK1, csMessage12);
			//ps.Message12(PS_WORK1, false); // clear the image at the background
		    lMessage12Id = PS_WORK1; //fix screen blinking

		  }
		  else  if (io.fVoid) // show the item description and price in the prompt and amount areas
		  {
			//lMessage12Id = MSG_KEYBARCODE; //tar 194600
			io.Reset();
		  }
          else
		  {
          // restricted items and visual items require special treatment, this takes precedence over coupons
            if (((io.lRestCode==1) && (lUnapprovedRestrictedItems > 0)) || ((io.lVisualCode==1) && (lUnapprovedVisualVerifyItems > 0)))
			{
			  //ps.Message12(MSG_RESTWARN, false);
			  lMessage12Id = MSG_RESTWARN;  //fix screen blinking
			}
            else if(((io.lCoupCode==1) || (io.lCoupCode==3)) && !IsUsingCouponSensor())//TAR374217
			{
              if(io.xd.ExceptionCode==XAPPROVALTHRESHOLDS ||
                 io.xd.ExceptionCode==XAPPROVALALWAYS)
			  {
			     //ps.Message12(MSG_RESTWARN, false);
			     lMessage12Id = MSG_RESTWARN;  //fix screen blinking
			  	 DMSayPhrase(SETREFUNDDISCOUNTASIDE);
			  }
			  else
              {
			     //ps.Message12(MSG_CPNREQAPP, false);
				 lMessage12Id = MSG_CPNREQAPP; //fix screen blinking
			  }
			}
            else
			{

				//ps.Message12(MSG_KEYBARCODE, false);
				//lMessage12Id = MSG_KEYBARCODE; //fix screen blinking

			}
		  }
		}
        else if (bTrxLimitExceed)
		{
			// show message that says item will cause trx limit exceed
			CString csAttractText;
			GetTrxLimitString(MSG_ITEMCAUSEDTRXLIMIT, csAttractText);
			ps.SetPSText(PS_WORK1, csAttractText);
			//ps.Message12(PS_WORK1); // clear the image at the background
			lMessage12Id = PS_WORK1; //fix screen blinking
			DMSayPhrase(TRANSACTIONLIMITEXCEEDED);
			bTrxLimitExceed = false;	// let user try to buy other items, don't scare him saying again, "limit exceeded..."
		}
        else
		{

			  //ps.Message12(MSG_KEYBARCODE, false);
			  //lMessage12Id = MSG_KEYBARCODE; //fix screen blinking
		}
	  }
      ps.ShowCMTotal(true ,false);
   }
   else
   {
      ms_nNumberOfItems = 0;
	  //fix screen blinking 
	  if ((bAlreadyInSnB == false) || (bIsSysMsgScreenState == true)) //tar 185972
	  {
		InitializeBasedOnOption();
		if (dm.fStateTakeawayBelt)
		{
			ps.SetCMLeadthruText(LTT_BELT_WELCOME);
		} 
		else
		{
			ps.SetCMLeadthruText(LTT_WELCOME);
		}
		//ps.SetButtonText(_T("CMButton3Global"), BTT_CANCELITEMS);
		
		//TAR 279197+
		if (!bXmlBtnVisibility)
		{
			COleVariant v;
			m_pAppPSX->GetConfigProperty(_T("CMButton3Global"), csCurContext, UI::PROPERTYVISIBLE, v);
			if ((COleVariant)VARIANT_TRUE == v)
			{
				bXmlBtnVisibility = true;
				SetButton(_T("CMButton3Global"), ps.GetPSText(nBtt3), true); //TAR 229500 - Migration from 3.2 to 4.0
			}
			
		}
		else
		{
			SetButton(_T("CMButton3Global"), ps.GetPSText(nBtt3), true); //TAR 229500 - Migration from 3.2 to 4.0
		}
		//TAR 279197-

		// TAR 205214 TK - Fix Screen blinking again	
		g_bSnBScreenIsShown = true;  

	  }

	  // Make Finish button flash unless there's something on the scale	226673 LPM021203
	  if (lDMScaleWeight<=0)
	  {
		ps.CMButtonFlash(_T("CMButton4LgFinish"),true);
	  }
	
	  if (!(io.d.fDetails && (io.nLoyaltyCardScanned == 1)) && !bTrxLimitExceed)
	  {
		if (!bAlreadyPlayedWelcome)
		{
			//Tar 228595 - this wave file should not be interruptible. LPM022103
			//Tar 201860 - playing please scan your item upon first entering state BZ
			trace(L6,_T("DMSayPhrase(WELCOMEPSB)"));	
			DMSayPhrase(WELCOMEPSB); //TAR 202910 - make wave file interruptable with SaySecurity BZ
			bAlreadyPlayedWelcome = true;
		}
	 }

	 if ((io.d.fDetails) && (io.nLoyaltyCardScanned) && (co.fStateAllowLoyaltyCard))
	 {
		CString csMessage12, csScannedLoyalty;
		CString csScanorKeyin = ps.GetPSText(MSG_SCANORKEYIN);
		  csScanorKeyin = ps.GetPSText(MSG_KEYBARCODE);

		if (io.nLoyaltyCardScanned == 2)
		{
			// loyalty scanned but this is not the first time
			csScannedLoyalty = ps.GetPSText(MSG_SCANNEDLOYALTY_NTHTIME);
		}
		else
		{
			// loyalty scanned, this is the first time
			csScannedLoyalty = ps.GetPSText(MSG_SCANNEDLOYALTY_FIRSTTIME);
			DMSaySecurity(SEC_NONE, ps.GetSoundName(FREQSHOPACCT));	//TAR 202910
			bAlreadyPlayedLCReminder = true; //Tar 200719 BZ 04/02/02
			//Tar 198997 BZ 03/21/02 +
			if (!bAlreadyPlayedWelcome)
			{
				DMSaySecurity(SEC_NONE, ps.GetSoundName(WELCOMEPSB));  //TAR 202910
				bAlreadyPlayedWelcome = true;
			}
			//Tar 198997 BZ 03/21/02 -

		}
		io.nLoyaltyCardScanned = 0; 
		csMessage12 = csScannedLoyalty + _T("\n\n\n") + csScanorKeyin;
		ps.SetPSText(PS_WORK1, csMessage12);
		//ps.Message12(PS_WORK1, false); // clear the image at the background
		lMessage12Id = PS_WORK1; //fix screen blinking
		//bAlreadyPlayedWelcome = true;	//Tar 198997 BZ 03/21/02 
	 }
	 else if (bTrxLimitExceed)
     {
		//if the transaction limit is exceeded with the first item, we go thru this way
	    // show message that says item will cause trx limit exceed
	    CString csAttractText;
	    GetTrxLimitString(MSG_ITEMCAUSEDTRXLIMIT, csAttractText);
	    ps.SetPSText(PS_WORK1, csAttractText);
	    //ps.Message12(PS_WORK1); // clear the image at the background
	    lMessage12Id =PS_WORK1; //fix screen blinking
		DMSayPhrase(TRANSACTIONLIMITEXCEEDED);
        // let user try to buy other items, don't scare him saying again, "limit exceeded..."
	    bTrxLimitExceed = false;	
		bAlreadyPlayedWelcome = true;
     }
	 else  if (io.fVoid) //if canceled item
     {
         //lMessage12Id = MSG_KEYBARCODE; //tar 194600
	     io.Reset();
     }
	 else
	 {

			//ps.Message12(MSG_KEYBARCODE, false);
			 //lMessage12Id = MSG_KEYBARCODE; //fix screen blinking
		
	 }

     // allow the EchoInput() func to accept up to max digits
	 ps.ShowCMTotal(true, false); 
     ps.AllowInput(MAX_DIGITS_ITEMCODE,false,false);

  }

  //Tar 200860 BZ 04/03/02 The value of lTrxTotalRewardPoints will be shown only if 
  //a loyalty card has been scanned or swiped.
  //This tar doesn't make any sense.  Reward Point is coming from TB and the app doesn't decide based on any condition.
//  if (fLoyaltyCardTrx)
	ps.ShowCMRewardInfoBox(lTrxTotalSavings, lTrxTotalRewardPoints, CustomerMessage);
//  else
//    ps.ShowCMRewardInfoBox(lTrxTotalSavings, 0, CustomerMessage);

	
  // if the customer scanned an item from Attract or FTU or
  // a weight items was re-sold with tare, etc... the waiting
  // flag will be true
  // check for any pending item sale 
  if (fItemIsWaiting && !GetISIPTimeOut())
  {
    if (!csDMLastBarCode.IsEmpty())
	{
		trace(L3, _T("Processing pending item sale: %s"), csDMLastBarCode);
		return TBItemSale(false,0,true);
	}
	else
	{
		// this should never happen unless somebody corrupts global var: csDMLastBarCode
		trace(L3, _T("Ignoring pending item sale"));
		fItemIsWaiting = false;
	}
  }
	
  //tar 183854
  if ((bIsScaleBroken == true) && !GetISIPTimeOut() && io.csItemCode.GetLength()>0) 
  {
	SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
  }
  bIsScaleBroken = false;

  trace(L6,_T("co.fStateLoyaltyCardReminderAfterFirstItemSold=%d, !io.nLoyaltyCardScanned =%d, lItems = %d, lVoidedItems=%d"), co.fStateLoyaltyCardReminderAfterFirstItemSold, !io.nLoyaltyCardScanned, lItems, lVoidedItems);
	 //LoyaltyCardReminderAfterFirstItemSold
     // TAR 195033 if Loyalty Card is not allowed, don't play the wave file
    if (co.fStateAllowLoyaltyCard && co.fStateLoyaltyCardReminderAfterFirstItemSold && !io.bLoyaltyCard && m_lItemsScanned == 1 && lVoidedItems == 0)  //TAR 202845 Make sure it is first item scanned 
	{
		if (!bAlreadyPlayedWelcome)	//Tar193234 BZ 
		{
            if (dm.fStateTakeawayBelt)
            {
			    trace(L6,_T("DMSaySecurity(SEC_NONE, ps.GetSoundName(LOYALTYCARDREMINDERAFTERFIRSTITEMSOLD_BELT))"));
			    DMSaySecurity(SEC_NONE, ps.GetSoundName(LOYALTYCARDREMINDERAFTERFIRSTITEMSOLD_BELT));  //TAR 202910
				bAlreadyPlayedWelcome = true;
				bAlreadyPlayedLCReminder = true; //Tar 200719 BZ 04/02/02
            }
            else
            {
			    trace(L6,_T("DMSaySecurity(SEC_NONE, ps.GetSoundName(LOYALTYCARDREMINDERAFTERFIRSTITEMSOLD))"));
			    DMSaySecurity(SEC_NONE, ps.GetSoundName(LOYALTYCARDREMINDERAFTERFIRSTITEMSOLD));	//TAR 202910
				bAlreadyPlayedWelcome = true;	
				bAlreadyPlayedLCReminder = true; //Tar 200719 BZ 04/02/02
            }
		}
		else
		{
			if (!bAlreadyPlayedLCReminder)		//Tar 200719 BZ 04/02/02
			{	
				trace(L6,_T("DMSaySecurity(SEC_NONE, ps.GetSoundName(FREQUENTSHOPPERCARD))"));
				DMSaySecurity(SEC_NONE, ps.GetSoundName(FREQUENTSHOPPERCARD));	//TAR 202910
				bAlreadyPlayedLCReminder = true;	 //Tar 200719 BZ 04/02/02
			}
		}
	}
	else   //Tar 197714 BZ 04/01/02 Play "Welcome, please scan your next item" after scanning item on attract
	{	
		if (!co.fStateAllowLoyaltyCard && !co.fStateLoyaltyCardReminderAfterFirstItemSold && !io.bLoyaltyCard && m_lItemsScanned == 1 && lVoidedItems == 0)  //TAR 202845 Make sure it is the first item scanned
		{
			if (!bAlreadyPlayedWelcome)	
			{
                // TAR 233716 - Don't play scan next item wave file  DEM
				bAlreadyPlayedWelcome = true;	//TAR 202580
			}
		}
	}

  // if there's no card in, enable MSR 
  if (!cDMLastTrack && !waitingForCardRemoval)
  {
    DMMSREnable();	// enable the MSR, turn light on
  }
  else  // Tar 149414
  {
	DMMSRDisable();
	// HFE determined to use separate audio and text for MCR LPM102302 215064
	if (co.fTenderIsMotorizedMSR )
	  lMessage12Id = MSG_CARDACCEPTED_SCANITEMS;
	else
	  lMessage12Id = MSG_CARDSWIPED_SCANITEMS;
  }

	
  if (nTBLastRealState == TB_VOIDITEMBEFORECOUPON)
  {
	//ps.Message12(MSG_CANCELITEMCPN, false);
	DMSayPhrase(SECURITYMED);
	lMessage12Id = MSG_CANCELITEMCPN; //fix screen blinking
  }	  
  // if items have been sold...
  // and no authorizatino is required...
  // ano no card is already inserted...
  // enable the cash acceptor
  if ( isAnySoldVoidedItems() && !co.IsInAssistMenus())
  {
    //if ( !isAnyApprovalRequired() )
    if (!cDMLastTrack)
    {
      // pass type of cash in dispenser ";100,500,1000,2000,5000,10000"
      DMCashAcceptorEnable(co.csTenderDenominations);	// Coin Acceptor Modifications
      DMCoinAcceptorEnable();							// Coin Acceptor Modifications
      DMAcceptorLight(DM_MISC_STATE_ON);
	  DMCoinAcceptorLight(DM_MISC_STATE_ON);            // Tar 139796 Coin Acceptor Light
    }
  }

  //TAR#125453 fDMScaleZeroed will now be false if fSSWtFromAttract is true, so need 
  //to OR those, instead of AND
  if ((fDMScaleZeroed || fSSWtFromAttract) &&
      lDMScaleWeight>0)   //Flash the "LookUpItem" button if there is an item on the scale --Feng2
  {
	  DMSayPhrase(PURCHASEPRODUCE);
	  fDMScaleZeroed = false;
	  FlashButtonsOnWeight(true);

	  ps.CMButtonFlash(_T("CMButton4LgFinish"),false);

      fSSWtFromAttract = false;		// TAR123172 only used if scan scale wt increase from attract screen LPM101499
  }
	
  if  ( (short(lTotalPaid)==(nDMCashInDrawer+nDMCashInserted)) && isAnySoldVoidedItems() ) //tar 376458 & 376815
  {
	/* TAR 346592 begin */
	if (TBIsAllowed(TB_FT_SUSPEND)){//TB allows Suspend Button to be enabled so App can too
		trace(L7, _T("+ra.OnCanSuspendNow, CanSuspend=true"));
		ra.OnCanSuspendNow(true) ;  // If it is not partially paid with other(credit, ATM or EBT payment)
		trace(L7, _T("-ra.OnCanSuspendNow"));
	}
	else{//TB diabled Suspend Button so App can not enable it
		trace(L7, _T("+ra.OnCanSuspendNow, CanSuspend=false"));
		ra.OnCanSuspendNow(false) ;
		trace(L7, _T("-ra.OnCanSuspendNow"));
	}
	  
	//trace(L7, _T("+ra.OnCanSuspendNow, CanSuspend=true"));
	//ra.OnCanSuspendNow(true) ;  // If it is not partially paid with other(credit, ATM or EBT payment)
	//trace(L7, _T("-ra.OnCanSuspendNow"));
	/* TAR 346592 end */
  }

  COleVariant v;
  trace(L7, _T("lMessageId & csCurContext= %d %s"),lMessage12Id, csCurContext );
  csNewMessage12 = ps.GetPSText(lMessage12Id);
  if(!csNewMessage12.IsEmpty())
  {
	  m_pAppPSX->SetConfigProperty( _T("ScanBagVideo"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );
	  m_pAppPSX->SetConfigProperty( _T("ScanBagAVIMessageModalBackGround"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );
	  m_pAppPSX->SetConfigProperty( _T("ScanBagMessageModalBackGround"), csCurContext,UI::PROPERTYVISIBLE, VARIANT_TRUE );
      m_pAppPSX->SetConfigProperty( _T("ScanBagTextArea"), csCurContext,UI::PROPERTYVISIBLE, VARIANT_TRUE );
	  v = ps.ConvertToEscapeSequence(csNewMessage12, _T("\\n"), _T("\n"));
	  m_pAppPSX->SetConfigProperty(_T("ScanBagTextArea"), csCurContext, UI::PROPERTYTEXTFORMAT, v);
  }
  else
  {
	  m_pAppPSX->SetConfigProperty( _T("ScanBagMessageModalBackGround"), csCurContext,UI::PROPERTYVISIBLE, VARIANT_FALSE );
	  m_pAppPSX->SetConfigProperty( _T("ScanBagTextArea"),csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );
	  v = _T("");
	  m_pAppPSX->SetConfigProperty(_T("ScanBagTextArea"), csCurContext, UI::PROPERTYTEXTFORMAT, v);
	  m_pAppPSX->SetConfigProperty( _T("ScanBagAVIMessageModalBackGround"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE );
   	  m_pAppPSX->SetConfigProperty( _T("ScanBagVideo"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE );
  }

  //+tar 275064
  if( IsMINIHardware() )
  {
    if( lTotalSale < 0 )
    {
        m_pAppPSX->SetConfigProperty( _T("CMButton4LgFinish"), csCurContext, UI::PROPERTYSTATE, ( long ) UI::Disabled);
   	    DMMSRDisable();
    }
    else
    {
        m_pAppPSX->SetConfigProperty( _T("CMButton4LgFinish"), csCurContext, UI::PROPERTYSTATE, ( long )UI::Normal );
    }
  }
  //-tar 275064

// Rel 4.0 Req. 6.5 Change Language During Transaction-->
  if(co.fStateDualLanguage && 
    (co.fStateAllowLanguageSelectionDuringTransaction ||
    (custInfo.GetOption(_T("PersonalizeLanguage")) == "Y")))
  {
     CString csButtonText;

	  //set button text
	 if(co.m_iNumberOfLanguagesSupported > 2)
	 {
		 ps.SetPSText(PS_WORK1, ps.GetPSText(BTT_LANGUAGE, SCOT_LANGUAGE_PRIMARY));
	 }
	 else if(co.m_iNumberOfLanguagesSupported == 2)
	 {
		 int i = SCOT_LANGUAGE_PRIMARY; //tar 389302/388448
		 int iButtonID;
		 
		 if(m_customerLanguage == SCOT_LANGUAGE_PRIMARY)
		 {
			 i = SCOT_LANGUAGE_SECONDARY;
		 }
		 else if(m_customerLanguage != SCOT_LANGUAGE_SECONDARY)//tar 389302/388448
		 {
			 //fix TAR388448+
			 trace(L2, _T("WARNING: Unexpected value %d for m_customerLanguage;reset to primary language"), m_customerLanguage); 
			 m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
			 //fix TAR388448-
			  //i = SCOT_LANGUAGE_PRIMARY;
		 }
		 
		/* comment out TAR388448+
		 else
		 {
			 ASSERT(FALSE); // shouldn't enter here
		 } 
		 comment out TAR388448- */ 

		 //Get the language name text for button
		 csButtonText = co.language[i].csName;
		 if(_istdigit(*csButtonText))
		 {
			 iButtonID = _ttoi(co.language[i].csName);
			 csButtonText = ps.GetPSText(iButtonID, (SCOT_LANGUAGE_TYPE)i);
		 }
		 ps.SetPSText(PS_WORK1, csButtonText);
	 }
	 else
	 {
		 ASSERT(FALSE); // shouldn't enter here because at least 2 languages should be supported
	 }

	ps.SetButtonText(_T("CMButton5MidiList"),PS_WORK1);
	//make visible the CMButton5MidiList
	m_pAppPSX->SetConfigProperty(_T("CMButton5MidiList"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);

  }
 

// <-- Change Language During Transaction

  csSavedSecMessage = EMPTY_STRING;
  bIsSysMsgScreenState = false; //tar 185972
  //fix screen blinking end

  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  //Need to restore POS Message in RAP by sending it over again
  trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_POSMESSAGE, SCOT_LANGUAGE_PRIMARY));
  ra.OnNormalItem(_T("[Alert]") + ps.GetPSText(RA_POSMESSAGE, SCOT_LANGUAGE_PRIMARY) + _T(" ") + csTBMessage2Scot);
  trace(L7, _T("-ra.OnNormalItem"));
  g_bGLOBALScanAndBagInitializeDone = true; //TAR195100 CMal/Chu021402

  if (co.IsInAssistMenus())
  {
	  ra.RequestEnableAssistMode(false);	//TAR356813
	  RETURNSTATE(SmAssistMenu)
  }
  // Rel 3.2 Requirement 4.5.1 - Single Mode RAP 
  ra.RequestEnableAssistMode(true); 

  if ((bAlreadyInSnB == false) || (bIsSysMsgScreenState == true)) 
  {
	  ps.ShowCMFrame();
	  //+TAR#295482 - workaround to fix the tar
	  //ps.ScrollDown(_T("SMReceipt"));
      //ps.ScrollDown(_T("CMReceipt"));
      //ps.ScrollDown(_T("CMReceiptVoid"));
	  //-TAR#295482 - workaround to fix the tar
  }
  else
  {
	  trace(L6, _T("Not showing screen because Already In SnB = %d, and Is Sys Msg Screen State = %d."), bAlreadyInSnB, bIsSysMsgScreenState);
  }

  if(co.fAutoRunOn)
  {
      nTimeOut = co.nAutoRunItemPurchaseDelay > 0 ? co.nAutoRunItemPurchaseDelay : 1;

	  //TAR406698
	  if(co.fAutoRunRandomTimer)
	  {
		  //+RFC375570
		  if (nTimeOut > 1) 
		  {		  
			  nTimeOut = 1 + ( rand() % co.nAutoRunItemPurchaseDelay );

		  }//-RFC375570
	  }//-406698
	  trace(L6, _T("AutoRunItemPurchaseDelay = %d"), nTimeOut );
  }

  return STATE_NULL;                // stay in this state
}

/////////////////////////////////////
void SMScanAndBagBase::UnInitialize(void)
{
	if(m_TimerResponse != 0) //RFC 276235
   {
	  KillTimer(hw, SCANSCALESB);
	  m_TimerResponse = 0;
  
   }
	//TAR195100 CMal/Chu021402 
	if (g_bGLOBALScanAndBagInitializeDone == false ) //if never init, don't uninit
		return;

//    g_bSnBScreenIsShown = false;  //TAR195100 CMal/Chu021402 
	// App cann't afford to leave the devices enabled even if it improves performance somehow
	// The other states behaviour  on unpexpected device inputs leads to
	// many unknown problems, for example: TAR: 149100, 142740
	if ( (!io.csOrigCode.IsEmpty() && io.csOrigCode[0]!=LINK_ITEM && io.csOrigCode[0]!=INFO_ITEM) || 
		 (nSAPrevSCOTInputID == UPC_SCANNED) || (nSAPrevSCOTInputID == CANCEL_ITEM) ||
		 io.csOrigCode.IsEmpty() )
	{
		SAWLDBScannerDisable();		// disable the scanner
	}

	DMCashAcceptorDisable();	// disable acceptor, turn light off	// Coin Acceptor Modifications
	if(BES_FINISH != getAnchorState()) //tar256688	
	{
		/* Tar 376685 - take out fix for 313901
		// TAR 355933: Only do an immediate disable for Glory recycler.
		if(dm.m_csCashRecyclerModel != _CASHDISPENSER_GLORY)	// TAR 313901
		{
			DMCoinAcceptorDisable();	// disable acceptor, turn light off	// Coin Acceptor Modifications
		}
		else
		{
			CString csDeposit;
			dm.CashRecyclerStopDeposit(csDeposit);				// TAR 313901
		}
		*/
		DMCoinAcceptorDisable();	// disable acceptor, turn light off	// Coin Acceptor Modifications

	}

	if (!cDMLastTrack && !waitingForCardRemoval)	// if no MSR track data, card not swiped or inserted yet.  Changed for PR 022598-8
	{
		DMMSRDisable();			// if no card inserted, disable
	}
	else						// there is track data, card was swiped or inserted.
	{
		DMMSRLight(DM_MISC_STATE_OFF);    // turn off MEI if there is any track data 
		if(!co.fTenderIsMotorizedMSR)	// if we're using a swipe reader
		{
			DMMSRDisable();         // Disable reader to prevent "Can't swipe card now" msgs
		}
	}

	FlashButtonsOnWeight(false);     //stop flashing buttons to make sure flash button timer stays in sync
	g_bGLOBALScanAndBagInitializeDone = false;	//TAR195100 CMal/Chu021402 
}

///////////////////////////////////////////
// PSButton1 - Lookup Item
///////////////////////////////////////////
SMStateBase *SMScanAndBagBase::PSButton1(void)
{
	// Tell security that we are exiting form scan& bag state
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
    if( co.fStateAllowLookUpItem )
    {
		if (co.fOperationsAllowMultiPick)
		{
			fInMultiSelectPickList = true;
			ps.SetPicklistItemsSold(0); //tar383390
			m_pAppPSX->ClearReceipt(_T("PickListReceipt")); 
			m_pAppPSX->ClearReceipt(_T("PickListReceiptVoid"));
		}
		RETURNSTATE(ProduceFavorites)
    }
    else
    {
       trace(L2, _T("SMScanAndBagBase::PSButton1 ignoring button press for unconfigured button") );
       return STATE_NULL;
    }
}




///////////////////////////////////////////
// PSButton2 - Key In Code
///////////////////////////////////////////
SMStateBase *SMScanAndBagBase::PSButton2(void)
{
	 // Tell security that we are exiting form scan& bag state
	 TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
    if( co.fStateAllowKeyInCode )
    {
        return setAndCreateAnchorState(BES_KEYINCODE);
    }
    else
    {
       trace(L2, _T("SMScanAndBagBase::PSButton2 ignoring button press for unconfigured button") );
       return STATE_NULL;
    }
}

//////////////////////////////////////
// PSButton3 - Cancel Items
//////////////////////////////////////
SMStateBase *SMScanAndBagBase::PSButton3(void)
{
  // Tell security that we are exiting form scan& bag state
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
  // TAR#120988 Unknown item approval on RAP side needs to reset the globals to zero  LPM111299
 /* SCOTDO_APPROVAL_INFO ApprovalInfo;
  ra.GetApprovalInfo(&ApprovalInfo);
  if ((lUnknownItemsKeyed > 2) && (!ApprovalInfo.bKeyedUnknown))
  {
	lUnknownItemsKeyed = 0;
	lUnknownItems = 0;
  }*/

  //*** NewUI ***//
  CString csItemEntryId = ps.GetSelection(_T("CMReceipt"));
  if (csItemEntryId.GetLength())
    RETURNSTATE(VoidItem)				  // void items

  return setAndCreateAnchorState(BES_FINISH); // default time out will be used.
}

//////////////////////////////////////
// PSButton4 - Finish
//////////////////////////////////////
SMStateBase *SMScanAndBagBase::PSButton4(void)
{
	// Tell security that we are exiting form scan& bag state
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
	lEOT_LGW = g_lLGW; //tar 194770
	// TAR#120988 Unknown item approval on RAP side needs to reset the globals to zero  LPM111299
/*	SCOTDO_APPROVAL_INFO ApprovalInfo;
	ra.GetApprovalInfo(&ApprovalInfo);
	if ((lUnknownItemsKeyed > 2) && (!ApprovalInfo.bKeyedUnknown))
	{
		lUnknownItemsKeyed = 0;
		lUnknownItems = 0;
	}*/
    //+SR713
    /*if(co.fStateCheckBasketReminder && lItems)
		RETURNSTATE(CheckBasketReminder)
	else
	{
		bIsInFinalization = true; //tar 223051
		return setAndCreateAnchorState(BES_FINISH);
	}*/
	bIsInPreTenderState = true;	
	return setAndCreateAnchorState(BES_FINISH);
	//-SR713
}


//////////////////////////////////////
// PSButton5 - Language selection button
//////////////////////////////////////
SMStateBase *SMScanAndBagBase::PSButton5(void)
{
	
    if(co.m_iNumberOfLanguagesSupported > 2)
	{
		// Tell security that we are exiting form scan& bag state
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );

		return setAndCreateAnchorState(BES_LANGUAGESELECT);
	}
	else if(co.m_iNumberOfLanguagesSupported == 2)
	{
	
		if(m_customerLanguage == SCOT_LANGUAGE_PRIMARY)
		{
			m_customerLanguage = SCOT_LANGUAGE_SECONDARY;
		}
		else
		{
			m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
		}
        SetPreferredLanguage(m_customerLanguage, _T("Button") );
		//USSF START
		USSFHOOK(_T("ChangeLanguage"), ChangeLanguage(m_customerLanguage);, ;);
		//ChangeLanguage(m_customerLanguage);
		//USSF END
		SendTBCommand(TBCMD_CUSTOMERLANGUAGE, co.language[m_customerLanguage].szCode);
		
		Initialize(); // refresh the screen
		
		return STATE_NULL;
	}
	else
	{
		ASSERT(FALSE); // shouldn't enter here
		return STATE_NULL;
	}
}

////////////////////////////////////////////////////
// PSNumericKey
////////////////////////////////////////////////////
SMStateBase *SMScanAndBagBase::PSNumericKey(const long lKey)
{
  PSEchoInput(lKey);
  return STATE_NULL;
}

///////////////////////////////////////
// PSEnterKey - sell item entered
///////////////////////////////////////
SMStateBase *SMScanAndBagBase::PSEnterKey(void)
{
  long lLen = ps.GetInput(csItemSent);
	
  if (lLen==0)
  {
    DMSayPhrase(KEYPADINVALID);
    return PSClearAllInput(FALSE);
  }
  // Tell security that we are exiting form scan& bag state
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
	
  FlashButtonsOnWeight(false);

  ps.CMButtonFlash(_T("CMButton4LgFinish"),false);

  DMSayPhrase(KEYPADVALID);
  return TBItemSale(false,0,false);
}

///////////////////////////////////////
// PSClearKey
///////////////////////////////////////
SMStateBase *SMScanAndBagBase::PSClearKey(void)
{
  PSClearInput();
  return STATE_NULL;
}

//////////////////////////////////////
// DMScanner - something scanned
//////////////////////////////////////
SMStateBase *SMScanAndBagBase::DMScanner(void)
{
  // Tell security that we are exiting form scan& bag state
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );

  // no need for TAB specific code here, removed ss.SetLGW LPM081902 TAR211049
  FlashButtonsOnWeight(false);

  ps.CMButtonFlash(_T("CMButton4LgFinish"),false);

  csItemSent = csDMLastBarCode;
  return TBItemSale(false,0,true);
}

/////////////////////////////////////////
// DMCardReader - card inserted
/////////////////////////////////////////
SMStateBase *SMScanAndBagBase::DMCardReader(void)
{
  lEOT_LGW = g_lLGW; //tar 194770
  // TAR#120988 Unknown item approval on RAP side needs to reset the globals to zero  LPM111299
  /*SCOTDO_APPROVAL_INFO ApprovalInfo;
  ra.GetApprovalInfo(&ApprovalInfo);
  if ((lUnknownItemsKeyed > 2) && (!ApprovalInfo.bKeyedUnknown))
  {
	lUnknownItemsKeyed = 0;
	lUnknownItems = 0;
  }*/

  if (lItems)	  // inserting a card here signifies transitioning to finish
  {
	//+SR713
	/*if(co.fStateCheckBasketReminder)
	{
		// Tell security that we are exiting form scan& bag state
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
		RETURNSTATE(CheckBasketReminder)
	}
	else
	{
		// Tell security that we are exiting form scan& bag state
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
		return setAndCreateAnchorState(BES_FINISH);
	}*/

	
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
    bIsInPreTenderState = true;
	return setAndCreateAnchorState(BES_FINISH);
	//-SR713
  }

  DisplayAuxScreen(AUX_SCREEN_SB_PREVSWIPE, true);

  // HFE determined to use separate audio and text for MCR LPM102302 215064
  CString csWork;
  if (co.fTenderIsMotorizedMSR)
  {
	DMSayPhrase(CARDACCEPTEDPLSCONTINUE);
	csWork = ps.GetPSText(MSG_CARDACCEPTED_SCANITEMS); 
  }
  else
  {
	DMSayPhrase(CARDSWIPEDPLEASECONTINUE);
	csWork = ps.GetPSText(MSG_CARDSWIPED_SCANITEMS);
  }
  trace(L7, _T("csWork & csCurContext= %s %s"),csWork, csCurContext );

  COleVariant v;
  m_pAppPSX->SetConfigProperty( _T("ScanBagVideo"),csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );
  m_pAppPSX->SetConfigProperty( _T("ScanBagAVIMessageModalBackGround"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );
  m_pAppPSX->SetConfigProperty( _T("ScanBagMessageModalBackGround"), csCurContext,UI::PROPERTYVISIBLE, VARIANT_TRUE );
  m_pAppPSX->SetConfigProperty( _T("ScanBagTextArea"),csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE );
  v = ps.ConvertToEscapeSequence(csWork, _T("\\n"), _T("\n"));
  m_pAppPSX->SetConfigProperty(_T("ScanBagTextArea"), csCurContext, UI::PROPERTYTEXTFORMAT, v);

  DMMSRDisable();  //TAR 275460

  return STATE_NULL;
}

///////////////////////////////////////
// DMAcceptor - cash inserted
///////////////////////////////////////
SMStateBase *SMScanAndBagBase::DMAcceptor(void)
{
  trace(L6,_T("DMAcceptor"));

  // +TAR 411550
  if(lBalanceDue <= 0)
  {
      return SMStateBase::DMAcceptor();     // Reject.
  }
  // -TAR 411550

  // Tell security that we are exiting form scan& bag state
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
  lEOT_LGW = g_lLGW; //tar 194770
	// TAR#120988 Unknown item approval on RAP side needs to reset the globals to zero  LPM111299
 /* SCOTDO_APPROVAL_INFO ApprovalInfo;
  ra.GetApprovalInfo(&ApprovalInfo);
  if ((lUnknownItemsKeyed > 2) && (!ApprovalInfo.bKeyedUnknown))
  {
	lUnknownItemsKeyed = 0;
	lUnknownItems = 0;
  }*/

  ps.HideButtons();
  //+SR713
  //if(co.fStateCheckBasketReminder)
  //	  RETURNSTATE(CheckBasketReminder)
  //else
  bIsInPreTenderState = true;
  return setAndCreateAnchorState(BES_FINISH);
   //-SR713
}


///////////////////////////////////////
// DMScale - something placed on scale
///////////////////////////////////////
SMStateBase *SMScanAndBagBase::DMScale(void)
{
  trace(L6,_T("+Scale"));
	
  // if we must select tare, don't prime virtual scale,
  // we must wait for the ENTER WEIGHT prompt to test
  // if we need to select a tare, weights will be keyed in
  // if not tare, then we can prime the virtual scale
  // update the on-screen weight.
  // if the current state is frozen, i.e. assist mode
  // then prime the scale now

	  //RFC 276235 start
  int ntimeoutvalue = co.getTimeOutScannerScaleOnAttract() ;
  SMStateBase *sExit = SMStateBase::DMScale();
  if (sExit!=STATE_NULL)
    return sExit;
	
  if (fDMScaleZeroed)
  {
	  if (lDMScaleWeight > 0 && m_TimerResponse == 0)
	  {
		  m_TimerResponse = SetTimer(hw, SCANSCALESB, ntimeoutvalue, &ScanScaleSBTimedOut);
		  trace(L6, _T("::DMScale-Start Timer from weight increase"));
	  }
	  if ((lDMScaleWeight == 0) && (m_TimerResponse != 0))
	  {
		  KillTimer(hw, SCANSCALESB);
		  m_TimerResponse = 0;
		  
	  }
	  else if ((lDMScaleWeight > 0) && (m_TimerResponse != 0)) 
	  {
		  KillTimer(hw, SCANSCALESB);
		  m_TimerResponse = 0;
		  if (lDMScaleWeight != 0)
		  {
			  //Now restart Timer
			  m_TimerResponse = SetTimer(hw, SCANSCALESB, ntimeoutvalue, &ScanScaleSBTimedOut);
		  }
	  }

	  if (lDMScaleWeight <= 0)
	{
		FlashButtonsOnWeight(false);	// turn off flash on keyin and lookup item
		//if ( isAnySoldVoidedItems() || fItemIsWaiting )	//226673 if the scanner scale wt returns to zero, then flash the finish and pay btn LPM021203 (UITS 94 rel 3.1)
		ps.CMButtonFlash(_T("CMButton4LgFinish"),true);
	}
  }

  trace(L6,_T("-Scale"));
  return STATE_NULL;
}

///////////////////////////////////////////
// InitializeBasedOnOption
///////////////////////////////////////////
void SMScanAndBagBase::InitializeBasedOnOption(void)
{
	 if (co.fCMRewardInfo)
	 {
		 if (co.fOperationsDisplayQuickPickItems)
			csCurContext = _T("QuickPickItemsWithReward");
		 else
			csCurContext = _T("ScanAndBagWithReward");
	 }
	 else
	 {
		 if (co.fOperationsDisplayQuickPickItems)
			csCurContext = _T("QuickPickItems");
		 else
			csCurContext = _T("ScanAndBag");
	 }
	 ps.SetCMFrame(csCurContext);

	 if (co.fOperationsDisplayQuickPickItems)
	 {
		 ps.DisplayQuickPickItem();
	 }
	 if(!co.fStateAllowLookUpItem)
		 ps.SetButtonText(_T("CMButton1LookUp"), PS_NOTEXT);
	 if(!co.fStateAllowKeyInCode)
		 ps.SetButtonText(_T("CMButton2KeyInCode"), PS_NOTEXT);

}

///////////////////////////////////////////
// FlashButtonsOnWeight
// Flash the Look Up Item button, the Key In Code button, or both (depending on options)
// if the input parameter is true, stop flashing them if its false.
///////////////////////////////////////////
void SMScanAndBagBase::FlashButtonsOnWeight(bool bFlashOn)
{

    if (co.fStateAllowLookUpItem && co.fStateFlashLookupItemButton)	//TAR 173138/9
       ps.CMButtonFlash(_T("CMButton1LookUp"), bFlashOn);
    if (co.fStateAllowKeyInCode && co.fStateFlashKeyInCodeButton)	//TAR 173138/9
       ps.CMButtonFlash(_T("CMButton2KeyInCode"), bFlashOn);

}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMScanAndBagBase::OnWtDecrease()
{
	// Tell security that we are exiting form scan& bag state
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
	// TAR 293493 - If in attendent mode and get the here clear the violation
	if (co.IsSecurityBypassOn()	)
	{
		OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR);
	}
	else
	{
		//BZ 06/07/02 TAR 203537: PickingUpItems state should still occur even when RemoteItemButton=N
		RETURNSTATE(PickingUpItems)
	}
    return STATE_NULL;
}

///////////////////////////////////////////////
SMStateBase* SMScanAndBagBase::DMTakeawayBagAreaBackup(bool isBackedUp)
{
    SMStateBase* baseNextState = SMStateBase::DMTakeawayBagAreaBackup(isBackedUp);
    if (isBackedUp)
    {
        RETURNSTATE(SecBagAreaBackup);
    }
    else
    {
        return baseNextState;
    }
}

/////////////////////////////////////////////////////////////////////////
// RFC 213876
SMStateBase* SMScanAndBagBase::OnWtIncreaseNotAllowed()
{
	// Tell security that we are exiting form scan& bag state
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
	return SMStateBase::OnWtIncreaseNotAllowed();
}

SMStateBase  *SMScanAndBagBase::TimedOut(void)
{
  // Tell security that we are exiting form scan& bag state
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );

  if(!co.fAutoRunOn)
  {
      return SMStateBase::TimedOut();
  }
  if(ms_nNumberOfItems < co.nAutoRunNumberOfItemsToSell)
  {
    csItemSent = co.csAutoRunItemCodeToSell;
    ms_nNumberOfItems++;
    return TBItemSale(false, 0, false);
  }
  else
  {
    nDMCashInserted = co.nAutoRunTotalCashToPay;
	return DMAcceptor();
  }
}

SMStateBase *SMScanAndBagBase::OnMatchedWt()
{
	trace(L2, _T("SMScanAndBagBase::OnMatchedWt, IGNORING MESSAGE"));
	return STATE_NULL;
}

SMStateBase *SMScanAndBagBase::PSButtonHelp(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
	return SMStateBase::PSButtonHelp();
}
VOID CALLBACK SMScanAndBagBase::ScanScaleSBTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	trace(L6, _T("+SMAttractBase::ScanScaleSBTimedOut(void)"));
	mp.sCurr->ScanScaleSB();
	trace(L6, _T("-SMAttractBase::ScanScaleSBTimedOut(void)"));
}

void SMScanAndBagBase::ScanScaleSB()
{
	
	
	//Once Timer has expired, we can now say the appropriate audio
	if(lDMScaleWeight != 0)
	{
		KillTimer(hw, SCANSCALESB);
		m_TimerResponse = 0;
		if (lDMScaleWeight > 0)
		{
			DMSayPhrase(PURCHASEPRODUCE);
			fDMScaleZeroed = false;
			FlashButtonsOnWeight(true);
			ps.CMButtonFlash(_T("CMButton4LgFinish"),false);
		}
		else ///if (lDMScaleWeight <= 0)
		{
			FlashButtonsOnWeight(false);	// turn off flash on keyin and lookup item
			//if ( isAnySoldVoidedItems() || fItemIsWaiting )	//226673 if the scanner scale wt returns to zero, then flash the finish and pay btn LPM021203 (UITS 94 rel 3.1)
			ps.CMButtonFlash(_T("CMButton4LgFinish"),true);
		}
		
	}
	trace(L6, _T("::ScanScaleSB-Audio was played"));
	
}

void SMScanAndBagBase::ChangeLanguage(SCOT_LANGUAGE_TYPE langNew)
{
    m_customerLanguage = langNew;
    ps.SetLanguage(m_customerLanguage);
    co.csCMCustomerMessage = ps.GetPSText(co.nCMCustomerMessage);
    CustomerMessage = co.csCMCustomerMessage;
    
    if(gpSCOTAuxDisp)
    {
        gpSCOTAuxDisp->SetLanguage(m_customerLanguage);
    }
    ps.SetTareListToCurrentLanguage();
    ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT);  
    m_LanguageUsedPreviously=m_customerLanguage;

	// Get the language name
	CString csLanguageName = co.language[langNew].csName;
	if(_istdigit(csLanguageName[0])) //if its a msg ID
	{
		int iMsgID = _ttoi(co.language[langNew].csName);
		csLanguageName = ps.GetPSText(iMsgID, (SCOT_LANGUAGE_TYPE) langNew);
	}
	
	rp.SetLanguageName(csLanguageName);
	
    
}

SMStateBase *SMScanAndBagBase::OnLookupComplete (long byteCount)
{
    if (GetPreferredLanguage() != m_customerLanguage)
    {
        SMStateBase* retState = Initialize();
        return retState;
    }
    else
    {
        return STATE_NULL;
    }
}

//******************************************************
//Method : PSButton7
//Purpose: Change the volume level
//Parameters:  
//Return Values: 
//******************************************************
SMStateBase  *SMScanAndBagBase::PSButton7()
{
	CVolumeControl VolumeControl(SETVOLUME);
	return STATE_NULL;
}

SMStateBase  *SMScanAndBagBase::PSListLookup(CString csItemInfo)
{
	//Parse the item information for the selected item.
	//csItemInfo is in ItemCode/CatId@@Description@@IsSubCategory@@IsItem@@Alphabetic format
	csItemSent = csItemInfo.Left( csItemInfo.Find(STR_SEPARATOR));
	int nLen = csItemInfo.GetLength()-(csItemSent.GetLength()-_tcsclen(STR_SEPARATOR));
	CString csDescWithRemainder = csItemInfo.Mid(csItemSent.GetLength()+_tcsclen(STR_SEPARATOR),nLen );
	int nIndex = csDescWithRemainder.Find(STR_SEPARATOR);
	CString csDesc, csSubCategory, csIsItem, csAlphabetic, csCategories; 
	if (nIndex != -1)
	{
		csDesc = csDescWithRemainder.Left(nIndex);
		CString csSubCatWithRemainder= csDescWithRemainder.Mid(nIndex + _tcsclen(STR_SEPARATOR));
		csSubCategory = csSubCatWithRemainder.Left(csSubCatWithRemainder.Find(STR_SEPARATOR));
		CString csIsItemwithRemainder = csSubCatWithRemainder.Mid(csSubCategory.GetLength() + _tcsclen(STR_SEPARATOR));
		csIsItem = csIsItemwithRemainder.Left(csIsItemwithRemainder.Find(STR_SEPARATOR));
		CString csAlphabeticwithRemainder = csIsItemwithRemainder.Mid(csIsItem.GetLength() + _tcsclen(STR_SEPARATOR));
		csAlphabetic = csAlphabeticwithRemainder.Left(csAlphabeticwithRemainder.Find(STR_SEPARATOR));	
		csCategories = csAlphabeticwithRemainder.Mid(csAlphabeticwithRemainder.Find(STR_SEPARATOR)+_tcsclen(STR_SEPARATOR));
	}

	if (csIsItem.CompareNoCase(_T("True")) != 0)
	{
		if (co.fOperationsAllowMultiPick)
		{
			fInMultiSelectPickList = true;
			m_pAppPSX->ClearReceipt(_T("PickListReceipt")); 
			m_pAppPSX->ClearReceipt(_T("PickListReceiptVoid"));
		}
	}

	CStringArray csParentCategories;

	if (csIsItem.CompareNoCase(_T("True")) == 0) //sellable item
	{
		ps.SetItemDesc(csDesc);

		if( csItemSent==_T("") ) //fail in picking list
		{
			DMSayPhrase(PICKLISTINVALID);
			return STATE_NULL;
		}

		if (VerifyQtyItem(csItemSent))     
		{
			io.Reset();
			
			// Saved for dynamic picklist purposes //
			SMStateBase::ms_bIsCurrentItemFromPickList = true;
			
			io.csOrigCode = csItemSent;	   // item code keyed
			io.csItemCode = csItemSent;	   // assumed to be the same
			SASendSCOTInput( UPC_MANUAL, csItemSent);
			
			RETURNSTATE( EnterQuantity )
		}
			
		return TBItemSale( FALSE, 0, false, true );			  //Sell item
	}
	else if (csSubCategory.CompareNoCase(_T("True")) == 0) //subcategory item
	{	
		ps.GetParentCategories(csCategories, csParentCategories);
		int nCount = csParentCategories.GetSize();
		CString csPrevLevel, csCurrLevel;
		if (nCount>0)
		{
			for (int i=nCount-1;i>=0;i--)
			{
				if (i>0)
				{
					csPrevLevel = csParentCategories.GetAt(i);
					csCurrLevel = csParentCategories.GetAt(i-1);
					ps.PushLevels(csPrevLevel, csCurrLevel);
				}
			}
			csPrevLevel = csParentCategories.GetAt(0);
			csCurrLevel = csItemSent;
			ps.PushLevels(csPrevLevel, csCurrLevel);
		}
		csItemSent = _T("SUBCATEGORY") + csItemSent;
	
		RETURNSTATE( ProduceFavorites )
	}
	else //category item
	{
		if ( !csAlphabetic.IsEmpty() )
		{
			csItemSent = csAlphabetic;
			ps.SetLastProduceLookupList(csItemSent);
		}
		else if ( csItemSent == TAB_FAV )
		{
			ps.SetLastProduceLookupList(csItemSent);
		}
		else
		{
			csItemSent = TAB_CATEGORY + csItemSent;
			ps.SetLastProduceLookupList(csItemSent);	
		}
		RETURNSTATE( ProduceFavorites )
	}
}

// +TAR 399985
long SMScanAndBagBase::DMSayPhrase(const int nMsg)
{
	long rc = 0;
	if (!co.IsInAssistMenus())
	{
		rc = SMStateBase::DMSayPhrase(nMsg);
	}
	return rc;
}
// -TAR 399985


// +TAR 399985
long SMScanAndBagBase::DMSaySecurity(const long nMsg, LPCTSTR szWave, HANDLE hWaitEvent/*=0*/)
{
	long rc = 0;
	if (!co.IsInAssistMenus())
	{
		rc = SMStateBase::DMSaySecurity(nMsg, szWave, hWaitEvent);
	}
	return rc;
}
// -TAR 399985

//TAR407959+
//for an item which has multiple linked items, linked item information could arrive in ScanAndBag
//we need to make sure that scanner is enabled.
void SMScanAndBagBase::TBGetItemDetails(ItemDetails &iDetails) 
{
    trace(L2, _T("SMScanAndBagBase::TBGetItemDetails"));
    
    #ifdef _BUILDLIB_
        SMStateBase::TBGetItemDetails(iDetails);
    #else
        SMState::TBGetItemDetails(iDetails);
    #endif
    
    bEASReEnableScanner = true;
    trace(L2, _T("SMScanAndBagBase::TBGetItemDetails - bEASReEnableScanner is set to true."));  
}
//TAR407959-
