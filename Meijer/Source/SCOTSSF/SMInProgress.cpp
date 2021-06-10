//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMInProgress.cpp
//
// Description: The InProgress state is used when the transition from a
//        state to the next state might take some time.  InProgress is a
//        "chameleon" state with respect to Customer Mode, Store Mode,
//        and Help Mode - it becomes the mode of the creating state and
//        therefore does not inherit from the above classes.
//
//
// Author:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////
 
#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMInProgress.h"           // MGV this state
#include "SMTakeCard.h"
#include "transbrokerssf.h"

#include "SMScanAndBag.h"			// RFQ 9545 FLMS

#define COMP_ID ID_SM               // base state component
#define T_ID _T("InProgress")

IMPLEMENT_DYNCREATE(SMInProgress, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////////////
SMInProgress::SMInProgress(const bool storeMode,
                           const bool helpMode,
                           const int timeout)
		:SMInProgressBase(storeMode, helpMode, timeout),
         m_bDisabledGoBack(false)
{
  IMPLEMENT_TIMESTAMP
		
}

////////////////////////////////////////
SMInProgress::SMInProgress()
: m_bDisabledGoBack(false)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMInProgress::Deliver(const bool storeMode,
                           const bool helpMode,
                           const int timeout)
{
  SMInProgressBase::Deliver(storeMode, helpMode, timeout);

  return this;
}

bool SMInProgress::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMInProgressBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMInProgress::Initialize(void)
{
    CString csCurFLState;
	CString strFLStatePropName = _T("NewFLState");
	SendTBCommand(TBCMD_GET_TBPROPERTY, &strFLStatePropName, &csCurFLState);
	//TAR352672
	//don't accept another cash or coins if it still in progress and came from SMCashPayment state.
	if(csCurFLState == _T("SMCashPayment"))
	{
		DMCashAcceptorDisable();	// Coin Acceptor Modifications
		DMCoinAcceptorDisable();	// Coin Acceptor Modifications
	}

	// If AllowLateEFTCancel Option Set, Disable Cancel Key.
	CString strOptionName = _T("AllowLateEFTCancel");
    CString strEFTLateCancel;
    SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &strEFTLateCancel);
	if(strEFTLateCancel == _T("N"))
	{
		//ps.Button(_T("ButtonGoBack"), BMP_CARDSWIPE, false);
        ps.ButtonState(_T("ButtonGoBack"), false, true );
        m_bDisabledGoBack = true;
	}

	SMStateBase *pRetState = SMInProgressBase::Initialize();
    

	strOptionName = _T("ShowPleaseWait");
    CString strPleaseWait;
    SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &strPleaseWait);
	if(strPleaseWait == _T("Y"))
	{
		ps.SetLanguage(m_customerLanguage);     //TAR 425099 //SSCOI-45231 - set only the Language when this method is about to set a frame
		ps.SetCMFrame(_T("CustomMessage"));
		m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		m_pAppPSX->SetConfigProperty(_T("ButtonHelp"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);

		ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
		CString csScreenTxt = _T("\n\n") + ps.GetPSText(TXT_PLEASEWAIT);
		ps.Message12(csScreenTxt, true);
		ps.LeadThruText(LTT_CASHPROCESSING);

		ps.ShowCMFrame();
	}

	//mini & super mini pay station
	//display retrieving receipt messages
	/*
	CString csOptionName = _T("FastLaneConfiguration");
	CString csFLConfig;
	SendTBCommand(TBCMD_GET_OPTIONS, &csOptionName, &csFLConfig);
	*/
	//if ( csFLConfig == _T("Mini") || csFLConfig == _T("SuperMini") )
	//{
		strOptionName = _T("IsResumeTrxInProgress");
		CString strIsResume = _T("0");
		SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strOptionName, &strIsResume);

		if( strIsResume == _T("1") && !g_bInRecallMode && !bFinishRecall)	//CRD 254582 make sure resume retrieving screen doesn't show up if it's FLMS
		{
            // SOTF 9034 - RRS
            // Display the RetrievingReceipt if it's not asking customer to sign the PINPad
            CString csPOSState;
            SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);
            trace(L6, _T("Test Current POSState: %s"), csPOSState);
            if(csPOSState != _T("SIGNATURE_VERIFY"))
            {
                //POS14413 - rrs
                // since there is no way to predict how long the resume process will be finished, we need to set the timeout
                // higher than standard timeout for selling an item which is 15 sec.
                // For the benchmark, 50 items in suspended trx would require about 20sec for the TB to process all of them
                // For now the timeout would be set to be 60 sec so that the system would not hang when there is a problem with the resume process.
                CString nResumeTimeOut = GetTBProperty(_T("ResumeProcessTimeOut"));
                if( nResumeTimeOut.GetLength() == 0 )
                {
                    nTimeOut = 60;
                }
                else
                {
                    nTimeOut = _ttoi(nResumeTimeOut);
                }
                trace(L6, _T("Set Timeout for Resuming Transaction(%d)"), nTimeOut);
                //POS14413 - rrs

                //POS40946 if the m_bIsFromDisplayingResumeItem is true, then it means that customer scanned the suspended barcode again
                if(m_bIsFromDisplayingResumeItem)
                {
                    m_bIsFromDisplayingResumeItem = false;                    
                }
				ps.SetLanguage(m_customerLanguage);     //TAR 425099 //SSCOI-45231 - set only the Language when this method is about to set a frame
			    ps.SetCMFrame(_T("RetrievingReceipt"));
			    ps.LeadThruText(SSF_LTT_RETRIEVING_RECEIPT);
			    ps.Message12(SSF_TXT_RETRIEVING_RECEIPT_WAIT, true);
			    ps.ShowCMFrame();

                //POS31054 - turn on the yellow light and add the red bar on the rap e-receipt area
                if( GetHookProperty(_T("PropMobShopOrderPrompt")) == _T("1") )
                {
                    CString csMessage = ps.GetPSText(SSF_TXT_ENHANCEDAM_MOBILE_RESUME, SCOT_LANGUAGE_PRIMARY);
                    
                    COleVariant	vValue;
                    vValue = csMessage;

                    m_pAppPSX->SetConfigProperty(_T("LaneMobileStateBackground"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);
                    m_pAppPSX->SetConfigProperty(_T("LaneMobileStateWindow"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);
                    m_pAppPSX->SetConfigProperty(_T("LaneMobileStateWindow"), _T(""), UI::PROPERTYTEXTFORMAT, vValue);

                    dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                                        DM_TRICOLORLIGHT_ON,
                                        DM_TRICOLORLIGHT_NORMALREQUEST);

                    m_bNeedYellowLightForMobile = true;
                }
                // e POS31054
            }
            // E SOTF 9034 - RRS
		}
	//}

    //POS39476/POS52766 - change the logo to mperks logo if its for mperks screen - rrs
        trace(L6, _T("POSState in SMInProgress(%s)"), GetHookProperty(_T("__POSState")) );
    if( GetHookProperty(_T("__POSState")).Find(_T("DIGITAL_COUPON")) != -1 )
    {
        m_pAppPSX->SetConfigProperty(_T("SmallmperksLogo"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_TRUE);        
    }
    else
    {        
        m_pAppPSX->SetConfigProperty(_T("SmallmperksLogo"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
    }
    //e POS39476/POS52766 - Reset the small logo - rrs

	return pRetState;
	//return ShowPleaseWait(_T(""));
}

void SMInProgress::UnInitialize(void)
{
    SMInProgressBase::UnInitialize();
    if ( m_bDisabledGoBack )
    {
        ps.ButtonState(_T("ButtonGoBack"), true, true );
    }
}

SMStateBase  *SMInProgress::PSButtonGoBack(void)
{
    // Cancel Tender - Including Swipe!
	
	// ONLY if not yet in processing on POS.
	if (GetTBProperty(_T("IsCardTenderSelected")) == _T("1"))
	{
		CString strTenderPropName = _T("TenderType");
		CString strTenderValue = _T("");
		SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strTenderPropName, &strTenderValue);
		SendTBCommand(TBCMD_SET_TB_STATE,_T("CANCEL_TENDER"));
		//Go Back to SMFinish!
		
		// CAN enter Remote Assist Mode after leaving this state
		//ra.OnSetTransactionState(RA_STATE_ASSISTOK);
		ra.RequestEnableAssistMode(true);   //TAR213249
		
		setAnchorState(BES_FINISH);
		CREATE_AND_DISPENSE(TakeCard)(false);
	}
	
	return STATE_NULL;
}

SMStateBase *SMInProgress::IdleTimer(const long lMsec)
{
	if (GetTBProperty(_T("IsCardTenderSelected")) != _T("1"))
	{
        ps.ButtonState( _T("ButtonGoBack"), false, true );
        m_bDisabledGoBack = true;
	}
	
    //POS139881
    CString csPOSState(_T(""));
    SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);
	
    trace(L6, _T("POSState at InProgress(%s)"), csPOSState);
    
    if( csPOSState.Find(_T("WAITING_ON_PINPAD")) >= 0 && getAnchorState() == BES_FINISH && nTenderType != TB_TENDER_CASH)
    {
		if (GetTBProperty(_T("IsCardTenderSelected")) == _T("1"))	//POS311880
			return TimedOut(); 
    }
	//e POS139881
	
	return SMInProgressBase::IdleTimer(lMsec);
}

