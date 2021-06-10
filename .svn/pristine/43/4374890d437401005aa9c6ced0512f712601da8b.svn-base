//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMSecUnExpectedIncreaseBase.cpp
//
// TITLE:	Class header for Sec Violation-SMSecUnExpectedIncrease
// AUTHOR:  	Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecUnExpectedIncrease.h"      
#include "SMSecUnexpectedDecrease.h"
#include "SMSecBagBoxThreshold.h"      
#include "SMSecItemRemovedThreshold.h"      
#include "SMSecNewItemEntry.h"      
#include "SMSecSkipBaggingThreshold.h"      
#include "SMPickingUpItems.h"
#include "SMScaleBroken.h"          // Reset scale
#include "SMBagAndEAS.h" 
#include "SMScanAndBag.h" 
#include "MPProcedures.h"			// RFC 233322
#include "SMSecBagBoxNotValid.h"//SR085
#else
#include "SMSecUnExpectedIncreaseBase.h"
#endif //_CPPUNIT
#ifndef _CPPUNIT
#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecUnExpectedIncrease")

#endif //_CPPUNIT
PSTEXT nBtt1;
#ifndef _CPPUNIT
DLLEXPORT extern HWND hw;           // RFC 233322
DLLEXPORT extern MPProcedures mp;   // RFC 233322

