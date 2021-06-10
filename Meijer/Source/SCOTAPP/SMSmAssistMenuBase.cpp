//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAssistMenuBase.CPP
//
// TITLE: Class implementation for Assist Mode keyboard
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#ifndef _CPPUNIT
#include "Common.h"                 // MGV common includes

#include "SMSmAssistMenuBase.h"

#include "SMFinish.h"
#include "SMScanAndBag.h"
#include "SMWaitForApproval.h"
#include "SMVoidItem.h"
#include "SMSmAssistMode.h"
#include "SMKeyInCode.h"
#include "SMSmUpdateItem.h"
#include "SMSmVoidAndRemoveItem.h"  // SR803
#include "SMSmRestrictedItems.h"
#include "SMSmGenericDelayed.h"
#include "IQueuedAction.h"
#else
#include "SMSmAssistMenuBase.h"
#endif

#ifndef _CPPUNIT

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmAssistMenuBase") //SSCOP-86

#endif
#define ASSISTMENU_CONTEXT  _T("SmAssistMenu")
#ifndef _CPPUNIT
#define ASSISTMENU_RETURN_TIMEOUT 2

IMPLEMENT_DYNCREATE(SMSmAssistMenuBase, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP
#endif
CString SMSmAssistMenuBase::m_csMenuContext= ASSISTMENU_CONTEXT;
#ifndef _CPPUNIT
bool SMSmAssistMenuBase::m_bShowEchoBox = false;

PSTEXTWAVE  SMSmAssistMenuBase::m_AudioID = PS_NOSOUND;
PSTEXT      SMSmAssistMenuBase::m_PromptTextID = PS_NOTEXT;
#endif
bool SMSmAssistMenuBase::bScannerEnabledSaved = false;       //sscoadk-3385
bool SMSmAssistMenuBase::bReturnToShoppingNow = false;

#ifndef _CPPUNIT
////////////////////////////////
SMSmAssistMenuBase::SMSmAssistMenuBase()
{
    IMPLEMENT_TIMESTAMP
        SMState::g_bBackToBackDataNeeded = true; // TAR 265143
    if (m_csMenuContext.IsEmpty())
    {
        m_csMenuContext = ASSISTMENU_CONTEXT;
    }
    m_bBlockScanData = false;   //RFQ 986
} 

////////////////////////////////
SMSmAssistMenuBase::SMSmAssistMenuBase(CString csMenuContext)
{
    IMPLEMENT_TIMESTAMP
        SMState::g_bBackToBackDataNeeded = true; // TAR 265143
    m_csMenuContext = csMenuContext;
    m_bBlockScanData = false;  //RFQ 986
} 


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Method:		SMStateBase  *SMSmAssistMenuBase::PSNumericKey
//
// Purpose:		Find keycode for numeric keys.
//
// Parameters:	const long lKey
//
// Returns:		SMStateBase
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SMStateBase  *SMSmAssistMenuBase::PSNumericKey(const long lKey)
{
    PSEchoInput(lKey);
    return STATE_NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Method:		SMStateBase  *SMSmAssistMenuBase::PSAlphaNumericKey
//
// Purpose:		Find keycode for numeric keys. Send these keycode to TB 
//
// Parameters:	const long lKey
//
// Returns:		SMStateBase
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SMStateBase *SMSmAssistMenuBase::PSAlphaNumericKey(const long lKey)
{
    trace(L0,_T("+-SMSmAssistMenuBase::PSAlphaNumericKey %d"),lKey);
    return STATE_NULL;                
}
#endif


SMStateBase* SMSmAssistMenuBase::Initialize()
{
#ifndef _CPPUNIT
    trace(L6,_T("+SMSmAssistMenuBase::Initialize()"));

    //SSCOADK-5989 + reformat scale unit and weights and measures
    if (co.fStateDualLanguage && m_customerLanguage != SCOT_LANGUAGE_PRIMARY && !ps.RemoteMode()
        && co.fStateDisplayWeightsAndMeasures && dm.IsDeviceConfigured(DMCLASS_SCALE))
    {
        //If the last state was not SMSmAuthorization then need to reset scale display
        if((this->csLastNonHelpStateName != CString(_T("SMSmAuthorization"))) && (!co.IsInAssistMenus()))
        {
           // If healthy, then set scale display.
           long retErrorCode = DMConfigureScaleDisplay();
           if( retErrorCode > -1)
           {
	         fStateFreeze = false;
	         CString csWork = ps.GetPSText(retErrorCode);

	         return SMStoreModeStateBase::createLevel1Message(
                        csWork,  // char *message
				        PS_NOTEXT,         // no canned message
				        PS_NOTEXT,         // no prompt
				        0,                 // no prompt value
				        -1,                // no device
				        false, false);     // no DM check health needed //SR672
           }
        }
    }
    //SSCOADK-5989 -
    
    //++SSCOADK-5019
    if (g_pDelay_TBExpectedStateChange != NULL && 2 == SMStateBase::m_nTBOutOfSyncError )
    {
		delete g_pDelay_TBExpectedStateChange;
		g_pDelay_TBExpectedStateChange = NULL;
		CKeyValueParmList parmList;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_SKIP_BAGGING);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		BSTR bstrResult;
		bstrResult = NULL;
		SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
		SysFreeString(bstrResult);
		trace(L6,_T("clear the state saved in the stack since we are out of sync with TB"));
    }

    if ( 2 == SMStateBase::m_nTBOutOfSyncError )
    {
        eo.Error(INFO,SM_CAT_LAN_WAN_COMM, SCOTAPP_TB_OUT_OF_SYNC_ASSISTMODE_RECOVER);
	    if ( !fSAInItemSale )  
    	{
		    SMStateBase::m_nTBOutOfSyncError = 0;
    	}
        
        trace(L6, _T("ra.OnNoRAPDataNeeded()"));
        ra.OnNoRAPDataNeeded();	
    }
    //--SSCOADK-5019

	if (fInMultiSelectPickList)
	{
		if ((ps.GetPicklistItemsSold() > 0) && !g_bOnItemOK && !io.d.fVoid)
		{
			//SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
			CKeyValueParmList parmList;
			CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_SKIP_BAGGING);
			_bstr_t bstrParmList((const _TCHAR *)parmList);
			BSTR bstrResult;
			bstrResult = NULL;
			SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
			SysFreeString(bstrResult);
			nSAPrevSCOTInputID = 0;
		}

		fInMultiSelectPickList = false;

		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("MultiPicklist") );
   		setAnchorState(BES_SCANANDBAG); //SSCOADK-5004
	}
