//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterCouponsBase.CPP
//
// TITLE: Class header for enter coupons State
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMEnterCoupons.h"       
#include "SMFinish.h"               // Finish state
#include "SMHelpOnWay.h"            // Help is comming
#include "SMVoidItem.h"             // Void Items state
#include "SMEnterCouponValue.h"     // Enter coupon value
#include "SMCheckBasketReminder.h"  // Check Basket Reminder State

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("EnterCouponsBase")

IMPLEMENT_DYNCREATE(SMEnterCouponsBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(EnterCoupons)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMEnterCouponsBase::SMEnterCouponsBase()
: csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
  saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
}

////////////////////////////////////////
SMStateBase  *SMEnterCouponsBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("EnterCoupons") );	// RFC 330050
	COleVariant v;
	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_CPNENTRY, SCOT_LANGUAGE_PRIMARY));
	bUpdateFoodStampTotal = true;

    //This flag is only useful if HelpOnWay came from this state.
    bPressHelpOnWay = false; 

    //TAR412506
    //When multiple ItemSolds for a coupon cause several audios to be generated 
	//while still displaying the "Scan your coupons" screen.  
	//When transferring from another screen (the help screen), the audio should be replayed.
    //+-SSCOADK-6512
    if (!GetISIPTimeOut() &&  ps.GetCurrentContext() != _T("EnterCoupons") && 
        !GetISIPTimeOut() &&  ps.GetCurrentContext() != _T("EnterCouponsWithReward"))
    {
        DMSayPhrase(SCANCOUPONS);	   // only do this once at first entry , TAR:147081
    }
	setAnchorState(BES_ENTERCOUPONS);    // allow to return here
    nSellState = getAnchorState();		  // save current sell entry state
	DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);	// 227226 - change txt on Aux display to "Use touchscreen to continue". LPM021003

	if (co.fCMRewardInfo && !co.IsInAssistMenus())
	{
		csCurContext = _T("EnterCouponsWithReward");
		ps.SetCMBackground(BMP_WELCOME_REWARD);
	}
	else
	{
		csCurContext = _T("EnterCoupons");
		ps.SetCMBackground(BMP_CPNENTRY);
	}
	ps.SetCMFrame(csCurContext);
	ps.ShowCMTotal(true ,false);
	ps.ShowCMTBText(csTBMessage2Scot);	//thp

	//*** NewUI - only need to call this function in ScanAndBag ***//
	//Need more testing, please don't remove the following line - aiqin
	//ps.ShowCMRewardInfoBox(); // customer receipt with reward info if configured to display
 
	if (bTrxLimitExceed)
	{
		// show message that says item will cause trx limit exceed
		CString csAttractText;
		GetTrxLimitString(MSG_ITEMCAUSEDTRXLIMIT, csAttractText);
		ps.SetPSText(PS_WORK1, csAttractText);
		v = ps.ConvertToEscapeSequence(csAttractText, _T("\\n"), _T("\n"));
        m_pAppPSX->SetConfigProperty(_T("ScanBagTextArea"),csCurContext, UI::PROPERTYTEXTFORMAT, v);
		//ps.Message12(PS_WORK1); // clear the image at the background
		// let user try to buy other items, don't scare him saying again, "limit exceeded..."
		bTrxLimitExceed = false;	
	}
	else if (io.d.fDetails)            // if we have details to show, show them
	{
		if (io.lRestCode==1)
		{
			// ps.SetCMBackground(BMP_CANCELITEM);
			// ps.HideEcho();
			v = ps.ConvertToEscapeSequence(ps.GetPSText(MSG_RESTWARN), _T("\\n"), _T("\n"));
			m_pAppPSX->SetConfigProperty(_T("ScanBagTextArea"), csCurContext, UI::PROPERTYTEXTFORMAT, v);
			//ps.Message12(MSG_RESTWARN, false);
		}
		else if (io.fVoid)				// show the item description and price in the prompt and amount areas
		{
			v = ps.ConvertToEscapeSequence(ps.GetPSText(MSG_SETCANCELEDASIDE), _T("\\n"), _T("\n"));
			m_pAppPSX->SetConfigProperty(_T("ScanBagTextArea"), csCurContext, UI::PROPERTYTEXTFORMAT, v);
			//ps.Message12(MSG_SETCANCELEDASIDE, false);
		}
		else if (io.nLoyaltyCardScanned)
		{
			PSTEXT nScannedLoyaltyID;
			if (io.nLoyaltyCardScanned == 2)
			{
				// loyalty scanned but this is not the first time
				nScannedLoyaltyID = MSG_SCANNEDLOYALTY_NTHTIME;
			}
			else
			{
				// loyalty scanned, this is the first time
				nScannedLoyaltyID = MSG_SCANNEDLOYALTY_FIRSTTIME;
				DMSayPhrase(FREQSHOPACCT);
				bAlreadyPlayedLCReminder = true;	//Tar 200719 bz 04/02/02
			}
			io.nLoyaltyCardScanned = 0; // do not play this wave file again
			v = ps.ConvertToEscapeSequence(ps.GetPSText(nScannedLoyaltyID), _T("\\n"), _T("\n"));
			m_pAppPSX->SetConfigProperty(_T("ScanBagTextArea"), csCurContext, UI::PROPERTYTEXTFORMAT, v);
			//ps.Message12(nScannedLoyaltyID, false); 
		}
		else if(io.xd.ExceptionCode==XAPPROVALTHRESHOLDS ||
            io.xd.ExceptionCode==XAPPROVALALWAYS)
		{
			v = ps.ConvertToEscapeSequence(ps.GetPSText(MSG_REFUNDORDISCOUNT), _T("\\n"), _T("\n"));
			m_pAppPSX->SetConfigProperty(_T("ScanBagTextArea"), csCurContext, UI::PROPERTYTEXTFORMAT, v);

		  //ps.Message12(MSG_REFUNDORDISCOUNT, false);
			DMSayPhrase(SETREFUNDDISCOUNTASIDE);
		}
		else
		{
			if (IsUsingCouponSensor()) //if coupon sensor is configured
			{
				v = ps.ConvertToEscapeSequence(ps.GetPSText(MSG_SCANCOUPONS), _T("\\n"), _T("\n"));
				m_pAppPSX->SetConfigProperty(_T("ScanBagTextArea"), csCurContext, UI::PROPERTYTEXTFORMAT, v);

				//ps.Message12(MSG_SCANCOUPONS, false); //need to be replaced with MSG_SCANCOUPON later
			}
			else
			{
				v = ps.ConvertToEscapeSequence(ps.GetPSText(MSG_COUPWARN), _T("\\n"), _T("\n"));
				m_pAppPSX->SetConfigProperty(_T("ScanBagTextArea"), csCurContext, UI::PROPERTYTEXTFORMAT, v);
				//ps.Message12(MSG_COUPWARN, false);
			}
		}
	}
	else
	{
		if (IsUsingCouponSensor()) //if coupon sensor is configured
		{
			v = ps.ConvertToEscapeSequence(ps.GetPSText(MSG_SCANCOUPONS), _T("\\n"), _T("\n"));
			m_pAppPSX->SetConfigProperty(_T("ScanBagTextArea"), csCurContext, UI::PROPERTYTEXTFORMAT, v);
			//ps.Message12(MSG_SCANCOUPONS, false);  //need to be replaced with MSG_SCANCOUPON later
		}
		else
		{
			v = ps.ConvertToEscapeSequence(ps.GetPSText(MSG_COUPWARN), _T("\\n"), _T("\n"));
			m_pAppPSX->SetConfigProperty(_T("ScanBagTextArea"), csCurContext, UI::PROPERTYTEXTFORMAT, v);
			//ps.Message12(MSG_COUPWARN,false);
		}
	}

  DMMSRDisable();					    	    // disable the MSR, turn light on
	
  // Blink the Finish and Pay button
  ps.CMButtonFlash(_T("CMButton4"),true);

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
 	
  trace(L2, _T("::Initialize - bEASReEnableScanner is set to true."));
  bEASReEnableScanner = true;
  SAWLDBScannerEnable();						// enable the scanner
	
  // if items have been sold
  // and no authorization is required
  // and no card is already inserted
  // enable the cash acceptor
  if ( isAnySoldVoidedItems() )
  {
    if ( !isAnyApprovalRequired() )
    {
      if (!cDMLastTrack && co.fTenderCashAllowed)
      {
        // pass type of cash in dispenser ";100,500,1000,2000,5000,10000"
        DMCashAcceptorEnable(co.csTenderDenominations);	// Coin Acceptor Modifications
        DMCoinAcceptorEnable();							// Coin Acceptor Modifications
        DMAcceptorLight(DM_MISC_STATE_ONE_HZ);
		DMCoinAcceptorLight(DM_MISC_STATE_ONE_HZ);      // Tar 139796 Coin Acceptor Light
      }
    }
  }
  //+tar 275064
  if( IsMINIHardware() )
  {
    if( lTotalSale < 0 )
    {
        m_pAppPSX->SetConfigProperty( _T("CMButton4LgFinish"), csCurContext, UI::PROPERTYSTATE, ( long ) UI::Disabled );
   	    DMMSRDisable();
    }
    else
    {
        m_pAppPSX->SetConfigProperty( _T("CMButton4LgFinish"), csCurContext, UI::PROPERTYSTATE, ( long )UI::Normal );
    }
  }
  //-tar 275064

  ps.ShowCMFrame();
  return STATE_NULL;                // stay in attract state
}

