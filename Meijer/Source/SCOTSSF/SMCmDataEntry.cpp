//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCmDataEntry.CPP
//
// TITLE: Class implementation for customer mode Data Entry state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMCmDataEntry.h"	    // MGV this state

#include "SMFinish.h"
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("CmDataEntry")

#define PRICE_INQUIRY_TIMEOUT 60

IMPLEMENT_DYNCREATE(SMCmDataEntry, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

#ifdef _TRADESHOWDEMO
#include "..\TradeShowDemo\SMCmDataEntryTSDemo.cpp"
#endif

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMCmDataEntry::SMCmDataEntry()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMCmDataEntry::Deliver(LPCTSTR pPtr)
{
  SMCmDataEntryBase::Deliver(pPtr);

  return this;
}

bool SMCmDataEntry::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMCmDataEntryBase::DeliverDescriptorList(dlParameterList);
}

#ifndef _TRADESHOWDEMO
SMStateBase * SMCmDataEntry::Initialize(void)
{
	m_pAppPSX->SetTransactionVariable( _T("NextGenData"), COleVariant(_T("EnterID")) );//SSCOI-45123
    CString csProperty = _T("IsReSwipeEBTCard");
	CString csValue;
    
    //SOTF 7042 
    CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMCmDataEntry::Initialize"));
    //sotf

    SMStateBase *pRetState = SMCmDataEntryBase::Initialize();

	//Start CRD 418158
	CString csSectionName = sm_pDataNeededEntry.GetSectionName();
	if(csSectionName == _T("CouponCustomerOverride"))
	{
		CString csSummary = sm_pDataNeededEntry.GetSummaryInstruction();
		CString csMsg;
		int nLen = csSummary.GetLength();
		csSummary = csSummary.Mid(1, nLen-2);		//remove $ character
		
		csMsg = ps.GetPSText(csSummary, SCOT_LANGUAGE_PRIMARY);
		// if message is not defined in SSCO Strings, show Operator Guidance message from POS
		if(csMsg == _T(""))
		{
			CString csOperatorGuidance = GetHookProperty(_T("OG_Guidance_Prop"));
			csMsg = csOperatorGuidance;
		}
		else
		{
			//if message has a character format specifier
			//need to get value from POS and include it in SSCO message
			//if(csMsg.Find(_T("%")) != -1)
			//{
				//csMsg.Format(msg, _T("sample"));
			//}
		}
		
		ps.Message12(csMsg, true);
		ps.ShowFrame(false);
	}
	//End CRD 418158

	if(getStateName(pRetState).Compare(_T("SMBagAndEAS")) == 0)
	{
		//resume the security
		trace(L7,_T("CMDataEntry, Setting hold weight=false"));
		CKeyValueParmList parmList;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_RESUME);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		BSTR bstrResult;
		bstrResult = NULL;
		SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
		SysFreeString(bstrResult);
	}

	SendTBCommand(TBCMD_GET_TBPROPERTY, &csProperty, &csValue);
	if (csValue == _T("1"))
	{
		ps.Button(_T("ButtonGoBack"),BTT_CANCELPAY, true);
	}
	
	csProperty = _T("IsPriceInquiryState");
    SendTBCommand(TBCMD_GET_TBPROPERTY, &csProperty, &csValue);
	if(csValue == _T("1"))
	{
       nTimeOut = PRICE_INQUIRY_TIMEOUT;
	   
	   CString csSummaryText = sm_pDataNeededEntry.GetSummaryInstruction();
       if(csSummaryText.Find(_T("Unknown")) != -1)
	   {
           ps.Message12(_T(""), false); 
	   }
	   else 
       {
           //POS39476/52222 - Display the item information if it's available
           if( csSummaryText == _T("") )
               ps.Message12(LTT_SMMEDITADD_INSTR, true);  
       }

       //POS73196/71821 - turn on the yellow light - rrs
       dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                                        DM_TRICOLORLIGHT_ON,
                                        DM_TRICOLORLIGHT_NORMALREQUEST);
       //e POS73196/71821 - turn on the yellow light - rrs
	}

    csProperty = _T("IsPriceInquiry");
	SendTBCommand(TBCMD_GET_TBPROPERTY, &csProperty, &csValue);
	if (csValue == _T("1"))
	{
		csValue = _T("0");
        SendTBCommand(TBCMD_SET_TBPROPERTY, &csProperty, &csValue);

		csProperty = _T("PriceInq_Price");
		SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csProperty, &csValue);
        CString csPrice = csValue;
        csPrice.Remove(_T('.'));
        DMSayAmount(_ttol((LPCTSTR) csPrice));
	}

    //SOTF 7042
    CString csContextToDisplay = sm_pDataNeededEntry.GetContextToDisplay();

	if ((m_bShopAndDropEnabled) && (csContextToDisplay == _T("mPerksPINEntry")))
	{
		m_bSNDAllowItemMenu = true;		// CRD 125377
		trace(L6, _T("m_bSNDAllowItemMenu=%d"), m_bSNDAllowItemMenu);
	}
	//  end CRD 125377
    if(csContextToDisplay == _T("CmDataEntry5Vertical"))
    {
        CString csButtonControlName;
        int nButtons=0;
        PSXRC rc;
        COleVariant vFont;
        CString csButtonFont = sm_pDataNeededEntry.GetButtonFont();

        if(!csButtonFont.IsEmpty())
        {
            rc = m_pAppPSX->GetConfigFont(csButtonFont, vFont);
	        if(PSX_SUCCESS != rc)
		    {	// failure so use original font for CMButtonXMidiList
			    trace(L4, _T("ButtonFont %s not found"),csButtonFont);
		        vFont = defaultFont;
		    }
	    }
	    else
	    {	// setup default font if buttonfont parameter not used
		    vFont = defaultFont;
	    }

        ps.Message12(sm_pDataNeededEntry.GetSummaryInstruction(), true);

       //SOTF 7789
       //clear all the buttons
       for (int x=1; x<6; x++)
        {
            CString csTemp;
            csTemp.Format (_T("%s%d%s"), _T("CMButton"), x , _T("MidiList") );
            ps.Button(csTemp, _T(" "), false);

        } 
       //sotf

        for (int i=1; i<6; i++)
        {
            csButtonControlName = EMPTY_STRING;
	        if(co.fOperationsCenterCustomerDataNeededButtons)
		    {	// only interesed in centering buttons - not maintaining specific button positions
                csButtonControlName.Format (_T("%s%d%s"), _T("CMButton"), (nButtons+1) , _T("MidiList") );
                if(sm_pDataNeededEntry.GetButtonText(i) !=_T(""))
			    {
                    ps.Button(csButtonControlName, sm_pDataNeededEntry.GetButtonText(i), true);
		            nButtons++;
                    rc = m_pAppPSX->SetConfigProperty(csButtonControlName, csCurContext, UI::PROPERTYFONT, vFont);
	                if(PSX_SUCCESS != rc)
                        trace(L4, _T("Failure Setting Button Font for %s, err=%x"),csCurContext,rc);
			    }
            }else
		    {
                csButtonControlName.Format (_T("%s%d%s"), _T("CMButton"), (i) , _T("MidiList") );
                if(sm_pDataNeededEntry.GetButtonText(i) !=_T(""))
			    {	
                    //POS84689 - display the button if does have text
                    m_pAppPSX->SetConfigProperty (csButtonControlName, ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_TRUE);

                    // using button parameter setting to determine position - no centering
                    ps.Button(csButtonControlName, sm_pDataNeededEntry.GetButtonText(i), true);
                    rc = m_pAppPSX->SetConfigProperty(csButtonControlName, csCurContext, UI::PROPERTYFONT, vFont);
	                if(PSX_SUCCESS != rc)
                        trace(L4, _T("Failure Setting Button Font for %s, err=%x"),csCurContext,rc);
			    }
                else
                {
                    //POS84689 - don't display the button if doesn't have text
                    m_pAppPSX->SetConfigProperty (csButtonControlName, ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);

                    //ps.Button(csButtonControlName, sm_pDataNeededEntry.GetButtonText(i), false);
                }
		    }
        }
    }
    //sotf
    // POS14806 - rrs
    // if the context is empty and no button for the customer to press and no viewtype and no inputmask then
    // its only a processing screen for the customer a, so the g_bTBNeedMoreData would need to set to false
    if( sm_pDataNeededEntry.GetViewType().GetLength() == 0 && sm_pDataNeededEntry.GetButtonCount() == 0 
        && sm_pDataNeededEntry.GetContextToDisplay().GetLength() == 0 && sm_pDataNeededEntry.GetInputMask().GetLength() == 0)
    {
        trace(L4, _T("CMDataEntry with empty context and no buttons, so reset the g_bTBNeedMoreData flag as no action is required"));
        g_bTBNeedMoreData = false;  // TAR 238061
        // e POS14806
    }
    // POS 15434 - rrs
    // reset the g_bTBNeedMoreData if it display the DigitalCouponPINEntry section
    else if( sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && sm_pDataNeededEntry.GetSectionName() == _T("DigitalCouponPINEntry") )
    {
        trace(L4, _T("CMDataEntry with DigitalCouponPINEntry context and no buttons, so reset the g_bTBNeedMoreData flag as no action is required"));
        g_bTBNeedMoreData = false;  
    }
    // e POS 15434 - rrs

