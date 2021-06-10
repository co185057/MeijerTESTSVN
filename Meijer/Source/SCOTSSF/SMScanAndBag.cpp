//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMScanAndBag.CPP
//
// TITLE: Class header for Scan and Bag State
//
// $Header:
//
// AUTHOR:    Feng Zhang
//
// CHANGE HISTORY
//
// CRD 537481 Name: CJ Estrada Date: Mar 5, 2021
// CRD 574057 Name: CJ Estrada Date: Mar 3, 2021
// CRD 558065 Name: CJ Estrada Date: Jan 14, 2021
// CRD 525299 Name: CJ Estrada Date: Nov 17, 2020
// CRD 503257 Name: CJ Estrada Date: Oct 2, 2020
// CRD124746 Work Request:Sprint 10.3 Name:Aparna Tunuguntla Date: March 19,2018
// POS320066 Work Request:77937 Name:Aparna Tunuguntla Date: April 06, 2017
// POS311584 Work Request:76936 Name:Alex Bronola  Date:January 23, 2017
// POS253532 WorkRequest:68941 Name:Saiprasad Srihasam Date:March 18, 2016
// POS248467 Work Request:68942 Name:Robert Susanto Date:April 14, 2016
// POS253532 WorkRequest:68941 Name:Saiprasad Srihasam Date:March 18, 2016
// POS240201 Work Request:68942 Name:Robert Susanto Date: January 7, 2016
// POS189696 Work Request:68237 Name:Saiprasad Srihasam Date: April 14, 2015
// POS187345 Work Request:66559 Name:Saiprasad Srihasam Date:March 16, 2015
// POS153822 Work Request:66559 Name:Robert Susanto    Date:January 13, 2015
// POS150388 Work Request:66559 Name:Saiprasad Srihasam    Date:December 18, 2014
// POS138636 Work Request:66559 Name:Saiprasad Srihasam    Date:December 04, 2014
// POS78131 WORK REQUEST:61232 Name: Robert Susanto Date: March 10, 2014
// POS83168 Work Request:61232 Name: Matt Condra   Date:February 11, 2014
// POS89889 WORK REQUEST:59313 Name: Robert Susanto Date: February 18, 2014
// POS48277/71132 Work Request:58657 Name: Robert Susanto   Date:September 13, 2013
// POS61464 Work Request:54416 Name: Robert Susanto   Date:April 16, 2013
// POS47402 Work Request:52635 Name: Robert Susanto   Date:November 29, 2012
// POS33020 Work Request:52635 Name: Matt Condra   Date:November 16, 2012
// POS39475 Work Request:52635 Name: Matt Condra    Date:November 10, 2012
// POS30981/POS31054 Work Request:50864 Name:Robert Susanto Date:June 29, 2012
// POS37599 WORK REQUEST:50864 Name: Robert Susanto Date: June 27, 2012
// POS36316 WORK REQUEST:19114 Name: Robert Susanto Date: June 8, 2012
// POS34664 WORK REQUEST:19114 Name: Robert Susanto Date: May 16, 2012
// POS34296 WORK REQUEST:19114 Name: Robert Susanto Date: May 8, 2012
// POS33288 WORK REQUEST:19114 Name: Robert Susanto Date: April 25, 2012
// POS24632 WORK REQUEST:19114 Name: Robert Susanto Date: March 22, 2012
// POS29825 WORK REQUEST:16656 Name: Robert Susanto Date: February 21, 2012
// POS29613 WORK REQUEST:16656 Name: Robert Susanto Date: February 14, 2012
// POS28888 WORK REQUEST:16656 Name: Matt Condra Date: February 1, 2012
// POS26005 WORK REQUEST:16656 Name: Robert Susanto Date: February 1, 2012
// POS28367 WORK REQUEST:16656 Name: Robert Susanto Date: January 26, 2012
// POS28366 WORK REQUEST:16656 Name: Robert Susanto Date: January 26, 2012
// POS25844 WORK REQUEST:16656 Name: Robert Susanto Date: January 18, 2012
// POS25510 WORK REQUEST:16656 Name: Matt Condra Date: December 12, 2011
// POS19072 WORK REQUEST:16656 Name: Matt Condra Date: November 29, 2011
// POS18905 WORK REQUEST:16656 Name: Robert Susanto Date: October 11, 2011
// POS11024 Work Request:16632 Name: Sandeep Shenoy Date: Oct 7, 2011
// POS11024 Work Request:16632 Name: Sandeep Shenoy Date: Oct 4, 2011
// POS15766 WORK REQUEST:15196 Name: Robert Susanto Date: July 8, 2011
// POS11024 Work Request:16632 Name: Sandeep Shenoy Date: Jul 6, 2011
// POS12088 WORK REQUEST:15196 Name: Robert Susanto Date: May 9, 2011
// POS9528 WORK REQUEST:15196 Name: Sandy Reedy Date: April 21, 2011
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "SMScanAndBag.h"           // MGV this state

//UploadTicket Port +
#include "MobileConstantsSSF.h"  
#include "MPProcedures.h"
DLLIMPORT extern MPProcedures mp;
//UploadTicket Port -
#include "transbrokerssf.h"
#include "SMRebateItem.h"
#include "SMInProgress.h"
#include "SMLaneClosed.h"
#include "SMScanReceipt.h"
#include "SMWaitForApproval.h"
#include "SMCheckBasketReminder.h"
#include "SMCashPayment.h"		 // Cash payment state
#include "SMProcessing.h"		 // CRD 554915

//pos18905
#include "SMAssistedTender.h"

#include "SMApplyPromotionalItems.h" // POS19072
#include "SMCustomMessage.h"         // POS39475
#include "SMInsertCoupon.h"          // POS33020

// +SSCOI-44479
#include "SMPLAEnterWeight.h"          //	enter wgt state 
#include "SMEnterQuantity.h"		
#include "LaneStateData.h" 				
#include "SMProduceFavorites.h"			
// -SSCOI-44479

#include "SMRecallingItems.h" // RFQ 9545 mobile shopper
#include "SMStatePS.h" // CRD124746
DLLIMPORT extern HWND hw; //CRD124746 

#include "SMShowExternalUrl.h"		// CRD 525312
#include "SMVoidTransApproval.h"    // CRD 125377
#include "SMSmAbort.h"				// CRD 125377
#include "SMRestrictedAgeYN.h"		// CRD 125377
#define SNDAGE_YESNOBUTTONS  20 // + MAXTIMER defined in the base ADK  currently 17

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ScanAndBag")