///////////////////////////////////////
void SMEnterCouponsBase::UnInitialize(void)
{
  // BeginCodeWLDBSA11
	SAWLDBScannerDisable();               // disable the scanner
	// EndCodeWLDBSA11

  if(!co.fLocaleLeaveCashAcceptorsEnabledDuringTransaction) //SSCOADK-836
  {
      DMCashAcceptorDisable();  // disable acceptor, turn light off	// Coin Acceptor Modifications
      DMCoinAcceptorDisable();  // disable acceptor, turn light off	// Coin Acceptor Modifications
  }
	
  if (!cDMLastTrack)	    // Changed for PR 022598-8
  {
    DMMSRDisable();			// if no card inserted, disable
  }
  DMMSRLight(DM_MISC_STATE_OFF);    // turn off MEI, card or no card
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("EnterCoupons") );	// RFC 330050
}

///////////////////////////////////////
SMStateBase  *SMEnterCouponsBase::PSButton1(void)// can't scan coupon
{
  fCantScanCoupon = true;
  
  // Begin TAR 197558
  // Make the light state flash red since this occurs after F&P tar 234247 RJF 041003
  
  // (*) SR898 ====================
  DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                     DM_TRICOLORLIGHT_BLINK_1HZ,
                     DM_TRICOLORLIGHT_NORMALREQUEST);
                     
  // Send an Approval needed for can't scan coupon to RAP
  CString csDesc, csInstruction;
  CString csAnchorStateName = getSCOTStateName();
  // TAR 238422 Due to UITS, Use Help Request event instead of Coupon event
  csDesc = ps.GetPSText(TXT_CANTSCANCOUPON, SCOT_LANGUAGE_PRIMARY);
  csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(BTT_HELP, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, _T(""), ps.GetPSText(BTT_CPNCANT, SCOT_LANGUAGE_PRIMARY));
  trace(L7, _T("+ra.OnCallForHelp, Message=%s"), csInstruction);
  ra.OnCallForHelp(csInstruction);
  trace(L7, _T("-ra.OnCallForHelp"));
  // end TAR 238422
  dm.saveTriColorLightStatus();	// tar 232247 RJF 041003

  RETURNSTATE(HelpOnWay)           // call for help
 // return STATE_NULL;
}