#endif
    SMStateBase* pRetState = STATE_NULL;
#ifndef _CPPUNIT
	nTimeOut=0;

	io.Reset(); //406738

    if (bReturnToShoppingNow)
    {
        bReturnToShoppingNow = false;
        return OnReturnToShopping();
    }

    EnableHandheldAssistMode(true);  //In case SMFinish disabled

	if (!co.IsInAssistMenus() && lUnapprovedRestrictedItems == 0)
	{
		//if (dm.fStateTakeawayBelt && ps.RemoteMode())
        if (ps.RemoteMode())  //sscoadk-5358
        {
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmRemoteAssistMode") );
        }
        else	
        {
            TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmAssistMode") );
        }

	}
    co.SetInAssistMenus(true);
    if (ra.GetRAPGenericDelayedPendingCount() == 0)
    {
        co.SetContextPrefixAM(_T("AM_"));
    }
    SendTBCommand(TBCMD_ASSISTMENUSET, true);

    if (m_GenericDelayedInterventionStatus)
    {
        //We are either starting or processing a GD intervention, 
        //Do not go to SmGenericDelayed if we are finalizing an age restriction
        //after processing a GD. 
        if (m_GenericDelayedInterventionStatus == GENERIC_DELAYED_COMPLETING_APPROVALS && lUnapprovedRestrictedItems)
        {
            if( lUnapprovedRestrictedItems >= 2 )
            {
                RETURNSTATE(VoidItem);
            }
            else
            {
                RETURNSTATE(SmVoidAndRemoveItem);  //return to void and remove voided item state
            }

        }
        else 
        {
            RETURNSTATE(SmGenericDelayed);
        }
    }

	CString csItemEntryId = ps.GetSelection(_T("SMReceipt"));
	if (csItemEntryId.Find(_T("ORIGPRICE")) != -1)
	{
        ps.ScrollDown(_T("SMReceipt"));
        ps.ScrollDown(_T("CMReceipt"));
        ps.ScrollDown(_T("CMReceiptVoid"));
	}

    g_bClearOccurred = true;

    //+SR803
    if( lUnapprovedRestrictedItems && !bStoreModeGoBack )
    {
        // if there's only one age-restricted item, then automatically void the item
        // and show operator screen to remove the voided item 
        // while if there are more than one age-restricted items, proceed to VoidItem 

        rp.m_bIsInterventionApproved = false; //report that the intervention was not approved 

        if(ps.RemoteMode())
        {
            CString csRAP;
            csRAP = _T("RAP");
            rp.SetInterventionTerminalNumber(csRAP);
        }

        if( lUnapprovedRestrictedItems >= 2 )
        {
            RETURNSTATE(VoidItem);
        }
        else
        {
            RETURNSTATE(SmVoidAndRemoveItem);  //return to void and remove voided item state
        }
    }
    //-SR803

	// Tar 213053 RAP AM neds to clear certain approvals like going into SM at lane
	/*
	if (ps.RemoteMode())
	{
		// remote approval
		if (lNotForSaleItems)
		{  // let ra know we approve recalled items from app
			trace(L7, _T("+ra.OnRecalledItemApproval"));
			ra.OnRecalledItemApproval();
			trace(L7, _T("-ra.OnRecalledItemApproval"));
		}

		if (lTimeRestrictedItems)
		{
			trace(L7, _T("+ra.OnTimeRestrictedItemApproval"));
			ra.OnTimeRestrictedItemApproval();
			trace(L7, _T("-ra.OnTimeRestrictedItemApproval"));
		}

		if (lQuantityRestrictedItems)
		{
			trace(L7, _T("+ra.OnQuantityRestrictedItemApproval"));
			ra.OnQuantityRestrictedItemApproval();
			trace(L7, _T("-ra.OnQuantityRestrictedItemApproval"));
		}
		if (fUnDeActivatedItem)
		{
			trace(L7, _T("+ra.OnUnDeActivatedItemApproval"));
			ra.OnUnDeActivatedItemApproval();
			trace(L7, _T("-ra.OnUnDeActivatedItemApproval"));
		}
		// We can NOT approve these here, because there is a separate screen
		// where these are approved (SMSmVisualItemsBase) so we can't approve
		// them twice, otherwise SCOTRA doesn't like it - TAR# 121732
#if 0
		// visual verify
		if (lVisualVerifyItems)
		{  // let ra know we approve recalled items from app
			trace(L7, _T("+ra.OnVisualValidationApproval"));
			ra.OnVisualValidationApproval();
			trace(L7, _T("-ra.OnVisualValidationApproval"));
		}
#endif

		if (lUnknownItems)
		{  // let ra know we approve unknown items from app
			trace(L7, _T("+ra.OnUnknownItemApproval"));
			ra.OnUnknownItemApproval();
			trace(L7, _T("-ra.OnUnknownItemApproval"));
		}
		if (lPriceRequiredItems)
		{  // let ra know we approve price required items from app
			trace(L7, _T("+ra.OnUnknownPriceApproval"));
			ra.OnUnknownPriceApproval();
			trace(L7, _T("-ra.OnUnknownPriceApproval"));
		}
  
		// if there is Other Payment pending approval, the approval should be cleared
		if (fOtherPayment)
		{  // let ra know that Other Payment has be cleared.
			trace(L7, _T("+ra.OnOtherPaymentApproval"));
			ra.OnOtherPaymentApproval() ;
			trace(L7, _T("-ra.OnOtherPaymentApproval"));
			//fOtherPayment = false ;    save this flag for outstanding approval printing
		}
	}	// Tar 213053 end   
	//SSCOADK-361
	
	if(!bStoreModeApprovals)
	{
        fOtherPayment = false;
        lNotForSaleItems = 0;
        lTimeRestrictedItems = 0;
        lUnknownItems = 0;
        lUnknownItemsKeyed = 0;
        lPriceRequiredItems = 0;
	    fUnDeActivatedItem = false; // TAR 297872
		
	}
	if(lRestrictedNotAllowedItems)
	{
		lRestrictedNotAllowedItems = 0;
        trace(L7, _T("+ra.OnRestrictedNotAllowedApproval"));
        ra.OnRestrictedNotAllowedApproval();
        trace(L7, _T("-ra.OnRestrictedNotAllowedApproval"));
	}
    lQuantityRestrictedItems = 0;
    fRequestForHelp = false;
    // Begin TAR 197558
    if (fCantScanCoupon) 
    {
        // Approve Can't scan coupon, when returning from store mode
        fCantScanCoupon = false;
        // But Clear Approval related to coupon
        // only if Approval needed for coupon threshold is not required
        if (!isCouponRatioApprovalRequired())
        {
            trace(L7, _T("+ra.OnCouponApproval"));
            ra.OnCouponApproval();
            trace(L7, _T("-ra.OnCouponApproval"));
        }
    }
    // End TAR 197558
    bDropoffApproved = true;
    bSuspendFailure  = false;
    bSuspendApproved = true;
    lCouponsInAssist = 0;	
	*/
    
    ClearSecurityViolationLight();

	//TAR 289859 Move the reset this flag here instead of down at uninitialize() 
	bStoreModeGoBack = false;		// track use of Go Back(now Store Authorization) for action required approval
