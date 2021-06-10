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
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSmAssistMenuBase.h"

#include "SMFinish.h"
#include "SMScanAndBag.h"
#include "SMWaitForApproval.h"
#include "SMVoidItem.h"
#include "SMSmAssistMode.h"
#include "SMKeyInCode.h"
#include "SMSmUpdateItem.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmAssistMode")

#define ASSISTMENU_CONTEXT  _T("SmAssistMenu")
#define ASSISTMENU_RETURN_TIMEOUT 2

IMPLEMENT_DYNCREATE(SMSmAssistMenuBase, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

CString SMSmAssistMenuBase::m_csMenuContext= ASSISTMENU_CONTEXT;
bool SMSmAssistMenuBase::m_bShowEchoBox = false;
bool SMSmAssistMenuBase::bReturnToShoppingNow = false;
PSTEXTWAVE  SMSmAssistMenuBase::m_AudioID = PS_NOSOUND;
PSTEXT      SMSmAssistMenuBase::m_PromptTextID = PS_NOTEXT;

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

SMStateBase* SMSmAssistMenuBase::Initialize()
{
    trace(L6,_T("+SMSmAssistMenuBase::Initialize()"));

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
	}

    SMStateBase* pRetState = STATE_NULL;
	nTimeOut=0;

	io.Reset(); //406738

    if (bReturnToShoppingNow)
    {
        bReturnToShoppingNow = false;
        return OnReturnToShopping();
    }

    EnableHandheldAssistMode(true);  //In case SMFinish disabled

	if (!co.IsInAssistMenus())
	{
		if (dm.fStateTakeawayBelt && ps.RemoteMode())
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmRemoteAssistMode") );
		else
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmAssistMode") );
	}
    co.SetInAssistMenus(true);
    co.SetContextPrefixAM(_T("AM_"));
    SendTBCommand(TBCMD_ASSISTMENUSET, true);


	CString csItemEntryId = ps.GetSelection(_T("SMReceipt"));
	if (csItemEntryId.Find(_T("ORIGPRICE")) != -1)
	{
        ps.ScrollDown(_T("SMReceipt"));
        ps.ScrollDown(_T("CMReceipt"));
        ps.ScrollDown(_T("CMReceiptVoid"));
	}

    g_bClearOccurred = true;


	// Tar 213053 RAP AM neds to clear certain approvals like going into SM at lane
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

	fOtherPayment = false;
	lNotForSaleItems = 0;
    lTimeRestrictedItems = 0;
    lUnknownItems = 0;
    lUnknownItemsKeyed = 0;
    lPriceRequiredItems = 0;
	fUnDeActivatedItem = false; // TAR 297872

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
    
    ClearSecurityViolationLight();

	//TAR 289859 Move the reset this flag here instead of down at uninitialize() 
	bStoreModeGoBack = false;		// track use of Go Back(now Store Authorization) for action required approval

    CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMSmAssistMenuBase::Initialize"));

    ps.SetFrame(m_csMenuContext);
    ps.Echo(PS_BLANK);
    
    if (!ps.RemoteMode())
    {  //Don't enable local scanner for remote mode
        SAWLDBScannerEnable();
    }
    else
    {
        SAWLDBScannerDisable();
    }
    tb.SetMonitorAll(false);  



    ps.ShowSMTotal(true);
    SetButtonStates();
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
		rc = tb.Start(co.language[SCOT_LANGUAGE_PRIMARY].szCode);  // use language code as szOverrideCode parameter
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
          trace(L6,_T("SMSmAssistModeBase::Started Transaction in AM, TB_SUCCESS, waiting on TB_READY to update keyboard"));
		}
	}

    return pRetState;
}
void SMSmAssistMenuBase::UnInitialize()
{
    trace(L6,_T("+SMSmAssistMenuBase::UnInitialize()"));
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
SMStateBase  *SMSmAssistMenuBase::PSButton5()
{
    trace(L6,_T("+SMSmAssistMenuBase::PSButton5()"));
    return OnAssistModeButton();
}

SMStateBase  *SMSmAssistMenuBase::PSButton8(void)
{
	//comment line to fix tar 399990
   // bStoreModeGoBack = true;  //TAR#346761 
    return OnReturnToShopping();
}
///////////////////////////////////////////
// Button 8 - Return to Shopping
///////////////////////////////////////////
SMStateBase  *SMSmAssistMenuBase::OnReturnToShopping(void)
{
    // + TAR 406595
     if(co.IsInAssistMenus())	
     {			
        co.SetInAssistMenus(false);
     }
    // - TAR 406595
    ps.ButtonState(_T("SMButton8"), false, true);
    if (!ps.RemoteMode())
    {
        fOtherPayment = false ; //Tar #158619	reset here so we can return to shopping if needed
        lNotForSaleItems = 0;
        lTimeRestrictedItems = 0;
        lUnknownItems = 0;
        lUnknownItemsKeyed = 0;
        lPriceRequiredItems = 0;
		if(lRestrictedNotAllowedItems)
		{
			lRestrictedNotAllowedItems = 0;
			trace(L7, _T("+ra.OnRestrictedNotAllowedApproval"));
			ra.OnRestrictedNotAllowedApproval();
			trace(L7, _T("-ra.OnRestrictedNotAllowedApproval"));
		}
        lQuantityRestrictedItems = 0;
        fRequestForHelp = false;
		fUnDeActivatedItem = false;
        bStoreModeApprovals = false;	// 3.1 req 4.4.5 streamline storemode screens
        
        
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
        
        //Reseting LGW only if SmartScale fires a weight event (in SMStateSABase.cpp)
        //That way app can remember if an error occurred before entering Store mode and process that error
        //ss.SetLastGoodWeight(0x8000000);		// TAR196221 what ever bag scale event happens in store mode should reset the smart scale LPM022002
    }	// end of if (!ps.RemoteMode())
	// TAR 294889
	if (dm.fStateTakeawayBelt)
	{
		CKeyValueParmList parmList;
		BSTR bstrResult;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_ASSIST_MODE_STOP);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		bstrResult = NULL;
		SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
	} // TAR 294889
    SendTBCommand(TBCMD_ASSISTMENUSET, false);
    ps.SetLanguage(m_customerLanguage);
    m_csMenuContext.Empty();
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
	if (best == BES_PUTBAGONSCALE)
	{
		setAnchorState(BES_SCANANDBAG);
	}

	if (isAnyApprovalRequired())

    
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
                    dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                        DM_TRICOLORLIGHT_ON,
                        DM_TRICOLORLIGHT_NORMALREQUEST);
                }
                trace(L7, _T("+ra.OnCancelApplicationError"));
                ra.OnCancelApplicationError();	// go back to wait for approval
                trace(L7, _T("-ra.OnCancelApplicationError"));
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
                    || isCouponRatioApprovalRequired() )
                {
                    // Tar 209546 if turning yellow, make sure red is off
                    dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                        DM_TRICOLORLIGHT_OFF,
                        DM_TRICOLORLIGHT_NORMALREQUEST);
                    dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                        DM_TRICOLORLIGHT_ON,
                        DM_TRICOLORLIGHT_NORMALREQUEST);
                }
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
            dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
                DM_TRICOLORLIGHT_BLINK_1HZ,
				            DM_TRICOLORLIGHT_NORMALREQUEST);  
        }
        else //Tar 180598: turn lane light green if nothing else to clear. Ch082001
        {
            dm.turnOffTriColorLight();
            dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
                DM_TRICOLORLIGHT_ON,
                DM_TRICOLORLIGHT_NORMALREQUEST);
        }
        
        //Tar #158619 only suspend gets us out of loop if we wait to reset here ->  fOtherPayment = false;
        // don't show Deposit Coupons screen if coupon sensor is configured
        if (lCoupons && !IsUsingCouponSensor()) 
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

    nTimeOut = ASSISTMENU_RETURN_TIMEOUT;
    return STATE_NULL;  //use timeout to wait one second for weight events
}

