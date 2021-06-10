//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCardProcessing.cpp
//
// TITLE: Class impliementation for check signature state
//
//
// AUTHOR:    Peter Denhoed
//
// CHANGE HISTORY:
//
// POS262867 Work Request:68941 Name:Robert Susanto Date: May 23, 2016
// POS257231 Work Request:68941 Name:Robert Susanto Date: May 2, 2016
// POS257231 Work Request:68941 Name:Robert Susanto Date: March 31, 2016
// POS189696 Work Request:68237 Name:Saiprasad Srihasam Date: April 14, 2015
// POS138634 Work Request:66559 Name:Kranthi Kumari Cherugondi Date:March 3, 2015
// POS39476/POS53087 Work Request:52635 Name:Robert Susanto Date:January 9, 2012
// POS39476/POS46373 Work Request:52635 Name:Robert Susanto Date:December 12, 2012
// POS22174 Work Request:16656 Name:Robert Susanto Date:October 21, 2011
// POS15437 Work Request:15196 Name:Robert Susanto Date:August 1, 2011
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // base state

#include "SMCardProcessing.h"     // MGV this state

#include "SMInProgress.h"
#include "SMFinish.h"
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("CardProcessing")

IMPLEMENT_DYNCREATE(SMCardProcessing, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////
SMCardProcessing::SMCardProcessing()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMCardProcessing::Initialize(void)
{
    //+TAR 388291
	CString csPOSState(_T(""));
    SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);

    CString csTenderType;
	long lTenderSelected = SendTBCommand(TBCMD_GET_CUSTOMER_SELECTED_TENDER, &csTenderType);
	if((bEnterTenderState)&& GetTBProperty(_T("IsFromMPerksPrompt")) != _T("1") && /* POS290569 */
	   (!lTenderSelected) && 
	   ( csPOSState == _T("PAYMENTMENU_DK1") ))	//if there is no tender selected we go back to SMFinish.
	{
		RETURNSTATE(Finish);	//TAR 373469
	}
	//-TAR 388291

	/*tar302531 start*/
	bool bFlag=fInLoyaltyCardProcessing;
	
	CString strGiftCardInUse = _T("GiftCardInUse");
	CString strGiftCard;
	SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strGiftCardInUse, &strGiftCard);
    
	if(strGiftCard==_T("1"))
		fInLoyaltyCardProcessing=true;
	
	/*tar302531 end*/

	//  SMCardProcessingBase::Initialize();
    SMStateBase  *pNextState;
	pNextState =  BaseInitialize();

	fInLoyaltyCardProcessing=bFlag;
  
	CString csTBState(_T(""));
	SendTBCommand(TBCMD_GET_TB_STATE,&csTBState);
	/*if(csTBState == _T("WIC_PROCESSING"))
	{
		ps.Message12(SSF_MSG_WIC_CARD_PROCESSING, true);
		m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("CardProcessing"), UI::PROPERTYVISIBLE, VARIANT_TRUE);	
	}
	else
		m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("CardProcessing"), UI::PROPERTYVISIBLE, VARIANT_FALSE);	*/
    
    // POS15437 -- rrs
    // Enable the Go Back button whenever the POS still displays the Cancel Tender on the Softkey
    CString strButton8 = _T("DK8_Label_Text");
    CString strButton8Value;
	SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strButton8, &strButton8Value);

    CString strButton8State = _T("DK8_Grayed_Prop");
    CString strButton8StateValue;
	SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strButton8State, &strButton8StateValue);
    trace(L6, _T("POS State = %s, Button8 Text = %s, State = %s"), csPOSState, strButton8Value, strButton8StateValue);
//POS189696
    if( ( strButton8Value.Find(_T("Cancel Tender")) == 0 && strButton8StateValue == _T("0") ) || csTBState == _T("WIC_PROCESSING") || csTBState == _T("SMART_WIC_PROCESSING") )
    {
        m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("CardProcessing"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
        ps.CMButton(_T("ButtonHelp"), BTT_HELP,true);

        //POS22174 - Make sure the CANCEL button is enabled again when POS has the Cancel Tendder button
        ps.CMButton(_T("ButtonGoBack"), BTT_CANCEL,true);
        pNextState = STATE_NULL;
    }
    else
    {
        m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("CardProcessing"), UI::PROPERTYVISIBLE, VARIANT_FALSE);	
        ps.CMButton(_T("ButtonHelp"), BTT_HELP,false);
        /*CREATE_AND_DISPENSE(InProgress)(storeMode(),
                            	helpMode(),
		                        co.getTimeOutTender());*/
    }
    // POS15437 -- rrs
 

  return pNextState;
}

// POS15437 - 
void SMCardProcessing::UnInitialize(void)
{
    ps.CMButton(_T("ButtonHelp"), BTT_HELP,true);
	SMCardProcessingBase::UnInitialize();
}// POS15437 - 