#endif
    //+SR819
	bool bRapValidHeadCashier;
	bool bInRemoteMode;

    //bInAssistMenuValidHeadCashier = g_bValidHeadCashier;  // SSCOP - 366 & 368
    bRapValidHeadCashier = ra.getRAPValidHeadCashier(ra.GetRemoteConnectionName());
	bInRemoteMode = ps.RemoteMode();

	trace(L6,_T("g_bValidHeadCashier = %d, getRAPValidHeadCashier(%s) = %d,  "), g_bValidHeadCashier, ra.GetRemoteConnectionName(), bRapValidHeadCashier);
	
    //In finalization and AssistMenu context is allowed in Scotopts at finalization
	if ( bIsInFinalization && bEnterTenderState  && co.fStateAssistMenuDuringFinalization ) 
	{
		//set context to the value of AssistMenuFinalizeHeadCashier for head cashier assistance at lane or RAP
		if( ((g_bValidHeadCashier && !bInRemoteMode) || (bRapValidHeadCashier && bInRemoteMode)) 
			&& !co.csStateAssistMenuFinalizeHeadCashier.IsEmpty() )
		{
			m_csMenuContext = co.csStateAssistMenuFinalizeHeadCashier;
		}
		
		//set context to the value of AssistMenuFinalizeContext for normal cashier assistance at lane or RAP		
		else if ( ((!g_bValidHeadCashier && !bInRemoteMode) || (!bRapValidHeadCashier && bInRemoteMode)) 
			&& !co.csStateAssistMenuFinalizeContext.IsEmpty() )
		{
			m_csMenuContext = co.csStateAssistMenuFinalizeContext;
		}
		
		//default is SmAssistMenu
		else
		{
			m_csMenuContext = ASSISTMENU_CONTEXT;
		}
        
	}
	//In itemization states
	else 
	{
		//set context to the value of AssistMenuItemizeHeadCashier for head cashier assistance at lane or RAP
		if( ((g_bValidHeadCashier && !bInRemoteMode) || (bRapValidHeadCashier && bInRemoteMode))
			&& !co.csStateAssistMenuItemizeHeadCashier.IsEmpty() )
		{
			m_csMenuContext = co.csStateAssistMenuItemizeHeadCashier; 
		}

		//set context to the value of AssistMenuItemizeContext for normal cashier assistance at lane or RAP
		else if ( ((!g_bValidHeadCashier && !bInRemoteMode) || (!bRapValidHeadCashier && bInRemoteMode)) 
			&& !co.csStateAssistMenuItemizeContext.IsEmpty() )
		{
			m_csMenuContext = co.csStateAssistMenuItemizeContext;
		}
        
		//default is SmAssistMenu
		else
		{
			m_csMenuContext = ASSISTMENU_CONTEXT;
		}

	}

	trace(L6,_T("m_csMenuContext = %s"), m_csMenuContext);
    //-SR819
