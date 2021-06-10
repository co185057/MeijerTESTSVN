//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMFinish.CPP
//
// TITLE: Class implementation for Finish State
//
// $Header:
//
// AUTHOR:    Feng Zhang
//
// CHANGE HISTORY
//
// CRD 574057 Name: CJ Estrada Date: Mar 3, 2021
// CRD 554916 Name: CJ Estrada Date: Feb 9, 2021
// CRD 554915 Name: CJ Estrada Date: Feb 08, 2021
// CRD 558065 Name: CJ Estrada Date: Jan 14, 2021
// CRD 525299 Name: CJ Estrada Date: Nov 17, 2020
// POS303460 Work Request:77937 Name:Veerasekhara Dasandam Date: Mar 16, 2017
// POS295147 Work Request:76936 Name:Alex Bronola Date: October 01, 2016
// POS295147 Work Request:76936 Name:Robert Susanto Date: September 30, 2016
// POS294487 Work Request:73599 Name:Alex Bronola Date: September 28, 2016
// POS293942 Work Request:76936 Name:Robert Susanto Date: September 20, 2016
// POS294724 Work Request:769369 Name:Alex Bronola Date: September 28, 2016
// POS294487 Work Request:73599 Name:Alex Bronola Date: September 28, 2016
// POS293942 Work Request:76936 Name:Robert Susanto Date: September 20, 2016
// POS291984/POS293066 Work Request:76936 Name:Alex Bronola Date: September 19, 2016
// POS293066 Work Request:76936 Name:Alex Bronola Date: September 14, 2016
// POS253532 WorkRequest:68941 Name:Saiprasad Srihasam Date:March 18, 2016
// POS223750 Work Request:68941 Name:Robert Susanto Date:Jan 11, 2016
// POS240201 Work Request:68942 Name:Robert Susanto Date: January 7, 2016
// POS227744 Work Request:68940 Name:Robert Susanto Date:Nov 6, 2015
// POS186112 Work Request:66559 Name:Robert Susanto Date:March 12, 2015
// POS138637 Work Request:66559 Name:Kranthi Kumari Cherugondi Date:March 3, 2015
// POS139881 Work Request:65347 Name:Robert Susanto Date:October 23, 2014
// POS72562 WORK REQUEST:56285 Name: Matt Condra Date: August 1, 2013
// POS69170 WORK REQUEST:56285 Name: Robert Susanto Date: June 28, 2013
// POS39476/POS54860 Work Request:52635 Name:Robert Susanto Date:February 12, 2012
// POS39476/POS46373 Work Request:52635 Name:Robert Susanto Date:December 12, 2012
// POS36905/POS42533 Work Request:52635 Name:Robert Susanto Date:October 12, 2012
// POS40495 WORK REQUEST:50864 Name: Robert Susanto Date: August 15, 2012
// POS30981/POS31054 Work Request:50864 Name:Robert Susanto Date:June 29, 2012
// POS35462 WORK REQUEST:19114 Name: Robert Susanto Date: May 31, 2012
// POS30981/POS31054 WORK REQUEST:50864 Name: Robert Susanto Date: May 23, 2012
// POS11024 Work Request:16632 Name:Sandeep Shenoy Date: October 20, 2011
// POS18905 WORK REQUEST:16656 Name: Robert Susanto Date: October 11, 2011
// POS18041 WORK REQUEST:16656 Name: Matt Condra Date: September 25, 2011
// POS18049 WORK REQUEST:16656 Name: Robert Susanto Date: September 2, 2011
// POS9528 WORK REQUEST:15196 Name: Sandy Reedy Date: April 21, 2011
// POS11014 Work Request:13752 Name:Matt Condra Date:April 4, 2011
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "SMFinish.h"               // MGV this state

#include "transbrokerssf.h"
#include "SMWaitForApproval.h"
#include "SMInProgress.h"
#include "SMTakeCard.h"
#include "SMSuspend.h"
#include "SMCustomMessage.h"
#include "SMCashPayment.h"
#include "SMSmGenericDelayed.h"	//CRD 185620
#include "SMProcessing.h"       //CRD 554915

#include "SMVoidTransApproval.h"    // CRD 125377
#include "SMSmAbort.h"				// CRD 125377
#include "SMRestrictedAgeYN.h"		// CRD 125377
#include "VolumeControl.h"
//pos18905
#include "SMAssistedTender.h"

#include "SMShowExternalUrl.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("Finish")

