//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCustomMessage.cpp
//
// TITLE: Class implementation for CustomMessage State
//
// POS334365 Work Request:79417 Name:Alex Bronola  Date:January 09, 2018
// POS324824 Work Request:79417 Name:Alex Bronola  Date:June 27, 2017
// POS311584 Work Request:76936 Name:Alex Bronola  Date:January 23, 2017
// POS297659 WORK REQUEST:76936 Name: Alex Bronola Date: Dec 15, 2016
// POS273548  WORK REQUEST:73594 Name: Robert Susanto Date: July  1, 2016
// POS87264 WORK REQUEST:61232 Name: Robert Susanto Date: March 19, 2014
// POS69170 WORK REQUEST:56285 Name: Robert Susanto Date: June 28, 2013
// POS62550 Work Request:54416 Name: Robert Susanto    Date:April 29, 2013
// POS61973 Work Request:54416 Name: Robert Susanto    Date:April 26, 2013
// POS53451 Work Request:52635 Name: Robert Susanto    Date:January 28, 2013
// POS39476/52217 Work Request:52635 Name: Robert Susanto    Date:January 8, 2013
// POS39476/51660 Work Request:52635 Name: Robert Susanto    Date:December 19, 2012
// POS50227 Work Request:52635 Name: Matt Condra    Date:January 2, 2013
// POS50102 Work Request:52635 Name: Matt Condra    Date:December 6, 2012
// POS33020 Work Request:52635 Name: Matt Condra    Date:November 16, 2012
// POS39475 Work Request:52635 Name: Matt Condra    Date:November 10, 2012
// POS36318 WORK REQUEST:50864 Name: Robert Susanto Date: June 15, 2012
// POS35460 WORK REQUEST:19114 Name: Robert Susanto Date: May 31, 2012
// POS18049 WORK REQUEST:16656 Name: Robert Susanto Date: November 21, 2011//
// POS11024 Work Request:16632 Name:Sandeep Shenoy Date:October 12, 2011
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
 
#include "SMState.h"                // Base state
#include "SMCustomMessage.h"        // This state

#include "SMInProgress.h"
#include "transbrokerssf.h"
#include "SMSmGenericDelayed.h"	//CRD 185620
#include "SMInsertCoupon.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("CustomMessage")