#ifndef _CPPUNIT
	// +SSCOP-1703
	//Update RAP window with SCOT state
    UpdateStateName(EMPTY_STRING); // csStateName will be update with MSG_ASSISTMODE inside this call.
	// -SSCOP-1703

    CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMSmAssistMenuBase::Initialize"));

    ps.SetFrame(m_csMenuContext);
    ps.Echo(PS_BLANK);
    
    if (!ps.RemoteMode())
    {  //Don't enable local scanner for remote mode
		if (!bEASReEnableScanner)
        {
            trace(L2, _T("::Initialize - bEASReEnableScanner is set to true explicitly"));
            bEASReEnableScanner = true;
        }		

        SAWLDBScannerEnable();
    }
    else
    {
        SAWLDBScannerDisable();
    }
    tb.SetMonitorAll(false);  



    ps.ShowSMTotal(true);
    SetButtonStates();
	ShowAssistModeButton();
    if (m_bShowEchoBox)
    {
        DMSayPhrase(m_AudioID);
        ps.Prompt(ps.ConvertToEscapeSequence(ps.GetPSText(m_PromptTextID, SCOT_LANGUAGE_PRIMARY), _T("\\n"), _T("\n")));
        m_pAppPSX->SetConfigProperty(_T("Prompt"), m_csMenuContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_bShowEchoBox = false;
        m_AudioID = PS_NOSOUND;
        m_PromptTextID = PS_NOTEXT;
    }
    else
    {
        m_pAppPSX->SetConfigProperty(_T("Prompt"), m_csMenuContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
    }
    ps.ShowFrame();
	CPSXUserInputLock userInputLock(_T("SMSmAssistMenuBase::Initialize"));  //TAR 371980 fix 

    trace(L6,_T("-SMSmAssistMenuBase::Initialize()"));

	if(fStartTrxInAM)	// RFC 228102 Initiate transactions that will require hand scanning at RAP
	{
		if(!ps.RemoteMode())	// shouldn't ever happen but log it if it does occur
		{
			trace(L6,_T("!!trying to start Transaction and enter AM in local AM -- s/w problem!\n"));
		}

		SASendSCOTInput( ITEMIZATION );
		trace(L7,_T("+ra.OnTransactionStart"));
		ra.OnTransactionStart();
		trace(L7,_T("-ra.OnTransactionStart"));
		trace(L7,_T("+ra.OnTransactionState, Itemization State"));
		ra.OnSetTransactionState(RA_STATE_ITEMIZATION);
		trace(L7,_T("-ra.OnTransactionState"));
        bIsTransactionStarted = true; //TAR219278 CHU 0110602: used in SmSMmediastatus 
		setAnchorState(BES_SCANANDBAG);

		if(co.fStateDisplayNumberOfItemsOnRAP)
		{
		  trace(L7, _T("+ra.OnAddOtherEvent COMMAND=EnableNumItems;"));
		  ra.OnAddOtherEvent(_T("COMMAND=EnableNumItems;"));
		  trace(L7, _T("-ra.OnAddOtherEvent"));
		  CString csNumItems = EMPTY_STRING;
		  csNumItems.Format(_T("%d"), GetDisplayedItemCount());	// RFC 371075
		  trace(L7, _T("+ra.OnNormalItem, Sending the number of items to RAP"));
		  ra.OnNormalItem(_T("[NumItems]") + csNumItems);
		  trace(L7, _T("-ra.OnNormalItem, NumItems = %s"), csNumItems);
		}
		else
		{
		  trace(L7, _T("+ra.OnAddOtherEvent COMMAND=DisableNumItems;"));
		  ra.OnAddOtherEvent(_T("COMMAND=DisableNumItems;"));
		  trace(L7, _T("-ra.OnAddOtherEvent"));
		}

		static long rc;
		rc = tb.Start(co.Getlanguage(SCOT_LANGUAGE_PRIMARY).szCode);  // use language code as szOverrideCode parameter
		ProcessTBReturn(rc);	//Process TB return value
        if(rc == TB_SUCCESS_IMMEDIATE)
		{
	      fInSignedOn  = true;		 // must be signed on by now
	      fInSignedOff = false;
	      if (TrainingModeObj.IsTMOn())
		  {  
	         // TAR 248037 - Need to remove the ***, there is no room on RAP
	         CString csRAPReceiptLine = ps.GetPSText(MSG_TRAINING_MODE_LINE,SCOT_LANGUAGE_PRIMARY);
             int i = csRAPReceiptLine.FindOneOf(_T("***"));
	         csRAPReceiptLine = csRAPReceiptLine.Right(csRAPReceiptLine.GetLength() - (i+3));
             csRAPReceiptLine.TrimLeft();
             i = csRAPReceiptLine.FindOneOf(_T("***"));
	         csRAPReceiptLine = csRAPReceiptLine.Left(csRAPReceiptLine.GetLength() - (csRAPReceiptLine.GetLength()-i+1)); 
             csRAPReceiptLine.TrimRight();
             csRAPReceiptLine = RAP_RECEIPT_LINE_SUMMARY + csRAPReceiptLine;

	         trace(L7, _T("+ra.OnNormalItem, Message=%s"), csRAPReceiptLine);
	         ra.OnNormalItem(csRAPReceiptLine);
	         trace(L7, _T("-ra.OnNormalItem"));
		  }
	      fStartTrxInAM = false;
          trace(L6,_T("SMSmAssistModeBase::Started Transaction in AM"));
		}
        else
		{
		  fStartTrxInAM = false; //SSCOADK-6570
          trace(L6,_T("SMSmAssistModeBase::Started Transaction in AM, TB_SUCCESS, waiting on TB_READY to update keyboard"));
		}
	}
#endif
    return pRetState;
}


#ifndef _CPPUNIT
void SMSmAssistMenuBase::UnInitialize()
{
    trace(L6,_T("+SMSmAssistMenuBase::UnInitialize()"));
          
  //SSCOADK-5989 + If the scale display needs to be reset for customer language
  if (co.GetfStateDualLanguage() && m_customerLanguage != SCOT_LANGUAGE_PRIMARY && g_bCustomerLanguageChanged
      && co.fStateDisplayWeightsAndMeasures && dm.IsDeviceConfigured(DMCLASS_SCALE))  
	{
      DMConfigureScaleDisplay();  
	}
  //SSCOADK-5989 -     
    
    if (!g_bBackToBackDataNeeded)
    {
        SAClearSecurityCondition();
    }
    ps.Prompt(_T(""));
    trace(L6,_T("-SMSmAssistMenuBase::UnInitialize()"));
}
SMStateBase  *SMSmAssistMenuBase::PSButton1()
{
    trace(L6,_T("+SMSmAssistMenuBase::PSButton1()"));
    return OnKeyInCode();  
}

SMStateBase  *SMSmAssistMenuBase::PSButton2()
{
    trace(L6,_T("+-SMSmAssistMenuBase::PSButton2()"));
    return OnVoidButton();
}
SMStateBase  *SMSmAssistMenuBase::PSButton3()
{
    trace(L6,_T("+-SMSmAssistMenuBase::PSButton3()"));
    return OnChangePrice();
}
SMStateBase  *SMSmAssistMenuBase::PSButton4()
{
    trace(L6,_T("+-SMSmAssistMenuBase::PSButton4()"));
    return OnChangeQuantity();
}
#endif //_CPPUNIT

SMStateBase  *SMSmAssistMenuBase::PSButton5()
{
    trace(L6,_T("+SMSmAssistMenuBase::PSButton5()"));

    // disable Key in code button to prevent being pressed
    ps.ButtonEnable(_T("SMButton1"), false);

    return OnAssistModeButton();
}

#ifndef _CPPUNIT

SMStateBase  *SMSmAssistMenuBase::PSButton8(void)
{
	//comment line to fix tar 399990
   // bStoreModeGoBack = true;  //TAR#346761 

    ps.ButtonEnable(_T("SMButton1"), false);
    ps.ButtonEnable(_T("SMButton2"), false);  
    ps.ButtonEnable(_T("SMButton3"), false);
    ps.ButtonEnable(_T("SMButton4"), false);
    ps.ButtonEnable(_T("SMButton5"), false);
    ps.ButtonEnable(_T("SMButton6"), false);
    ps.ButtonEnable(_T("SMButton7"), false);
    ps.ButtonEnable(_T("SMButton8"), false);


	//SSCOADK-2074+
	if ( (nCustomerAge > 0) && !co.GetfOperationsAllowSaleofRestrictedItems()) 
	{ 
         nCustomerAge = 0; 
         fAgeObtained = false; 
	} 
	//SSCOADK-2074-
	
    return OnReturnToShopping();
}
#endif
///////////////////////////////////////////
// Button 8 - Return to Shopping
///////////////////////////////////////////
SMStateBase  *SMSmAssistMenuBase::OnReturnToShopping(void)
{
#ifndef _CPPUNIT
    // + TAR 406595
     if(co.IsInAssistMenus() && !ps.RemoteMode()) //SSCOADK-1569: it is set in PSOnRemoteDisconnected() for RemoteMode
     {
        co.SetInAssistMenus(false);
     }
    // - TAR 406595

    bFromAssistMenu = true; //SSCOB-18178 SSCOADK-6286
    ps.ButtonState(_T("SMButton8"), false, true);

	// TAR 294889
	if (dm.fStateTakeawayBelt)
	{
		CKeyValueParmList parmList;
		BSTR bstrResult;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_ASSIST_MODE_STOP);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		bstrResult = NULL;
		SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
		::SysFreeString(bstrResult); // cs185088 mem
	} // TAR 294889
    SendTBCommand(TBCMD_ASSISTMENUSET, false);
    //ps.SetLanguage(m_customerLanguage);
    m_csMenuContext.Empty();
	if (co.GetfStateDualLanguage() && m_customerLanguage != SCOT_LANGUAGE_PRIMARY)  
	{
        //SSCOADK-5824
        g_bCustomerLanguageChanged = true; //SSCOADK-5989
	}

    if(gpSCOTAuxDisp)
        gpSCOTAuxDisp->SetLanguage(m_customerLanguage);
    
    BEST best;
    if(fCancelSuspend)
    {
        fCancelSuspend = false;
        setAnchorState(nSellState);
    }
    else
        best = getAnchorState();
	//TAR 410287 Need to set anchor state to Scan and Bag 
	if (best == BES_PUTBAGONSCALE || best == BES_ENTERQUANTITY || best == BES_SELECTTARE || best == BES_VOIDITEM)
	{
		setAnchorState(BES_SCANANDBAG);
        best = getAnchorState();
	}

    
    //TAR# 195639 Money accepted while system was entering assist mode should also be checked for.
    //If cash payment is greater than amount due, system should go to Finish state upon return.
    if (nDMCashInserted && ((lBalanceDue - nDMCashInserted) <= 0))
    {
        setAnchorState(BES_FINISH);
    }

	if (isAnyApprovalRequired())
		dm.restoreTriColorLight();

    if (!ps.RemoteMode())
    {
        if (isAnyApprovalRequired())
            
        {
          //  dm.restoreTriColorLight();
            if (best == BES_FINISH) // gotta approve
            {
                if (lSAYellowNeedingApproval)
                {
                    // (*) SR898 ====================
                    DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                                       DM_TRICOLORLIGHT_ON,
                                       DM_TRICOLORLIGHT_NORMALREQUEST);
                }
                trace(L7, _T("+ra.OnCancelApplicationError"));
                ra.OnCancelApplicationError();	// go back to wait for approval
                trace(L7, _T("-ra.OnCancelApplicationError"));
                
                //SSCOADK-4397, Tell security mangaer that we are exiting Assistmode and Setting wait for approval
                TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode") );	

                CREATE_AND_DISPENSE(WaitForApproval)(false,false);
            }
            else if (best == BES_SCANANDBAG) // went in here from pressing help, can wait to approve later
            {	
                
                //if (lSAYellowNeedingApproval)
                // the flag lSAYellowNeedingApproval is not set propoerly
                // So using the explicit tests if we need a yellow light before we go to shopping.
                // When we enter the Store Authorization screen, the initialize make the light green
                // as most approvals are just approved by returning to shopping. 
                // Only these 3 (excluding void as that is always handled as an immediate Intervention
                // so light turns back to Red) if still not approved need a yellow light.
                if (isRestrictedItemApprovalRequired() || isVisualItemApprovalRequired() 
                    || isCouponRatioApprovalRequired())
                {
                    // Tar 209546 if turning yellow, make sure red is off
                    
                    // (*) SR898 ====================
                    DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                                       DM_TRICOLORLIGHT_OFF,
                                       DM_TRICOLORLIGHT_NORMALREQUEST);
                                       
                    // (*) SR898 ====================
                    DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                                       DM_TRICOLORLIGHT_ON,
                                       DM_TRICOLORLIGHT_NORMALREQUEST);
                }
                TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode") );	//SSCOP-3429
                return createAnchorState();
            }
            else if (best == BES_START)		//TAR#122340		Kotes
            {
                if (lSAYellowNeedingApproval)
                {
                    trace(L7, _T("+ra.OnCancelApplicationError"));
                    ra.OnCancelApplicationError();		// go back to wait for approval
                    trace(L7, _T("-ra.OnCancelApplicationError"));
                    CREATE_AND_DISPENSE(WaitForApproval)(false,false);
                }
            }
            
        }
        else if (dm.GetPrinterPaperLow())
        {
            // (*) SR898 ====================
            DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                               DM_TRICOLORLIGHT_BLINK_1HZ,
                               DM_TRICOLORLIGHT_NORMALREQUEST);  
        }
        else //Tar 180598: turn lane light green if nothing else to clear. Ch082001
        {
            // (*) SR898 ====================
            DMTurnOffTriColorLight();
            
            // (*) SR898 ====================
            DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                               DM_TRICOLORLIGHT_ON,
                               DM_TRICOLORLIGHT_NORMALREQUEST);
        }
        
        //Tar #158619 only suspend gets us out of loop if we wait to reset here ->  fOtherPayment = false;
        // don't show Deposit Coupons screen if coupon sensor is configured
        if (lCoupons && lUnapprovedCoupons && !IsUsingCouponSensor()) 
        {
            if (!bDropoffRequired && (best != BES_SCANANDBAG))
                return DepositCouponsIfNecessary();
        }
    }
    else
    {
        bSuspendFailure = false;  //if in remote mode, need to reset suspend failure value
        trace(L6, _T("RAP exiting remote store mode"));
    }