IMPLEMENT_DYNCREATE(SMSecUnExpectedIncreaseBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

#endif //_CPPUNIT
bool SMSecUnExpectedIncreaseBase::bTABIsFlushing = false;
static bool bTABHasBeenApproved = false; //221089 CHU 112502
#ifndef _CPPUNIT
SMSecUnExpectedIncreaseBase *SMSecUnExpectedIncreaseBase::m_pUnExpectedIncrease = NULL; //TAR379130/379131

//////////////////////////////////////////
SMSecUnExpectedIncreaseBase::SMSecUnExpectedIncreaseBase(const BEST prevState, const BEST approvedNextState)
: csCurContext(EMPTY_STRING)
{
  m_PrevState = prevState;		//state we were in when the unexpected increase occurred
  m_NextState = approvedNextState;	//state we should goto when the unexpected increase is approved
  IMPLEMENT_TIMESTAMP
}
//////////////////////////////////////////
SMSecUnExpectedIncreaseBase::SMSecUnExpectedIncreaseBase()
: csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSecUnExpectedIncreaseBase::Deliver(const BEST argPrevState, const BEST argApprovedNextState)
{
  m_PrevState = argPrevState;
  m_NextState = argApprovedNextState;
  return this;
}
////////////////////////////////////////
bool SMSecUnExpectedIncreaseBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, both ints
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMSecUnExpectedIncreaseBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver((BEST)DLParamDWORD(dlParameterList),    
                (BEST)DLParamDWORD(dlParameterList));   
        return true;
    }
    else
    {
        trace(L2, _T("+SMSecUnExpectedIncreaseBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}
///////////////////////////////////////
SMStateBase  *SMSecUnExpectedIncreaseBase::Initialize(void)
{

	if ((ImmediateInterventionStack.GetCount() == 0) && (secEventParms.m_csDisposition != KEY_OSE_DISPOSITION_FORGIVEN))
    {
		if(bQuietNeedSecApproval)
		{
			trace(L6, _T("No intervention and bQuietNeedSecApproval is reset to false, go back to backtolgw. "));
			return OnBackToLGW();
		}
		trace(L6, _T("No intervention and bQuietNeedSecApproval is false, go back to AnchorState. "));
		return createAnchorState();
    }
	ra.RequestEnableAssistMode(false); //Tar 238294

	// SR682-Show/hide Weight Added and Number Of Items Sold under Unexpected Increase screen.	
	if(co.fOperationsShowWeightInfoOnLane == true)
		ps.ShowCMWeightNumber(g_csExpectedWeight, g_csDeltaWeight, lItems);
	else
		ps.HideCMWeightNumber();

    if(DATANEEDED_MOREDATA == m_View) //tar 252987 && 272557
    {
       m_PrevView = m_View;
    }

    csCurContext = _T("SecUnExpectedIncrease");  // Tar 238987
	bTABHasBeenApproved = false; //221089 CHU 112502

   // TAR 205139 First check to insure that the condition still exists.  If returning from a
   // system message screen, that may not be true

	if (bIsInFinalization && g_bAllowRemove) //tar 194770
		g_bAllowRemove = false;
	

	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, csCurContext );

	// RFC 233322 RJF 042103
	// Need to check if we should delay notifying RAP/pager of increase
	// in order to give customer chance to fix it themselves
	nEscalationRAPTimeOut = co.getTimeOutDelaySecurityNotification();
	if(nEscalationRAPTimeOut != 0)
	{
		SetTimer(hw, RAP_ESCALATION, nEscalationRAPTimeOut, &EscalationTimedOut);
	}
	else		// don't delay, turn on red
	{
		cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("UnexpectedIncrease"));
		m_View = DATANEEDED_UNEXPECTEDINCREASE;
		CString csInstruction;
		csInstruction.Format(ps.GetPSText(RA_WEIGHTINCREASEINFO, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTINCREASE, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits, _T(""));
		ra.RequestEnableAssistMode(true); //Tar 238294

		if( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_OUTSTANDING )
		{
			ra.OnRAPDataNeeded(csInstruction, cDataEntry.ToString(), m_View);
			SAEnterSecurityCondition();
		}
		else if( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_DELAYED )
		{
		  trace(L7, _T("+ra.OnNormalItem, Message = %s"), csInstruction);
		  ra.OnNormalItem(csInstruction);
		  trace(L7, _T("-ra.OnNormalItem"));
		}
	}
	return STATE(SecOperatorPWState)::Initialize();
}

#endif //_CPPUNIT
////////////////////////////////////////////
void SMSecUnExpectedIncreaseBase::showScreen(bool passwordVerified)
{ 	
    ps.SetPSText(PS_WORK1,ps.GetPSText(BTT_STORELOGIN,SCOT_LANGUAGE_PRIMARY));
    ps.SetButtonText(_T("ButtonStoreLogIn"),PS_WORK1);
	bTABIsFlushing = false;	// 197562 - weight on belt at attract causes prob w/ next x-action

	nTimeOut = co.getTimeOutPeriodicSecurityAudio();		// RFC 233322

	if (passwordVerified)
		return;


		if (inputMode == PROMPTING_FOR_OPERATOR)
		{
            // NewUI
			STATE(SecOperatorPWState)::ShowEnterID();  //SR93 Biometrics support
			ps.SetCMLeadthruText( TXT_ENTER_ID );
			ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
			ps.ShowCMFrame(false);
            // End NewUI
			return;
		}
		else
		{  // Display Wait for Approval....   
			// TAR 200470 - this problem was discovered as a side effect of the show being slow
            // NewUI
			ps.SetCMFrame(_T("SecUnExpectedIncrease"));
			if (!co.GetfOperationsSuppressAudioInUnexpectedIncrease())
			{
				// the wave and background changes depending on if Belt is installed or not
				if (dm.fStateTakeawayBelt)
				{
					// TAR 195852 - changed to the play the correct wave file t.k

					//TAR379130/379131
					if ( co.getTimeOutDisplayIncreaseMisMatchScreenTimer() !=0 )
					{
						SetTimer(hw, SHOWSCREEN, co.getTimeOutDisplayIncreaseMisMatchScreenTimer()*1000, &ShowScreenTimedOut);	
					}
					else
					{
					    DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_UNEXPECTEDITEMONBELT));	// play wave here so it doesn't replay on the key pad screen
					}
				}
				else
				{
					// TAR 195852 - changed to the play the correct wave file t.k
					DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_UNEXPECTEDITEM));	// play wave here so it doesn't replay on the key pad screen
				}
			}
			else //TAR379130/379131
			{
				if (dm.fStateTakeawayBelt && co.getTimeOutDisplayIncreaseMisMatchScreenTimer() !=0 )
				{
					SetTimer(hw, SHOWSCREEN, co.getTimeOutDisplayIncreaseMisMatchScreenTimer()*1000, &ShowScreenTimedOut);	
				}
                else if( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_FORGIVEN )
                {
                    SetTimer(hw, SHOWSCREEN, co.getTimeOutDisplayIncreaseMisMatchScreenTimer()*1000, &ShowScreenTimedOut);
                }
			}
			COleVariant v = VARIANT_FALSE;
			if(nEscalationRAPTimeOut == 0)			// RFC 233322
			{
				v = VARIANT_TRUE;				// show the store mode button
			}
			m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, v);

            // End NewUI
		}

    if (dm.fStateTakeawayBelt)
    {
    	// tar 395320 +
		// only show the instruction txt if DisplayIncreaseMisMatchScreenTimer == 0 in takeAwayBelt
		if( co.getTimeOutDisplayIncreaseMisMatchScreenTimer() == 0 
			|| nEscalationRAPTimeOut == 0 )   //SSCOP-528
		{
	    	ps.SetCMLeadthruText(LTT_BELT_ATTENTION);
			ps.Message12(MSG_BELT_UNEXPECTED_ITEM, true);
		}//tar 395320 - 

		// During the new security re-write decided that it does not make sense to display
		// Bag/Box button for the Takeaway belt. Since all items will be moved to the end
		// of the belt to be bagged. Got the PM approval
		m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);

    }
    else
    {
    	ps.SetCMLeadthruText(MSG_BAGGING_ALERT);
	    ps.Message12(MSG_UNEXPECTED_WT, true);
		// During the new security re-write decided that it does not make sense to display
		// Bag/Box button for the Takeaway belt. Since all items will be moved to the end
		// of the belt to be bagged. Got the PM approval
		COleVariant v;
		//+Don't prompt for im using my own bag button 
		if(co.GetfOperationsAllowContainerBtn())
		{
	   		nBtt1 = BTT_BAGBOX;
	
			if((co.GetfOperationsDisableUnexpIncContainerBtnAftFirstItem() && (lItems <=0 || (lItems==1 && m_bScanOnAttractAtQuietMode))) //litems=1 is allowed if it is scan at quietmode
					|| (!co.GetfOperationsDisableUnexpIncContainerBtnAftFirstItem()))
			{
				v = VARIANT_TRUE;
			}
			else
			{
				   v = VARIANT_FALSE;
			}

			ps.SetButtonText(_T("CMButton1Med"), nBtt1);
		}
		else
		{
			v = VARIANT_FALSE;
		}//-Don't prompt for im using my own bag button

		m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
    }

	if (bIsInFinalization && !co.GetfOperationsAllowContainerBtnInFinalization()) // after F&P do not display bag/box - SR085 allows an option to display the bag/box
		m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);

	//SR085 +
	if(m_PrevState != BES_BAGANDEAS)
	{
		m_pAppPSX->SetConfigProperty(_T("CMSmallReceipt"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);//show the CMReceipt only if coming from quiet mode violation
	}
	else
	{
		m_pAppPSX->SetConfigProperty(_T("CMSmallReceipt"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);	// 408782
	}
	//SR085 -

    // End NewUI
	SAWLDBScannerDisable();			// Disable the scanner
//+SSCOP-2528
	COleVariant vReturn;
	if( ps.GetLCID(ps.GetLanguage()) == _T("0401"))
	{
		CString csTemp( _T("") );
		m_pAppPSX->GetConfigProperty(_T("CMWeightObservedAmount"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT,vReturn);
		csTemp = vReturn.bstrVal;
		if(csTemp != _T("2"))
			m_pAppPSX->SetConfigProperty(_T("CMWeightObservedAmount"), ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT, _T("2")); 
		m_pAppPSX->GetConfigProperty(_T("CMItemNumberAmount"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT,vReturn);
		csTemp = vReturn.bstrVal;
		if(csTemp != _T("2"))
			m_pAppPSX->SetConfigProperty(_T("CMItemNumberAmount"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT, _T("2"));
		m_pAppPSX->GetConfigProperty(_T("CMWeightExpectedAmount"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT,vReturn);
		csTemp = vReturn.bstrVal;
		if(csTemp != _T("2"))
			m_pAppPSX->SetConfigProperty(_T("CMWeightExpectedAmount"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT, _T("2"));
		m_pAppPSX->GetConfigProperty(_T("CMWeightRemovedText"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT,vReturn);
		csTemp = vReturn.bstrVal;
		if(csTemp != _T("0"))
			m_pAppPSX->SetConfigProperty(_T("CMWeightRemovedText"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT, _T("0"));
		m_pAppPSX->GetConfigProperty(_T("CMWeightAddedText"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT,vReturn);
		csTemp = vReturn.bstrVal;
		if(csTemp != _T("0"))
			m_pAppPSX->SetConfigProperty(_T("CMWeightAddedText"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT, _T("0"));
		m_pAppPSX->GetConfigProperty(_T("CMItemNumberText"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT,vReturn);
		csTemp = vReturn.bstrVal;
		if(csTemp != _T("0"))
			m_pAppPSX->SetConfigProperty(_T("CMItemNumberText"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT, _T("0"));
		m_pAppPSX->GetConfigProperty(_T("CMItemNumberText"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT,vReturn);
		csTemp = vReturn.bstrVal;
		if(csTemp != _T("0"))
			m_pAppPSX->SetConfigProperty(_T("CMWeightExpectedText"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT, _T("0"));
		
	}
//-SSCOP-2528

	ps.ShowCMTBText(csTBMessage2Scot);	//thp

    DisplayAuxScreen(AUX_SCREEN_DEFAULT, true); //tar 193987

	trace(L7, _T("+ra.OnTransactionState, Itemization State"));
	ra.OnSetTransactionState(RA_STATE_SECURITY_VIOLATION);
	trace(L7, _T("-ra.OnTransactionState"));
	//TAR379130/379131
	if (dm.fStateTakeawayBelt && (co.getTimeOutDisplayIncreaseMisMatchScreenTimer() !=0 ))
	{
		trace(L6, _T("Do not display the screen"));
	}
    else if( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_FORGIVEN )
    {
        trace(L6, _T("Do not display the screen"));
    }
	else
	{
		ps.ShowCMFrame(false);
	}

}
#ifndef _CPPUNIT
//////////////////////////////////////
void SMSecUnExpectedIncreaseBase::UnInitialize(void)
{
	ra.RequestEnableAssistMode(true); //Tar 238294
	// this is needed for forgiving unexpected increases from scan & bag 
	// In scan & bag we send a "begin" to security manager, if we get an unexpected 
	// increase in scan&bag , we need to send "end" here.
	if (m_PrevState == BES_SCANANDBAG)
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
	}

	// RFC 233322 RJF 041603 ensure that the RAP escalation timer has stopped
    KillTimer(hw,RAP_ESCALATION); 
	// 3.2 patch B _ RFC 257450
    KillTimer(hw,UNEXPECTED_INCREASE);       // ensure that the timer has stopped

	KillTimer(hw, SHOWSCREEN); //TAR379130/379131
	if (bIsInFinalization) //tar 194770
		g_bAllowRemove = true;
    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 

	bQuietNeedSecApproval = false; //aiqin
	if(m_bScanOnAttractAtQuietMode)
	{
		m_bScanOnAttractAtQuietMode = false;
	}

	ClearSecurityViolationLight();

	//TAR237079 CHU/TP 051303 
	if ( g_bWtIncAtContextHelpComingFromAttract )
		g_bWtIncAtContextHelpComingFromAttract = false; 
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, csCurContext );
}

///////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSecUnExpectedIncreaseBase::PSButton1(void) //Bag/Box container
{
	trace(L6, _T("+SMSecUnExpectedIncreaseBase Bag/Box btn pressed"));
	DMSaySecurity( SEC_NONE, _T("blank.wav") );			// to interrupt the wave file 

	//+SR93 Biometrics support
	if(co.m_bOperationsUseFingerPrintDevice && fInValidOperatorPassword)
	{
			return STATE(SecOperatorPWState)::PSButton1();
	}
	//-SR93 Biometrics support

	//+TAR 391553
	if (inputMode == PROMPTING_FOR_PASSWORD || fInValidOperatorPassword) // TAR 457861
	{
		return STATE(SecOperatorPWState)::PSButton1();
	}
	//-TAR 391553

	CKeyValueParmList parmList;
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_BAG_BOX);
	_bstr_t bstrParmList((const _TCHAR *)parmList);
	BSTR bstrResult;
	bstrResult = NULL;
	SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
	SysFreeString(bstrResult);

	return STATE_NULL;
}

///////////////////////////////////////
SMStateBase  *SMSecUnExpectedIncreaseBase::PSButtonGoBack(void) // store login
{
	//TAR193505+: At EnterID screen, "go back" should be back to SMSecUnExpectedIncreaseBase screen

		switch(inputMode)
		{  
			case SHOW_WAIT_FOR_APPROVAL:
			case PROMPTING_FOR_PASSWORD:
				break;
			case PROMPTING_FOR_OPERATOR:
				{
					//+SR93 Biometrics support
					if (co.m_bOperationsUseFingerPrintDevice)
					{
						FT_Abort();
					}
					//-SR93 Biometrics support

					inputMode = SHOW_WAIT_FOR_APPROVAL;
					if (co.GetfStateDualLanguage())
						ps.SetLanguage(m_customerLanguage);  //TAR 196315: Set back to customerLanuguage for dual language
					nEscalationRAPTimeOut = 0;
					showScreen(false);
					return STATE_NULL;  
				}
		}

	//TAR193505-

    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//interrupt wave file
	promptForOperator();
    return STATE_NULL;
}
  
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMSecUnExpectedIncreaseBase::EscalationTimedOut
//
// Purpose:		handles passing timeout event to Escalation2RAP using the current state pointer
//
// Parameters:	none
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
VOID CALLBACK SMSecUnExpectedIncreaseBase::EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	mp.sCurr->Escalation2RAP();
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMSecUnExpectedIncreaseBase::Escalation2RAP
//
// Purpose:		2nd timeout routine handling the delayed security notification
//
// Parameters:	none
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
void SMSecUnExpectedIncreaseBase::Escalation2RAP()
{   // RFC 233322 - separate timer for escalation to RAP
    KillTimer(hw,RAP_ESCALATION);       // ensure that the RAP escalation timer has stopped
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("UnexpectedIncrease"));
	m_View = DATANEEDED_UNEXPECTEDINCREASE;
	CString csInstruction;
	csInstruction.Format(ps.GetPSText(RA_WEIGHTINCREASEINFO, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTINCREASE, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits, _T(""));
	ra.RequestEnableAssistMode(true); //Tar 238294

	if( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_OUTSTANDING )
	{
		ra.OnRAPDataNeeded(csInstruction, cDataEntry.ToString(), m_View);
		SAEnterSecurityCondition();
	}
	else if( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_DELAYED )
	{
	  trace(L7, _T("+ra.OnNormalItem, Message = %s"), csInstruction);
	  ra.OnNormalItem(csInstruction);
	  trace(L7, _T("-ra.OnNormalItem"));
	}
	m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
}

///////////////////////////////////////
SMStateBase *SMSecUnExpectedIncreaseBase::TimedOut(void) // timeout
{
	if (!co.fOperationsSuppressAudioInUnexpectedIncrease)	
	{
	    if (dm.fStateTakeawayBelt)
		{
			DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_UNEXPECTEDITEMONBELT));
		}
		else
		{
			DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_UNEXPECTEDITEM));
		}
	}
	return STATE_NULL;
}

//////////////////////////////////////
SMStateBase *SMSecUnExpectedIncreaseBase::stateAfterPasswordVerify()
{
	trace(L2, _T("::stateAfterPasswordVerify - bEASReEnableScanner is set to true."));
//+TAR 444373 - DO NOT CALL SetLanguage() to change the language code to get the right message.  We should 
// pass in the language code to GetPSText()
//  ps.SetLanguage(m_customerLanguage);
//-TAR 444373
	lSARedNeedingApproval = false;   // TAR 197015 Clear alert flag.
	// do same thing that OnRAPClearSecurityBtn() does
    //NewSec
 //   OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR);

	return OnRAPClearSecurityBtn(_T("Lane"));
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecUnExpectedIncreaseBase::OnBackToLGW(void)
{	
	//SSCOADK-6410
    if ( inputMode == PROMPTING_FOR_OPERATOR || inputMode == PROMPTING_FOR_PASSWORD)
    {
        DMScannerDisable();
    }

	// TAB re-enable scanner and clear Wait For Approval State
	TABReEnableScannerAndClearWaitForApprovalState();

	DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 

	// tar 211789 - AH and DE 08/15/02  delay item sale if unexpected weight increase sec violation
	if (bQuietNeedSecApproval && fItemIsWaiting)
	{
	    SAClearSecurityCondition();
		//218558 set scanner flag to re-enabled only in state that isn't handling EAS automatically
		trace(L2, _T("bEASReEnableScanner is set to true."));
		bEASReEnableScanner = true;
		setAnchorState(BES_SCANANDBAG);
		SMStateBase* retState = TBItemSale(false,0,true);

		// go to inprogress 
		return retState;
	}
	else
	{
		//218558 don't need the following code
		// see if need to wait on EAS because returning to the bag and EAS screen LPM022002 TAR196236
		// so if the EAS event has not been received, only if no quiet mode violations are being processed 212202 
		//if (!bEASReEnableScanner)				// this is set to true once the EAS event is received
		//	g_bMatchedWtWaitingOnEAS = true;	// set the waiting on EAS back to true when leaving this state.

		//218558 if matched waiting on EAS, and received EAS and customer corrected
		// their own violation (BackToLGW), then return to ScanAndBag
		// and if we are not in finalization
		if (g_bMatchedWtWaitingOnEAS && m_bEASHappened && !bIsInFinalization)
			m_PrevState = BES_SCANANDBAG;
		//TAR213414+ CHU 08/30/02: the next state should be the anchorstate
		//if there is no approval left. We should go back to anchorstate which is the state 
		//before we get in Approval state(s)
		else if ( !isAnyApprovalRequired() ) //&& m_PrevState != BES_RAPDATANEEDED) dale no TAR LPM093002
		{
			if ((m_PrevState == BES_WAITFORAPPROVAL) && !g_bgVerifySig && !g_bgVoidTrx)
			{
				if(SecMgr.GetOutstandingInterventionCount() == 0)
				{
					m_PrevState = getAnchorState();
                    //+SSCOADK-1330
                    if(dm.fStateTakeawayBelt)
                    {
                        m_NextState = m_PrevState;
                    }
                    //-SSCOADK-1330
				}
			}
		    //TAR213414-
			
			//TAR 221089+ CHU 112502 We need to transport the approved item 
			if(dm.fStateTakeawayBelt)
			{
				//+Tar 232049-CHU From a finalization state --> UnexpectedIncrease --> Transport, when
				//it leaves Transport, sometimes it causes ScotApp die. The reason is that
				//for some finalization states, they use BES_STATERETURN as the m_nextState
				//when they create UnexpectedIncrease. 
				//We don't worry about finalization states because the scanner
				//should be disabled in these states(except Finish state). For these states, approved item is
				//transfered to the end of the belt without going transport state.
				//The reason we want to go to Transport item because we don't want to be
				//at ScanAndBag with the approved item is still going down the belt
				if ( bTABHasBeenApproved && !bIsInFinalization)  
				{
					if (m_NextState == BES_STATERETURN)
					{
						//Just in case if the next state is BES_STATERETURN, 
						m_NextState = getAnchorState();
					} //- Tar 232049- CHU
					TABstateBeforeTransport = m_NextState; //Save it so we come back from Transport
					//TAR 363023
					if (!g_bReadyForItem)
					{
						m_NextState = BES_TRANSPORTITEM; 
						trace(L2, _T("Going to Transport state and the TABstateBeforeTransport is %d"), TABstateBeforeTransport);
					}
					else
						trace(L2, _T("Not going to Transport state and the TABstateBeforeTransport is %d"), TABstateBeforeTransport);
				}
			}
			//TAR221089-
			//+tar410912
			if(getAnchorState() == BES_UNKNOWNITEM)
			{
				setAnchorState(BES_SECUNEXPECTEDINCREASE);
			}
			//-tar410912
		}
		
		return ClearedSecViolSetReturnState(m_PrevState, m_NextState);
	}
}

#endif
///////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSecUnExpectedIncreaseBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{


	if(dm.fStateTakeawayBelt)	
	{
		bTABIsFlushing = true;	// 197562 - weight on belt at attract causes prob w/ next x-action
		bTABHasBeenApproved = true; //221089 CHU 112502: approved either from RAP or at lane
	}

    if(m_NextState == BES_SECSKIPBAGGINGTHRESHOLD)
    {
	    OverrideSecurityException(lpszAuthorizationType, KEY_EXCEPTION_OPERATION_CLEAR);
    }
    else
    {
        //SSCOP-6833 clear all in case if there are exceptions that app hasn't got chance to parse it
	    OverrideSecurityException(lpszAuthorizationType, KEY_EXCEPTION_OPERATION_CLEAR, KEY_EXCEPTION_CATEGORY_IMMEDIATE_WT_CHANGES); //sscop-5525
    }

	//Clear RAP Approval
	if ( (m_PrevState == BES_BAGANDEAS) && (!m_bEASHappened) )	// don't want to set bEASReEnableScanner to true
	{
		trace(L2, _T("Special case, EAS not detected yet. Returning to Bag Item. Clearing RAP DataNeeded."));
	}
	else
	{
		trace(L2, _T("bEASReEnableScanner is set to true. Clearing RAP DataNeeded."));
		bEASReEnableScanner = true;
	}
	ra.OnNoRAPDataNeeded();  
	//+tar410912/412614
	if(getAnchorState() == BES_UNKNOWNITEM || getAnchorState() == BES_DEGRADEDCONTINUETRANS || 
	   getAnchorState() == BES_TIMERESTRICTEDITEM || getAnchorState() == BES_COUPONTOOHIGH ||
	   getAnchorState() == BES_COUPONNOMATCH || getAnchorState() == BES_ITEMNOTFORSALE ||
	   getAnchorState() == BES_ITEMQUANTITYEXCEEDED || m_NextState == BES_SECSKIPBAGGINGTHRESHOLD)//397993
	{
		return STATE_NULL;
	}
	//-tar410912/412614

	//427943+
	//wait here so SM will tell FL where to go next once SM processes OverrideSecurityException
	return STATE_NULL;
//USSF START
	//USSFHOOK(_T("OnBackToLGW"), nextState = OnBackToLGW(), return nextState;);
	//return OnBackToLGW();
//USSF END
	//427943-
}

#ifndef _CPPUNIT
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecUnExpectedIncreaseBase::OnWtIncreaseNotAllowed()	 
{
		// SR682-Refresh Weight Added in the event of a weight change.
	if(co.fOperationsShowWeightInfoOnLane == true)
		ps.ShowCMWeightNumber(g_csExpectedWeight, g_csDeltaWeight, lItems);

	// send to RAP but stay in same state
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("UnexpectedIncrease"));
	m_View = DATANEEDED_UNEXPECTEDINCREASE;
	CString csInstruction, csDesc;
	csDesc.Format(ps.GetPSText(RA_UNEXPECTEDINCREASEINFO, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits);
	csInstruction.Format(ps.GetPSText(RA_WEIGHTINCREASEINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTINCREASE, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits, _T(""));
	ra.OnRAPDataNeeded(csInstruction, cDataEntry.ToString(), m_View);
	ra.RequestEnableAssistMode(true); //Tar 238294

	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecUnExpectedIncreaseBase::DMTakeawaySendScaleStatus()	 
{
	// Takeaway Belt has encountered a POSSIBLE security condition and requests
	// current scale information in order to determine what action to take.
	// Send a Bad weight signal to TAB control, since we're in a weight error screen...
    trace(L6, _T("+::DMTakeawaySendScaleStatus"));
    DMTakeawayBeltNotify(tabEventStableBadWeight);
    trace(L6, _T("-::DMTakeawaySendScaleStatus"));
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecUnExpectedIncreaseBase::OnWtDecrease()
{	
	// if LRW is less than previous total wt & there was a quiet mode violation with a scanned item 212202
	if(bQuietNeedSecApproval && fItemIsWaiting)  
	{
		return OnBackToLGW();
	}
	else if (   ((m_PrevState == m_NextState) && (m_PrevState == BES_ASSISTEDTENDER))
	         || ((m_PrevState == m_NextState) && (m_PrevState == BES_RAPDATANEEDED)) )
	{
		if (bIsInFinalization)	 //We are at finialization stage
		{
			if(!g_bIsForgiveUnexpectedDecreaseDuringTendering && !(lBalanceDue < 0.01))
				return FinalizationWtDecreaseNotAllowed(m_PrevState);
			else
				return STATE_NULL;
		}
		if (m_PrevState == BES_ASSISTEDTENDER)
			return VoidingWtIncrease(m_PrevState, m_NextState);
		else
			return ThresholdViolWtDecrease(m_PrevState, m_NextState);
	}

	// SR695 - Check if we are in finalization and are either forgiving decreases or the balance was paid in full
	// If so, no exception will be raised in the event of an Increase -> Decrease below LGW
	else if ((bIsInFinalization && g_bIsForgiveUnexpectedDecreaseDuringTendering)||(bIsInFinalization && (lBalanceDue < 0.01)))
	{
        // TAR 228518 - clear security 
        ra.OnNoRAPDataNeeded();
        // TAR 228518
		FinalizationWtDecrease();
		return createAnchorState();
	}

	else if (getAnchorState() == BES_START) // TAR 257709
	{
		AttractWtDecrease(); 
		return createAnchorState();
	} // end TAR 257709
	else
	{
		return STATE(SecOperatorPWState)::OnWtDecrease();
	}
}

///////////////////////////////////////////
// EAS Motion Sensor detected motion ended
///////////////////////////////////////////
SMStateBase  *SMSecUnExpectedIncreaseBase::DMdf_EASNoMotion(void) 
{
	//218558 allow EAS to occur in unexpected increase  LPM103002
	if(!bIsInFinalization)
		m_bEASHappened = true;	// remember this for when the violation is corrected by the customer	
	// The sensor has been detected, remember it 
	SASendSCOTInput(PRODUCT_TAG_DEACTIVATED, io.csItemCode, 0, 0, 0, 0, 0);
	// wait here until the increase in corrected
	return STATE_NULL;
}

#endif //_CPPUNIT
//TAR379130/379131+
//******************************************************
VOID CALLBACK SMSecUnExpectedIncreaseBase::ShowScreenTimedOut ( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
#ifndef _CPPUNIT
	trace(L6, _T("+SMSecUnExpectedIncreaseBase::ShowScreenTimedOut "));
	m_pUnExpectedIncrease->HandleShowScreenTimedOut();
	trace(L6, _T("-SSMSecUnExpectedIncreaseBase::ShowScreenTimedOutt ")); 
#endif //_CPPUNIT
}
#ifndef _CPPUNIT

void SMSecUnExpectedIncreaseBase::HandleShowScreenTimedOut()
{
	trace(L6, _T("+SMSecUnExpectedIncreaseBase::HandleShowScreenTimedOut"));
	KillTimer(hw, SHOWSCREEN);
	
	// tar 395320 +
	if (dm.fStateTakeawayBelt && (co.getTimeOutDisplayIncreaseMisMatchScreenTimer() !=0 ))
	{
		trace(L6, _T("DisplayIncreaseMisMatchScreenTimer!=0, Delayed display of screen at TAB"));
		ps.SetCMLeadthruText(LTT_BELT_ATTENTION);
		ps.Message12(MSG_BELT_UNEXPECTED_ITEM, true);
	}//tar 395320 -

	if (!co.fOperationsSuppressAudioInUnexpectedIncrease)
	{
	   STATE(SecOperatorPWState)::DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_UNEXPECTEDITEMONBELT));	// play wave here so it doesn't replay on the key pad screen
	}
	trace(L6, _T("+SMSecUnExpectedIncreaseBase::HandleShowScreenTimedOut, saySecurity"));
	trace(L6, _T("Display the screen"));
	ps.ShowCMFrame(false);
	trace(L6, _T("-SMSecUnExpectedIncreaseBase::HandleShowScreenTimedOut"));
}
//TAR379130/379131-

//SR801+
//************************************************************************
// Function:   	OnTBNeedData
// Purpose:		Override function of SMStateBase::OnTBNeedData.
//              To handle dataneeded arriving during intervention.
// Parameters:	void
// Returns:		the state that is returned by SMStateBase::OnTBNeedData
// Calls:		SMStateBase::OnTBNeedData
// Side Effects:	
// Notes:		
//************************************************************************ 
SMStateBase *SMSecUnExpectedIncreaseBase::OnTBNeedData(void)
{
	SMStateBase * theState = STATE(SecOperatorPWState)::OnTBNeedData();
	m_NextState = theState->getAnchorState();
	return theState;
}
//SR801-
bool SMSecUnExpectedIncreaseBase::IgnoreTBMessageInSecurityState()
{
	if (nTBLastState == TB_READY)
		return false;
	else 
		return true; //default
}
#endif //CPPUNIT