///////////////////////////////////////
SMStateBase  *SMEnterCouponsBase::PSButton2(void)
{
  return STATE_NULL;
}

///////////////////////////////////////
SMStateBase  *SMEnterCouponsBase::PSButton3(void)// cancel coupons
{
  if (lItems) RETURNSTATE(VoidItem) // void
  return STATE_NULL;
}

///////////////////////////////////////
SMStateBase  *SMEnterCouponsBase::PSButton4(void)// finish
{
  SetTenderStarted(false); //SR797
  return setAndCreateAnchorState(BES_FINISH);
}

///////////////////////////////////////
SMStateBase  *SMEnterCouponsBase::DMScanner(void)// scanned item
{
  csItemSent = csDMLastBarCode;
  SMStateBase *retval = TBItemSale(false,0,true);

  //Commented out for TAR 119725
//  if (!io.IsCoupon())
//    setAnchorState(BES_SCANANDBAG);

  return retval;
}

/////////////////////////////////////////
SMStateBase  *SMEnterCouponsBase::PSCancelKey(void)// override where needed
{

  DMSayPhrase(KEYPADINVALID);
  return STATE_NULL;
}

//////////////////////////////////////////
SMStateBase  *SMEnterCouponsBase::DMCardReader(void)
{
  if (lItems)	  // inserting a card here signifies transitioning to finish.
  {
	DMSayPhrase(CARDACKNOWLEDGEDSELECTCARDTYPE);
	if(co.fStateCheckBasketReminder)
		RETURNSTATE(CheckBasketReminder)
	else
		return setAndCreateAnchorState(BES_FINISH);
  }
	
  // This line was removed because SCOT should not say anything here, especially "Thank You"
  // DMSayPhrase("Sec9 ThankYou");
  return STATE_NULL;
}

////////////////////////////////////////
SMStateBase  *SMEnterCouponsBase::DMAcceptor(void)
{
  ps.HideButtons();
  
  if(co.fStateCheckBasketReminder)
	RETURNSTATE(CheckBasketReminder)
  else
	return setAndCreateAnchorState(BES_FINISH);
}