#endif
    //sscoadk-3385
    if (dm.IsScannerEnabled())
    {
       //disabling it now, and enabling it after timed out to avoid scanning
       trace(L6, _T("Disabling the scanner for now until it is timed out"));
       DMScannerDisable();
       bScannerEnabledSaved = true; 
    }

    if(!isProcessingGenericDelayedIntervention())
    {
        setAnchorState(BES_SCANANDBAG);
    }

#ifndef _CPPUNIT
    nTimeOut = ASSISTMENU_RETURN_TIMEOUT;
#endif
    return STATE_NULL;  //use timeout to wait one second for weight events
}

#ifndef _CPPUNIT
void SMSmAssistMenuBase::SetButtonStates()
{
    trace(L6,_T("+SMSmAssistMenuBase::SetButtonStates()"));

    CString csItemEntryId = ps.GetSelection(_T("CMReceipt")); //SSCOADK-1878
    trace(L6, _T("csItemEntryId from CMReceipt is %s"), csItemEntryId);

    //RFQ 986  - Button states will now be controlled by PSX xml "Action" for 
    //           "SelectedItem" control in PSX context definition

    if (m_GenericDelayedInterventionStatus)
    {
       trace(L6, _T("No assist menu when in GenericDelayed: GDstatus = %d"), m_GenericDelayedInterventionStatus);
       return;
    }
    CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMSmAssistMenuBase::SetButtonState"));
    COleVariant v = _T(" ");
	m_pAppPSX->SetConfigProperty(_T("SelectedItem"), m_csMenuContext, UI::PROPERTYTEXT, v);

	v = csItemEntryId;
	m_pAppPSX->SetConfigProperty(_T("SelectedItem"), m_csMenuContext, UI::PROPERTYTEXT, v);
	if (!csItemEntryId.GetLength())
	{
		ps.ButtonEnable(_T("SMButton2"), false);
        ps.ButtonEnable(_T("SMButton3"), false);
        ps.ButtonEnable(_T("SMButton4"), false);
	}
    else
	{
		ps.ButtonEnable(_T("SMButton2"), true);

        COleVariant v;
		m_pAppPSX->GetConfigProperty(_T("SMButton3"), m_csMenuContext, UI::PROPERTYVISIBLE, v);
        if (v.boolVal == VARIANT_TRUE)
        {
            ps.ButtonEnable(_T("SMButton3"), true);
        }
        else
        {
            ps.ButtonEnable(_T("SMButton3"), false);
        }

		m_pAppPSX->GetConfigProperty(_T("SMButton4"), m_csMenuContext, UI::PROPERTYVISIBLE, v);
        if (v.boolVal == VARIANT_TRUE)
        {
            ps.ButtonEnable(_T("SMButton4"), true);
        }
        else
        {
            ps.ButtonEnable(_T("SMButton4"), false);
        }
	} 
    ps.ButtonEnable(_T("SMButton1"), true);
    ps.ButtonState(_T("SMButton8"), true, true);
    //RFQ 986
    trace(L6,_T("-SMSmAssistMenuBase::SetButtonStates()"));

}