IMPLEMENT_DYNCREATE(SMFinish, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP
#define TIMEOUT_DISABLED 0
//#define MEIJER_1_BTN     3
#define MEIJER_CREDIT_BTN 3

static bool g_bAskedCustomerToSwipeCard = false;
long SMFinish::m_lTimeOutCtr = 0;

////////////////////
// Constructor
////////////////////
SMFinish::SMFinish()
{
	IMPLEMENT_TIMESTAMP
	m_lTimeOutCtr = 0;
}

SMStateBase * SMFinish::Initialize(void)
{
    //sotf 6657
    //SOTF 8113
    //SendTBCommand(TBCMD_NOTIFY_ACS_REMOTE_ASSISTMODE_STATUS, false);

	CString csPOSState;
    SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);

	//Start CRD 185620 554916
	//clear generic delayed first for EAS if there's EAS items present
	if ((lUnapprovedEASItems || lUnapprovedMobileGenericDelayed || bUnapprovedAccountLookupFailed) && (m_GenericDelayedInterventionStatus== GENERIC_DELAYED_NOT_PROCESSING &&
		isAnyApprovalRequired(false)))
	{
		CREATE_AND_DISPENSE(WaitForApproval)(false,false);
	}
	//End CRD 185620

	if(isRestrictedItemApprovalRequired() && GetHookProperty(_T("IsDelayedAgeRestrictedProp")) == _T("1"))
    {
        //by doing this, the existing code that navigate the POS to payment menu will be executed.
        trace(L6, _T("isRestrictedItemApprovalRequired is true. Clearing it to let the POS triggered an intervention to Assistmode."));
		
		//Start SSCOI-72966 / CRD 407201
        //lUnapprovedRestrictedItems =0;    
        //fApprovedForRestrictedItems = true;
		//End SSCOI-72966 / CRD 407201

		//+POS291984, POS293066
		ps.LeadThruText(TXT_PLEASEWAIT);
		
		setAnchorState(BES_FINISH);
		SendTBCommand(TBCMD_ENTERING_FINISH_AND_PAY, false);
		
		if(csPOSState.Find(_T("AGE_VERIFY")) != -1) // POS2939432 - display WaitForApproval when it comes back to SMFinish after clearing any other intevention
		{
			return TimedOut();
		}
		
		CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(), co.getTimeOutTender());
		//-POS291984, POS293066
    }
	
	if (true == m_bCancellingCashTender) 
    {
		
		nTenderType = TB_TENDER_NONE;
        m_bCancellingCashTender = false;
	}
    
    //+SOTF 8036
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
    //-SOTF 8036   
	
	//Start CRD 291249
	//In the event SCO returns to SMFinish after clearing a time restricted item, 
	//instead of sending TBEnterTender 0 from the base, if trx is empty and no approval required, cancel the transaction if mPerks is active, send TBFinish if not
	// SOTF6400
	if (DigitalCouponActiveWithNoItems ())
	{
		return TBCancelTransaction(false);
	}
	else if(IsTrxnCompletelyClean()) 	//+SOTF 7555
	{
		SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1); 
		bIsInFinalization = true; 
		return TBFinish();
	}
    //-SOTF 7555
	//End CRD 291249

    // we could end up here while in cash payment after inserting cash/coins and swiping a credit card.
    // the escrowed cash needs to be processed first.
	//+SSCOI-45180
    //if( nDMCashInserted && !isAnyApprovalRequired() ) // TAR365515
	if( nDMCashInserted && !isAnyApprovalRequired() && 
		nPreviousState == BES_CASHPAYMENT && 
		GetHookProperty(_T("IsDelayedAgeRestrictedProp")) != _T("1"))//-SSCOI-45180
    {
		CString csPosState(_T(""));
        SendTBCommand(TBCMD_GET_POSSTATE, &csPosState);
        // POS2888333, 287563 and 290569 
	    // process only the cash inserted within these PosState
		// sometimes, it gets here while the posstate is at, high_value_promo, fscardentered, etc.
		nPreviousState = BES_LASTSTATE; //SSCOI-45180
		//(+ POS303460)we will get into this condition only if the partial cash tender is done and immediately card is swiped before the inserted cash is processed
	    if((csPOSState.Find(_T("PAYMENTMENU_")) != -1) && (nTenderType != TB_TENDER_CASH))
		{
			trace(L6, _T(" SMFinish::Initialize() cash escrowed in acceptor - process the cash tender now."));
        
			//SOTF 7885
			SASendSCOTInput( TENDER_START );
			ra.OnSetTransactionState(RA_STATE_TENDER);
			//sotf 

			return TBEnterTender(TB_TENDER_CASH, (nDMCashInserted) );
		}
    }
	m_bIsCreditSelected = false;

	CString csTransMode = GetHookProperty(_T("PropTransMode"));

    if(csPOSState.Find(_T("AGE_VERIFY")) == -1) //POS293066
       SetTBProperty(_T("NewFLState"), _T("SMFinish"));

    SetTBProperty(_T("IsDebitSelected"), _T("0"));

	if( GetHookProperty(_T("IsResumeComplete")) == _T("1"))
	{
        SetHookProperty(_T("IsResumeTrxInProgress"), _T("0"));
	}

    m_bMJPlayedUsePinpad = false;
    //inform the TB that we're back at finish and pay this handles the wacky case where FL is at finish
    //the POS is at item and TB state is at itemization due to the fact that we entered and exited AM
    //sotf 6226

    //+SOTF 7555
    CString strStatePropName = _T("IsDirtyTrxWithNoItems");
	CString strIsDirtyTrx = _T("0");
	SendTBCommand(TBCMD_GET_TBPROPERTY, &strStatePropName, &strIsDirtyTrx);

    //POS240201 use wicmode bmp - rrs
    bool bDirtyTrx = ((lItems !=0) || (lVoidedItems >0) || (lCoupons >0) || (strIsDirtyTrx == _T("1")) ||
		              SendTBCommand(TBCMD_ANY_REWARD_CARDS, NULL) || (csTransMode.Find(_T("wicmode"))!= -1) );

    // POS18905
     if( !isAnyApprovalRequired() && GetHookProperty(_T("PropHasRebateCheck")) == _T("1") && bDirtyTrx && lBalanceDue > 0)
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

    //-SOTF 7555

    SetTBProperty(_T("FLState"), _T("TENDER"));
    SetHookProperty(_T("EBTTenderType"), _T("0"));

    // Set to 1 to prevent InProgress GoBack handling for non-credit tenders
    // Will be set to 0 once a card tender is selected.
    SetTBProperty(_T("IsCardTenderSelected"), _T("0"));

    nTimeOut = 1;
    CString csTenderType;
    long lTenderSelected = SendTBCommand(TBCMD_GET_CUSTOMER_SELECTED_TENDER, &csTenderType);
    
    //sotf 6226
    if((lTenderSelected) && (!isAnyApprovalRequired()))
    {
        // TBEnterTenderState(true); //commented for POS295147 aab
        //SOTF 7885
		trace(L6, _T("lTenderSelected:%d isAnyApprovalRequired():%d"),lTenderSelected,isAnyApprovalRequired());
        SASendSCOTInput( TENDER_START );
        ra.OnSetTransactionState(RA_STATE_TENDER);
        //sotf 
        return TimedOut();
    }
    
	//move here for POS294487/294724
	if((!isAnyApprovalRequired()) && (bDirtyTrx))
    {
		
        SendTBCommand(TBCMD_ENTERING_FINISH_AND_PAY, false);
    }

    g_bAskedCustomerToSwipeCard = false;

    if ( (lBalanceDue<=0) && 
         !isAnyApprovalRequired() &&  // CQ 5863
         (bDirtyTrx)) //CQ 7555
    {
        TBEnterTenderState(true);

        if(g_bgVerifySig)
        {
	        CREATE_AND_DISPENSE(WaitForApproval) (false,false, true) ; //(bVerifyTrans, bVoidTrans, bVerifySig)  
        }
        
        // No need to print a gift receipt 
        // There is also no TBMsgMap context which will allow for 
        // for the reset 
        // CQ 8180
        // if we still have non voided items then don't clear
        // the gift receipt flag
        if(lItems <= 0)
        {
            SetHookProperty(_T("PrintGiftReceipt"), _T("0"));
        }
       
		// Need to avoid 0 CASH tender when we know the POS is already finished.
		// Instead go to take card with fEOT = true.
		
		if (GetHookProperty(_T("TRXENDDone")) != _T("1"))
		{
            //SOTF 7885
            SASendSCOTInput( TENDER_START );
            ra.OnSetTransactionState(RA_STATE_TENDER);
            //sotf 
	        return TBEnterTender(TB_TENDER_CASH, (nDMCashInserted) );	// may be zero or +ve
		}
		else
		{
			// POS all done - Go to TakeCard.
			// NOT ONLY is tender not needed, it is not possible!
			trace(L6, _T("POS Complete! - DO NOT DO TENDER FOR 0 CASH"));
			lChangeDue += nDMCashInserted;  // Give Back Any Escrowed CASH!
			nDMCashInserted = 0;

            //SOTF 7885
            SASendSCOTInput( TENDER_START );
            ra.OnSetTransactionState(RA_STATE_TENDER);
            //sotf 

			CREATE_AND_DISPENSE(TakeCard) (true);
		}
    }

    SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);

    //Don't display the payment screen for WICEBT transaction not requiring
    //payment exceeding balance due.
    //sotf 6226
    //POS240201 use wicmode bmp - rrs
	if( (csTransMode.Find(_T("wicmode")) != -1 ) && (csPOSState.Find(_T("WIC_PAYMENT")) == 0) && (!isAnyApprovalRequired()) ) // SOTF 6376
	{
		ps.LeadThruText(TXT_PLEASEWAIT);

        //SOTF 7885
        SASendSCOTInput( TENDER_START );
        ra.OnSetTransactionState(RA_STATE_TENDER);
        //sotf 

		return STATE_NULL;
	}

	//fastlane need to go back here in SMFinish because customer can still choose 
	//either credit or debit at the pinpad after pressing request for credit button
	//but not to display the payment type. this is only used if streamline elect payment is not turn on.
    //sotf 6226
	if(( GetTBProperty(_T("IsRequestForCredit")) == _T("1")) && (!isAnyApprovalRequired()))
	{
        SetTBProperty(_T("IsRequestForCredit"), _T("0"));
		
		DMCashAcceptorDisable();	
		DMCoinAcceptorDisable();	

		ps.SetCMFrame(_T("CardProcessing"));
		// Start CRD 125377 
		if (!m_bShopAndDropEnabled) // Yes button pressed on screen and PrintGiftReceipt enabled
		{
			DMSayPhrase(_T("UsePINPadCompleteTransaction"));
		}
		// End CRD 125377 
		ps.SetCMLeadthruText(LTT_PINPADENTRY);
		ps.Message12(MSG_PINPADENTRY, true);
		ps.ShowCMFrame(false);

        //SOTF 7885
        SASendSCOTInput( TENDER_START );
        ra.OnSetTransactionState(RA_STATE_TENDER);
        //sotf 

		return TimedOut();
	}    

    //sotf 6226
	if (bSuspendRequested && !bSuspendApproved && m_bIsSuspendFromRap && !isAnyApprovalRequired() ) 
	{
		trace(L6, _T("SMFinish::Initialize: Returning Suspend state."));
		bSuspendRequested = false;
		RETURNSTATE(Suspend)
	}

    // POS139881
    if( csPOSState.Find(_T("WAITING_ON_PINPAD")) >= 0 )
    {
        return TimedOut();
     }
     //e POS139881
	// Start CRD 125377 
	if (m_bShopAndDropEnabled)
	{ 
		m_bSNDExitedAM = false;			// CRD 125377
		m_bSNDSettingAge = false;		// CRD 125377
		m_bSNDBackToFnP = true;
		
		lUnknownItems = 0;
		if (!m_bSNDmPerksCancel)
		{
	
	        trace(L6, _T("Before any changes lBalanceDue=%d lItems=%d lTotalSale=%d fInSignedOn=%d fInSignedOff=%d"), lBalanceDue, lItems, lTotalSale, fInSignedOn, fInSignedOff);

			lBalanceDue = 5;
			lItems =  1;
			lTotalSale =  5;

			trace(L6, _T("After changes and before call lBalanceDue=%d lItems=%d lTotalSale=%d fInSignedOn=%d fInSignedOff=%d"), lBalanceDue, lItems, lTotalSale, fInSignedOn, fInSignedOff);
		}
		/* doing  this later. Need POS to allow for Void Total to reset pin pad
		else if (m_bSNDmPerksCancel)
		{
			trace(L6, _T("Cancelling transaction"));
			PSButton3(); // TBCancelTransaction(true);
		}
		*/
	}
	// End CRD 125377

	m_bIsSuspendFromRap = false;
    SMStateBase  *pRetState = SMFinishBase::Initialize();

	// Start CRD 558065, 574057 - move here to display Payment screen first
	if(ProcessCitiApplicationDetails())
	{
		RETURNSTATE(Processing)
	}
	// End CRD 558065

	// + CRD127047
	/*	CRD 135431 - does not need this implementation for Meijer Go. New fix for 127047 is to set CashAllowed in scotopts.000 to N

  if(!co.fTenderCashAllowed ||
		( co.IsDegradedModeOn() && (dm.GetDegradedModeType()  == DEGRADEDMODE_CARD_ONLY)) || 
		m_csTransactionMode == TRANSMODE_MEIJERGO)
	{
		DMSayPhrase(_T("SelectPaymentType"));
	}*/
	//CRD 135431
	//else
	//{
	//	DMSayPhrase(_T("InsertCashSelectPaymentType"));
	//}
	// - CRD127047
	nTenderType = TB_TENDER_CASH;

    // POS40495 - workaround by doing another checking and set the requestenableassistmode accordingly rrs
    trace(L6, _T(">>>> GetRequestEnableAssistMode (%d)"), ra.GetRequestEnableAssistMode());
    if( !ra.GetRequestEnableAssistMode() )
    {
        ra.RequestEnableAssistMode(true);
        trace(L6, _T(">>>>> Set the GetRequestEnableAssistMode to true"));
    }
    // e POS40495 - workaround by doing another checking and set the requestenableassistmode accordingly rrs

	//mini & super mini paystation
	CString strOptionName = _T("FastLaneConfiguration");
	CString m_csFLConfig;
	SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &m_csFLConfig);

	if ( GetHookProperty(_T("IsResumeComplete")) == _T("1") )
	{
        SetHookProperty(_T("IsResumeComplete"), _T("0"));
        //POS11024:Set this flag to false once resume is complete
        fSAInItemSale = false;      
	}

	if ( m_csFLConfig == _T("Mini") )
	{
		ps.Button(_T("ButtonGoBack"), SSF_BTT_BACKTO_SHOPPING, true);
	}
	else if ( m_csFLConfig == _T("SuperMini") )
	{
		ps.SetCMLeadthruText(SSF_LTT_INSERT_PAYMENT);
		ps.Button(_T("ButtonGoBack"), PS_NOTEXT, false);
	}
	
	if ( m_csFLConfig == _T("SuperMini") || (m_csFLConfig == _T("Mini") && !m_bIsNewTrxSelected))
	{
		DMScannerDisable();
		ps.Button(_T("CMButton2"), SSF_BTT_PRINTGIFTRECEIPT, true);
	}
	else
		ps.Button(_T("CMButton2"), PS_NOTEXT, false);

	//POS9528 Start. Set IsResumeStarted = 0.
	CString strStatePropValue;
   strStatePropName = _T("IsResumeStarted");
	strStatePropValue = _T("0");
	SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strStatePropName, &strStatePropValue);

   if (strStatePropValue == _T("1"))
   {
	   strStatePropValue = _T("0");
	   SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strStatePropName, &strStatePropValue);
   }
   //POS9528 End


    //POS31054 - Turn on the yellow light all the time for the Mobile SSCO transaction
    if( m_bNeedYellowLightForMobile )
    {
        dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                                        DM_TRICOLORLIGHT_ON,
                                        DM_TRICOLORLIGHT_NORMALREQUEST);
    }
    // e POS31054
    
	//+ POS253532	
    CString csValue;
    CString strPropName = _T("IsFromResumeTrx");
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strPropName, &csValue);
    trace(L3, _T("Message Screen - IsFromResumeTrx=%s"), csValue);
    if(csValue == _T("1") && pRetState == STATE_NULL)
    {
            csValue = _T("0");
          SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strPropName, &csValue);
    
    }
    //- POS253532

	// Start CRD 125377 
	if (m_bShopAndDropEnabled)
	{
		CString csCurContext(_T("Finish")); 

		// Rel 4.0 Req. 6.5 Change Language During Transaction-->
		if(co.GetfStateDualLanguage() && 
		  (co.fStateAllowLanguageSelectionDuringTransaction ||
		  (custInfo.GetOption(_T("PersonalizeLanguage")) == "Y")))
		{
			CString csButtonText;

			//set button text
			if(co.GetNumberOfLanguagesSupported() > 2)
			{
				ps.SetPSText(PS_WORK1, ps.GetPSText(BTT_LANGUAGE, SCOT_LANGUAGE_PRIMARY));
			}
			else if(co.GetNumberOfLanguagesSupported() == 2)
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
			 
				//Get the language name text for button
				csButtonText = co.Getlanguage(i).csName;
				if(_istdigit(*csButtonText))
				{
					iButtonID = _ttoi(co.Getlanguage(i).csName);
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

		}//+SSCOP-692
		else 
		{
			m_pAppPSX->SetConfigProperty(_T("CMButton5MidiList"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
		}//-SSCOP-692
 
		if (co.IsShowVolumeControl() )
		{
			m_pAppPSX->SetConfigProperty(_T("ButtonVolumeControl"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
			m_pAppPSX->SetConfigProperty(_T("CmdBtn_ButtonVolumeControl"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);
		}else
		{
			m_pAppPSX->SetConfigProperty(_T("ButtonVolumeControl"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
			m_pAppPSX->SetConfigProperty(_T("CmdBtn_ButtonVolumeControl"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
			
		}
 	}
	// end CRD 125377 

	bIsGenericDelayedProcessed = false;	//CRD 223409
	SetHookProperty(_T("IsInStoreMode"),_T("0"));	//CRD 232267
	
   return pRetState;
}

void SMFinish::UnInitialize(void)
{
    m_lTimeOutCtr = 0;
	SMFinishBase::UnInitialize();
}

SMStateBase * SMFinish::DMAcceptor(void)
{
    return SMFinishBase::DMAcceptor();
}

SMStateBase * SMFinish::DMCardReader(void)
{
    return SMFinishBase::DMCardReader();
}

SMStateBase * SMFinish::DMScanner(void)
{
    SMStateBase * pRet = STATE_NULL;
	
    if ( GetTBProperty(_T("IsThereItemNeedToDisplayed")) == _T("1"))
	{
        // Save off the barcode and load the resumed items in the e-receipt list
        // The item sale will be performed once the resume is complete in TB_RESUME_ITEM handler
        trace(L3, _T("Barcode scanned in resumed transaction. Load the e-receipt before processing scan.")); 
        SetTBProperty(_T("ScannedBarcodeAfterResume"), csDMLastBarCode);
		
        /* SOTF8031+ */
        m_bIsFromScanReceipt = true;
		//lItems = 0;		// TAR363054. POS9528 backout TAR363054.
		ps.LeadThruText(TXT_PLEASEWAIT);
		
		if(SendTBCommand(TBCMD_RESUME_ITEMS, NULL) == 0)
		{
			// POS18041: No resume items to display so InProgress state does not need to be activated.
			trace(L3, _T("No resume items to display.")); 
			SetHookProperty(_T("IsResumeComplete"), _T("1"));
			SetTBProperty(_T("IsThereItemNeedToDisplayed"), _T("0"));
			return SMFinishBase::PSButtonGoBack();
		}
		
		// POS11014: While resumed items are being retrieved, make sure tender buttons
		// cannot be accessed by changing state to InProgress.
		RETURNSTATE(InProgress);
        /* SOTF8031- */
	}
    else
    {
        pRet = SMFinishBase::DMScanner();
    }
	
    return pRet;
}

SMStateBase * SMFinish::PSListLookup(CString csItemInfo)
{
    return SMFinishBase::PSListLookup(csItemInfo);
}

SMStateBase * SMFinish::SAEventOccurred(const long lEvent)
{
    return SMFinishBase::SAEventOccurred(lEvent);
}

SMStateBase * SMFinish::PSButtonGoBack(void)
{
    m_bIsFromScanReceipt = false;
	m_bIsFromPaymentGoback = true;

	//CRD 366323
	SendTBCommand(TBCMD_NOTIFY_EXIT_STOREMODE_TO_SCANANDBAG);
	
	// Start CRD 125377 
	if (m_bShopAndDropEnabled) // Yes button pressed on screen and PrintGiftReceipt enabled
	{
		m_bSNDBackToFnP = false;
	}
	// End CRD 125377
    if(g_bAskedCustomerToSwipeCard)
    {
		SendTBCommand(TBCMD_SET_TB_STATE,_T("CANCEL_TENDER"));
        return Initialize();
    }
	else if ( GetTBProperty(_T("IsThereItemNeedToDisplayed")) == _T("1"))
	{
		//lItems = 0;		// TAR363054. POS9528 backout TAR363054.
		ps.LeadThruText(TXT_PLEASEWAIT);
		
		if(SendTBCommand(TBCMD_RESUME_ITEMS, NULL) == 0)
		{
			// POS18041: No resume items to display so InProgress state does not need to be activated.
			trace(L3, _T("No resume items to display.")); 
			SetHookProperty(_T("IsResumeComplete"), _T("1"));
			SetTBProperty(_T("IsThereItemNeedToDisplayed"), _T("0"));
			return SMFinishBase::PSButtonGoBack();
		}
		
		// POS11014: While resumed items are being retrieved, make sure tender buttons
		// cannot be accessed by changing state to InProgress.
		RETURNSTATE(InProgress);
	}
	else
	{
		return SMFinishBase::PSButtonGoBack();
	}
}

SMStateBase * SMFinish::PSButtonHelp(void)
{
    return SMFinishBase::PSButtonHelp();
}

SMStateBase * SMFinish::checkCashAcceptorCassette()
{
    return SMFinishBase::checkCashAcceptorCassette();
}

SMStateBase * SMFinish::ButtonPressed(int buttonindex) 
{
    m_bIsCreditSelected = false;
    
    SetTenderButton(buttonindex);
    int iSelectedTenderType = GetTenderButton().nTenderType;
    bool bWithCashBack = GetTenderButton().bCashBack;
    
    SetTBProperty(_T("CashBackValue"), bWithCashBack ? _T("1") : _T("0"));
   
    //just go ahead and process non-electronic tenders
    if(((iSelectedTenderType >= TB_TENDER_COUPON) && (iSelectedTenderType <=TB_TENDER_CASH)) ||
       ((iSelectedTenderType >= TB_TENDER_CHECK) && (iSelectedTenderType <= TB_TENDER_CHECK_LANE_RAP)))
    {
        nTimeOut = TIMEOUT_DISABLED;
        return SMFinishBase::ButtonPressed(buttonindex);
    }
	else if (iSelectedTenderType == TB_TENDER_CREDIT)
	{
		m_bIsCreditSelected = true;
		SendTBCommand(TBCMD_SEND_PRESELECTED_TENDER, TB_PRESELECTED_TENDER_CREDIT);
	}
	else if (iSelectedTenderType == TB_TENDER_DEBIT)
	{
        SetTBProperty(_T("IsDebitSelected"), _T("1"));
		SendTBCommand(TBCMD_SEND_PRESELECTED_TENDER, TB_PRESELECTED_TENDER_DEBIT);
	}
	else if ( iSelectedTenderType == TB_TENDER_EBT )
	{
		SendTBCommand(TBCMD_SEND_PRESELECTED_TENDER, TB_PRESELECTED_TENDER_EBT_CASH);
	}
	else if ( iSelectedTenderType == TB_TENDER_FOODSTAMPS )
	{
		SendTBCommand(TBCMD_SEND_PRESELECTED_TENDER, TB_PRESELECTED_TENDER_EBT_FOOD);
	}
	//+POS138637
    	else if ( iSelectedTenderType == TB_TENDER_USERDEFINED1 )
	{
       		 SetHookProperty(_T("PropFromRTS"), _T("0"));
        		SendTBCommand(TBCMD_SET_TB_STATE,_T("FINISH_WIC_START"));
        		CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(), co.getTimeOutTender());  
    	 }
	//-POS138637
        //POS223750 EMV
        //POS227744 PROTOTYPE EMV TENDER
        /*else if ( iSelectedTenderType == TB_TENDER_PIP_GENERIC )
        {
            SendTBCommand(TBCMD_SEND_PRESELECTED_TENDER, TB_PRESELECTED_TENDER_CREDIT);
            trace(L6, _T("ButtonPressed(%d), EMV Tender Test"));
        }*/
        //e - POS227744 PROTOTYPE EMV TENDER
        //e - POS223750 EMV

    CString csSelectedTenderType;
    long lActualTenderSelected = SendTBCommand(TBCMD_GET_CUSTOMER_SELECTED_TENDER, &csSelectedTenderType);

 
    //customer selected tender so makes sure it matches the one selected
    if(lActualTenderSelected) 
    {        
        
        return TimedOut();

    }else 
    {
         //no electronic tender selected on the pinpad yet, so wait
         //until customer selects one
         ps.SetCMFrame(_T("UsePinPad")); //tar 177485
		 ps.SetCMLeadthruText(SSF_TXT_PLEASE_INSERT_TAP_SWIPE_CARD); // SSCOI-47187
         ps.ShowCMFrame();
        
        g_bAskedCustomerToSwipeCard = true;

        if(     co.fOperationsPartialIntegratedPINPad 
		&& !co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bIntegratedMSR  //PIP 
		&& !fInLoyaltyCardProcessing) 
	    {
		    DMSayPhrase(USEPINPAD);
            m_bMJPlayedUsePinpad = true;
        }
         return STATE_NULL;
    }
}

SMStateBase * SMFinish::ProcessTender(TBTENDERTYPE nTenderType)
{
    return SMFinishBase::ProcessTender(nTenderType);
}

SMStateBase * SMFinish::StandardCardProcessing(int buttonindex)
{
    return SMFinishBase::StandardCardProcessing(buttonindex);
}

void SMFinish::ConfigureTenderOptions(void)
{
    SMFinishBase::ConfigureTenderOptions();
}

SMStateBase * SMFinish::OnWtDecrease(void)
{
    return SMFinishBase::OnWtDecrease();
}

SMStateBase * SMFinish::OnWtIncreaseNotAllowed(void)
{
    return SMFinishBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMFinish::PSConnectRemote( CString& strMachineName, CString& strRemoteContext )
{
    return SMFinishBase::PSConnectRemote( strMachineName, strRemoteContext );
}

//POS186112
// Method    : IsPaymentButtonAvailable
// Parameters: void
// Returns   : bool (true, false)
// Purpose   : A method to check if there is payment button is 
//             display in FinishAndPay screen. 
///////////////////////////////////////////////////////////////////////
//SR742
bool SMFinish::IsPaymentButtonAvailable()
{
    bool bReturnValue = SMFinishBase::IsPaymentButtonAvailable();
	
	trace(L3, _T("IsPaymentButtonAvailable(%d), WICOptionLoaded(%d)"), bReturnValue, SMState::lWICOptionloaded);
	
	if( !bReturnValue && SMState::lWICOptionloaded == 2)
	{
		CString csCtlName = _T("SmartWICTenderImage");		
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
	}
	return bReturnValue;
}
//SR742-
//e - POS186112

SMStateBase  *SMFinish::TimedOut()
{

    CString csTenderType;
    long lTenderSelected = SendTBCommand(TBCMD_GET_CUSTOMER_SELECTED_TENDER, &csTenderType);
	
	// +POS2888333, 287563 and 290569 
	// sometimes the cash that was inserted got skipped due to card activity - process it
	// once it got inside back in payment menu. This fixes a lot of scenario for unaccounted pre-inserted cash
	// but it may still trigger to assistmode if the customer_tender_select will occur first prior to mPerks prompt.
	CString csPOSState;
    SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);

	if(csPOSState.Find(_T("AGE_VERIFY")) != -1) //POS293066
	{
		trace(L6, _T(" SMFinish::TimedOut() - We got here while POS is at Age Verify screen. Triggering WaitForApproval."));
		CREATE_AND_DISPENSE(WaitForApproval) (false,false, false) ; //(bVerifyTrans, bVoidTrans, bVerifySig)  
	}
	
	if( csPOSState.Find(_T("PAYMENTMENU_")) != -1 || lTenderSelected)
	{
		if((nDMCashInserted == 0 && nDMCashTendered) || (nDMCashInserted))
		{
			if(nDMCashInserted == 0)
			{
		       nDMCashInserted = nDMCashTendered;

			   m_nDMCashPaid -= nDMCashTendered;
               if(m_nDMCashPaid <0) m_nDMCashPaid = 0;

		       nDMCashTendered = 0;
			}
		    trace(L6, _T(" SMFinish::TimedOut() - overriding the card tender for the previous cash inserted."));
		
		    //SOTF 7885
		    SASendSCOTInput( TENDER_START );
		    ra.OnSetTransactionState(RA_STATE_TENDER);
		    //sotf 
		
     		return TBEnterTender(TB_TENDER_CASH, (nDMCashInserted) );
		}
	}
    // -POS2888333, 287563 and 29056
	
    if(lTenderSelected)
    {
        SetTBProperty(_T("IsCardTenderSelected"), _T("1"));
        trace(L6, _T("TenderType is %s"), csTenderType); // POS72562

        /*  POS39476 - Remove all the tender type definition as in DG51, there will only be 1 electronic payment tender button - rrs
        int iCredit =-1;
        int iGift =-1;
        int iFoodStamps =-1;
        int iEBT =-1;
        int iDebit = -1;
        int iDebit1 = -1;
		int iDiscover= -1;

        for(int i = 0; i < MAXBUTTONNUM; i++)
	    {
            int iTenderType;

            iTenderType = co.TenderList[ps.GetLanguage()][i].nTenderType;

		    if((iTenderType == TB_TENDER_CREDIT))
			    iCredit = i+1;

            if((iTenderType == TB_TENDER_GIFTCARD))
			    iGift = i+1;

            if((iTenderType == TB_TENDER_EBT))
                iEBT = i+1;

            if((iTenderType == TB_TENDER_FOODSTAMPS))
                iFoodStamps = i+1;

             if((iTenderType == TB_TENDER_DEBIT) || ( nTenderType == TB_TENDER_PIP_DEBIT ))
                 if(iDebit == -1)
                 {
                    iDebit = i+1;
                 }else if (iDiscover == -1)
				 {
					iDiscover = i+1;
				 }
				 else
                 {
                    iDebit1 = i+1;
                 }
        }
        * e POS39476 */

        //POS39476/46373 - Assign all the electronic tender type to be 1
        int iCredit =1;
        int iGift =1;
        int iFoodStamps =1;
        int iEBT =1;
        int iDebit = 1;
        int iDebit1 = 1;
		int iDiscover= 1;

		int iWIC = 1;

        nTimeOut = TIMEOUT_DISABLED;

		int iButtonPressed = 0;

        
        

        if(csTenderType.Find(_T("ATM")) != -1)
        {   
            if(co.TenderList[ps.m_languageUsed][iDebit1-1].bCashBack && !m_bIsCreditSelected)
				iButtonPressed = iDebit1;
                //return SMFinishBase::ButtonPressed(iDebit1);
			else if (m_bIsCreditSelected && SendTBCommand(TBCMD_IS_CUST_REQUEST_CREDIT_ENABLED, NULL))
			{
                SetHookProperty(_T("TenderType"));
				SendTBCommand(TBCMD_SET_TB_STATE,_T("TENDER_CUSTOMER_REQUESTED_CREDIT"));
				iButtonPressed = iCredit;
			}
            else
				iButtonPressed = iDebit;
                //return SMFinishBase::ButtonPressed(iDebit);

        }
        if(csTenderType.Find(_T("CREDIT"))!= -1)
        {  
            
	        //return SMFinishBase::ButtonPressed(iCredit);
			if(csTenderType.Find(_T("DISCOVER"))!= -1)
			{
				iButtonPressed = iDiscover;

				//disable always the CustRequestCreditBtn
                SetHookProperty(_T("CustRequestCreditBtn"), _T("1"));
			}
			else
				iButtonPressed = iCredit;
        }
        if(csTenderType.Find(_T("GIFT"))!= -1)
        {

            //return SMFinishBase::ButtonPressed(iGift);
			iButtonPressed = iGift;
        }

        if(csTenderType.Find(_T("EBT CASH"))!= -1) //EBT CASH          
		{
            //return SMFinishBase::ButtonPressed(iEBT);
			iButtonPressed = iEBT;
		}

        // POS42533 - Handle new Tender Description OTC CARD - rrs 
        if(csTenderType.Find(_T("OTC CARD"))!= -1)
        {
            iButtonPressed = iCredit;			
		}
        // e POS42533

        if(csTenderType.Find(_T("Starbucks"))!= -1)
        {
           // POS72562: Starbucks gift card tender is an electronic tender type.
           iButtonPressed = iGift;
        }

        // POS243714/SSCO-41852 - Handle ELECTRONIC WIC - aab 
        if(csTenderType.Find(_T("WIC"))!= -1)
        {
            iButtonPressed = iWIC;			
		}

        //POS69170 - check if the description doesn't have the GIFT in there
        if( csTenderType.Find(_T("FOOD"))!= -1 && csTenderType.Find(_T("GIFT")) == -1 )
        {
            //return SMFinishBase::ButtonPressed(iFoodStamps);
			iButtonPressed = iFoodStamps;
            
            //+TAR 393529
			SendTBCommand(TBCMD_SET_TB_STATE, _T("TENDER_PAYMENT_EBT"));

            // POS18049 - make the sleep 200ms longer.  The original time was 300ms, and change it to 500ms
			Sleep(500);	// wait for contexts from POS to arrive.

            // POS35462 - check if we get the EBT FS total from the POS by validating the TestTenderType variable
            // if the POS doesn't respond yet after 500ms then  the TestTenderType will be empty and we would need to wait another 500ms - rrs
            CString csTestTenderType = GetHookProperty(_T("TestTenderType")); 
            trace(L6, _T("TestTenderType for EBT FOOD tender is (%s)"), csTestTenderType);
                
            if( csTestTenderType == _T("") )
            {
                Sleep(500);
            }
            // e POS35462

			CString csEBTAmount = GetHookProperty(_T("PropTenderAmountDue"));
            trace(L6, _T("EBT Total After Sleep (%s)"), csEBTAmount);
			//-TAR 393529

			//TAR350241+
			//if(lFoodStampsDue == 0)
            if ( csEBTAmount == _T("0") )	//TAR 393529 : if value is 0 return insufficient foodstamp.
			{
				CustomMessageObj.Reset();

				CustomMessageObj.csLeadThruText = ps.GetPSText(SSF_TXT_FOODSTAMPS); //POS54860 - Use msg ID 9347
				CustomMessageObj.csScreenText = ps.GetPSText(MSG_INSUFFICIENTFOODSTAMP);    

				RETURNSTATE(CustomMessage)
			}
			//TAR350241-            
		}

		//TAR353069 - This is a weird situation wherein FastLane registed as -1 for debit tender type 3
		//so, we are going to force it here to 3 in case this will happen again.
		if(iButtonPressed == -1) iButtonPressed = 3;
        if(iButtonPressed)
		{
			SetTenderButton(iButtonPressed);
			int iSelectedTenderType = GetTenderButton().nTenderType;
			bool bWithCashBack = GetTenderButton().bCashBack;
    
            SetTBProperty(_T("CashBackValue"), bWithCashBack ? _T("1") : _T("0"));

			CString strOptionName = _T("FastLaneConfiguration");
			CString csFLConfig;
			SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &csFLConfig);
			if(csFLConfig == _T("SuperMini")) 
			{
				ps.SetCMBackground(BMP_CARDPROCESSING);
				ps.LeadThruText(LTT_CASHPROCESSING);
				DMSayPhrase(USEPINPAD);
				delete getRealStatePointer( TBEnterTender(TBTENDERTYPE(co.TenderList[ps.m_languageUsed][iButtonPressed-1].nTenderType),lBalanceDue));
				//TAR350787
				//CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(), co.getTimeOutTender());
			}
			else
			{ // (+) SSCOI-49366: Check the current TBState if tender was already been selected.
				CString csTBState;
				SendTBCommand(TBCMD_GET_TB_STATE, &csTBState);
				
				trace(L6, _T("*_* TBState = %s --- POSState = %s"), csTBState, csPOSState);

				if ( csTBState != _T("TENDER_SELECTED") )
					return SMFinishBase::ButtonPressed(iButtonPressed);
				else
					return STATE_NULL;
			} // (-) SSCOI-49366
		}
       
    }


    //// POS139881
    //CString csPOSState;
    //SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);
    if( csPOSState.Find(_T("WAITING_ON_PINPAD")) >= 0 && ps.GetCurrentContext() != _T("CardProcessing") )
    {        
        trace(L6, _T("Display CardProcessing for Waiting for customer"));
        ps.SetCMFrame(_T("CardProcessing"));

        if( !m_bMJPlayedUsePinpad )
        {
            DMSayPhrase(_T("UsePINPadCompleteTransaction")); //POS295147 rrs
            m_bMJPlayedUsePinpad = true;
        }

		ps.SetCMLeadthruText(LTT_PINPADENTRY);
		ps.Message12(MSG_PINPADENTRY, true);
		ps.ShowCMFrame(false);
     }
     //e POS139881

	//tar 381835
	if(++m_lTimeOutCtr < (co.getTimeOutContinueTransaction() * 3))
		return STATE_NULL;
	else
	{
		m_lTimeOutCtr = 0;
		return SMFinishBase::TimedOut();
	}
}

SMStateBase  *SMFinish::PSButton1(void)
{
	//12 is assigned to "other payments" in scottend.000
	return ButtonPressed(12);
}

SMStateBase  *SMFinish::PSButton2(void)
{
    if( GetTBProperty(_T("FastLaneConfiguration")) == _T("SuperMini") )
    {
        SendTBCommand(TBCMD_SET_TB_STATE,_T("GIFTRECEIPT_FLAGGED"));
    }
    else
    {
        trace(L3, _T("Gift Receipt button pressed on Finish & Pay screen. Ignored because we're not a Paystation."));
    }
    return STATE_NULL;
}

bool SMFinish::PSAssistMenuLegalForState()
{
    return SMFinishBase::PSAssistMenuLegalForState();
}

bool SMFinish::storeMode()
{
    return SMFinishBase::storeMode();
}

//////////////////////////////////////
// PSButton3 - Cancel Order
//////////////////////////////////////
SMStateBase * SMFinish::PSButton3(void)
{
	trace(L6, _T("SMFinish::PSButton3"));
	
	// Start CRD 125377 
	if (m_bShopAndDropEnabled) // Yes button pressed on screen and PrintGiftReceipt enabled
	{
		m_bSNDxCancelTrans = true;		// CRD 125377
		// CREATE_AND_DISPENSE(VoidTransApproval)(BES_SCANANDBAG, BES_TAKERECEIPT);
		// RETURNSTATE(SmAbort)

		return TBCancelTransaction(true);
		//setAnchorState(BES_FINISH);
		//RETURNSTATE(RestrictedAgeYN)
	}
	return STATE_NULL;
	// End CRD 125377 
}



//////////////////////////////////////
// PSButton5 - Language selection button
//////////////////////////////////////
SMStateBase *SMFinish::PSButton5(void)
{
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
	
    if(co.GetNumberOfLanguagesSupported() > 2)
	{
		// Tell security that we are exiting form scan& bag state
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
		
		return setAndCreateAnchorState(BES_LANGUAGESELECT);
	}
	else if(co.GetNumberOfLanguagesSupported() == 2)
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
		//USSFHOOK(_T("ChangeLanguage"), ChangeLanguage(m_customerLanguage);, ;);
		//ChangeLanguage(m_customerLanguage);
		//USSF END
		SendTBCommand(TBCMD_CUSTOMERLANGUAGE, CString(co.Getlanguage(m_customerLanguage).szCode));
		
		
        //Load Quickpick if the Pref is empty
        if (co.fOperationsDisplayQuickPickItems)
        {
			ps.DisplayQuickPickItem();
        }
        Initialize(); // refresh the screen
		
		
		return STATE_NULL;
	}
	else
	{
		ASSERT(FALSE); // shouldn't enter here
		return STATE_NULL;
	}
}



//******************************************************
//Method : PSButton7
//Purpose: Change the volume level
//Parameters:  
//Return Values: 
//******************************************************
SMStateBase  *SMFinish::PSButton7()
{
	/*
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
    if (nPSLastParm > 0)
        CVolumeControl VolumeControl(SETVOLUMELEVEL, 0, ((nPSLastParm > 100) ? 100 : nPSLastParm));
    else if (nPSLastParm == -1) // if -1
        CVolumeControl VolumeControl(SETVOLUMELEVEL, 0, 0);
    else
        CVolumeControl VolumeControl(SETVOLUME);
    custInfo.SetDataString(("VolumeChangeMethod"),_T("Button"));
    */
	return STATE_NULL;
}


SMStateBase * SMFinish::PSCmdButton(CString csContext, CString csControl, CString csCommand)
{
	trace(L6, _T("SMFinish::PSCmdButton csContext=%s csControl=%s  csCommand=%s"), csContext, csControl,  csCommand);
	// Start CRD 525299
	if(csControl == _T("CmdBtn_ApplyMeijerCard"))
	{
		RETURNSTATE(ShowExternalUrl);
	}
	// End CRD 525299
	return SMState::PSCmdButton(csContext, csControl, csCommand);
}