SMStateBase * SMCardProcessing::OnWtIncreaseNotAllowed(void)
{
    return SMCardProcessingBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMCardProcessing::OnWtDecrease(void)
{
    return SMCardProcessingBase::OnWtDecrease();
}

SMStateBase * SMCardProcessing::DMScanner(void)
{
    return SMCardProcessingBase::DMScanner();
}

SMStateBase * SMCardProcessing::DMAcceptor(void)
{
    return SMCardProcessingBase::DMAcceptor();
}

SMStateBase *SMCardProcessing::PSButtonHelp()
{
	return SMStateBase::PSButtonHelp();
}

SMStateBase  *SMCardProcessing::BaseInitialize(void)
{
    // Tell security that we entered this state
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("CardProcessing") );
    // the below line has to be the first thing in this function
	// otherwise, there will be a race condition where RAP operator will manage to get into assistmode at RAP
	// CANNOT enter Remote Assist Mode at this state
    ra.RequestEnableAssistMode(false);  // TAR246532

	//TAR245485
	setAnchorState(BES_CARDPROCESSING);
	//TAR241756+ CHU 070203
	//If we are in this state and we have to go to SystemMessage, when we 
	//come back from SystemMessage, TB could have already finished its tasks
	//and told ScotApp to go to a new state, we need to go to that state instead of 
	//sitting here which makes ScotApp hung
    SMStateBase *sReturnState = CkStateStackAndReturn();
    if (sReturnState != STATE_NULL)
	  return sReturnState;
	//TAR241756-

	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_CARDPROCESSING, SCOT_LANGUAGE_PRIMARY));
   //@@@INFOPOINT

	ps.SetCMFrame(_T("CardProcessing"));
	
	CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMCardProcessingBase::Initialize"));

    //POS39476 - Move this logic up - rrs
    CString csTBState(_T(""));
    SendTBCommand(TBCMD_GET_TB_STATE,&csTBState);

    if(     co.fOperationsPartialIntegratedPINPad 
		&& !co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bIntegratedMSR  //PIP 
		&& !fInLoyaltyCardProcessing) // merged TAR233032 to 3.1.1 - fixed by Chen
	{
        //we now have a unique wav file for the PIN entry screen
        //when we go to PIN entry we now play the use pin pad to compete
        //transaction.  On Select Payment Type we instruct the user to swipe
        //card on PIN Pad

//        if (!m_bMJPlayedUsePinpad)
//        {
//		    DMSayPhrase(_T("UsePINPadCompleteTransaction"));
//            m_bMJPlayedUsePinpad = true;
//        }

        //POS39476/53087 - display UsePINPadForWIC screen for WIC EBT Card - rrs
        if( csTBState.Find(_T("WIC")) >= 0 )
        {
            ps.SetCMFrame(_T("UsePinPadForWIC"));             
            //+POS138634
            if (SMState::lWICOptionloaded == 2)
             {  
                ps.SetCMLeadthruText(SSF_TXT_INSERT_SMARTCARD);
            }
            //-POS138634
        }
        // e - POS39476/53087 - display UsePINPadForWIC screen for WIC EBT Card - rrs
        else
        {
			// Start CRD 125377 
			if (!m_bShopAndDropEnabled) // Yes button pressed on screen and PrintGiftReceipt enabled
			{
				DMSayPhrase(_T("UsePINPadCompleteTransaction"));
			}
			// End CRD 125377 
		    ps.SetCMLeadthruText(LTT_PINPADENTRY);
		    ps.Message12(MSG_PINPADENTRY, true);
        }
	}
	else
	{
        
        //POS39476 - display UsePINPadForWIC screen for WIC EBT Card - rrs
        if( csTBState.Find(_T("WIC")) >= 0 )
        {
            ps.SetCMFrame(_T("UsePinPadForWIC"));             
            //+POS138634
        if (SMState::lWICOptionloaded == 2)
             {  
                ps.SetCMLeadthruText(SSF_TXT_INSERT_SMARTCARD);
            }
            //-POS138634
        }
        //e POS39476 - display UsePINPad screen for WIC EBT Card - rrs
        else
        {
            ps.SetCMLeadthruText(LTT_CARDPROCESSING);
		    ps.Message12(MSG_CARDPROCESSING, true);
        }
	}

    ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	//@@@INFOPOINT
	
  // Display the Auxilary card processing screen
  DisplayAuxScreen(AUX_SCREEN_CARD_PROCESSING, TRUE);
	ps.ShowCMFrame(false);
  delete getRealStatePointer( sReturnState );

 
  if( csTBState == _T("WIC_PROCESSING") || csTBState == _T("SMART_WIC_PROCESSING")) //POS189696
  {
		trace(L3, _T("Processing WIC CARD remaining in Card Processing state and not going to in progess"),  csDMLastBarCode); 
		return STATE_NULL;
  }
  // POS15437 -- rrs
  /*
  CREATE_AND_DISPENSE(InProgress)(storeMode(),
                            	helpMode(),
		                        co.getTimeOutTender());*/
  return STATE_NULL;
}