void SMSmAssistMenuBase::ShowAssistModeButton()	//SSCOADK-667
{
	STATE(StoreModeMenu)::ShowAssistModeButton();
    //Override the display of the command button - CmdBtn_AssistMode 
    if(!co.fStateAssistMode)
		ps.ButtonState(_T("SMButton5"), false, false);
	else
		ps.ButtonState(_T("SMButton5"), true, true);
	
}

SMStateBase *SMSmAssistMenuBase::DMScanner(void)
{
    //RFQ 986
    if (m_bBlockScanData)
    {
        trace(L6, _T("Ignoring scan received while waiting for CmdBtn command response"));
        return STATE_NULL;
    }
    //RFQ 986
    csItemSent = csDMLastBarCode;

	io.Reset(); //406738
    /*SSCOP-193  Scanner will be enabled upon return to SmAssistMenu state, can't enable here because we don't
    know what state TB will return
    // If not in remote mode, reenable scanner
    if (!ps.RemoteMode())
    {
        trace(L2, _T("::DMScanner, if ps.RemoteMode() = false, - bEASReEnableScanner is set to true."));
        bEASReEnableScanner = true;
        SAWLDBScannerEnable();
    }*/

	setAnchorState(BES_SCANANDBAG);	// TAR 399975
    return TBItemSale(false,0,true);  
}