void SMSmAssistMenuBase::SetButtonStates()
{
    trace(L6,_T("+SMSmAssistMenuBase::SetButtonStates()"));
    CString csItemEntryId = ps.GetSelection(_T("SMReceipt"));

    //RFQ 986  - Button states will now be controlled by PSX xml "Action" for 
    //           "SelectedItem" control in PSX context definition

    CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMSmAssistMenuBase::SetButtonState"));
    COleVariant v = _T(" ");
	m_pAppPSX->SetConfigProperty(_T("SelectedItem"), m_csMenuContext, UI::PROPERTYTEXT, v);

	v = csItemEntryId;
	m_pAppPSX->SetConfigProperty(_T("SelectedItem"), m_csMenuContext, UI::PROPERTYTEXT, v);
/*	if (!csItemEntryId.GetLength())
	{
		ps.ButtonEnable(_T("SMButton2"), false);
        ps.ButtonEnable(_T("SMButton3"), false);
        ps.ButtonEnable(_T("SMButton4"), false);
	}
    else
	{
		ps.ButtonEnable(_T("SMButton2"), true);
        ps.ButtonEnable(_T("SMButton3"), true);
        ps.ButtonEnable(_T("SMButton4"), true);
	} */
    ps.ButtonState(_T("SMButton8"), true, true);
    //RFQ 986
    trace(L6,_T("-SMSmAssistMenuBase::SetButtonStates()"));

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
    
    // If not in remote mode, reenable scanner
    if (!ps.RemoteMode())
    {
        trace(L2, _T("::DMScanner, if ps.RemoteMode() = false, - bEASReEnableScanner is set to true."));
        bEASReEnableScanner = true;
        SAWLDBScannerEnable();
    }
	setAnchorState(BES_SCANANDBAG);	// TAR 399975
    return TBItemSale(false,0,true);  
}