IMPLEMENT_DYNCREATE(SMCustomMessage, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMCustomMessage::SMCustomMessage()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMCustomMessage::Initialize(void)
{
    COleVariant v; // POS39475
    static COleVariant vFontInstructionTextArea (_T("")); // POS33020

	//	start CRD 125377
	trace(L6, _T("m_bShopAndDropEnabled=%d m_bSNDAllowItemMenu=%d"), m_bShopAndDropEnabled, m_bSNDAllowItemMenu);
	if (m_bShopAndDropEnabled)
	{
		m_bSNDAllowItemMenu = true;		// CRD 125377
		trace(L6, _T("m_bSNDAllowItemMenu=%d"), m_bSNDAllowItemMenu);
	}
	//  end CRD 125377
	//(+) VSTS 131568
	if(SMState::m_bCustomVoidItemMessage)
	{
		CustomMessageObj.csScreenText = ps.GetPSText(SSF_TXT_VOIDITEM);
		trace(L6, _T("fStateFreeze is true,csSetItemAsideText : %s"), csSetItemAsideText);
	}

  //(-) VSTS 131568
  
    if ( (CustomMessageObj.csLeadThruText == ps.GetPSText(SSF_LTT_HIGHVALUE_PROMO)) &&
		(GetHookProperty(_T("tb_ExpirationDate")) == "" && GetHookProperty(_T("tb_int")) == "780") //POS324824
       )
    {
        // POS50102: A false TB trigger occurred for the mPerks high value promo custom message screen,
        // which means the POS state is in the middle of transitioning out of HIGHVALUE_PROMO_CHOICE.
        trace(L4, _T("False high value promo screen TB trigger, return AnchorState %d"),getAnchorState());

        // Tell Security Manager that CustomMessage is starting because the UnInitialize() base
        // routine will tell Security Manager that CustomMessage is ending.
        TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("CustomMessage") );

        // Return to the anchor state.
        return createAnchorState();
    }
	
	

	//(+) VSTS 131568
	csSetItemAsideText = _T("");
    csSetItemAsideText = CustomMessageObj.csScreenText;
	trace(L6, _T("csSetItemAsideText : %s"), csSetItemAsideText);

    //POS18049 - freeze this state since we need to alert the customer that the EBT FS Total is ZERO
    if( csSetItemAsideText == ps.GetPSText(SSF_TXT_VOIDITEM))
    {
        fStateFreeze = true;// freeze this state
		SMState::m_bCustomVoidItemMessage = true;
        trace(L6, _T("fStateFreeze is true,csSetItemAsideText : %s"), csSetItemAsideText);
    }
	//(-) VSTS 131568

	// +SSCOI-46988
	if (GetHookProperty(_T("PrintGiftReceipt")) != _T("0"))
		m_pAppPSX->SetTransactionVariable(_T("PrintGiftReceipt"), VARIANT_TRUE);
	else
		m_pAppPSX->SetTransactionVariable(_T("PrintGiftReceipt"), VARIANT_FALSE);
	// -SSCOI-46988
	
    //POS36318 - Lock the display so that we could change the logo to mperks logo if its for mperks screen - rrs
    CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMCustomMessage::Initialize"));

	SMStateBase *pRet = SMCustomMessageBase::Initialize();
	//this is added here because this buttons have been turned not visible in SMInprogress added screen 
	if (1 != SMStateBase::m_nTBOutOfSyncError)
	{
		m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_TRUE);
		m_pAppPSX->SetConfigProperty(_T("ButtonHelp"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_TRUE);
	}
    
	//(+) POS260040
	csFoodStampText = _T("");
    csFoodStampText = CustomMessageObj.csScreenText;
    //POS18049 - freeze this state since we need to alert the customer that the EBT FS Total is ZERO
    if( csFoodStampText == ps.GetPSText(MSG_INSUFFICIENTFOODSTAMP) )
    {
        fStateFreeze = true;// freeze this state
        trace(L6, _T("fStateFreeze is true,csFoodStampText : %s"), csFoodStampText);
    }
    // e POS18049
    //(-) POS260040

    //POS36318 - change the logo to mperks logo if its for mperks screen - rrs
    if ((GetHookProperty(_T("__POSState")).Find(_T("DIGITAL_COUPON")) != -1) ||
        ((GetHookProperty(_T("__POSState")).Find(_T("HIGHVALUE_PROMO_CHOICE")) != -1) && (GetHookProperty(_T("HighValuePromoChoice")) == _T("1"))) || // POS39475
        ((GetHookProperty(_T("__POSState")).Find(_T("VENDOR_COUPON_CHOICE")) != -1) && (GetHookProperty(_T("CouponChoice")) == _T("1")))              // POS33020
       )
    {
        m_pAppPSX->SetConfigProperty(_T("SmallmperksLogo"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_TRUE);
        //m_pAppPSX->SetConfigProperty(_T("SmallLogo"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
    }
    else
    {        
        //m_pAppPSX->SetConfigProperty(_T("SmallLogo"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetConfigProperty(_T("SmallmperksLogo"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
    }
    //e POS36318 - Reset the small logo - rrs

    // POS39476/52217 No need to modify the font type - rrs
    // POS33020: Just pass thru this one time to save the font for the typical instructional text.
    /*CString csFontInstructionTextArea(vFontInstructionTextArea.bstrVal);
    if (csFontInstructionTextArea == "")
    {
        PSXRC rc = m_pAppPSX->GetConfigProperty(_T("InstructionTextArea"), ps.GetCurrentContext(), UI::PROPERTYFONT, vFontInstructionTextArea);
        if (PSX_SUCCESS == rc)
        {
            // Save the instructional text font in case it gets changed by a POS33020 Coupon Choice screen.
            csFontInstructionTextArea = vFontInstructionTextArea.bstrVal;
            trace(L6, _T("Font for InstructionTextArea on %s is %s"), ps.GetCurrentContext(), csFontInstructionTextArea);
        }
        else
        {
            // Failed to get instructional text font so default font is AnimationTextFont which is "Arial,24,0,0,0".
            trace(L4, _T("Failed to get font for InstructionTextArea on %s, err=%x"), ps.GetCurrentContext(), rc);
            csFontInstructionTextArea = _T("Arial,24,0,0,0");
            vFontInstructionTextArea = csFontInstructionTextArea;
            trace(L6, _T("Deafault font for InstructionTextArea on %s is %s"), ps.GetCurrentContext(), csFontInstructionTextArea);
        }
    }*/

	//POS311584
	CString csPOSState = GetTBProperty(_T("SavedLastPosState"));
	trace(L6, _T("SMCustomMessage::Initialize():PosState=%s"), csPOSState);


	CString csMsgLevel=  GetHookProperty(_T("OG_MsgLevel_Prop"));
    int iMsgLevel = _ttoi((LPCTSTR)csMsgLevel);


    // POS39475: Start
    if ( (csPOSState.Find(_T("HIGHVALUE_PROMO_CHOICE")) != -1) && (GetHookProperty(_T("HighValuePromoChoice")) == _T("1")) )
    {
        // POS39475: High value mPerks promo details must be displayed so the customer can choose to use it or not.
        CString csPromoDesc = GetHookProperty(_T("tb_char"));  
        CString csPromoExpDate = GetHookProperty(_T("tb_ExpirationDate"));  
        CString csMessage = CustomMessageObj.csScreenText + _T("\n") + csPromoDesc + _T("\n") + csPromoExpDate;
        trace(L6, _T("HighValuePromoChoice: Text:<%s>; Desc:<%s>; Date:<%s>"), CustomMessageObj.csScreenText, csPromoDesc, csPromoExpDate);
        ps.Message12(csMessage, true);

        // Display the Yes and No buttons instead of the other buttons.
        m_pAppPSX->SetConfigProperty (_T("CMButton1Med"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty (_T("CMButton1Lg"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty (_T("CMButton2Lg"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty (_T("CMButton1"), ps.GetCurrentContext(), UI::PROPERTYSTATE, (long)UI::Normal);
        m_pAppPSX->SetConfigProperty (_T("CMButton1"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetConfigProperty (_T("CMButton2"), ps.GetCurrentContext(), UI::PROPERTYSTATE, (long)UI::Normal);
        m_pAppPSX->SetConfigProperty (_T("CMButton2"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_TRUE);
		m_pAppPSX->SetTransactionVariable(_T("HighValuePromoChoice"), VARIANT_TRUE); //SSCOI-45597
		m_pAppPSX->SetConfigProperty(_T("ButtonHelp"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE); //SSCOI-45600

        if (csPromoExpDate == "")
        {
           // This should not happen but just in case the data looks suspicious, hide the buttons
           // so that the customer has to tap the Help button and then the attendant can see in
           // Assist mode what is going on.
           trace(L6, _T("HighValuePromoChoice: Bad data"));
           m_pAppPSX->SetConfigProperty (_T("CMButton1"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
           m_pAppPSX->SetConfigProperty (_T("CMButton2"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		   m_pAppPSX->SetConfigProperty(_T("ButtonHelp"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_TRUE); //SSCOI-45600
        }

        // Center High value mPerks promo details on screen.
        v = _T("1");
        m_pAppPSX->SetConfigProperty(_T("InstructionTextArea"), ps.GetCurrentContext(),UI::PROPERTYTEXTALIGNMENT, v);

        // POS39476/52217 No need to modify the font type - rrs
        // Make sure font for instructional text area is the typical setting.
        //m_pAppPSX->SetConfigProperty(_T("InstructionTextArea"), ps.GetCurrentContext(), UI::PROPERTYFONT, vFontInstructionTextArea);

        // Update RAP window with 'mPerks Promo' state and enable RAP 'Assist Mode' button.
        UpdateStateName(ps.GetPSText(SSF_LTT_HIGHVALUE_PROMO, SCOT_LANGUAGE_PRIMARY));
        ra.RequestEnableAssistMode(true);
    }
    else if ( (csPOSState.Find(_T("VENDOR_COUPON_CHOICE")) != -1) && (GetHookProperty(_T("CouponChoice")) == _T("1")) )
    {
        //POS39476/51660 - Use MeijerDigitalVendorCouponChoice to display the coupon selection - rrs
        ps.SetCMFrame(_T("MeijerDigitalVendorCouponChoice"));

        // POS33020: Coupon choice details must be displayed so the customer can choose between (mPerks/digital) E coupon and vendor (paper) coupon.
        // But first, take care of notifying attendant.

        // We could be here due to mPerks being entered after coupon entries which means that CM 
        // does an internal void on the paper coupon which causes a void scenario at FastLane 
        // turning on the red security light. So, make sure the red light is off since the customer
        // can handle it from here. 
        dm.setTriColorLight(DM_TRICOLORLIGHT_RED, DM_TRICOLORLIGHT_OFF, DM_TRICOLORLIGHT_NORMALREQUEST);

        // Update RAP window with 'Coupon Selection' state and enable RAP 'Assist Mode' button.
        UpdateStateName(ps.GetPSText(SSF_LTT_COUPONCHOICE_SELECTION, SCOT_LANGUAGE_PRIMARY));
        ra.RequestEnableAssistMode(true);

        CString csECouponDescAndCode = GetHookProperty(_T("tb_char"));  
        CString csECouponValueAndExpDate = GetHookProperty(_T("tb_ExpirationDate"));  
        CString csPaperCouponTitle = ps.GetPSText(SSF_TXT_COUPONCHOICE_VENDOR);;  
        CString csPaperCouponDescAndCode = GetHookProperty(_T("item_code"));  
        CString csPaperCouponValueAndExpDate = GetHookProperty(_T("tb_mnemonic"));  

        //POS39476/51660 - split the digital and vendor coupon message - rrs
        CString csMessage1 = CustomMessageObj.csScreenText + _T("\n") + csECouponDescAndCode + _T("\n") + csECouponValueAndExpDate;
        CString csMessage2 = csPaperCouponTitle + _T("\n") + csPaperCouponDescAndCode + _T("\n") + csPaperCouponValueAndExpDate;
        trace(L6, _T("Title LTT: %s"), CustomMessageObj.csLeadThruText);
        trace(L6, _T("ECouponChoice: Text:<%s>; Desc:<%s>; Date:<%s>"), CustomMessageObj.csScreenText, csECouponDescAndCode, csECouponValueAndExpDate);
        trace(L6, _T("PaperCouponChoice: Text:<%s>; Desc:<%s>; Date:<%s>"), csPaperCouponTitle, csPaperCouponDescAndCode, csPaperCouponValueAndExpDate);

        //POS39476/51660 - digital coupon is displayed in the InstructionTextArea, Vendor coupon is displayed in MessageBoxNoEcho - rrs
        ps.SetCMLeadthruText(CustomMessageObj.csLeadThruText);

        ps.Message12(csMessage1, true);
        ps.Message12(csMessage2, false);

        ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);

        // Display proper label on mPerks coupon button.
        CString csECouponButton = ps.GetPSText(SSF_BTT_COUPONCHOICE_MPERKS) + _T("\n") + ps.GetPSText(SSF_BTT_COUPONCHOICE_MPERKS2);  

        //POS87264
        if(ps.RemoteMode() )
        {
            csECouponButton = ps.GetPSText(SSF_BTT_COUPONCHOICE_MPERKS);
        }
        //E POS87264

        v = csECouponButton;
        m_pAppPSX->SetConfigProperty(_T("CMButton1Lg"), ps.GetCurrentContext(), UI::PROPERTYTEXTFORMAT, v);

        // Display proper label on paper coupon button.
        CString csPaperCouponButton = ps.GetPSText(SSF_BTT_COUPONCHOICE_VENDOR) + _T("\n") + ps.GetPSText(SSF_BTT_COUPONCHOICE_VENDOR2);  
        //POS87264
        if(ps.RemoteMode() )
        {
            csPaperCouponButton = ps.GetPSText(SSF_BTT_COUPONCHOICE_VENDOR);
        }
        //E POS87264

        
        v = csPaperCouponButton;       

        m_pAppPSX->SetConfigProperty(_T("CMButton2Lg"), ps.GetCurrentContext(), UI::PROPERTYTEXTFORMAT, v);

        // Display the mPerks and paper buttons instead of the other buttons.
        m_pAppPSX->SetConfigProperty (_T("CMButton1Med"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty (_T("CMButton1"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty (_T("CMButton2"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty (_T("CMButton1Lg"), ps.GetCurrentContext(), UI::PROPERTYSTATE, (long)UI::Normal);
        m_pAppPSX->SetConfigProperty (_T("CMButton1Lg"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetConfigProperty (_T("CMButton2Lg"), ps.GetCurrentContext(), UI::PROPERTYSTATE, (long)UI::Normal);
        m_pAppPSX->SetConfigProperty (_T("CMButton2Lg"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_TRUE);

        if (csECouponDescAndCode == "")
        {
           // This should not happen but just in case the data looks suspicious, hide the buttons
           // so that the customer has to tap the Help button and then the attendant can see in
           // Assist mode what is going on.
           trace(L6, _T("CouponChoice: Bad data"));
           m_pAppPSX->SetConfigProperty (_T("CMButton1Lg"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
           m_pAppPSX->SetConfigProperty (_T("CMButton2Lg"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        }

        // POS39476/51660center-justify coupon choice details on screen.
        /*v = _T("1");
        m_pAppPSX->SetConfigProperty(_T("InstructionTextArea"), ps.GetCurrentContext(),UI::PROPERTYTEXTALIGNMENT, v);

        // Make sure font for instructional text area is smaller than the typical setting
        // due to the increase in information being displayed for coupon choice.
        PSXRC rc = m_pAppPSX->GetConfigFont(_T("FastLaneSMCouponChoiceFont"), v);
        if (PSX_SUCCESS == rc)
        {
            m_pAppPSX->SetConfigProperty(_T("InstructionTextArea"), ps.GetCurrentContext(), UI::PROPERTYFONT, v);
        }
        else
        {
            trace(L4, _T("Failed to get FastLaneSMCouponChoiceFont font, err=%x"), rc);
        }*/

        ps.ShowFrame();
    }
	else if ( (csPOSState.Find(_T("MESSAGE_300")) != -1) && iMsgLevel > 2)
    {
        trace(L6, _T("EBT Card Insufficient Fund."));
        // Display the Yes and No buttons instead of the other buttons.
        m_pAppPSX->SetConfigProperty (_T("CMButton1Med"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty (_T("CMButton1Lg"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty (_T("CMButton2Lg"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty (_T("CMButton1"), ps.GetCurrentContext(), UI::PROPERTYSTATE, (long)UI::Normal);
        m_pAppPSX->SetConfigProperty (_T("CMButton1"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetConfigProperty (_T("CMButton2"), ps.GetCurrentContext(), UI::PROPERTYSTATE, (long)UI::Normal);
        m_pAppPSX->SetConfigProperty (_T("CMButton2"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetTransactionVariable(_T("HighValuePromoChoice"), VARIANT_TRUE); //re-use this to display Yes and No buttons.
	}
    else
    {
        // Normal custom message screen, so only let the middle ok button be visible.
        m_pAppPSX->SetConfigProperty (_T("CMButton1"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty (_T("CMButton2"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty (_T("CMButton1Lg"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty (_T("CMButton2Lg"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty (_T("CMButton1Med"), ps.GetCurrentContext(), UI::PROPERTYSTATE, (long)UI::Normal);
        m_pAppPSX->SetConfigProperty (_T("CMButton1Med"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_TRUE);

        // POS39476/51660 Normal setting is to cemter-justify details on screen.
        v = _T("1");
        m_pAppPSX->SetConfigProperty(_T("InstructionTextArea"), ps.GetCurrentContext(),UI::PROPERTYTEXTALIGNMENT, v);

        // POS39476/52217 No need to modify the font type - rrs
        // Make sure font for instructional text area is the typical setting.
        //m_pAppPSX->SetConfigProperty(_T("InstructionTextArea"), ps.GetCurrentContext(), UI::PROPERTYFONT, vFontInstructionTextArea);
    }
    // POS39475: End


	return pRet;
}

void SMCustomMessage::UnInitialize(void)
{
    SMCustomMessageBase::UnInitialize();
 
    //(+) POS260040
    trace(L6, _T("csFoodStampText : %s"), csFoodStampText);
    //POS18049 - unfreeze this state since we need to alert the customer that the EBT FS Total is ZERO
    if( csFoodStampText == ps.GetPSText(MSG_INSUFFICIENTFOODSTAMP) )
    {
        fStateFreeze = false;// unfreeze this state
        trace(L6, _T("SMCustomMessage::UnInitialize()"));
    }
    // e POS18049
    //(-) POS260040

	//(+) VSTS 131568
	 trace(L6, _T("csSetItemAsideText : %s"), csSetItemAsideText);
	if(csSetItemAsideText == ps.GetPSText(SSF_TXT_VOIDITEM))
    {
        fStateFreeze = false;// freeze this state
        trace(L6, _T("fStateFreeze is false,csSetItemAsideText : %s"), csSetItemAsideText);
    }
    //(-) VSTS 131568

    // POS61973 - rrs - remove the custom button for highvalue from assistmenu
    if(co.IsInAssistMenus())
    {
        m_pAppPSX->SetConfigProperty(_T("CMButton1"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);        
        m_pAppPSX->SetConfigProperty(_T("CMButton2"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);            
    }
    //e POS61973 rrs
    
	CString csPOSState = GetTBProperty(_T("SavedLastPosState"));
    trace(L6, _T("Posstate=<%s>"), csPOSState);


	if(csPOSState.Find(_T("HIGHVALUE_PROMO_CHOICE")) != -1 || csPOSState.Find(_T("MESSAGE_300")) != -1 ) //VSTS 148195
	{

		trace(L6, _T("High Value Promo choice Posstate=<%s>"), csPOSState);
	    m_pAppPSX->SetTransactionVariable(_T("HighValuePromoChoice"), VARIANT_FALSE); //SSCOI-45597
	}
}

bool SMCustomMessage::PSAssistMenuLegalForState()
{
    return SMCustomMessageBase::PSAssistMenuLegalForState();
}

SMStateBase * SMCustomMessage::PSButton1(void)
{
    SMStateBase *retState;
    CString strTenderPropName = _T("TenderType");
    CString strTenderValue = _T("");
    int iTenderSelected;
    bool bWaitForReturnCoupon = false;  // POS33020

   // (+) VSTS 131568
	if(csSetItemAsideText == ps.GetPSText(SSF_TXT_VOIDITEM))
    {
	SMState::m_bCustomVoidItemMessage = false;
	 fStateFreeze = false;// freeze this state
	trace(L6, _T("SMState::m_bCustomVoidItemMessage is false,csSetItemAsideText : %s - %d"), csSetItemAsideText, lUnapprovedEASItems);

		//CRD 185620 if no more unapproved EAS items after voiding, them complete the intervention
		if(!lUnapprovedEASItems && bEASApproveFromVoid)
		{
			bGenericDelayedFinishSent = true;	//CRD 232271
			m_GenericDelayedInterventionStatus = GENERIC_DELAYED_COMPLETE;
			SendTBCommand(TBCMD_EASCOMPLETE, false);
			RETURNSTATE(SmGenericDelayed);
		}
		// (-) VSTS 131568
	}


	//POS311584..
	CString csPOSState = GetTBProperty(_T("SavedLastPosState"));
	// CString csPOSState = GetHookProperty(_T("__POSState"));

	if (csPOSState.Find(_T("MPERKS_RETRY_LIMIT_EXCEEDED")) != -1)
	{
		trace(L6, _T("Press Cancel button at POS to clear the error message."));
		SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F2}")));
	}

    trace(L6, _T("SMCustomMessage::PSButton1():PosState=%s"), csPOSState);


	if (csPOSState.Find(_T("HOMEDLVRY_TRANS_MPERKS_NOT_ALLOWED")) != -1)
	{
		trace(L6, _T("Press OK to operator message for mPerks for home delivery transaction."));
		SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F1}")));
	}

	//+SSCOI-46770
	if (csPOSState.Find(_T("PRICE_REQUIRED_INVALID_AMOUNT")) != -1)
	{
		trace(L6, _T("Press OK button at POS to clear the error message."));
		SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F1}")));
	}
	//-SSCOI-46770

	//POS273548
   if ( (csPOSState.Find(_T("HIGHVALUE_PROMO_CHOICE")) != -1) && (GetHookProperty(_T("HighValuePromoChoice")) == _T("1")) )
   {
      // POS39475: Customer responded Yes to high value mPerks promo choice.

      // Disable buttons after being tapped to prevent multiple key presses; other race conditions.
      m_pAppPSX->SetConfigProperty (_T("CMButton1"), ps.GetCurrentContext(), UI::PROPERTYSTATE, (long)UI::Disabled);
      m_pAppPSX->SetConfigProperty (_T("CMButton2"), ps.GetCurrentContext(), UI::PROPERTYSTATE, (long)UI::Disabled);

	  //additional fix for Bug244609
	  fSAInItemSale = false;

      trace(L6, _T("HighValuePromoChoice: Yes"));

	  //if(bIsInFinalization)// POS312426 //This should be unconditional as the security was disabled upon mPerks activation
	  //{
		//CustomMessageObj.bGoBackScanAndBag = true; //POS311584 - return to scan and bag and let the customer press the F&P button
	  //}
	  if(nPreviousState == BES_BAGANDEAS && !SMState::m_bSecurityDisabledForMPerks)
	  {		  
		  CustomMessageObj.bGoBackScanAndBag = false;
	  }
	  else
	  {
		  CustomMessageObj.bGoBackScanAndBag = true;
	  }

      // Clear high value promo data before sending response to sales app.
      SetHookProperty(_T("HighValuePromoChoice"), _T("0"));
      SetHookProperty(_T("tb_char"), _T(""));
      SetHookProperty(_T("tb_ExpirationDate"), _T(""));

      // Send Yes response back to sales app.
      CString csResponse = GetHookProperty(_T("HighValuePromoResponseYes"));
      SetHookProperty(_T("ResponseCodeToSendToPOS"), csResponse);
      SendTBCommand(TBCMD_SEND_ANYRESPONSE_TO_POS, NULL);

      if (SendTBCommand(TBCMD_GET_CUSTOMER_SELECTED_TENDER, &strTenderValue) > 0)
      {
         // POS39475: The system is currently in a tender payment state. Thus, we can deduce that we
         // just responded to a tender-based high value promo. This means that the sales
         // app is currently at the Payment Menu dynaframe. So, send it the "ELEC. PYMT
         // SELECTED" softkey to start the payment process over again.
         CString csResponse = GetHookProperty(_T("TenderSelectedSoftkey"));
         trace(L6, _T("Selected tender is %s; Send softkey %s to restart payment process"), strTenderValue, csResponse);
         SetHookProperty(_T("ResponseCodeToSendToPOS"), csResponse);
         SendTBCommand(TBCMD_SEND_ANYRESPONSE_TO_POS, NULL);
      }

	  // RFQ 9545
	  if (g_bInRecallMode || bFinishRecall)	//CRD 286248 go back to scan and bag when bFinishRecall is true, means customer hasn't completed the transfer yet
	  {
		  trace(L6, _T("goto scan and bag since we are in recall mode FLMS"));
		  CustomMessageObj.bGoBackScanAndBag = true;
	  }
   }
   else if ( (csPOSState.Find(_T("VENDOR_COUPON_CHOICE")) != -1) && (GetHookProperty(_T("CouponChoice")) == _T("1")) )
   {
      // POS33020: Customer selected E (mPerks/digital) coupon over paper (vendor) coupon.

      // Disable buttons after being tapped to prevent multiple key presses; other race conditions.
      m_pAppPSX->SetConfigProperty (_T("CMButton1Lg"), ps.GetCurrentContext(), UI::PROPERTYSTATE, (long)UI::Disabled);
      m_pAppPSX->SetConfigProperty (_T("CMButton2Lg"), ps.GetCurrentContext(), UI::PROPERTYSTATE, (long)UI::Disabled);

      trace(L6, _T("CouponChoice: mPerks"));

      // Clear coupon choice data before sending response to sales app.
      SetHookProperty(_T("CouponChoice"), _T("0"));
      SetHookProperty(_T("tb_char"), _T(""));
      SetHookProperty(_T("tb_ExpirationDate"), _T(""));
      SetHookProperty(_T("item_code"), _T(""));
      SetHookProperty(_T("tb_mnemonic"), _T(""));

      if (GetHookProperty(_T("tb_Item_rec_flags")) == _T("1"))
      {
         // The tb_Item_rec_flags sales app flag indicates that the paper coupon was processed before mPerks entry,
         // so we are in the middle of a "coupon reversal" at the sales app.
         // However, the paper coupon may not of been inserted in the collection bin because the customer could
         // of done the mPerks entry at the InsertCoupon screen thus bypassing the coupon insertion. That's where
         // the m_bCouponInsertionBypassed fastlane flag is used. It tells us that the paper coupon was or was not
         // bypassed.
         SetHookProperty(_T("tb_Item_rec_flags"), _T("0"));
         if (!m_bCouponInsertionBypassed)
         {
            // The paper coupon has already been deposited in the bin. so setup to make sure the Return Coupon
            // store mode screen is activated based on the sales app sending the Return Coupon operator message.
            trace(L6, _T("E coupon selected but Paper coupon already deposited; Return Coupon screen should activate shortly; lCoupons=%d; m_lInsertedCouponsTotal=%d"), lCoupons, m_lInsertedCouponsTotal);
         }
         else
         {
            // The paper coupon was bypassed but the sales app does not know that. We want to make sure the Return Coupon
            // store mode screen is NOT activated.
            trace(L6, _T("E coupon selected but Paper coupon bypassed; Return Coupon screen should NOT activate; lCoupons=%d; m_lInsertedCouponsTotal=%d"), lCoupons, m_lInsertedCouponsTotal);
         }

         // Let's make sure to wait for Return Coupon operator message from sales app.
         bWaitForReturnCoupon = true;
      }
      else
      {
         // The E coupon got selected over the paper coupon. Ensure that the Insert Coupon screen does not become 
         // active when leaving this Custom Message screen.
         trace(L6, _T("E coupon selected; Insert Coupon screen should NOT activate; lCoupons=%d; m_lInsertedCouponsTotal=%d"), lCoupons, m_lInsertedCouponsTotal);
         m_bCouponInsertionBypassed = false;

         // POS50227: Tell Security Manager that paper coupon won't be used so that next item sold will not be
         // out-of-sync with the Security Manager.
         SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
      }

      // Send mPerks coupon response back to sales app.
      CString csResponse = GetHookProperty(_T("CouponChoiceResponseMperks"));
      SetHookProperty(_T("ResponseCodeToSendToPOS"), csResponse);
      SendTBCommand(TBCMD_SEND_ANYRESPONSE_TO_POS, NULL);

      if (SendTBCommand(TBCMD_GET_CUSTOMER_SELECTED_TENDER, &strTenderValue) > 0)
      {
         // POS33020: The system is currently in a tender payment state. Thus, we can deduce that we
         // just responded to a coupon collision during tendering. This means that the sales
         // app is currently at the Payment Menu dynaframe. So, send it the "ELEC. PYMT
         // SELECTED" softkey to start the payment process over again.
         CString csResponse = GetHookProperty(_T("TenderSelectedSoftkey"));
         trace(L6, _T("Selected tender is %s; Send softkey %s to restart payment process"), strTenderValue, csResponse);
         SetHookProperty(_T("ResponseCodeToSendToPOS"), csResponse);
         SendTBCommand(TBCMD_SEND_ANYRESPONSE_TO_POS, NULL);
      }

      // Make sure Custom Message screen goes away and returns to the previous state.
      retState = SMCustomMessageBase::PSButton1();

      if (bWaitForReturnCoupon)
      {
         delete getRealStatePointer( retState); 
         RETURNSTATE(InProgress);
      }

      return retState;
   }
   else if ( (csPOSState.Find(_T("EMV_PLEASE_REMOVE_CARD")) != -1) )	//POS273548
   {
	   // Send mPerks coupon response back to sales app.
	   CString csResponse = _T("{F1}");
	   SetHookProperty(_T("ResponseCodeToSendToPOS"), csResponse);
	   SendTBCommand(TBCMD_SEND_ANYRESPONSE_TO_POS, NULL);	   
	   	   
	   // Make sure Custom Message screen goes away and returns to the previous state.
	   //retState = SMCustomMessageBase::PSButton1(); 	   
	   CustomMessageObj.Reset();
	   return STATE_NULL;

		//return retState;
   }
   else if ( (csPOSState.Find(_T("MESSAGE_300")) != -1) )
   {
	   CString csMsgLevel=  GetHookProperty(_T("OG_MsgLevel_Prop"));
       int iMsgLevel = _ttoi((LPCTSTR)csMsgLevel);

	   if(iMsgLevel == 2)
		   SetHookProperty(_T("ResponseCodeToSendToPOS"), _T("{F2}"));
       else
           SetHookProperty(_T("ResponseCodeToSendToPOS"), _T("{F1}"));

       SendTBCommand(TBCMD_SEND_ANYRESPONSE_TO_POS, NULL);
   }
   else
   {
      iTenderSelected = SendTBCommand(TBCMD_GET_CUSTOMER_SELECTED_TENDER, &strTenderValue);

      //POS35460 - don't check the lFoodStampsDue instead check the PropTenderAmountDue as the lFoodStampsDue would still have the original FS total even after the FS partial tender -  rrs
      CString csTenderAmount = GetHookProperty(_T("PropTenderAmountDue"));    

      //POS62550 - for negative balance - also perform the cancel tender - rrs
      long lTenderAmount = _ttol(csTenderAmount);

      //POS69170 - check to make sure the tender descriptor doesn't have GIFT text on it
      if(
          ( strTenderValue.Find(_T("FOOD"))!= -1 && lTenderAmount <= 0 && strTenderValue.Find(_T("GIFT")) == -1) ||   //POS62550
          ( strTenderValue.Find(_T("MEIJER CREDIT")) != -1 )
        )
      {
         strTenderValue = _T("");
         SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strTenderPropName, &strTenderValue);
         SendTBCommand(TBCMD_SET_TB_STATE,_T("CANCEL_TENDER"));

        // POS18049 - unfreeze the state
        fStateFreeze = false; // unfreeze this state
        // e POS18049
      }

      // CString csPOSString = _T("{F1}");
      // Send Data only if its set in app xml
      SendTBCommand(TBCMD_SEND_ANYRESPONSE_TO_POS, NULL);
   }
	
    retState = SMCustomMessageBase::PSButton1();  // OK Button

    //POS53451 - go back to InProgress if it's in the middle of resuming trx
    CString strOptionName = _T("IsResumeTrxInProgress");
	CString strIsResume = _T("0");
	SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strOptionName, &strIsResume);


   //POS11024:Bag Alone items should not go to scan and bag
   //CRD 261285
   //CRD 286248 go back to scan and bag when bFinishRecall is true, means customer hasn't completed the transfer yet
	if ( !bFinishRecall && !g_bInRecallMode && (fSAInItemSale || GetHookProperty(_T("__POSState")) == _T("BAG_ITEM_ALONE_28") || strIsResume == _T("1")))
	{
		delete getRealStatePointer( retState); 
		RETURNSTATE(InProgress);
	}
    else
	{
		return retState;
	}
}

// POS39475: Start
//////////////////////////////////////
SMStateBase *SMCustomMessage::PSButton2(void) // No Button (POS39475)
{
   SMStateBase *retState;
   CString strTenderValue = _T("");


   CString csPosState = GetTBProperty(_T("SavedLastPosState"));
   trace(L6, _T("SMCustomMessage::PSButton2():PosState=%s"), csPosState);

   if ( (csPosState.Find(_T("HIGHVALUE_PROMO_CHOICE")) != -1) && (GetHookProperty(_T("HighValuePromoChoice")) == _T("1")) )
   {
      // POS39475: Customer responded No to high value mPerks promo choice.

      // Disable buttons after being tapped to prevent multiple key presses; other race conditions.
      m_pAppPSX->SetConfigProperty (_T("CMButton1"), ps.GetCurrentContext(), UI::PROPERTYSTATE, (long)UI::Disabled);
      m_pAppPSX->SetConfigProperty (_T("CMButton2"), ps.GetCurrentContext(), UI::PROPERTYSTATE, (long)UI::Disabled);
      
	  //POS297659 related to POS244609
	  fSAInItemSale = false;
	  if(nPreviousState == BES_BAGANDEAS && !SMState::m_bSecurityDisabledForMPerks)
	  {		  
		  CustomMessageObj.bGoBackScanAndBag = false;
	  }
	  else
	  {
		  CustomMessageObj.bGoBackScanAndBag = true;
	  }
      trace(L6, _T("HighValuePromoChoice: No"));

      // Clear high value promo data before sending response to sales app.
      SetHookProperty(_T("HighValuePromoChoice"), _T("0"));
      SetHookProperty(_T("tb_char"), _T(""));
      SetHookProperty(_T("tb_ExpirationDate"), _T(""));

      // Send No response back to sales app.
      CString csResponse = GetHookProperty(_T("HighValuePromoResponseNo"));
      SetHookProperty(_T("ResponseCodeToSendToPOS"), csResponse);
      SendTBCommand(TBCMD_SEND_ANYRESPONSE_TO_POS, NULL);

      if (SendTBCommand(TBCMD_GET_CUSTOMER_SELECTED_TENDER, &strTenderValue) > 0)
      {
         // POS39475: The system is currently in a tender payment state. Thus, we can deduce that we
         // just responded to a tender-based high value promo. This means that the sales
         // app is currently at the Payment Menu dynaframe. So, send it the "ELEC. PYMT
         // SELECTED" softkey to start the payment process over again.
         CString csResponse = GetHookProperty(_T("TenderSelectedSoftkey"));
         trace(L6, _T("Selected tender is %s; Send softkey %s to restart payment process"), strTenderValue, csResponse);
         SetHookProperty(_T("ResponseCodeToSendToPOS"), csResponse);
         SendTBCommand(TBCMD_SEND_ANYRESPONSE_TO_POS, NULL);
      }

      //POS53451 - go back to InProgress if it's in the middle of resuming trx
      CString strOptionName = _T("IsResumeTrxInProgress");
	  CString strIsResume = _T("0");
	  SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strOptionName, &strIsResume);

      trace(L6, _T("NO Button is pressed in High Value prompt, ResumeTrxInProgress(%s)"), strIsResume);
	  //CRD 261285
	  //CRD 286248 do not go to InProgress state when bFinishRecall is also true, means customer hasn't completed the transfer yet
      if ( !g_bInRecallMode &&  !bFinishRecall && strIsResume == _T("1") )
      {
          retState = SMCustomMessageBase::PSButton1();
		  delete getRealStatePointer( retState); 
          RETURNSTATE(InProgress);
	  }

	  // RFQ 9545
	  //CRD 286248 go back to scan and bag when bFinishRecall is true, means customer hasn't completed the transfer yet
	  if (g_bInRecallMode || bFinishRecall)
	  {

		trace(L6, _T("goto scan and bag since we are in recall mode FLMS"));
		CustomMessageObj.bGoBackScanAndBag = true;
	  }

   }
   else if ( (GetHookProperty(_T("__POSState")).Find(_T("VENDOR_COUPON_CHOICE")) != -1) && (GetHookProperty(_T("CouponChoice")) == _T("1")) )
   {
      // POS33020: Customer selected paper (vendor) coupon over E (mPerks/digital) coupon.

      // Disable buttons after being tapped to prevent multiple key presses; other race conditions.
      m_pAppPSX->SetConfigProperty (_T("CMButton2Lg"), ps.GetCurrentContext(), UI::PROPERTYSTATE, (long)UI::Disabled);
      m_pAppPSX->SetConfigProperty (_T("CMButton1Lg"), ps.GetCurrentContext(), UI::PROPERTYSTATE, (long)UI::Disabled);

      trace(L6, _T("CouponChoice: Paper"));

      // Clear coupon choice data before sending response to sales app.
      SetHookProperty(_T("CouponChoice"), _T("0"));
      SetHookProperty(_T("tb_char"), _T(""));
      SetHookProperty(_T("tb_ExpirationDate"), _T(""));
      SetHookProperty(_T("item_code"), _T(""));
      SetHookProperty(_T("tb_mnemonic"), _T(""));

      if (GetHookProperty(_T("tb_Item_rec_flags")) == _T("1"))
      {
         // The tb_Item_rec_flags sales app flag indicates that the paper coupon was processed before mPerks entry.
         // However, the paper coupon may not of been inserted in the collection bin because the customer could
         // of done the mPerks entry at the InsertCoupon screen thus bypassing the coupon insertion. That's where
         // the m_bCouponInsertionBypassed fastlane flag is used. It tells us that the paper coupon was or was not
         // bypassed.
         SetHookProperty(_T("tb_Item_rec_flags"), _T("0"));
         if (!m_bCouponInsertionBypassed)
         {
            // The paper coupon has already been deposited in the bin so setup to make sure Insert Coupon processing
            // does not occur later after the sales app processes the paper coupon.
            m_bCouponInsertionBypassed = true;
            m_lInsertedCouponsTotal = lCoupons + 10;
            trace(L6, _T("Paper coupon selected & already deposited; Insert Coupon screen should NOT activate after coupon processed; lCoupons=%d; m_lInsertedCouponsTotal=%d"), lCoupons, m_lInsertedCouponsTotal);
         }
         else
         {
            trace(L6, _T("Paper coupon selected but bypassed; Insert Coupon screen should activate after coupon processed; lCoupons=%d; m_lInsertedCouponsTotal=%d"), lCoupons, m_lInsertedCouponsTotal);
         }
      }

      // Send mPerks coupon response back to sales app.
      CString csResponse = GetHookProperty(_T("CouponChoiceResponseVendor"));
      SetHookProperty(_T("ResponseCodeToSendToPOS"), csResponse);
      SendTBCommand(TBCMD_SEND_ANYRESPONSE_TO_POS, NULL);

      if (SendTBCommand(TBCMD_GET_CUSTOMER_SELECTED_TENDER, &strTenderValue) > 0)
      {
         // POS33020: The system is currently in a tender payment state. Thus, we can deduce that we
         // just responded to a coupon collision during tendering. This means that the sales
         // app is currently at the Payment Menu dynaframe. So, send it the "ELEC. PYMT
         // SELECTED" softkey to start the payment process over again.
         CString csResponse = GetHookProperty(_T("TenderSelectedSoftkey"));
         trace(L6, _T("Selected tender is %s; Send softkey %s to restart payment process"), strTenderValue, csResponse);
         SetHookProperty(_T("ResponseCodeToSendToPOS"), csResponse);
         SendTBCommand(TBCMD_SEND_ANYRESPONSE_TO_POS, NULL);
      }

      // Make sure Custom Message screen goes away and wait for paper coupon to be processed.
      retState = SMCustomMessageBase::PSButton1();
		delete getRealStatePointer( retState); 
		CREATE_AND_DISPENSE(InsertCoupon)(getAnchorState(), BES_SCANANDBAG);
   }
   else if ( csPosState.Find(_T("MESSAGE_300")) != -1 )
   {
      SetHookProperty(_T("ResponseCodeToSendToPOS"), _T("{F2}"));
      SendTBCommand(TBCMD_SEND_ANYRESPONSE_TO_POS, NULL);
   }
   // The base has no PSButton2() function, so call PSButton1() to clean up before exiting.
   retState = SMCustomMessageBase::PSButton1();

   return retState;
}
// POS39475: End

SMStateBase * SMCustomMessage::PSEnterKey(void)
{
    return SMCustomMessageBase::PSEnterKey();
}

// POS39475: Start
SMStateBase  *SMCustomMessage::TimedOut(void)
{
	//POS311584
	CString csPOSState = GetTBProperty(_T("SavedLastPosState"));
	trace(L6, _T("SMCustomMessage::TimedOut():PosState=%s"), csPOSState);

   if ( ((csPOSState.Find(_T("HIGHVALUE_PROMO_CHOICE")) != -1) && (GetHookProperty(_T("HighValuePromoChoice")) == _T("1"))) || // POS39475
		 (csPOSState.Find(_T("DIGITAL_COUPON_NOT_A_MEMBER")) != -1) || 
		 (csPOSState.Find(_T("DIGITAL_COUPON_LOOKUP_FAILED")) != -1) ||
		 (csPOSState.Find(_T("MESSAGE_300")) != -1) ||
        ((csPOSState.Find(_T("VENDOR_COUPON_CHOICE")) != -1) && (GetHookProperty(_T("CouponChoice")) == _T("1"))) ||              // POS33020
		 (csPOSState.Find(_T("PRICE_REQUIRED_INVALID_AMOUNT")) != -1) ||  // POS334365 
		(csPOSState.Find(_T("BAG_ITEM_ALONE_28")) != -1)	//CRD 235549
	  )
	{
		// A high value mPerks promo choice (POS39475) or a coupon choice (POS33020) is pending
		// and the sales app is waiting synchronously for a response. So, let's not give the
		// customer the option of canceling the transaction at the 'Continue?' screen without
		// first responding to this choice. (Note: The Help button can be used if they have a question.)
		return STATE_NULL;
	}
	
	return SMCustomMessageBase::TimedOut();
}
// POS39475: End