SMStateBase  *SMSmAssistMenuBase::OnVoidButton()
{
    RETURNSTATE(VoidItem);
}

#endif //_CPPUNIT
SMStateBase *SMSmAssistMenuBase::OnAssistModeButton()
{
    RETURNSTATE(SmAssistMode);
}
#ifndef _CPPUNIT

SMStateBase *SMSmAssistMenuBase::OnKeyInCode()
{
    RETURNSTATE(KeyInCode);
}

SMStateBase  *SMSmAssistMenuBase::OnChangePrice()
{
    CREATE_AND_DISPENSE(SmUpdateItem)(ITEMUPDATE_PRC);
}

SMStateBase  *SMSmAssistMenuBase::OnChangeQuantity()
{
    CREATE_AND_DISPENSE(SmUpdateItem)(ITEMUPDATE_QTY);
}

SMStateBase *SMSmAssistMenuBase::TBParse(MessageElement *me)
{
    SMStateBase *pState = STATE(StoreModeMenu)::TBParse( me);

    SetButtonStates();
    return pState;
}
#endif

SMStateBase  *SMSmAssistMenuBase::TimedOut(void)
{
#ifndef _CPPUNIT
    trace(L6,_T("+SMSmAssistMenuBase::TimedOut()"));
	SynchronizeStoreloginWithLaunchPad(_T("operation=set-signed-off")); // TAR 301706
    co.SetInAssistMenus(false);
	if (dm.fStateTakeawayBelt && ps.RemoteMode())
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmRemoteAssistMode") );
		else
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode") );
    	//+TAR408719
	// +TAR 392605
	// Mimicked this condition from a fix in SMSmAssistModeBase (TAR 369813).
	// Will pop a state only if fStateAssist is true.. 
	//if(!fStateAssist) 
	//{
	//	SMStateBase* sReturnState = CkStateStackAndReturn();  //TAR213624 CHU 041503
	//	if (sReturnState != STATE_NULL)
	//		return sReturnState;
	//}
	// set fStateAssist to false to avoid being in a frozen state...
	//-TAR408719
	fStateAssist = false;
	// -TAR 392605
#endif
    //sscoadk-3385
    if (bScannerEnabledSaved)
    {
       //scanner was enabled before timed out so enabling it now
       trace(L6, _T("Enabling the scanner since it was enabled before timed out"));
       DMScannerEnable();
       bScannerEnabledSaved = false; 
    }
    
    BEST best;
    if(fCancelSuspend)
    {
        fCancelSuspend = false;
        setAnchorState(nSellState);
    }
    else
        best = getAnchorState();

    if (best == BES_START)	// if came from attract then return there instead of to the thank you screen 
        return setAndCreateAnchorState(BES_START);
    
    if (getAnchorState()==BES_INSERTCOUPON) 
    {
        if (bEnterTenderState)
            return setAndCreateAnchorState(BES_ENTERCOUPONS);
        else
            return setAndCreateAnchorState(BES_SCANANDBAG);
    }
	else if (getAnchorState()==BES_VOIDITEM)
		return setAndCreateAnchorState(BES_SCANANDBAG);

    else
    {
       if(getAnchorState() == BES_PRODUCEFAVORITES && co.GetfOperationsAllowMultiPick())
       {
            fInMultiSelectPickList = true;
       }
        // otherwise, back to caller...
        return createAnchorState();
    }
}

#ifndef _CPPUNIT
SMStateBase  *SMSmAssistMenuBase::PSRemoteButton1(void)		// RFC 209075 RJF 07/31/02
{
	trace(L6, _T("SMSmAssistModeBase::PSRemoteButton1(), g_bAllowRemoteBtn1=%d"),g_bAllowRemoteBtn1);

	return PSButton8();
}/////////////////////////////////////

void SMSmAssistMenuBase::SetAssistMessage(PSTEXT textID, PSTEXTWAVE waveID)
{
    if(g_csAssistModeStateName == _T("SmAssistMode")) 
    {
        trace(L6,_T("::SetAssistMessage no sound if in assistmode"));
        m_AudioID = PS_NOSOUND; //no sound if in assistmode
    }
    else
    {
        m_AudioID = waveID;
    }
    m_PromptTextID = textID;
    if (textID == PS_NOTEXT && waveID == PS_NOSOUND)
    {
        m_bShowEchoBox = false;
    }
    else
    {
        m_bShowEchoBox = true;
    }
}