SMStateBase  *SMCardProcessing::PSButtonGoBack(void)
{  
    //POS22174 - Make sure the CANCEL button is disabled after pressing it to prevent multiple presses
    ps.CMButton(_T("ButtonGoBack"), BTT_CANCEL,false);
	
    CString csTBState(_T(""));
    SendTBCommand(TBCMD_GET_TB_STATE,&csTBState);
    if( csTBState == _T("WIC_PROCESSING") || csTBState == _T("SMART_WIC_PROCESSING")) //POS189696
    {
		SendTBCommand(TBCMD_SET_TB_STATE,_T("WIC_CANCEL"));
		// +POS138637
        if (bEnterTenderState)
        {
            return setAndCreateAnchorState(BES_FINISH);
        }
		// -POS138637
    }
    else
    {
        // POS15437 SET the TB_STATE to CANCEL_TENDER when CANCEL button is pressed.
        SendTBCommand(TBCMD_SET_TB_STATE,_T("CANCEL_TENDER"));
    }    
	
	CREATE_AND_DISPENSE(InProgress)(storeMode(),
		helpMode(),
		co.getTimeOutTender()); 
}

SMStateBase  *SMCardProcessing::TimedOut(void)
{
	CString csTBState(_T(""));
	SendTBCommand(TBCMD_GET_TB_STATE,&csTBState);
	
    CString csPOSState(_T(""));
    SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);
	
    //pos15437 fallback mechanism if the MSR hangs, enable the HELP button so that attendant could help
    ps.CMButton(_T("ButtonHelp"), BTT_HELP,true);
	
	// Start CRD 125377 
	if (m_bShopAndDropEnabled) // Yes button pressed on screen and PrintGiftReceipt enabled
	{
		DMSayPhrase(_T("UsePINPadCompleteTransaction")); //POS295147 rrs
	}
	// End CRD 125377 
	
    // We should investigate potentially returning 
    // STATE_NULL for all SMCardProcessing::TimedOut() calls
    // For the time being, limiting the scope is prudent
	
	if( csTBState == _T("WIC_PROCESSING") ||
        csPOSState == _T("VERIFY_SIGNATURE") ||
        csTBState == _T("SMART_WIC_PROCESSING")) //POS189696
	{
		return STATE_NULL;
	}
	else
    {
		return SMCardProcessingBase::TimedOut();
    }
}

// POS15437 - 
SMStateBase *SMCardProcessing::TBParse(MessageElement *me)
{
    SMStateBase *returnState = SMCardProcessingBase::TBParse(me);

    // Enable the Go Back button whenever the POS still displays the Cancel Tender on the Softkey
    CString csTBState(_T(""));
	SendTBCommand(TBCMD_GET_TB_STATE,&csTBState);

    CString csPOSState(_T(""));
    SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);

    CString strButton8 = _T("DK8_Label_Text");
    CString strButton8Value;
	SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strButton8, &strButton8Value);

    CString strButton8State = _T("DK8_Grayed_Prop");
    CString strButton8StateValue;
	SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strButton8State, &strButton8StateValue);
    trace(L7, _T("POS State = %s, Button8 Text = %s, State = %s"), csPOSState, strButton8Value, strButton8StateValue);
    
    if( strButton8StateValue == _T("0") && strButton8Value.GetLength() == 0 )
    {
        strButton8 = _T("DK8_Label_Alt_Prop");            
	    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strButton8, &strButton8Value);
        trace(L7, _T("Button8 Text from Alt = %s, State = %s"), strButton8Value, strButton8StateValue);
    }


    if( ( strButton8Value.Find(_T("Cancel Tender")) == 0 && strButton8StateValue == _T("0") ) || csTBState == _T("WIC_PROCESSING") || csTBState == _T("SMART_WIC_PROCESSING")) //POS189696
    {
        m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("CardProcessing"), UI::PROPERTYVISIBLE, VARIANT_TRUE);    

        //POS22174 - Make sure the CANCEL button is enabled again when POS has the Cancel Tendder button
        ps.CMButton(_T("ButtonGoBack"), BTT_CANCEL,true);
        
        ps.CMButton(_T("ButtonHelp"), BTT_HELP,true);
    }
    else
    {
        m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("CardProcessing"), UI::PROPERTYVISIBLE, VARIANT_FALSE);	
        ps.CMButton(_T("ButtonHelp"), BTT_HELP,false);
        /*CREATE_AND_DISPENSE(InProgress)(storeMode(),
                            	helpMode(),
		                        co.getTimeOutTender());*/
    }
    

    return returnState;
    // We should investigate potentially returning 
    // STATE_NULL for all SMCardProcessing::TimedOut() calls
    // For the time being, limiting the scope is prudent
}