SMStateBase  *SMSmAssistMenuBase::OnVoidButton()
{
    RETURNSTATE(VoidItem);
}

SMStateBase *SMSmAssistMenuBase::OnAssistModeButton()
{
    RETURNSTATE(SmAssistMode);
}

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
#ifdef _BUILDLIB_
	SMStateBase *pState = SMStateBase::TBParse( me);
#else
    SMStateBase *pState = SMState::TBParse( me);
#endif

    SetButtonStates();
    return pState;
}

SMStateBase  *SMSmAssistMenuBase::TimedOut(void)
{
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
        // otherwise, back to caller...
        return createAnchorState();
}

SMStateBase  *SMSmAssistMenuBase::PSRemoteButton1(void)		// RFC 209075 RJF 07/31/02
{
	trace(L6, _T("SMSmAssistModeBase::PSRemoteButton1(), g_bAllowRemoteBtn1=%d"),g_bAllowRemoteBtn1);

	return PSButton8();
}/////////////////////////////////////

void SMSmAssistMenuBase::SetAssistMessage(PSTEXT textID, PSTEXTWAVE waveID)
{
    m_AudioID = waveID;
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
        retState = STATE(State)::PSCmdButton(csContext, csControl, csCommand);
    }


    trace(L6, _T("-SMSmAssistMenuBase::PSCmdButton()"));
	return retState;
}

SMStateBase *SMSmAssistMenuBase::PSOnRemoteDisconnected(const long lParm)
{
    trace(L6,_T("+SMSmAssistMenuBase::PSOnRemoteDisconnected()"));
    m_csMenuContext.Empty();
    return STATE(State)::PSOnRemoteDisconnected(lParm);
}

SMStateBase *SMSmAssistMenuBase::OnRAPUnsolicitedDataReceived(LPCTSTR sData)
   {
   trace (L6, _T("SMSmAssistMenuBase::OnRAPUnsolicitedDataReceived(%s)."), sData);
   
   return SMStateBase::OnRAPUnsolicitedDataReceived(sData);
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