/////////////////////////////////////

bool SMSmAssistMenuBase::IsFullAssistNeeded()
{
    bool bFullAssistNeeded = false;
    if (bIsInFinalization)
    {
        bFullAssistNeeded = true;
    }
    return bFullAssistNeeded;
}

/////////////////////////////////////
// RFQ 986
// PSSwitchContext 
// 
// Purpose: Handle Switch Context event from PSX
// Parameters
//   bRemote - whether the switch occurred on the remote display
//   csNewContext - the name of the new context
SMStateBase *SMSmAssistMenuBase::PSSwitchContext(BOOL bRemote, CString csNewContext)
{
    trace(L6, _T("+SMSmAssistMenuBase::PSSwitchContext(%d, %s)"),bRemote, csNewContext);
    // Set Frame if context change is in the active context
    if( bRemote == ps.RemoteMode())
    {
        if (csNewContext.Find(_T("RAPSingle")) != 0) //Don't switch for RAP toggle
        {
            m_csMenuContext = csNewContext;
            ps.SetFrame(csNewContext);
            SetButtonStates();
        }
    }
    m_bBlockScanData = false;
    trace(L6, _T("-SMSmAssistMenuBase::PSSwitchContext()"),bRemote, csNewContext);

    return STATE_NULL;
}


/////////////////////////////////////
// RFQ 986
// PSCmdButton
// 
// Purpose: Handle "Command" button press.  This method is called
//          when a click occurs on a button or button list named "CmdBtn_X..."
//          where any sequence of characters can follow the "_"
// Parameters
//   csContext - name of the context 
//   csControl - name of the control
//   csCommand - contents of the control "Data", defined either in a button's "Data"
//               property or a button list's "List" property
//        		<Control Name="CmdBtn_Button1" Type="Button">
//                  <Data>control string</Data>
//              </Control>
//              <Control Name="CmdBtn_List1" Type="ButtonList">
//                  <List>
//                     <Button>Label1,-,control string 1</Button>
//                     <Button>Label1,-,control string 2</Button>
//                  </List>
//              </Control>
///////////////////////////////////////
SMStateBase *SMSmAssistMenuBase::PSCmdButton(CString csContext, CString csControl, CString csCommand)
{
    trace(L6,_T("+SMSmAssistMenuBase::PSCmdButton(%s, %s, %s)"), csContext, csControl, csCommand);
    
    CString csValue;
    
    csValue.Empty();
    m_pAppPSX->GetCustomDataVar(_T("IgnoreScans"), csValue, csControl, csContext);
    if (csValue.CompareNoCase(_T("true")) == 0)
    {
        m_bBlockScanData = true; //Ignore scans until next context switch
    }

    SMStateBase *retState = STATE_NULL;
	if (csCommand.CompareNoCase(_T("OnChangePrice")) == 0)
        retState = OnChangePrice();
    else if (csCommand.CompareNoCase(_T("OnChangeQuantity")) == 0)
        retState = OnChangeQuantity();
    else if (csCommand.CompareNoCase(_T("OnReturnToShopping")) == 0)
        retState = OnReturnToShopping();
    else if (csCommand.CompareNoCase(_T("OnVoidButton")) == 0)
        retState = OnVoidButton();
    else if (csCommand.CompareNoCase(_T("OnAssistModeButton")) == 0)
        retState = OnAssistModeButton();
    else if (csCommand.CompareNoCase(_T("OnKeyInCode")) == 0)
        retState = OnKeyInCode();
    else 
    {
        //retState = STATE(State)::PSCmdButton(csContext, csControl, csCommand);   //SR819
		retState = STATE(StoreModeMenu)::PSCmdButton(csContext, csControl, csCommand);   //SR819
    }


    trace(L6, _T("-SMSmAssistMenuBase::PSCmdButton()"));
	return retState;
}

SMStateBase *SMSmAssistMenuBase::PSOnRemoteDisconnected(const long lParm)
{
    trace(L6,_T("+SMSmAssistMenuBase::PSOnRemoteDisconnected()"));
    m_csMenuContext.Empty();
    return STATE(StoreModeMenu)::PSOnRemoteDisconnected(lParm);
}

SMStateBase *SMSmAssistMenuBase::OnRAPUnsolicitedDataReceived(LPCTSTR sData)
   {
   trace (L6, _T("SMSmAssistMenuBase::OnRAPUnsolicitedDataReceived(%s)."), sData);
   
   return STATE(StoreModeMenu)::OnRAPUnsolicitedDataReceived(sData);
}

CString SMSmAssistMenuBase::ExtractString(CString strData, CString sStringName)
{
    CString sString;
    bool bStatus = false;
    
    int iFind = strData.Find(sStringName);
    
    if (iFind >= 0)
    {
        sString = strData.Mid(iFind);
        iFind = sString.Find(_T("="));
        ASSERT(iFind > 0);
        if (iFind > 0)
        {
            sString = sString.Mid(iFind + 1);
            iFind = sString.Find(_T(";"));
            if (iFind > 0)
            {
                sString = sString.Left(iFind);
                bStatus = true;
            }
        }
        if (!bStatus)
            sString.Empty();
    }
    return sString;
}

SMStateBase *SMSmAssistMenuBase::OnWtDecrease()
{
	trace (L6, _T("SMSmAssistMenuBase::OnWtDecrease()"));
	OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR);
	return STATE_NULL;
}

SMStateBase *SMSmAssistMenuBase::DepositCouponsIfNecessary()
{
  if (bIsInFinalization)
      return setAndCreateAnchorState(BES_FINISH);
  else
      return setAndCreateAnchorState(BES_SCANANDBAG);
}

#endif _CPPUNIT