// (+) POS154789
    else if( sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && sm_pDataNeededEntry.GetSectionName() == _T("WICConfirmation") )
    {
        trace(L4, _T("CMDataEntry with WICConfirmation context and no buttons, so reset the g_bTBNeedMoreData flag as no action is required"));
        g_bTBNeedMoreData = false;  
    }
// (-) POS154789
	else if ( sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && sm_pDataNeededEntry.GetSectionName() == _T("MPerksPrompt") && nTenderType == TB_TENDER_CASH && nDMCashInserted == 0)
	{
		//POS273636 - if we are here in the middle of cash tender, restore the nDMCashInserted value as it has been reset by TBEnterTender  - rs
		trace(L4, _T("CMDataEntry with mPerks Apply in the middle of cash tender, cash Tendered(%d)"), nDMCashTendered);
		nDMCashInserted = nDMCashTendered;
	}
	// +POS311306 / SSCOI-47878 / SSCOI-47988
	else if( sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && sm_pDataNeededEntry.GetSectionName() == _T("GiftPhonePrice") )
	{
		m_pAppPSX->SetTransactionVariable( _T("GiftPhonePricePrompt"), VARIANT_TRUE );
	}
	// -POS311306 / SSCOI-47878 / SSCOI-47988
	//Start CRD 335964 if coupon with 992 family code is scanned, reset fSAInItemSale the coupon is always blocked by the POS
	else if( sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && sm_pDataNeededEntry.GetSectionName() == _T("992FamilyCoupon"))
	{
		fSAInItemSale = false;
	}
	//End CRD 335964
	
	trace(L4, _T("section name(%s), nTenderType(%d), nDMCashInserted(%d)"), sm_pDataNeededEntry.GetSectionName(), nTenderType, nDMCashInserted);

	// start RFQ 9545
	if ((csCurContext == _T("mPerksEntry")) && (g_bInRecallMode))
	{
		// Start  Bug 106589
		csCurContext = _T("Processing");
        ps.SetCMFrame(csCurContext);

        ps.SetCMLeadthruText(TTL_LOADING_TRANSACTION);
        ps.Message12(MSG_LOADING_TRANSACTION, true);
        ps.ShowCMTBText(csTBMessage2Scot);  
        ps.CMButton(_T("ButtonHelp"),BTT_HELP,false);
        UpdateStateName(ps.GetPSText(MSG_LOADING_TRANSACTION, SCOT_LANGUAGE_PRIMARY));
		
		// start RFQ 9545 make sure amount display is disabled
		m_pAppPSX->SetConfigProperty(_T("Amount1Label"), _T("Processing"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		m_pAppPSX->SetConfigProperty(_T("Amount2Label"), _T("Processing"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		m_pAppPSX->SetConfigProperty(_T("Amount1"), _T("Processing"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		m_pAppPSX->SetConfigProperty(_T("Amount2"), _T("Processing"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		// end RFQ 9545
		
		trace(L6, _T("Showframe different frame since we are in recall mode FLMS CurrentContext %s was mPerksEntry"), csCurContext );
        ps.ShowFrame(false);
		// End  Bug 106589

	}
	else
	{
		ps.ShowFrame(false);        
	}
	// end RFQ 9545

  
    //+SSCOI-48082
	int g_nChipCardTimer;
	g_nChipCardTimer = cr.GetIntOption(_T("Timeout"),_T("ChipCardDetectedTimer"),0,_T("Scotopts"));
	if (csContextToDisplay == _T("ChipCardEntry"))
	{
		nTimeOut=g_nChipCardTimer;
		trace(L4, _T("Chip Card Screen Timer is set to g_nChipCardTimer(%d), nTimeOut(%d)"),g_nChipCardTimer,nTimeOut);
		m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);

	}
	//-SSCOI-48082


	return  pRetState;
}

void SMCmDataEntry::UnInitialize(void)
{
    CString csProperty = _T("IsReSwipeEBTCard");
	CString csValue = _T("0");
	SendTBCommand(TBCMD_SET_TBPROPERTY, &csProperty, &csValue);
	m_pAppPSX->SetTransactionVariable( _T("GiftPhonePricePrompt"), VARIANT_TRUE );// POS311306 / SSCOI-47878 / SSCOI-47988
}
#endif //_TRADESHOWDEMO

bool SMCmDataEntry::PSAssistMenuLegalForState()
{
    return SMCmDataEntryBase::PSAssistMenuLegalForState();
}

#ifndef _TRADESHOWDEMO
//SOTF 7789
SMStateBase * SMCmDataEntry::PSButton1(void)
{
	//+SSCOI-55023
	CString csPOSState = GetTBProperty(_T("SavedLastPosState"));
	if((g_bInRecallMode) &&
		(csPOSState.Find(_T("CM_PROMO_LINKEDITEM")) != -1) &&
		bIsItemizationAfterSkipBaggingSent)
	{
		bIsItemizationAfterSkipBaggingSent = false;
	}
	//-SSCOI-55023
    CString csContextToDisplay = sm_pDataNeededEntry.GetContextToDisplay();
	trace(L6, _T("m_bShopAndDropEnabled = %d csContextToDisplay =%s"), m_bShopAndDropEnabled, csContextToDisplay);
	if ((m_bShopAndDropEnabled) && (csContextToDisplay == _T("mPerksEntry")))
	{
		SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F2}")));    
		Sleep(100);
		
		m_bSNDmPerkEntered = true;	// CRD 125377
		m_bSNDmPerksCancel = true;	// CRD 125377
		RETURNSTATE(Finish)
	} 
    if(csContextToDisplay == _T("CmDataEntry5Vertical"))
    {
        m_sButton = sm_pDataNeededEntry.GetButtonCommand(1);
		//USSF START
		USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
		//return HandleDataNeeded();
		//USSF END
    }else
    {
        return SMCmDataEntryBase::PSButton1();
    }
}
#endif //_TRADESHOWDEMO

SMStateBase * SMCmDataEntry::PSButton2(void)
{
    CString csContextToDisplay = sm_pDataNeededEntry.GetContextToDisplay();
    if(csContextToDisplay == _T("CmDataEntry5Vertical"))
    {
        m_sButton = sm_pDataNeededEntry.GetButtonCommand(2);
		//USSF START
		USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
		//return HandleDataNeeded();
		//USSF END
    }else
    {
        return SMCmDataEntryBase::PSButton2();
    }
}

SMStateBase * SMCmDataEntry::PSButton3(void)
{
    CString csContextToDisplay = sm_pDataNeededEntry.GetContextToDisplay();
    if(csContextToDisplay == _T("CmDataEntry5Vertical"))
    {
        m_sButton = sm_pDataNeededEntry.GetButtonCommand(3);
		//USSF START
		USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
		//return HandleDataNeeded();
		//USSF END
    }else
    {
        return SMCmDataEntryBase::PSButton3();
    }
}

SMStateBase * SMCmDataEntry::PSButton4(void)
{
    CString csContextToDisplay = sm_pDataNeededEntry.GetContextToDisplay();
    if(csContextToDisplay == _T("CmDataEntry5Vertical"))
    {
		
		m_sButton = sm_pDataNeededEntry.GetButtonCommand(4);
		//USSF START
		USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
		//return HandleDataNeeded();
		//USSF END
    }else
    {
        return SMCmDataEntryBase::PSButton4();
    }
}

SMStateBase * SMCmDataEntry::PSButton5(void)
{
    CString csContextToDisplay = sm_pDataNeededEntry.GetContextToDisplay();
    if(csContextToDisplay == _T("CmDataEntry5Vertical"))
    {
		m_sButton = sm_pDataNeededEntry.GetButtonCommand(5);
		//USSF START
		USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
		//return HandleDataNeeded();
		//USSF END
    }else
    {
        return SMCmDataEntryBase::PSButton5();
    }
}
//sotf

SMStateBase * SMCmDataEntry::PSButton6(void)
{
    return SMCmDataEntryBase::PSButton6();
}

SMStateBase * SMCmDataEntry::PSButton7(void)
{
    return SMCmDataEntryBase::PSButton7();
}

SMStateBase * SMCmDataEntry::PSNumericKey(const long lKey)
{
    return SMCmDataEntryBase::PSNumericKey(lKey);
}

SMStateBase * SMCmDataEntry::PSEnterKey(void)
{
    return SMCmDataEntryBase::PSEnterKey();
}

SMStateBase * SMCmDataEntry::PSDecimalKey(const long ch)
{
    return SMCmDataEntryBase::PSDecimalKey(ch);
}

SMStateBase * SMCmDataEntry::PSOtherSpecialKeys(const long ch)
{
    return SMCmDataEntryBase::PSOtherSpecialKeys(ch);
}

SMStateBase * SMCmDataEntry::PSBackSpaceKey(void)
{
    return SMCmDataEntryBase::PSBackSpaceKey();
}

SMStateBase * SMCmDataEntry::PSShiftKey(void)
{
    return SMCmDataEntryBase::PSShiftKey();
}

SMStateBase * SMCmDataEntry::PSAltGrKey(void)
{
    return SMCmDataEntryBase::PSAltGrKey();
}

SMStateBase * SMCmDataEntry::PSHotKey(CString & csHotKey) 
{
    return SMCmDataEntryBase::PSHotKey(csHotKey) ;
}

SMStateBase * SMCmDataEntry::PSCharKey(TCHAR c)
{
    return SMCmDataEntryBase::PSCharKey(c);
}

SMStateBase * SMCmDataEntry::PSAlphaKey(const long lKey)
{
    return SMCmDataEntryBase::PSAlphaKey(lKey);
}

SMStateBase * SMCmDataEntry::PSAlphaNumericKey(const long lKey)
{
    return SMCmDataEntryBase::PSAlphaNumericKey(lKey);
}

#ifndef _TRADESHOWDEMO
SMStateBase * SMCmDataEntry::PSButtonGoBack(void)
{
    CString csProperty = _T("IsReSwipeEBTCard");
	CString csValue = _T("0");
    //+SSCOI-48082
	CString csContextToDisplay = sm_pDataNeededEntry.GetContextToDisplay();
	if (csContextToDisplay == _T("ChipCardEntry"))
	{
		RETURNSTATE(Finish)
	} 
	//-SSCOI-48082
	trace(L6, _T("m_bShopAndDropEnabled = %d csContextToDisplay =%s"), m_bShopAndDropEnabled, csContextToDisplay);
	if ((m_bShopAndDropEnabled) && (csContextToDisplay == _T("mPerksEntry")))
	{
		SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F8}")));    
		Sleep(100);
		
		m_bSNDmPerkEntered = true;	// CRD 125377
		m_bSNDmPerksCancel = true;	// CRD 125377
		RETURNSTATE(Finish)
	} 
	SendTBCommand(TBCMD_GET_TBPROPERTY, &csProperty, &csValue);
	if (csValue == _T("1"))
	{
		SendTBCommand(TBCMD_SET_TB_STATE,_T("TENDER_PAYMENT_EBT_CANCEL"));
		
		RETURNSTATE(Finish);
	}
	else
		return SMCmDataEntryBase::PSButtonGoBack();
}
#endif //_TRADESHOWDEMO

SMStateBase * SMCmDataEntry::PSClearKey(void)
{
    return SMCmDataEntryBase::PSClearKey();
}

SMStateBase * SMCmDataEntry::PSSpaceKey(void)
{
    return SMCmDataEntryBase::PSSpaceKey();
}

SMStateBase * SMCmDataEntry::DMScanner(void)
{
    CString csProperty = _T("IsPriceInquiryState");
	CString csValue;
	SendTBCommand(TBCMD_GET_TBPROPERTY, &csProperty, &csValue);
	if (csValue == _T("1"))
	{
		nTimeOut = PRICE_INQUIRY_TIMEOUT;
		m_sInput = csDMLastBarLabel;
		
        //POS73196/71821 - turn on the yellow light - rrs
        //return HandleDataNeeded();
        SMStateBase *pReturnState = HandleDataNeeded();
		
        dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
			DM_TRICOLORLIGHT_ON,
			DM_TRICOLORLIGHT_NORMALREQUEST);
		
        return pReturnState;
		//e POS73196/71821 - turn on the yellow light - rrs        
	}
	// start CRD 125377
    CString csContextToDisplay = sm_pDataNeededEntry.GetContextToDisplay();
	trace(L6, _T("SMCmDataEntry::DMScanner SND csContextToDisplay =%s"), csContextToDisplay);
	if ((m_bShopAndDropEnabled) && (csContextToDisplay == _T("mPerksEntry")))
	{
		trace(L6, _T("Scanned something on mPerks Pin Entry go back to Itemization"));

		SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F8}")));    
		Sleep(100);
        return TBItemSale(false,0,true);
	}
	//  end CRD 125377
   return SMCmDataEntryBase::DMScanner();
}

SMStateBase * SMCmDataEntry::DMScannerEncrypted(void)
{
    return SMCmDataEntryBase::DMScannerEncrypted();
}

SMStateBase * SMCmDataEntry::DMCardReader(void)
{
    return SMCmDataEntryBase::DMCardReader();
}

SMStateBase * SMCmDataEntry::TimedOut(void)
{
    CString csProperty = _T("IsPriceInquiryState");
	CString csValue;
	SendTBCommand(TBCMD_GET_TBPROPERTY, &csProperty, &csValue);
	if (csValue == _T("1"))
	{
		dm.turnOffTriColorLight();   //POS77355
		SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F1}")));
	}
    //+SSCOI-48082
	CString csContextToDisplay = sm_pDataNeededEntry.GetContextToDisplay();
	int g_nChipCardTimer;
	g_nChipCardTimer = cr.GetIntOption(_T("Timeout"),_T("ChipCardDetectedTimer"),0,_T("Scotopts"));
	trace(L4, _T("SMCmDataEntry::TimedOut() --- ChipCardDetectedTimer[%d],nTimeOut[%d]"), g_nChipCardTimer,nTimeOut);
	if (csContextToDisplay == _T("ChipCardEntry"))
	{
		trace(L4, _T("SMCmDataEntry::TimedOut() --- ChipCardDetectedTimer[%d],nTimeOut[%d]"), g_nChipCardTimer,nTimeOut);
		m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("ChipCardEntry"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
		CString csTitle2;
		csTitle2.Format(ps.GetPSText(BTT_GOBACK, SCOT_LANGUAGE_PRIMARY));
		COleVariant v;
		v = csTitle2;
		m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("ChipCardEntry"), UI::PROPERTYTEXTFORMAT, v);
		trace(L4, _T("SMCmDataEntry::TimedOut() --- Enabling GoBack button in ChipCardEntry Screen"));
	}
	//-SSCOI-48082
    return SMCmDataEntryBase::TimedOut();
}

//TAR335785+
SMStateBase * SMCmDataEntry::OnWtIncreaseNotAllowed(void)
{
    m_bMJWtIncreaseAtSmCmDataEntry = true;
	SMStateBase *returnState = SMStateBase::OnWtIncreaseNotAllowed();	 
     return returnState;
}
//TAR335785-

SMStateBase * SMCmDataEntry::OnWtDecrease(void)
{
    return SMCmDataEntryBase::OnWtDecrease();
}

SMStateBase * SMCmDataEntry::OnMatchedWt(void)
{
    return SMCmDataEntryBase::OnMatchedWt();
}

SMStateBase * SMCmDataEntry::OnTBNeedData(void)
{
    return SMCmDataEntryBase::OnTBNeedData();
}

SMStateBase * SMCmDataEntry::OnBackToLGW(void)
{
    return SMCmDataEntryBase::OnBackToLGW();
}
CString SMCmDataEntry::GetButtonCommand(int nButtonIndex)
{
    return SMCmDataEntryBase::GetButtonCommand(nButtonIndex);
}

SMStateBase * SMCmDataEntry::OnRAPUnsolicitedDataReceived(LPCTSTR sData)
{
    return SMCmDataEntryBase::OnRAPUnsolicitedDataReceived(sData);
}

/////////////////////////////////// 
// POS37599 - Would still need to go back to the PIN Entry screen after pressing GO BACK from Help screen
SMStateBase  *SMCmDataEntry::PSButtonHelp(void)// help
{
    if( sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && sm_pDataNeededEntry.GetSectionName() == _T("DigitalCouponPINEntry") )
    {
        trace(L4, _T("CMDataEntry with DigitalCouponPINEntry context and no buttons, Help button is pressed so set the g_bTBNeedMoreData flag"));
        g_bTBNeedMoreData = true;  
    }
	
    return SMCmDataEntryBase ::PSButtonHelp();
}

//////////////////////////////////////////////////////////////////////////////////////////
//POS73196/71821 - turn on the yellow light - rrs
SMStateBase *SMCmDataEntry::RAParse(MessageElement *me)
{
    SMStateBase *sUser = STATE_NULL;
    CString csStateName;  
	trace(L4,_T("SMCmDataEntry::RAParse"));
	
    sUser = SMCmDataEntryBase::RAParse(me);
	
    switch (me->raInfo.msg)
    {    
	case WMRA_REQUEST_APPROVAL:
        {
            CString csProperty = _T("IsPriceInquiryState");
            CString csValue;
            SendTBCommand(TBCMD_GET_TBPROPERTY, &csProperty, &csValue);
			if(csValue == _T("1"))
			{
                //POS73196/71821 - turn on the yellow light - rrs
                dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
					DM_TRICOLORLIGHT_ON,
					DM_TRICOLORLIGHT_NORMALREQUEST);
				//e POS73196/71821 - turn on the yellow light - rrs
            }
            break;
        }
	default:
        {
        }
    }
	
    return sUser;
}