IMPLEMENT_DYNCREATE(SMScanAndBag, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

#ifdef _TRADESHOWDEMO
#include "..\TradeShowDemo\SMScanAndBagTSDemo.cpp"
#endif

////////////////////////////
SMScanAndBag::SMScanAndBag()
{
  IMPLEMENT_TIMESTAMP
	//+4.2 card dispenser
		#ifdef _TESTSSF
		  m_pAppPSX->SetConfigProperty( _T("SMButton6"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
		#endif
	 //-4.2 card dispenser
}

	//+4.2 card dispenser
	#ifdef _TESTSSF
	#include "Test_CardDispenser\SMScanAndBagTest.cpp"
	#endif
	//-4.2 card dispenser

#ifndef _TRADESHOWDEMO
SMStateBase * SMScanAndBag::Initialize(void)
{
    //sotf 7745
    CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMScanAndBag::Initialize"));
	
	//(+) CRD124746
	if(SMState::m_bActivateHHScanner) 
	{
		StopHHScannerTimer();
		trace(L6, _T("*** Scan and bag killing timer intialize"));
	}
	//(-) CRD124746

	// Start CRD 537481
	if(bApplyMeijerStartedFromAttract)
	{
		bApplyMeijerStartedFromAttract = false;
		RETURNSTATE(ShowExternalUrl)
	}
	// End CRD 537481

	bEASApproveFromVoid = false;

	nPreviousState = BES_LASTSTATE; //SSCOI-45180;
	
    if (m_bCouponInsertionBypassed && (true == io.d.fCoupon)) //POS320066 Preventing to prompt Insert Coupon for Extra Coupon
    {
       // POS33020: Paper coupon insertion got bypassed due to mPerks entry on Insert 
       // Coupon screen so go back to Insert Coupon screen to take care of this.
       trace(L6, _T("Coupon needs inserted, go to InsertCoupon; lCoupons=%d; m_lInsertedCouponsTotal=%d"), lCoupons, m_lInsertedCouponsTotal);
       CREATE_AND_DISPENSE(InsertCoupon)(getAnchorState(), BES_SCANANDBAG);
    }

    // POS39475: Start
    //if ( (GetHookProperty(_T("__POSState")).Find(_T("HIGHVALUE_PROMO_CHOICE")) != -1) && (GetHookProperty(_T("HighValuePromoChoice")) == _T("1")) )
    //CString csPOSState = GetHookProperty(_T("__POSState")); //POS311584
	CString csPOSState = GetTBProperty(_T("SavedLastPosState"));
    trace(L6, _T("Posstate=<%s>"), csPOSState);
	CString csTBState(_T(""));
    SendTBCommand(TBCMD_GET_TB_STATE, &csTBState);

	if(csPOSState.Find(_T("ITEMMENU_DK")) == -1 || csTBState != _T("ITEMIZATION"))
	{
		//POS is not ready for next item, probably at applying promotions for items in the transaction already as customer entered mPerks in the middle of transaction
		bPOSatmPerksPromotions = true;
		trace(L6, _T("SCO came to S&B while POS is not ready for next item. Setting bPOSatmPerksPromotions = true"));
	}

	// CRD 269544 reset this variable if POS State is back at Item Menu
	if ((bIsItemizationAfterSkipBaggingSent) && (csPOSState.Find(_T("ITEMMENU")) != -1)) 
	{
		trace(L6, _T("must reset bIsItemizationAfterSkipBaggingSent because POS is already at Item Menu"));
		bIsItemizationAfterSkipBaggingSent = false;
	}
	//End CRD 269544

	//Fix to enable the scanner right away when entering scan and bag
	if(!g_bInRecallMode && csPOSState.Find(_T("ITEMMENU_DK")) != -1 && bPOSatmPerksPromotions)
	{
		trace(L6, _T("Enabling the scanner and resetting bPOSatmPerksPromotions"));
		if(!dm.IsScannerEnabled())
		{
			bEASReEnableScanner = true;
			SAWLDBScannerEnable();
		}
		bPOSatmPerksPromotions = false;
	}

    if ( ( (csPOSState.Find(_T("HIGHVALUE_PROMO_CHOICE")) != -1) && (GetHookProperty(_T("HighValuePromoChoice")) == _T("1")) ) ||
         ( (csPOSState.Find(_T("VENDOR_COUPON_CHOICE")) != -1) && (GetHookProperty(_T("CouponChoice")) == _T("1")) )
       )
    {
        //Bug 244609 - Setting the fSAInItemSale to false prevent from going into SMInProgress state when you press the 
        //Yes button from High Value promo choice prompt.
        fSAInItemSale = false; 

       // Customer needs to respond to this high value mPerks promo choice (POS39475) or
       // this vendor coupon choice (POS33020) because the sales app is synchronously
       // waiting on the response before it continues.
       trace(L6, _T("%s pending; ScreenText=<%s>"), csPOSState, CustomMessageObj.csScreenText);
       if ( ( (csPOSState.Find(_T("HIGHVALUE_PROMO_CHOICE")) != -1) && (CustomMessageObj.csScreenText != ps.GetPSText(SSF_TXT_HIGHVALUE_PROMO)) ) ||
            ( (csPOSState.Find(_T("VENDOR_COUPON_CHOICE")) != -1) && (CustomMessageObj.csScreenText != ps.GetPSText(SSF_TXT_COUPONCHOICE_MPERKS)) )
          )
       {
          // TB has not yet processed this incoming choice, so let's wait for
          // TB to send the TB_MESSAGESCREEN state.
          trace(L6, _T("%s pending; Go to InProgress"), csPOSState);
          RETURNSTATE(InProgress);
       }
       else
       {
          // TB has processed this incoming choice and has sent the TB_MESSAGESCREEN
          // state but the system has bumped us out of the CustomMessage screen [e.g.
          // due to a BagScale security event in SMStateBase::OnMatchedWt()]. So,
          // let's go back to CustomMessage.
          trace(L6, _T("%s pending; Go to CustomMessage"), csPOSState);
          RETURNSTATE(CustomMessage);
       }
    }
    // POS39475: End

    bTransactionComplete = false;  /// TAR 360761

    //POS29613 - reset the appini wic option
    strWICEBTCMFeature = _T("");
    strWICEBTCheckFeature = _T("");

    //SOTF 7695
    SendTBCommand(TBCMD_ASSISTMENUSET, false);

    //sotf 6657
    //SOTF 8113
    //SendTBCommand(TBCMD_NOTIFY_ACS_REMOTE_ASSISTMODE_STATUS, false);

	//This may seem strange.  But if we are in scan and bag set the TABStateBefortTransport
	//to Scan and Bag so that DMTakeawayReadyForItem() always returns back
	//to the right state.  The senario is if we have an unexpected increase, the 
	//TABStateBefortTransport will get set to the state before the violation (which may be
	//RAPDataNeeded).  After the violation, security will go to the anchor state--which is correct.
	//But on the next item scanned, DMTakeawayReadyForItem() will check TABStateBefortTransport and it is still
	//set to the previous value so we end up going back to the wrong state
	TABstateBeforeTransport = BES_EMPTY_STATE;
	
	//+ POS253532
    CString csValue;
    CString strPropName = _T("IsFromResumeTrx");
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strPropName, &csValue);
    trace(L3, _T("Message Screen - IsFromResumeTrx=%s"), csValue);
    if(csValue == _T("1"))
    {
     // csValue = _T("0");
     //SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strPropName, &csValue);
        return TransitionToPayment(false);
    }
    //- POS253532

	if (true == m_bCancellingCashTender) 
    {
		
		nTenderType = TB_TENDER_NONE;
        m_bCancellingCashTender = false;
	}
        
	//TAR348914+
	if( GetTBProperty(_T("SavedLastPosState")) == _T("SIGNIN_MENU") )
	{
		RETURNSTATE(LaneClosed);
	}
	//TAR348914-

	//TAR367891+
    //CString csTBState(_T(""));
    SendTBCommand(TBCMD_GET_TB_STATE, &csTBState);

	//TAR371078+
	if(csTBState == _T("ASSISTMODE_TRXEND"))
	{
		return TBFinish();
	}
	//TAR371078-
    
    // +TAR 399339 
    if(dm.fStateTakeawayBelt)
    {
        trace(L6, _T("Setting TABstateBeforeTransport = %d in Scan&Bag"), BES_EMPTY_STATE);
	    TABstateBeforeTransport = BES_EMPTY_STATE;
    }
    // -TAR 399339 

	CString strTBPropValue = _T("SMScanAndBag");
	CString strTBPropName = _T("NewFLState");
    SendTBCommand(TBCMD_SET_TBPROPERTY, &strTBPropName, &strTBPropValue);

	//mini & super mini paystation
	CString strStatePropName = _T("IsResumeComplete");
	CString strStatePropValue;
	SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strStatePropName, &strStatePropValue);



    //POS48277 - MobilePayStation    
    if( GetTBProperty(_T("FastLaneConfiguration")) == _T("MobilePayStation") )
    {
        trace(L6, _T("MobilePayStation, POSState(%s) and isresumecomplete(%s), transaction clean(%d), lItems(%d)"), csPOSState,strStatePropValue,IsTrxnCompletelyClean(), lItems);
        if( IsTrxnCompletelyClean() && csPOSState != _T("DIGITAL_COUPON_ACTIVE") && csPOSState != _T("MOB_SHOP_ORDER_NOT_FOUND") )
        {
            if( csPOSState == _T("DIGITAL_COUPON_LOOKUP_FAILED"))
            {

            }
            return TransitionToPayment(false);
        }

        //POS89889 - this logic would check if the Resume has not been completed and no items yet on the customer e-receipt, then don't change the screen.
        // This is needed to preven the screen flashing while FL is in the Retrieving order state.
        if( strStatePropValue != _T("1") && lItems == 0 )
        {
            return STATE_NULL;
        }
        //e POS89889
    }
    //e POS48277 - MobilePayStation

	if(m_bIsFromDisplayingResumeItem && strStatePropValue == _T("1"))
	{
		strStatePropValue = _T("0");
		SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strStatePropName, &strStatePropValue);
	}
	if( strStatePropValue == _T("1"))
	{
        //SOTF 7891
        //on resume transaction clear the coupon flags so that there are
        //no coupon interventions
        SMStateBase::lUnapprovedCoupons = 0;

		m_bIsFromScanReceipt = false;

		CString strOptionName = _T("IsResumeLikePayStation");
		CString csResumeOption;
		SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &csResumeOption);

		strStatePropName = _T("IsResumeTrxInProgress");
		CString strPropValue = _T("0");
		SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strStatePropName, &strPropValue);

		if(strPropValue == _T("1") && !m_bIsFromPaymentGoback && !m_bIsFromDisplayingResumeItem &&
			csResumeOption != _T("N"))
		{
            // sotf 6226
            CString csAge;
            if ((SendTBCommand(TBCMD_ACS_DELAYED_AGE_RESTRICTION) == 1) &&
                (SendTBCommand(TBCMD_HIGHEST_AGE_REQUIRED, &csAge) == 1) &&
                ( GetTBProperty(_T("IsThereItemNeedToDisplayed")) == _T("1")))
	        {
               //POS9528 Start
               strStatePropName = _T("IsResumeStarted");
               strStatePropValue = _T("0");
               SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strStatePropName, &strStatePropValue);
 
               if (strStatePropValue == _T("1") )
               {
                  strStatePropValue = _T("0");
                  SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strStatePropName, &strStatePropValue);
               }   
               //POS9528 End

		        //lItems = 0;   POS9528 do not set lItems to 0		
		        ps.LeadThruText(TXT_PLEASEWAIT);
		        SendTBCommand(TBCMD_RESUME_ITEMS, NULL);
		        return STATE_NULL;
            }
            // -sotf
		
			//Start CRD 241464
			strStatePropName = _T("IsSCOFLMS");
			strPropValue = _T("0");
			SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strStatePropName, &strPropValue);
		
			if(strPropValue == _T("0"))
			{	
				//Start CRD 50327 Wait until POS is at Item Menu before going to Payment
				if(csPOSState.Find(_T("ITEMMENU")) != -1)
				{
					return TransitionToPayment(false);
				}
				else 
				{
					bResumeCompleteWaitingForItemMenu = true;		
				}
				//End CRD 50327
			}
			//End CRD 241464
			
		}
	}
	
   /* SOTF8644 - Clear IsResumeStarted when returning from payment */
	strStatePropName = _T("IsResumeStarted");
	strStatePropValue = _T("0");
	SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strStatePropName, &strStatePropValue);

   if (strStatePropValue == _T("1") && m_bIsFromPaymentGoback)
   {
	   strStatePropValue = _T("0");
	   SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strStatePropName, &strStatePropValue);
   }

   m_bIsFromDisplayingResumeItem = false;

	if( m_bIsFromPaymentGoback )
	{
		m_bIsFromPaymentGoback = false;
		m_bIsNewTrxSelected = true;
	}
	
	CString strOptionName = _T("FastLaneConfiguration");
	CString csFLConfig;
	SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &csFLConfig);
	
	trace(L7, _T("csFLConfig = %s and m_bIsNewTrxSelected = %d"), csFLConfig, m_bIsNewTrxSelected);
	
	if ( csFLConfig == _T("SuperMini") || (csFLConfig == _T("Mini") && !m_bIsNewTrxSelected))
	{
		DMScannerEnable();

		if (m_bNeedScanReceipt || m_bIsFromScanReceipt)
		{
			m_bNeedScanReceipt = false;
			return new SMScanReceipt();
		}
		else
		{
			return TransitionToPayment(true);
		}
	}
    /* SOTF8031+ */
    else if (m_bIsFromScanReceipt)
    {
		DMScannerEnable();

		m_bNeedScanReceipt = false;
		// return new SMScanReceipt();
		return TransitionToPayment(true);
    }
    /* SOTF8031- */

    SendTBCommand(TBCMD_CHECK_FORCE_LOAD, NULL);

	strStatePropName = _T("FLState");
	strStatePropValue = _T("ITEMIZATION");
	SendTBCommand(TBCMD_SET_TBPROPERTY, &strStatePropName, &strStatePropValue);

	strStatePropName = _T("PropIsFromPayment");
	strStatePropValue = _T("0");
	SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strStatePropName, &strStatePropValue);

	//fix for TAR#306687
	CString csPropFromRTS = _T("PropFromRTS");
	CString csFromRTS = _T("0");
	SendTBCommand(TBCMD_SET_HOOKPROPERTY, &csPropFromRTS, &csFromRTS);
	//end of fix

	CString csPropFromAttract = _T("PropFromAttract");
	CString csFromAttract = _T("0");
	SendTBCommand(TBCMD_SET_TBPROPERTY, &csPropFromAttract, &csFromAttract);

    // Need to allow for restricted item approval
    // since you can't currently go to WaitforApproval directly 
    // if you are in state InProgress.
    if (m_bNeedImmediateApproval)
    {
      	trace(L7, _T("SMScanAndBag::Initialize Need Immediate Approval = %d"), m_bNeedImmediateApproval);
        m_bNeedImmediateApproval = false;
        CREATE_AND_DISPENSE(WaitForApproval)(false,false);
    }

	//Start CRD 263537 - do not reset these variables related to resume while in recall mode
	if(!g_bInRecallMode)
	{
		strStatePropName = _T("IsResumeTrxInProgress");
		CString strPropValue = _T("0");
		SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strStatePropName, &strPropValue);

		SetHookProperty(_T("IsSCOFLMS"),_T("0"));	//CRD 241464
	}
	//End CRD 263537

	bAlreadyInSnB = false;

	// Start CRD 125377 can likely clean this up 
	if (m_bShopAndDropEnabled)
	{
		// make if statement a method in SMState  which returns true or false
		CString csTBState(_T(""));
        SendTBCommand(TBCMD_GET_TB_STATE, &csTBState);
        CString csPOSState(_T(""));
        SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);
		
		trace(L6, _T("csPOSState=%s csTBState=%s m_bSNDAllowItemMenu=%d"), csPOSState, csTBState, m_bSNDAllowItemMenu);
		
		if (((m_bSNDExitedAM) && (csTBState == _T("ITEMIZATION")) && (m_bSNDAllowItemMenu)) ||
			((csPOSState == _T("ITEMMENU_DK1")) && (csTBState == _T("ITEMIZATION")) && (m_bSNDAllowItemMenu)) ||
			((csPOSState == _T("DIGITAL_COUPON_NOT_A_MEMBER")) && (csTBState == _T("ITEMIZATION")) && (m_bSNDAllowItemMenu)) ||
			((csPOSState == _T("DIGITAL_COUPON_LOOKUP_FAILED")) && (csTBState == _T("ITEMIZATION")) && (m_bSNDAllowItemMenu)) ||
			((csPOSState == _T("TMD_CARD_SCANNED")) && (csTBState == _T("ITEMIZATION"))) ||
			((csPOSState == _T("DIGITAL_COUPON_ACTIVE")) && (csTBState == _T("ITEMIZATION"))))
		{
			m_bSNDAllowItemMenu = false;	
			m_bSNDExitedAM = false;			// CRD 125377

			trace(L6, _T("m_bSNDmPerkEntered = %d m_bSNDAllowItemMenu = %d m_bSNDAgeEntered = %d"), m_bSNDmPerkEntered, m_bSNDAllowItemMenu, m_bSNDAgeEntered);

			if (!m_bSNDmPerkEntered) 
			{
				return PSButton7(); // mPerks button
			}
			else if ((m_bSNDAgeEntered) || (m_bSNDBackToFnP))
			{
				trace(L6, _T("going to Payment 1"));
				return setAndCreateAnchorState(BES_FINISH);
			}

		}
		else if (((m_bSNDmPerkEntered) && (m_bSNDAgeEntered)) || 
				 ((m_bSNDmPerkEntered) && (m_bSNDBackToFnP)))
		{
			m_bSNDExitedAM = false;			// CRD 125377
	
			trace(L6, _T("going to Payment 2"));
			return setAndCreateAnchorState(BES_FINISH);
		}
		else if ((csPOSState == _T("MPERKS_PIN_ENTRY_LIMIT_REACHED")) && (csTBState == _T("ITEMIZATION")))
		{
			trace(L6, _T("going to Payment invalid pin entry max reached"));

			m_bSNDmPerkEntered = true;	// CRD 125377
			m_bSNDmPerksCancel = true;	// CRD 125377
			return setAndCreateAnchorState(BES_FINISH);
		}
		/*
		else if ((csPOSState == _T("PAYMENTMENU_DK1")) && (csTBState == _T("TB_ENTERTENDERSTATE_IN")))
		{
			trace(L6, _T("going to Payment changed language"));
			
			m_bSNDmPerkEntered = true;	// CRD 125377
			m_bSNDmPerksCancel = true;	// CRD 125377
			return setAndCreateAnchorState(BES_FINISH);
		}
*/
		m_bSNDExitedAM = false;			// CRD 125377
	}
	// end CRD 125377
    if ((!m_bMJPlayedWelcome)) // || (m_bShopAndDropEnabled))
    {
		// start CRD 125377
		if (m_bShopAndDropEnabled)
		{
			DMSaySecurity( SEC_NONE, _T("SNDAnyAgeRestricted.wma") );	// to interrupt the wave file if it's playing 153796
		}
		else
		{
        // POS25510: Remove 'Welcome, please scan your first item ...' audible message when the Start button is tapped.
        // POS26005 - Uncommented out this play audio since we control the audio from the m_bMJPlayedWelcome flag that could be set up from Attract
        DMSaySecurity( SEC_NONE, _T("WelcomeScanFirst_Reminders-prt2.wma") );	// to interrupt the wave file if it's playing 153796
		}
		// end CRD 125377
        m_bMJPlayedWelcome = true;
        bAlreadyPlayedWelcome = true;
    }

	if (_T("1") == GetHookProperty(_T("EnableDigitalCoupons")))
	{
        UpdatemPerksButton(true);
	}
	else
	{
		UpdatemPerksButton(false);
	}
	// -SOTF6400

    SMStateBase *pRetState;
    int nISIPTimeOutValue = GetISIPTimeOut();
    trace(L6, _T("ISIPTimeOut=%d"), nISIPTimeOutValue);
    // POS248467 rrs
    /*if ( (GetHookProperty(_T("WaitForPosScannerReady")) != _T("0")) &&
         (!nISIPTimeOutValue)
       )
    {
        // POS83168: The POS sales app is not ready for scanned input, so make sure the
        // scanner is not enabled when executing the base initialization routine.
        trace(L6, _T("Do not enable scanner because POS is not ready for scanned input"));
        SetISIPTimeOut(1);
        pRetState = SMScanAndBagBase::Initialize();
        SAWLDBScannerDisable(); // Make sure fScannerRequested is false.
        SetISIPTimeOut(0);
    }
    else
    {
        pRetState = SMScanAndBagBase::Initialize();
    }************************/
    // e - POS248467 rrs

	// start RFQ 9545 mobile shopper
	SMRecallingItems::g_nTimeoutRecallingWaitForPos = 0;  // RFQ 9545
	SMRecallingItems::g_bRecallingWaitForPos = false;	   // RFQ 9545	

	trace(L6, _T("SMScanAndBag::Initialize - g_bInRecallMode= %d g_bEnteringRecallMode= %d bFinishRecall=%d"), g_bInRecallMode, g_bEnteringRecallMode, bFinishRecall);

	if (g_bInRecallMode)
	{
		CString csWaitingForPOS = GetTBProperty(_T("xWaitForPosScannerReady"));
		CString csPosState = GetHookProperty(_T("__POSState"));
		
		trace(L6, _T("SMScanAndBag::Initialize -- xWaitForPosScannerReady = %s"), csWaitingForPOS);
		trace(L6, _T("SMScanAndBag::Initialize -- POSState = %s"), csPosState);
		trace(L6, _T("SMScanAndBag::Initialize -- m_bIsLoyaltyProcessed = %d"), m_bIsLoyaltyProcessed);
		
		// + 154609 Items not transferring after audit		
        if((csWaitingForPOS == _T("1")) && (csPosState.Find(_T("ITEMMENU_DK1")) != -1) && !m_bIsLoyaltyProcessed)
		{
			trace(L6, _T("SMScanAndBag::Initialize -- No returning #1"));		
		}
		
		else if((csWaitingForPOS == _T("1")) && (csPosState.Find(_T("PROCESSING")) != -1) && !m_bIsLoyaltyProcessed)
		{
			trace(L6, _T("SMScanAndBag::Initialize -- No returning #2"));        
		} 
		else
		{
			//if waiting for POR and POS State is not Processing
			if(csWaitingForPOS == _T("1"))
			{
				trace(L6, _T("SMScanAndBag::Initialize -- Returning STATE_NULL"));
				return STATE_NULL;
				
			}
			// start 140001
			else if (csPOSState.Find(_T("ITEM_NEEDQTY")) != -1)
			{
				bQuantityRequiredActive = true;
				trace(L6, _T("ITEM_NEEDQTY waiting for TB to catchup - bQuantityRequiredActive %d"), bQuantityRequiredActive);
				return STATE_NULL;
			}
			// end 140001
			//start 176339
			else if(csPOSState.Find(_T("HIGHVALUE_PROMO_CHOICE")) != -1)
			{
				bHighValueActive = true;
				trace(L6, _T("HIGHVALUE_PROMO_CHOICE waiting for TB to catchup after pressing YES/NO button"));
				return STATE_NULL;
			}
			//end 176339
			else
			{
				trace(L6, _T("SMScanAndBag::Initialize -- No returning #3"));				
			}
			
		}
		// - 154609 Items not transferring after audit		
        
	}
	//+NextGen due to high value prompts and POS is already went to finished basket transfer screen but got dismissed due to the prompts, return to Recalling Items state so we can show FinishRecall screen
	else if(bFinishRecall && !g_bInRecallMode)
	{
		RETURNSTATE(RecallingItems)
	}
	//-NextGen

	pRetState = SMScanAndBagBase::Initialize();

	// Start CRD 558065, 574057 - move here to display ScanAndBag first
	if(ProcessCitiApplicationDetails())
	{
		RETURNSTATE(Processing)
	}
	// End CRD 558065

    // Tell RCM of the true state of the suspend button - base criteria is incorrect
    ra.OnCanSuspendNow( CanSuspendTransaction() ) ;  

    //POS240201 use wicmode bmp - rrs
    if( GetHookProperty(_T("PropTransMode")).Find(_T("wicmode")) != -1 )
    {
        // POS29613 - Move this method below as we only need to change the button when the wic-cm feature is turned on - rrs
        // POS19072: In WIC transactions, label button #5 as 'Apply Promotional Items'.
        // ps.SetButtonText(_T("CMButton5"), SSF_BTT_APPLYPROMOTIONALITEMS);
        // POS28888: Move changes for POS28366 & POS28367 in here to make sure buttons are properly set in all situations.
        // POS28366 - retrieve the wicebt cm config from appini
        CString strSectionName = _T("WICEBT");
        CString strEntryName = _T("WICEBTEnableCM");	    
    	SendTBCommand(TBCMD_GET_APPINI_ENTRY_VAL, (const TCHAR *)strSectionName, 
	     			  (const TCHAR *)strEntryName, &strWICEBTCMFeature);
        // e - POS28366

        // POS28367 - retrieve the wic rebate check config from appini
        strEntryName = _T("WICEBTSplitTender");	    
    	SendTBCommand(TBCMD_GET_APPINI_ENTRY_VAL, (const TCHAR *)strSectionName, 
	    			  (const TCHAR *)strEntryName, &strWICEBTCheckFeature);
        // e POS28367

        if ( (GetHookProperty(_T("WICFreeItemPhaseActive")) == _T("1")) )
        {
            // POS19072: 'Free Item' phase is active, so setup Scan&Bag screen properly:
            //  -> Title = Scan Promotional Items
            //  -> 'Cancel Items' Button (#3): Hide because voiding is not allowed during Free Item phase.
            //  -> 'Apply Promotional Items' Button (#5): Hide because it has already been used once.
            ps.LeadThruText(SSF_LTT_SCANPROMOTIONALITEMS);
            m_pAppPSX->SetConfigProperty (_T("CMButton3Global"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
            m_pAppPSX->SetConfigProperty (_T("CMButton5"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_FALSE);

            //POS25844 - still display the Apply Rebate check button so this line would need to be commented out - rrs
            //m_pAppPSX->SetConfigProperty (_T("CMButton6"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_FALSE); // POS18905 hide the Apply Rebate Check
        }
        else
        {
            ps.LeadThruText(SSF_MSG_SCAN_WIC_ITEMS);            

            // POS19072: Make sure the "Cancel Items" button #3 is visible.
            m_pAppPSX->SetConfigProperty (_T("CMButton3Global"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_TRUE);

            // POS29613 -  only display the wic cm button when the appini option is turned on  rrs
            if( strWICEBTCMFeature == _T("1") )
            {
                ps.SetButtonText(_T("CMButton5"), SSF_BTT_APPLYPROMOTIONALITEMS);
            
                if ( (GetHookProperty(_T("CustomerHasWICFreeItems")) == _T("1")) )
                {
                    // POS19072: Customer has already indicated they have free/promotional items or the feature is off, so disable the 'Apply Promotional Items' Button (#5).
                    m_pAppPSX->SetConfigProperty (_T("CMButton5"), _T("ScanAndBag"), UI::PROPERTYSTATE, ( long ) UI::Disabled);
                }
                else
                {
                    // POS28888: The feature is on, but the customer has not indicated they have free/promotional items, so enable the 'Apply Promotional Items' Button (#5).
                    m_pAppPSX->SetConfigProperty (_T("CMButton5"), _T("ScanAndBag"), UI::PROPERTYSTATE, ( long ) UI::Normal);
                }
            }
            else
            {
                ps.SetButtonText(_T("CMButton5"), SSF_BTT_PRINTGIFTRECEIPT);
            }
            //e pos29613
            /* POS24632 - Remove the Rebate Check functionality
            // POS18905 - Change the button text to 'Apply Rebate Check'

            CString csPropRebateCheck = _T("PropHasRebateCheck");
	        CString csPropRebateCheckValue = _T("0");

            // POS29613 only display the rebate check when the appini option is turned on - rrs
            if (strWICEBTCheckFeature != _T("") )
            {
                if( GetHookProperty(_T("PropHasRebateCheck")) == _T("0") )  
                {
                    ps.SetButtonText(_T("CMButton6"), SSF_BTT_APPLYREBATECHECK);
                    COleVariant v = (long)UI::Normal;
                    
	                m_pAppPSX->SetConfigProperty (_T("CMButton6"), _T("ScanAndBag"), UI::PROPERTYSTATE, v);            
                } 
                else
                {
                    ps.SetButtonText(_T("CMButton6"), SSF_BTT_APPLYREBATECHECK_YES);
                    COleVariant v = (long)UI::Disabled;
	                m_pAppPSX->SetConfigProperty (_T("CMButton6"), _T("ScanAndBag"), UI::PROPERTYSTATE, v);
                }
                // e POS18905
            }
            // e - pos29613
            e POS24632 rrs */
        }

        //POS28888: Removed changes for POS28366 & POS28367 because they have been moved above so that the 'Apply Promotional Items' & 'Apply Rebate Check' buttons are properly set in all situations.

    }
    else
    {
        // POS19072: In typical transactions, label button #5 as 'Print Gift Receipt' and make sure the "Cancel Items" button #3 is visible.
        ps.SetButtonText(_T("CMButton5"), SSF_BTT_PRINTGIFTRECEIPT);

        //POS36316 - Don't do anything with cancel item button state if there is a partial FS tender as the base has disabled the button - rrs
        if (!bPartialTenderWithFoodStamps )
        {
            m_pAppPSX->SetConfigProperty (_T("CMButton3Global"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
        }
        //e POS36316 - Don't do anything with cancel item button state if there is a partial FS tender as the base has disabled the button - rrs

        /* POS24632 Remove the Rebate Receipt Functionality
        //POS25844 - rrs
        ps.SetButtonText(_T("CMButton6"), SSF_BTT_PRINTREBATERECEIPT);
        COleVariant v = (long)UI::Normal;
	    m_pAppPSX->SetConfigProperty (_T("CMButton6"), _T("ScanAndBag"), UI::PROPERTYSTATE, v);
        // e - POS25844 - rrs
        e POS24632 rrs */

        // pos28366 - reenable the gift receipt button
        COleVariant v = (long)UI::Normal;
        m_pAppPSX->SetConfigProperty (_T("CMButton5"), _T("ScanAndBag"), UI::PROPERTYSTATE, v);
        // e pos28366
    }

	
    //SOTF 7745
    if (co.IsShowVolumeControl() )
    {
        m_pAppPSX->SetConfigProperty(_T("ButtonVolumeControl"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty(_T("CmdBtn_ButtonVolumeControl"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);
    }else
    {
        m_pAppPSX->SetConfigProperty(_T("ButtonVolumeControl"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty(_T("CmdBtn_ButtonVolumeControl"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);

    }
    //sotf

    //POS24632 display the UseOwnBags with CMButton6
    if(!co.fOperationsCustomerBagAllowed || !_tcsicmp(co.csHWType, _T("SS90"))) //Don't enable for SS90
    {        
	    m_pAppPSX->SetConfigProperty (_T("CMButton6"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
    }
    else
    {
        COleVariant v = (long)UI::Normal;
	    m_pAppPSX->SetConfigProperty (_T("CMButton6"), _T("ScanAndBag"), UI::PROPERTYSTATE, v);
    }
    // e POS24632

    //POS31054 - Turn on the yellow light all the time for the Mobile SSCO transaction
    if( m_bNeedYellowLightForMobile )
    {
        dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                                        DM_TRICOLORLIGHT_ON,
                                        DM_TRICOLORLIGHT_NORMALREQUEST);
    }
    // e POS31054


// POS138636
    //POS153822 - still enable the WIC button after item sold as the DF Electronic WIC button is still enabled on the POS, remove the lTotalSale>0 from if logic below - rrs

// (+) POS187345
	// Start CRD 125377 
	if (m_bShopAndDropEnabled) 
	{
		SAWLDBScannerDisable(); //CRD 155511
		trace(L6, _T("enable Yes No buttons"));
		if (m_bSNDmPerkEntered)
		{
			if (m_bSNDTMDEntered)
			{
				m_pAppPSX->SetTransactionVariable(_T("ShopAndDropEnabledYesNoButtons"), VARIANT_TRUE); 
			}
			else
			{
				trace(L6, _T("Initialize 1 lSDNRestrictedAgeTimerAmt=%d"), lSDNRestrictedAgeTimerAmt);
				SetTimer(hw, SNDAGE_YESNOBUTTONS, lSDNRestrictedAgeTimerAmt, &SNDYesNoButtonTimeOut);
			}
		}
		else
		{
			trace(L6, _T("Initialize 2 lSDNRestrictedAgeTimerAmt=%d"), lSDNRestrictedAgeTimerAmt);
			SetTimer(hw, SNDAGE_YESNOBUTTONS, lSDNRestrictedAgeTimerAmt, &SNDYesNoButtonTimeOut);
		}
	}
	else 
	{
if( (GetHookProperty(_T("IsWIC_EBT_ON")) == _T("1")) && 
        (GetTBProperty(_T("EnableWICButton")) == _T("Y"))  && _tcsicmp(co.csHWType, _T("SS90"))) //Don't enable for SS90"
	{
    	if(SMState::m_bWICEBTSNBEnable == false)
	    {
        	m_pAppPSX->SetConfigProperty (_T("CMButton8"), _T("ScanAndBag"), UI::PROPERTYSTATE, ( long ) UI::Disabled);
	    }
		else
	    {
	        m_pAppPSX->SetConfigProperty (_T("CMButton8"), _T("ScanAndBag"), UI::PROPERTYSTATE, ( long ) UI::Normal);
	    }
		trace(L6, _T("WIC Feature enabled in POS and in FastLane")); 
	}
	else
	{
		 m_pAppPSX->SetConfigProperty(_T("CMButton8"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		trace(L6, _T("WIC Feature disabled either in POS or FL."));
	}
	}
	// End CRD 125377 
// (-) POS187345
// POS138636

	// start RFQ 9545
	if (g_bInRecallMode)
	{	
		m_pAppPSX->SetConfigProperty (_T("CMButton7"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        //POS33288 Use mPerks Button Video contrl to display the shimmering mperks effect - rrs
        m_pAppPSX->SetConfigProperty (_T("mPerksButtonVideo"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_FALSE);	
        // e POS33288
		
        //POS34296 - Dont Display the static mperks image if POS turned off mperks feature - rrs
        m_pAppPSX->SetConfigProperty( _T("ScanBagImage"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_FALSE );	
	}
	// end RFQ 9545
	
	//(+) CRD124746
	if(SMState::m_bActivateHHScanner)
	{
	  StartHHScannerTimer();
	  trace(L6, _T("*** Scan and bag started timer intialize"));
	}
	//(-) CRD124746
	
	SetHookProperty(_T("IsInStoreMode"),_T("0"));	//CRD 232267

	UpdateVoidRemoveButtonState(csPOSState);

    return pRetState;
}
#endif

void SMScanAndBag::UnInitialize(void)
{
	//(+) CRD124746
	StopHHScannerTimer();
	trace(L6, _T("*** Scan and bag killing timer unintialize"));
	//(-) CRD124746
	// Start CRD 125377 
	if (m_bShopAndDropEnabled) 
	{
		trace(L6, _T("Kill SNDAGE_YESNOBUTTONS timer"));
		KillTimer(hw,SNDAGE_YESNOBUTTONS); 

		//m_pAppPSX->SetConfigProperty(_T("CMButton2KeyInCode"), _T("ScanAndBag"), UI::PROPERTYSTATE, (long) UI::Disabled);
		//m_pAppPSX->SetConfigProperty(_T("CMButton2KeyInCode"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		
		//m_pAppPSX->SetConfigProperty(_T("CMButton8"), _T("ScanAndBag"), UI::PROPERTYSTATE, (long)UI::Disabled);
		//m_pAppPSX->SetConfigProperty(_T("CMButton8"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	}
    SMScanAndBagBase::UnInitialize();
}

SMStateBase *SMScanAndBag::OnPLAHandInView()
{
    return SMScanAndBagBase::OnPLAHandInView();
}

SMStateBase * SMScanAndBag::OnWtDecrease(void)
{
    return SMScanAndBagBase::OnWtDecrease();
}

SMStateBase * SMScanAndBag::OnWtIncreaseNotAllowed(void)
{
    return SMScanAndBagBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMScanAndBag::OnMatchedWt(void)
{
    return SMScanAndBagBase::OnMatchedWt();
}

bool SMScanAndBag::PSAssistMenuLegalForState()
{
    return SMScanAndBagBase::PSAssistMenuLegalForState();
}

SMStateBase * SMScanAndBag::PSButton1(void)
{
	if (g_bInRecallMode)
	{
		trace(L6, _T("Ignore buton press in FLMS"));
		return STATE_NULL;
	}
	else
	{
		return SMScanAndBagBase::PSButton1();
	}
}

SMStateBase * SMScanAndBag::PSButton2(void)
{
	trace(L6, _T("SMScanAndBag::PSButton2"));

	if (m_bShopAndDropEnabled) // Yes button pressed on screen and PrintGiftReceipt enabled
	{
		trace(L6, _T("SMScanAndBag::PSButton2 shop and drop is enabled"));
		m_bSNDSettingAge = true;			// CRD 125377
		
		CString csTBState(_T(""));
		SendTBCommand(TBCMD_GET_TB_STATE, &csTBState);
		CString csPOSState(_T(""));
		SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);
		
		trace(L6, _T("csPOSState=%s csTBState=%s m_bSNDAllowItemMenu=%d"), csPOSState, csTBState, m_bSNDAllowItemMenu);
		
		SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F8}{F1}")));    
		
		return STATE_NULL;
	}

	if (g_bInRecallMode)
	{
		trace(L6, _T("Ignore buton press in FLMS"));
		return STATE_NULL;
	}
	else
	{
		return SMScanAndBagBase::PSButton2();
	}
}

//////////////////////////////////////
// PSButton3 - Cancel Items
//////////////////////////////////////
SMStateBase * SMScanAndBag::PSButton3(void)
{
	trace(L6, _T("SMScanAndBag::PSButton3"));

	// Start CRD 125377 
	if (m_bShopAndDropEnabled) 
	{
		trace(L6, _T("SMScanAndBag::PSButton3 - cancel order button pressed"));
		m_bSNDxCancelTrans = true;		// CRD 125377
		return TBCancelTransaction(true);
		// CREATE_AND_DISPENSE(VoidTransApproval)(BES_SCANANDBAG, BES_TAKERECEIPT);
		//RETURNSTATE(SmAbort)
	
		//setAnchorState(BES_SCANANDBAG);
		//RETURNSTATE(RestrictedAgeYN)
	}
	// End CRD 125377 
    SMStateBase *pRetState = SMScanAndBagBase::PSButton3(); 
	
	// SOTF6400
	if (DigitalCouponActiveWithNoItems ())
	{
		delete getRealStatePointer(pRetState);
		return TBCancelTransaction(false);
	}
	else 
	{
		//+SOTF 7555
		if(IsTrxnCompletelyClean())
		{
			delete getRealStatePointer(pRetState);
			SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1); 
			bIsInFinalization = true; //tar 194770
			return TBFinish();
		}
		//-SOTF 7555
		return pRetState;
	} 
}

#ifndef _TRADESHOWDEMO
SMStateBase * SMScanAndBag::PSButton4(void)
{
    // POS12088
    // Tell security that we are exiting form scan& bag state
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
	lEOT_LGW = g_lLGW; //tar 194770
    // e - POS12088
	
	// POS19072: Start
    //POS240201 use wicmode bmp - rrs
	if ( (GetHookProperty(_T("PropTransMode")).Find(_T("wicmode")) != -1)   &&
		(GetHookProperty(_T("CustomerHasWICFreeItems")) == _T("1"))     &&
		(GetHookProperty(_T("WICFreeItemPhaseActive")) == _T("0")) &&
		(lTotalSale > 0) // POS 29825 - only go to free item when the total trx is > 0 rrs
		)
	{
		// Finish&Pay button tapped for first time during WIC EBT transaction, and
		// customer has free/promotional items, so force sales app into the WIC Free Item phase.
		SendTBCommand(TBCMD_ENTERING_WIC_FREE_ITEM_PHASE, true);
		
		// Wait until sales app enters the WIC Free Item phase, at which time the Scan&Bag's 
		// initialization routine will take care of setting up the 'Scan Promotional Items' screen.
		RETURNSTATE(InProgress)
	}
	// POS19072: End
	
	return TransitionToPayment(true);
}  
#endif
/*
---------------------------------------------------------------------------------------------------
 Handle the PrintGiftReceipt button in ScanAndBag.

 Set the GiftReceiptPrinting flag to true, and display information screen.
---------------------------------------------------------------------------------------------------
*/
SMStateBase * SMScanAndBag::PSButton5(void)
{
	trace(L6, _T("SMScanAndBag::PSButton5"));

	// Start CRD 125377 
	if (m_bShopAndDropEnabled) // Need Language button to work
	{
		return SMScanAndBagBase::PSButton5();	
	}
	// end CRD 125377 
    // POS12088
    // Tell security that we are exiting form scan& bag state
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
	// e - POS12088
	
    // POS29613: only display WIC CM button when the appini option is turned on
    //POS240201 use wicmode bmp - rrs
    if ( strWICEBTCMFeature == _T("1") && GetHookProperty(_T("PropTransMode")).Find(_T("wicmode")) != -1 )
	{
		// POS19072: Disable "Apply Promotional Items" button because it has been tapped and is
		// no longer needed in this transaction.
		COleVariant v = (long)UI::Disabled;
		m_pAppPSX->SetConfigProperty(_T("Button5"), _T("ScanAndBag"), UI::PROPERTYSTATE, v);
		
		// POS19072: Indicate that customer has promotional/free items to apply.
		SetHookProperty(_T("CustomerHasWICFreeItems"), _T("1"));
		
		// POS19072: Display 'Are you finished entering your WIC eligible items?' screen to customer.
		RETURNSTATE(ApplyPromotionalItems)
	}
	else
	{
		// trigger the event
		SendTBCommand(TBCMD_SET_TB_STATE,_T("GIFTRECEIPT_FLAGGED"));
	}
	
	return STATE_NULL;
}

/*
---------------------------------------------------------------------------------------------------
 POS24632 - Replace the PrintRebateReceipt functionality with the Use Own Bag function
---------------------------------------------------------------------------------------------------
*/
SMStateBase * SMScanAndBag::PSButton6(void)
{
	trace(L6, _T("SMScanAndBag::PSButton6"));
    trace(L6, _T("+SMScanAndBag: Use Own Bags bttn pressed"));
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag"));
	
    if(!co.IsSecurityBypassOn())
    {
        CKeyValueParmList parmList;
        CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_CUST_HAS_BAG);
        _bstr_t bstrParmList((const _TCHAR *)parmList);
        BSTR bstrResult;
        bstrResult = NULL;
        SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
        SysFreeString(bstrResult);
    }
    
    if(!fSAInTransaction)
        SASendSCOTInput( ITEMIZATION );
	
	//395551+
	setAnchorState(BES_PUTBAGONSCALE);
	//395551-
	g_savedAnchorStateFromAttract = BES_PUTBAGONSCALE; //416582
	
    return setAndCreateAnchorState(BES_PUTBAGONSCALE);
}

//---------------------------------------------------------------------------------------------------
// Trigger the Telephone Entry Screen - SOTF5803
//---------------------------------------------------------------------------------------------------
SMStateBase * SMScanAndBag::PSButton7(void)
{
	trace(L6, _T("SMScanAndBag::PSButton7"));
    //POS37599 disable RAP AM
    ra.RequestEnableAssistMode(false);
    //e POS37599
	
    // POS12088
    // Tell security that we are exiting form scan& bag state
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
	// e - POS12088
	
	// SOTF6353 - Disable button after press to prevent multiple key presses; other race conditions
	COleVariant v = (long)UI::Disabled;
	m_pAppPSX->SetConfigProperty (_T("CMButton7"), _T("ScanAndBag"), UI::PROPERTYSTATE, v);
	
    // POS33288 rrs
    m_pAppPSX->SetConfigProperty (_T("mPerksButtonVideo"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_FALSE);	
    // e POS33288
	
	// SOTF6400 - Trigger the event 
	SendTBCommand(TBCMD_SET_TB_STATE,_T("TB_DIGITAL_COUPON"));
	
    // POS47402 - set the state to InProgress to prevent accidental button click while waiting the POS to change its menu - rrs
	RETURNSTATE(InProgress);
}

SMStateBase * SMScanAndBag::DMScanner(void)
{
    return SMScanAndBagBase::DMScanner();
}

//SOTF7745
SMStateBase * SMScanAndBag::PSCmdButton(CString csContext, CString csControl, CString csCommand)
{
	// SSCOI-45354
	// This is a migration issue. Removing code below after corrected the migration error.
	//CLaneStateData laneStateData(m_pAppPSX);
	//laneStateData.SendNextGenUIData(_T("RemoteProxyUserInputCompleted"), true);
	// -SSCOI-45354
	trace(L6, _T("SMScanAndBag::PSCmdButton csContext=%s csControl=%s  csCommand=%s"), csContext, csControl,  csCommand);
    if(csControl == _T("CmdBtn_ButtonVolumeControl"))
    {
		//SSCO base volume control button
		return  SMScanAndBagBase::PSButton7();
		
    }
	// Start CRD 125377 
    else if(csControl == _T("SND_YesButton"))
	{
		if (m_bShopAndDropEnabled) // Yes button pressed on screen and PrintGiftReceipt enabled
		{
			m_bSNDSettingAge = true;			// CRD 125377
			
			CString csTBState(_T(""));
			SendTBCommand(TBCMD_GET_TB_STATE, &csTBState);
			CString csPOSState(_T(""));
			SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);
			
			trace(L6, _T("csPOSState=%s csTBState=%s m_bSNDAllowItemMenu=%d"), csPOSState, csTBState, m_bSNDAllowItemMenu);
			
			SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F8}{F1}")));    
			
			return STATE_NULL;
		}
		else
		{
			return  SMScanAndBag::PSCmdButton(csContext, csControl, csCommand);
		}
	}
	else if(csControl == _T("SND_NoButton"))
	{
		if (m_bShopAndDropEnabled) // No button pressed on screen and WicEbt button enabled to use button 8
		{
			trace(L6, _T("going to Payment"));
			return setAndCreateAnchorState(BES_FINISH);
		}
		else
		{
			return  SMScanAndBag::PSCmdButton(csContext, csControl, csCommand);
		}
	}
	// end CRD 125377
	// Start CRD 525299
	else if(csControl == _T("CmdBtn_ApplyMeijerCard"))
	{
		RETURNSTATE(ShowExternalUrl);
	}
	// End CRD 525299
	else
    {
		
		return  SMScanAndBag::PSCmdButton(csContext, csControl, csCommand);
    }
}
//sotf

SMStateBase * SMScanAndBag::DMCardReader(void)
{
    return SMScanAndBagBase::DMCardReader();
}

SMStateBase * SMScanAndBag::DMAcceptor(void)
{
 
  //+SSCOI-45214
  return SMScanAndBagBase::DMAcceptor();
  /* trace(L6,_T("DMAcceptor"));

  //+SR078 Send Clear Allow Late Bagging message to SM
  if(g_bAllowLateBagging)
  {
	  CKeyValueParmList parmList;
	  CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_CLEAR_ALLOW_LATE_BAGGING);
	  _bstr_t bstrParmList((const _TCHAR *)parmList);
	  BSTR bstrResult;
	  bstrResult = NULL;
	  SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
	  SysFreeString(bstrResult);
  }
  //-SR078
  */
  // SSCOP-3604+ taking out the fix for 411550
  /* +TAR 411550
  if(lBalanceDue <= 0)
  {
    //return SMStateBase::DMAcceptor();     // Reject.
	#ifdef _BUILDLIB_
	    return SMStateBase::DMAcceptor();
	#else
	    return SMState::DMAcceptor();
	#endif 
  }
  // -TAR 411550
  */
  // SSCOP-3604+

  // Tell security that we are exiting form scan& bag state
  /*TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
  lEOT_LGW = g_lLGW; //tar 194770*/
	// TAR#120988 Unknown item approval on RAP side needs to reset the globals to zero  LPM111299
 /* SCOTDO_APPROVAL_INFO ApprovalInfo;
  ra.GetApprovalInfo(&ApprovalInfo);
  if ((lUnknownItemsKeyed > 2) && (!ApprovalInfo.bKeyedUnknown))
  {
	lUnknownItemsKeyed = 0;
	lUnknownItems = 0;
  }*/

  //ps.HideButtons();
  //+SR713
  //if(co.fStateCheckBasketReminder)
  //	  RETURNSTATE(CheckBasketReminder)
  //else
  /*bIsInPreTenderState = true;

	SMStateBase *retState = TBTenderSelected(TB_TENDER_CASH);
	if(retState != STATE_NULL)
		return retState;
  
  if (nDMCashInserted<lBalanceDue)
  {
    RETURNSTATE(CashPayment)
  }

  ps.ShowAmountDueAmountPaid(lBalanceDue, lTotalPaid + nDMCashInserted);
  return TBEnterTender(TB_TENDER_CASH, nDMCashInserted);
  //return setAndCreateAnchorState(BES_FINISH);
  */
  //-SSCOI-45214
}

SMStateBase * SMScanAndBag::DMScale(void)
{
    return SMScanAndBagBase::DMScale();
}

SMStateBase * SMScanAndBag::PSButtonHelp(void)
{
    return SMScanAndBagBase::PSButtonHelp();
}

SMStateBase * SMScanAndBag::DMTakeawayBagAreaBackup(bool isBackedUp)
{
    return SMScanAndBagBase::DMTakeawayBagAreaBackup(isBackedUp);
}

SMStateBase * SMScanAndBag::PSNumericKey(const long lKey)
{
    return SMScanAndBagBase::PSNumericKey(lKey);
}

SMStateBase * SMScanAndBag::PSEnterKey(void)
{
    return SMScanAndBagBase::PSEnterKey();
}

SMStateBase * SMScanAndBag::PSClearKey(void)
{
    return SMScanAndBagBase::PSClearKey();
}

void SMScanAndBag::InitializeBasedOnOption()
{
    SMScanAndBagBase::InitializeBasedOnOption();
}

void SMScanAndBag::FlashButtonsOnWeight(bool bFlashOn)
{
    SMScanAndBagBase::FlashButtonsOnWeight(bFlashOn);
}

SMStateBase * SMScanAndBag::TimedOut(void)
{
    SendTBCommand(TBCMD_CHECK_FORCE_LOAD, NULL);
    return SMScanAndBagBase::TimedOut();
}
void SMScanAndBag::ChangeLanguage(SCOT_LANGUAGE_TYPE langNew)
{
    SMScanAndBagBase::ChangeLanguage(langNew);
}

SMStateBase *SMScanAndBag::OnLookupComplete (long byteCount)
{
    return SMScanAndBagBase::OnLookupComplete ( byteCount);
}

SMStateBase * SMScanAndBag::PSListLookup(CString csItemInfo)
{
// +SSCOI-44479
	CLaneStateData laneStateData(m_pAppPSX);
    laneStateData.SendNextGenUIData( _T("IsQuickPick"), true);
#ifndef _CPPUNIT
	//+SR078 Send Clear Allow Late Bagging message to SM
	if(g_bAllowLateBagging)
	{
	  CKeyValueParmList parmList;
	  CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_CLEAR_ALLOW_LATE_BAGGING);
	  _bstr_t bstrParmList((const _TCHAR *)parmList);
	  BSTR bstrResult;
	  bstrResult = NULL;
	  SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
	  SysFreeString(bstrResult);
	}
	//-SR078
#endif //_CPPUNIT
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
		if (co.GetfOperationsAllowMultiPick())
		{
			fInMultiSelectPickList = true;
			m_pAppPSX->ClearReceipt(_T("PickListReceipt")); 
			m_pAppPSX->ClearReceipt(_T("PickListReceiptVoid"));
		}
	}

	CStringArray csParentCategories;

	if (csIsItem.CompareNoCase(_T("True")) == 0) //sellable item
	{
#ifndef _CPPUNIT
		ps.SetItemDesc(csDesc);

		if( csItemSent==_T("") ) //fail in picking list
		{
			DMSayPhrase(PICKLISTINVALID);
			return STATE_NULL;
		}

        TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
		 //+ Dynamic Quickpick
        if ( _ttoi(custInfo.GetOption(_T("PersonalizedPicklistItems"))) > 0 )
		{  
            CString csQuickPickItemCode;

            csQuickPickItemCode = csItemSent;   
            g_csQuickPickItemSold.Add(csQuickPickItemCode);
            g_bHaveQuickPick = true;
            g_bHaveSoldFromQuickPick = false;
		}
        //- Dynamic Quickpick

#endif //_CPPUNIT
		if (VerifyQtyItem(csItemSent))     
		{
			io.Reset();
			
			// Saved for dynamic picklist purposes //
			SMStateBase::ms_bIsCurrentItemFromPickList = true;
			
			io.csOrigCode = csItemSent;	   // item code keyed
			io.csItemCode = csItemSent;	   // assumed to be the same
            if (!co.IsPicklistAssistEnable()) // dont Send SASendScot when PLA is enable, Let TBItemSale called by PLAEnterWeight do
            {
                SASendSCOTInput( UPC_MANUAL, csItemSent);
            }
			
            SetNextGenUICurrentItem(io.csOrigCode);
		
			// +SSCOI-44479
			 return TBItemSale( FALSE, 0, false, true ); 	
			//RETURNSTATE( EnterQuantity ) 	
			// -SSCOI-44479
		}

        if (co.IsPicklistAssistEnable() && ps.IsPickListItem(csItemSent))
        {
            io.Reset();
            io.csOrigCode = csItemSent;
            io.csItemCode = csItemSent;
            RETURNSTATE(PLAEnterWeight)
        }
        else
        {
            return TBItemSale( FALSE, 0, false, true );         //Sell item
        }
	}
	else if (csSubCategory.CompareNoCase(_T("True")) == 0) //subcategory item
	{
#ifndef _CPPUNIT
	    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
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

#endif // _CPPUNIT
		RETURNSTATE( ProduceFavorites )
	}
	else //category item
	{
#ifndef _CPPUNIT
        TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
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
#endif // _CPPUNIT
		RETURNSTATE( ProduceFavorites )
	}
   // return SMScanAndBagBase::PSListLookup(csItemInfo);

// -SSCOI-44479
}

long SMScanAndBag::DMSayPhrase(LPCTSTR szWave)
{
    return SMScanAndBagBase::DMSayPhrase(szWave);
}

#ifndef _TRADESHOWDEMO
long SMScanAndBag::DMSayPhrase(const int nMsg)
{
    return SMScanAndBagBase::DMSayPhrase(nMsg);
}
#endif

long SMScanAndBag::DMSaySecurity(const long nMsg, LPCTSTR szWave, HANDLE hWaitEvent/*=0*/)
{
    return SMScanAndBagBase::DMSaySecurity(nMsg, szWave, hWaitEvent);
}

void SMScanAndBag::TBGetItemDetails(ItemDetails &iDetails)
{
    SMScanAndBagBase::TBGetItemDetails(iDetails);
}

void SMScanAndBag::ProcessWeightOnScale(void)
{
    SMScanAndBagBase::ProcessWeightOnScale();
}

SMStateBase * SMScanAndBag::TransitionToPayment(bool bTBWillDriveToPayment)
{
    //Determine CheckBasketReminder for TAB setting
	CString strTABCheckBasket = GetTBProperty(_T("PromptCheckBasketOnTab"));
	CString strIsTAB = GetTBProperty(_T("IsTABUnit"));
	
	if(strTABCheckBasket == _T("Y") && strIsTAB == _T("1"))
		SMCheckBasketReminder::bCheckTAB = true;
	
	if((SMCheckBasketReminder::bCheckTAB || co.fStateCheckBasketReminder) && lItems)
    {
		RETURNSTATE(CheckBasketReminder)
    }
	else
	{
		// SOTF6400
		if (DigitalCouponActiveWithNoItems ())
		{
			return TBCancelTransaction(false);
		}
		else
		{
            //+SOTF 7555
            if(IsTrxnCompletelyClean())
            {
                SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1); 
                bIsInFinalization = true; 
                return TBFinish();
            }
            //-SOTF 7555
			
            //SOTF 6226
            //if we have any approvals needed we need to handle
            //them first   
			
			bIsInFinalization = true; //tar 223051            
			
            // POS18905
            if( !isAnyApprovalRequired() && GetHookProperty(_T("PropHasRebateCheck")) == _T("1") && lBalanceDue > 0)
            {
                CString sDesc;
                sDesc.Format(ps.GetPSText(RA_ASSISTED_TENDER,SCOT_LANGUAGE_PRIMARY), ps.GetPSText(SSF_BTT_REBATECHECK,SCOT_LANGUAGE_PRIMARY));
                int nOtherPaymentIndex = GetButtonIndex(TB_TENDER_ASSISTTENDER);
				
                SetTenderButton(nOtherPaymentIndex);
                nTenderType = TB_TENDER_ASSISTTENDER;
                setAnchorState(BES_FINISH);
                trace(L6, _T("RebateCheck Desc (%s)"), sDesc);
                CREATE_AND_DISPENSE(AssistedTender)(sDesc);
            }
            // e pos 18905
			
			if(( bTBWillDriveToPayment ) && (!isAnyApprovalRequired(false)))
			{
				// let's have TB drive to payment before transitioning FL
				SendTBCommand(TBCMD_ENTERING_FINISH_AND_PAY, true);
				setAnchorState(BES_FINISH);
                //SOTF 8036
				//RETURNSTATE(InProgress);
                CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(), nTimeOut);
				
				
			}
			else
			{
				return setAndCreateAnchorState(BES_FINISH);
			}
		}
	}
}

//POS15766 - Implement TBParse to enable-disable mPerks button dynamically
SMStateBase *SMScanAndBag::TBParse(MessageElement *me)
{	
    // return SMScanAndBagBase::TBParse(me);
    SMStateBase *pState = SMScanAndBagBase::TBParse(me);
	CString csPOSState = GetTBProperty(_T("SavedLastPosState"));
	CString csWaitingForPOS = GetTBProperty(_T("xWaitForPosScannerReady"));


	//check mPerks tb_int 764 and update button state (this ACS context might arrive late after S&B initialization at times)
	COleVariant vType( _T("") );
	if (PSX_SUCCESS == m_pAppPSX->GetConfigProperty(_T("CMButton7"), _T("ScanAndBag"), UI::PROPERTYSTATE, vType))
	{
		if(_T("1") == GetHookProperty(_T("EnableDigitalCoupons")))
		{
			if (vType.lVal != UI::Normal)
			{
				//enable the button
				UpdatemPerksButton(true);
			}
		}
		else
		{
			if (vType.lVal != UI::Disabled)
			{				
				//disable the button
				UpdatemPerksButton(false);
			}
		}
    }

	if(!g_bInRecallMode && bPOSatmPerksPromotions && csPOSState.Find(_T("ITEMMENU_DK")) != -1) //SCO has kept the scanner disabled to prevent scanning while promotion prompts are acknowledged. Now we're ready to enable the scanner
	{
		trace(L6, _T("Enabling the scanner and resetting bPOSatmPerksPromotions"));
		if(!dm.IsScannerEnabled())
		{
			bEASReEnableScanner = true;
			SAWLDBScannerEnable();
		}
		bPOSatmPerksPromotions = false;
	}
	
	CString csTBState(_T(""));
    SendTBCommand(TBCMD_GET_TB_STATE, &csTBState);

	trace(L3, _T("*** SMScanAndBag::TBParse - after calling base TBParse: %d PosState = %s xWaitForPosScannerReady = %s"), m_TBLastState, csPOSState, csWaitingForPOS);
	if (m_TBLastState == TB_POS_SCANNER_READY) //&& (GetHookProperty(_T("__POSState")).Find(_T("ITEMMENU")) != -1))
    {
		// trace(L6, _T("Ready to send next FLMS"));
		if (g_bInRecallMode)
		{
			if (csPOSState.Find(_T("HIGHVALUE_PROMO_CHOICE")) != -1)
			{
				bHighValueActive = true;
				trace(L6, _T("High Value Ignore in Entering Recall Mode FLMS"));
				return pState;
			}

			// start 140001 
			if (csPOSState.Find(_T("ITEM_NEEDQTY")) != -1)
			{
				bQuantityRequiredActive = true;
				trace(L6, _T("ITEM_NEEDQTY Ignore in Entering Recall Mode FLMS"));
				return pState;
			}
			// end 140001
			if (g_bEnteringRecallMode)
			{
				trace(L6, _T("Ignore in Entering Recall Mode FLMS"));
			}
			/*
			else if (m_bRecallModeWaitforItemSold)
			{
				SMRecallingItems::g_nTimeoutRecallingWaitForPos = 0;  // RFQ 9545
				SMRecallingItems::g_bRecallingWaitForPos = false;	   // RFQ 9545	
				
				SetTBProperty(_T("xWaitForPosScannerReady"), _T("1"));
				trace(L6, _T("Do not process next item until ITEMSOLD processed - in Recall Mode FLMS"));
				return pState;
			}
			*/
			else if(!bFinishRecall)
			{
				SMRecallingItems::g_nTimeoutRecallingWaitForPos = 0;  // RFQ 9545
				SMRecallingItems::g_bRecallingWaitForPos = false;	   // RFQ 9545	

				trace(L6, _T("Recall Mode FLMS"));
				RETURNSTATE(RecallingItems)
			}
			else
			{
				trace(L6, _T("Ignore TB_POS_SCANNER_READY: bFinishRecall - %d"), bFinishRecall);
			}
		}
    }
	//CRD 290267 an item has been sold and we are still in RecallMode so we have to return to Recalling State to dismiss custom messages and show recalling screen
	else if(m_TBLastState == TB_ITEMSOLD && g_bInRecallMode)
	{
		RETURNSTATE(RecallingItems)
	}
	// start 140001
	else if ((bQuantityRequiredActive) && (m_TBLastState == 11) && (csPOSState.Find(_T("ITEMMENU")) != -1)) 
	{
		bQuantityRequiredActive = false;
		if (g_bInRecallMode)
		{
			SMRecallingItems::g_nTimeoutRecallingWaitForPos = 0;  // RFQ 9545
			SMRecallingItems::g_bRecallingWaitForPos = false;	   // RFQ 9545	
			
			trace(L6, _T("Quantity Required Recall Mode FLMS"));
			RETURNSTATE(RecallingItems)
		}
    }
	// start 140001
	else if ((bQuantityRequiredActive) && (m_TBLastState == 11) && (csPOSState.Find(_T("ITEMMENU")) != -1)) 
	{
		bQuantityRequiredActive = false;
		if (g_bInRecallMode)
		{
			SMRecallingItems::g_nTimeoutRecallingWaitForPos = 0;  // RFQ 9545
			SMRecallingItems::g_bRecallingWaitForPos = false;	   // RFQ 9545	
			
			trace(L6, _T("Quantity Required Recall Mode FLMS"));
			RETURNSTATE(RecallingItems)
		}
	}
	// end 140001 
	else if (((bHighValueActive) && (m_TBLastState == 11) && (csPOSState.Find(_T("ITEMMENU")) != -1)) ||
		     ((bHighValueActive) && (m_TBLastState == 11001) && (csPOSState.Find(_T("ITEMMENU")) != -1)))
	{
		bHighValueActive = false;
		if (g_bInRecallMode)
		{
			SMRecallingItems::g_nTimeoutRecallingWaitForPos = 0;  // RFQ 9545
			SMRecallingItems::g_bRecallingWaitForPos = false;	   // RFQ 9545	
		
			trace(L6, _T("High Value Recall Mode FLMS"));
			RETURNSTATE(RecallingItems)
		}
	}
	else if ((bHighValueActive) && (m_TBLastState == 11) && (csPOSState.Find(_T("MEP_AVAILABLE")) != -1)) 
	{
		trace(L6, _T("set TB to ITEMIZATION so we can process the extended protection warranty"));
        SendTBCommand(TBCMD_SET_TB_STATE,_T("ITEMIZATION"));
	}
	else if ((csPOSState.Find(_T("ITEMMENU")) != -1) && (csWaitingForPOS == _T("0")) && (m_bRecallModeWaitforItemSold))
	{
		trace(L6, _T("Waiting for POS is zero, and TB State is 11 and POS State is ITEMMENU and m_bRecallModeWaitforItemSold is true. "));
        if (g_bInRecallMode)
		{
			trace(L6, _T("g_bInRecallMode is true"));

			//CRD 293397 - if POS is at Item Menu BUT we are still waiting for mPerks response, do not go to Recalling State
			CString csWaitForMperks = GetHookProperty(_T("RecallModeWaitForMperks"));
			if (g_bEnteringRecallMode)
			{
				trace(L6, _T("Ignore in Entering Recall Mode FLMS g_bEnteringRecallMode is true"));
			}			
			else if(csWaitForMperks == _T("0"))
			{
				SMRecallingItems::g_nTimeoutRecallingWaitForPos = 0;  // RFQ 9545
				SMRecallingItems::g_bRecallingWaitForPos = false;	   // RFQ 9545	
				
				trace(L6, _T("Recall Mode FLMS"));
				RETURNSTATE(RecallingItems)
			}
			else
			{
				trace(L6, _T("IGNORING waiting for mPerks #"));
			}
		} 
		else 
		{
			trace(L6, _T("IGNORING "));
        }
	}
	else if((g_bInRecallMode) &&
			(csPOSState.Find(_T("CM_PROMO_LINKEDITEM")) != -1) &&
			//(csWaitingForPOS == _T("1")) &&
			(m_TBLastState == 11) &&
			!(csTBState.Find(_T("ITEMIZATION_AFTER_SKIPBAGGING")) != -1) &&
			!bIsItemizationAfterSkipBaggingSent)
	{
		bIsItemizationAfterSkipBaggingSent = true;	//set this flag to TRUE when ITEMIZATION_AFTER_SKIPBAGGING has been already sent to avoid double sending when TB_IGNORE is set more than once
		SendTBCommand(TBCMD_SET_TB_STATE, _T("ITEMIZATION_AFTER_SKIPBAGGING"));
		trace(L6,_T("CM_PROMO_LINKEDITEM and recall mode FLM set POS state to ITEMIZATION_AFTER_SKIPBAGGING, csWaitingForPOS = %s"), csWaitingForPOS);
	} //-SSCOI-55023
	
	//Start CRD 269544 - check if POS is still at MEP_AVAILABLE and tb state is at Itemization
	if ( ((csPOSState.Find(_T("MEP_AVAILABLE")) != -1) || (csPOSState.Find(_T("MEP_AVAILABLE_NON_BAG")) != -1)) &&  csTBState.Find(_T("ITEMIZATION")) != -1 && !bIsItemizationAfterSkipBaggingSent) 
	{
		trace(L6, _T("sent itemization after skip bagging"));
		bIsItemizationAfterSkipBaggingSent = true;
		SendTBCommand(TBCMD_SET_TB_STATE,_T("ITEMIZATION_AFTER_SKIPBAGGING"));
	}

	// reset this variable if POS State is back at Item Menu
	if ((bIsItemizationAfterSkipBaggingSent) && (m_TBLastState == 11) && (csPOSState.Find(_T("ITEMMENU")) != -1)) 
	{
		trace(L6, _T("must reset bIsItemizationAfterSkipBaggingSent because POS is already at Item Menu"));
		bIsItemizationAfterSkipBaggingSent = false;
	}
	//End CRD 269544
	
	//CRD 503257 if POS is finally at ITEMMENU and resume completed, proceed to Finish
	if((csPOSState.Find(_T("ITEMMENU")) != -1) && bResumeCompleteWaitingForItemMenu)
	{
		trace(L6, _T("Resume completed and POS is now at Item Menu. Proceed to Payment"));
		bResumeCompleteWaitingForItemMenu = false;
		return TransitionToPayment(false);
	}

	UpdateVoidRemoveButtonState(csPOSState);

	return pState;
}

//POS78131 - SSCOB-1164+
SMStateBase* SMScanAndBag::DMTakeawayReadyForItem()
{
	trace(L6, _T("SMScanAndBagBase::DMTakeawayReadyForItem"));
	return STATE_NULL;
}
// e POS78131 SSCOB-1164-

// POS138636

SMStateBase  *SMScanAndBag::PSButton8(void)
{
	trace(L6, _T("SMScanAndBag::PSButton8"));
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
	if (m_bShopAndDropEnabled) // No button pressed on screen and WicEbt button enabled to use button 8
	{
		trace(L6, _T("going to Payment"));
		return setAndCreateAnchorState(BES_FINISH);
	}
    SetHookProperty(_T("PropFromRTS"), _T("0"));
	//(+) POS189696
	if(SMState::lWICOptionloaded == 2)
	{
		SendTBCommand(TBCMD_SET_TB_STATE,_T("SMART_WIC_START")); 
	}
	//(-) POS189696
	else
	{
		SendTBCommand(TBCMD_SET_TB_STATE,_T("WIC_START"));
	}
	CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(), co.getTimeOutTender());  
	
}

// POS138636

SMStateBase *SMScanAndBag::CUParse(MessageElement *me)
{
    return SMScanAndBagBase::CUParse(me);
}

//UploadTicket Port +
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMScanAndBag::SNDYesNoButtonTimeOut
//
// Purpose:		To enable Yes No Age Restricted buttons in NGUI
// Parameters:	none
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
VOID CALLBACK SMScanAndBag::SNDYesNoButtonTimeOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	m_pAppPSX->SetTransactionVariable(_T("ShopAndDropEnabledYesNoButtons"), VARIANT_TRUE);
}
//#ifdef _SSCOMOBILE
SMStateBase *SMScanAndBag::OnMobileTransactionStart(CString csTransactionInfo)
{
    SMStateBase *retState = STATE_NULL;

    if (csTransactionInfo.Find(MOBILE_START) == 0)
    {
        mp.LogTransition((CString)_T("SMStartMobileReplay"), nTBLastRealState); 
        g_bInRecallMode = true;
        rp.ModeChange(rp.m_lAttendantEntryID, _T("RECALLMODE"), ENTER);

        trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(MSG_RECALLMODE_BEGIN, SCOT_LANGUAGE_PRIMARY));
        ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + ps.GetPSText(MSG_RECALLMODE_BEGIN, SCOT_LANGUAGE_PRIMARY)); // inform RAP about entering/leaving recall mode
        trace(L7, _T("-ra.OnNormalItem"));

        CKEYVALUEPARMLIST parmList;
        CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_ENTER_RECALLMODE);
        _bstr_t bstrParmList((const _TCHAR *)parmList);
        BSTR bstrResult;
        bstrResult = NULL;
        SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
        SysFreeString(bstrResult);
        return STATE_NULL;
    }
    else if (csTransactionInfo.Find(MOBILE_RESCAN_REQUIRED) == 0)
    {
        retState = GenerateRAPDataNeeded(_T(" "), _T(""), ms_MobileAuditDataEntryINI, ms_MobileAuditDataEntryINISection);
    }
    return retState;
}
//UploadTicket Port -

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMScanAndBag::UpdateVoidRemoveButtonState
//
// Purpose:		To hide/show Remove button if the item is voidable
// Parameters:	csPOSState (current POS state)
//
// Returns:		void
// VSTS-267145, 403336, 401635
/////////////////////////////////////////////////////////////////////////////////////
void SMScanAndBag::UpdateVoidRemoveButtonState(CString csPOSState)
{	
	if (!g_bInRecallMode && (csPOSState.Find(_T("ITEMMENU")) != -1))
	{
		CString newItemReceiptId = ps.GetLastItemReceiptEntryId();
		//Get the void button status (hook property label_asa_3) at POS (0 - Enabled, 1 - Disabled)
		CString strVoidLabelPropName = _T("IsItemNotVoidable");
		CString strVoidLabelEnabled;
		SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strVoidLabelPropName, &strVoidLabelEnabled);
		COleVariant varVoidLabelEnabled = strVoidLabelEnabled == _T("0") ? VARIANT_TRUE : VARIANT_FALSE;
		
		//Check if the item on SCO receipt is consistent with POS. If not, update the receipt item status. (This is to eliminate unnecessary duplicate update calls)
		COleVariant vValue;
		vValue.Clear();
		vValue.boolVal = VARIANT_FALSE;
		ps.GetPSXObject()->GetReceiptItemVariable(_T("CMSMReceipt"), newItemReceiptId, _T("ITEM_SELECTABLE"), vValue);
		
		//Get the icon_label_3 text to make sure the label actually correspond to Void Current Item
		CString strLabel3TextPropName = _T("Label3Text");
		CString strLabel3Text;
		SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strLabel3TextPropName, &strLabel3Text);
		
		trace(L8, _T("Receipt item details: newItemReceiptId=<%s>, itemSelectableStatus=<%d>, strLabel3Text=<%s>, strVoidLabelEnabled=<%s>"), newItemReceiptId, varVoidLabelEnabled.boolVal, strLabel3Text, strVoidLabelEnabled);
		if (newItemReceiptId != _T("") && strLabel3Text == _T("Void Current Item"))
		{
			if (varVoidLabelEnabled.boolVal != vValue.boolVal)
			{
				trace(L6, _T("Marking this item: Selectable = <%d>"), varVoidLabelEnabled.boolVal);
				ps.GetPSXObject()->SetReceiptItemVariable(_T("CMSMReceipt"), newItemReceiptId, _T("ITEM_SELECTABLE"), varVoidLabelEnabled.boolVal);
				ps.GetPSXObject()->UpdateReceiptControls(_T("CMSMReceipt"));
			}
		}
	}
}

void SMScanAndBag::UpdatemPerksButton(bool enable)
{
	if(enable)
	{
		m_pAppPSX->SetConfigProperty (_T("CMButton7"), _T("ScanAndBag"), UI::PROPERTYSTATE, (long)UI::Normal);
        m_pAppPSX->SetConfigProperty (_T("mPerksButtonVideo"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
        
        //POS34296 - Display the static mperks image all the time if POS turned on mperks feature - rrs
        m_pAppPSX->SetConfigProperty( _T("ScanBagVideo"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );
        m_pAppPSX->SetConfigProperty( _T("ScanBagTextArea"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );
        m_pAppPSX->SetConfigProperty( _T("ScanBagImage"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE );
	}
	else
	{
		m_pAppPSX->SetConfigProperty (_T("CMButton7"), _T("ScanAndBag"), UI::PROPERTYSTATE, (long)UI::Disabled);
        m_pAppPSX->SetConfigProperty (_T("mPerksButtonVideo"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty( _T("ScanBagImage"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );
	}
}