bool SMInProgress::inProgress()
{
    return SMInProgressBase::inProgress();
}

bool SMInProgress::storeMode()
{
    return SMInProgressBase::storeMode();
}

bool SMInProgress::helpMode()
{
    return SMInProgressBase::helpMode();
}

SMStateBase * SMInProgress::OnWtIncreaseNotAllowed(void)
{
    return SMInProgressBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMInProgress::OnWtDecrease(void)
{
    return SMInProgressBase::OnWtDecrease();
}
bool SMInProgress::PSRemoteLegalForState()
{
    return SMInProgressBase::PSRemoteLegalForState();
}

bool SMInProgress::PSAssistMenuLegalForState()
{
    return SMInProgressBase::PSAssistMenuLegalForState();
}

SMStateBase * SMInProgress::TimedOut()
{
    if (GetTBProperty(_T("IsCardTenderSelected")) == _T("1"))
	{
        trace(L6, _T(" !!! Timedout in SMInProgress and Card Tender was selected!!!, potential out-of-sync situation betweeen TB and SCOT"));
        return PSButtonGoBack();
    }
    else
    {
        //+TAR396928
        if( fSAInItemSale )
        {
			// start RFQ 9594
			// This means the item was likely not sent to the POS. See TBMsgMap.xml
			if (g_bInRecallMode)
			{
				trace(L6, _T("SMInProgress::TimedOut - In Mobile shopper ticket processing item not be sent to POS. Resent FLMS item to POS..."));
				m_bResendFLMSitem = true;
	            
				trace(L6, _T("SMInProgress::TimedOut - Item sale may have timed out at the POS. Sending CANCEL_ITEM."));
				SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);

				trace(L2, _T("SMInProgress::TimedOut - Changing Anchor State to ScanAndBag."));
				return setAndCreateAnchorState(BES_SCANANDBAG);
			}
			// end RFQ 9594

            trace(L6, _T("Item sale may have timed out at the POS. Sending CANCEL_ITEM."));
            SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
        }
        //-TAR396928
        return SMInProgressBase::TimedOut();
    }
}

SMStateBase * SMInProgress::DMScanner()
{
    return SMInProgressBase::DMScanner();
}

SMStateBase * SMInProgress::DMCardReader()
{
    return SMInProgressBase::DMCardReader();
}

SMStateBase * SMInProgress::OnBackToLGW(void)
{
    return SMInProgressBase::OnBackToLGW();
}

SMStateBase * SMInProgress::OnTBNeedData(void)
{
    return SMInProgressBase::OnTBNeedData();
}

SMStateBase * SMInProgress::DMAcceptor(void) //TAR352672
{	
   return SMInProgressBase::DMAcceptor();
}

SMStateBase * SMInProgress::PSButtonHelp(void)
{
    return SMInProgressBase::PSButtonHelp();
}

SMStateBase * SMInProgress::PSButton1(void)
{
    return SMInProgressBase::PSButton1();
}

SMStateBase * SMInProgress::DMParse( const MessageElement* me)
{
    return SMInProgressBase::DMParse(me);
}

SMStateBase * SMInProgress::SASecMgrParse( const MessageElement* me)
{
    return SMInProgressBase::SASecMgrParse(me);
}

SMStateBase * SMInProgress::TBParse(MessageElement* me)
{
    return SMInProgressBase::TBParse(me);
}
