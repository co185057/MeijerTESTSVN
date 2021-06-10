//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMBagAndEASBase.CPP
//
// TITLE: Class implementation for Bag Item and EAS Deactivate Theft Tag state
//
// AUTHOR: Tara Duncan, Thanhhieu Pham, Leila Parker
//
/////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMBagAndEAS.h"      
#include "SMScaleBroken.h"          // Reset scale
#include "SMSecUnExpectedIncrease.h"
#include "SMSecMisMatchWeight.h"
#include "SMSecUnexpectedDecrease.h"
#include "SMSecSkipBaggingThreshold.h"
#include "SMSecUnExpectedIncrease.h"
#include "SMInSAProgress.h"         // this state

#include "MPProcedures.h"

//USSF Start
#include "USSFProcedures.h"
#include "SMUSSFManager.h"
#include "USSFCommon.h"
//USSF End

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("BagAndEAS")
DLLEXPORT extern CSCOTDataEntry cDataEntry;
DLLEXPORT extern HWND hw;           // RJF 5/28/02 RFC 202655
DLLEXPORT extern MPProcedures mp;   // RJF 5/28/02 RFC 202655

IMPLEMENT_DYNCREATE(SMBagAndEASBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

static bool bScreenIsShown = false;
extern bool bAlreadyInSnB;
// RJF 5/28/02 RFC 202655
static CString csExpectedWts=EMPTY_STRING;
SMBagAndEASBase *SMBagAndEASBase::m_pBagAndEAS = NULL;//SR778

//////////////////////////////////////////
SMBagAndEASBase::SMBagAndEASBase()
: nCountForPeriodicBagItemAudio(0), csCurContext(EMPTY_STRING), m_bNeedToClearApproval(false),
m_bLoginProcessing(false)
{
  IMPLEMENT_TIMESTAMP
  m_pBagAndEAS = this;	//SR778
}

///////////////////////////////////////
SMStateBase  *SMBagAndEASBase::Initialize(void)
{
  //TAR273070
  bDoNotResetTimeOutOnDMEvent = true;  

  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("BagAndEAS") );

  //TAR 321110 - ensure CkStateStackAndReturn state will return here
  if (g_pDelay_WaitForApprovalStateChange != NULL)
  {
    setAnchorState(BES_BAGANDEAS);
  }

  //Move this code up here 415358
  if (!dm.fStateMotionSensor)
  {
	  m_bEASHappened = true;	// 219883 pretend motion sensor because it is not installed LPM121002
  } 

  //tar 238301
  SMStateBase *sReturnState = CkStateStackAndReturn();
  if (sReturnState != STATE_NULL)
	  return sReturnState;

  if (co.IsSecurityBypassOn())
  {
      m_bEASHappened = true;
//USSF START
	  USSFHOOK(_T("OnMatchedWt"), nextState = OnMatchedWt(), return nextState;);
	//return OnMatchedWt();
//USSF END
  }
  //
  ra.RequestEnableAssistMode(false); //Tar 238294


  //Update RAP window with SCOT state
  if (dm.fStateTakeawayBelt)
	UpdateStateName(ps.GetPSText(LTT_BELT_PLACEONBELT, SCOT_LANGUAGE_PRIMARY));
  else
	UpdateStateName(ps.GetPSText(MSG_BAGITEM, SCOT_LANGUAGE_PRIMARY));


  CString csContextName = ps.GetCurrentContext();

  // +SR730 RFC380486 - Iterate through the contexts found under the scotopts.xxx TransitionToScanAndBagContexts
  // option and look for the current context. If it is found, show a ScanAndBag context variant.
  int nListSize = co.csaTransitionContextList.GetSize();
  for(int i = 0; i < nListSize; i++)
  {
	  if (_tcsicmp(csContextName, co.csaTransitionContextList[i]) == 0)
	  {
		  if (co.fCMRewardInfo)
		  {
			  if (co.fOperationsDisplayQuickPickItems)
			  {
				  csContextName = _T("QuickPickItemsWithReward");
			  }
			  else
			  {
				  csContextName = _T("ScanAndBagWithReward");
			  }
		  }
		  else
		  {
			  if (co.fOperationsDisplayQuickPickItems)
			  {
				  csContextName = _T("QuickPickItems");
			  }
			  else
			  {
				  csContextName = _T("ScanAndBag");
			  }
		  }
		  ps.SetCMFrame(csContextName);
		  ps.ShowFrame();
		  break;
	  }
  }
  // -SR730 RFC380486

  // If Scan and Bag is already shown and the context name is ScanAndBag, change the tiltle and set the time out to co.getTimeOutDisplayBagItemScreenTimer()
  // Else, do not change the title and the timeout will be set to 1.
  if (true == g_bSnBScreenIsShown && ( (csContextName.Find(_T("ScanAndBag")) != -1) || (csContextName.Find(_T("QuickPickItems")) != -1))) //TAR 393811
  {
	  COleVariant v;
	  CString csLeadthru = EMPTY_STRING;
	  if (dm.fStateTakeawayBelt)
	  {
		csLeadthru = ps.GetPSText(LTT_BELT_PLACEONBELT); // title for TAB
	  }
	  else if(dm.EASDeactivatorPresent() && dm.fStateMotionSensor) 
	  {
		csLeadthru = ps.GetPSText(MSG_BAGITEM);			// instructions for EAS
	  }
	  else
	  {
		csLeadthru = ps.GetPSText(MSG_BAGITEM);			// instructions for Non EAS
	  }
	  v = ps.ConvertToEscapeSequence(csLeadthru, _T("\\n"), _T("\n"));
	  if (co.fCMRewardInfo)
	  {
		m_pAppPSX->SetConfigProperty(_T("LeadthruText"), _T("ScanAndBagWithReward"), UI::PROPERTYTEXTFORMAT, v);
		m_pAppPSX->SetConfigProperty(_T("LeadthruText"), _T("QuickPickItemsWithReward"), UI::PROPERTYTEXTFORMAT, v);
	  }
	  else
	  {
		m_pAppPSX->SetConfigProperty(_T("LeadthruText"), _T("ScanAndBag"), UI::PROPERTYTEXTFORMAT, v);
		m_pAppPSX->SetConfigProperty(_T("LeadthruText"), _T("QuickPickItems"), UI::PROPERTYTEXTFORMAT, v);
	  }
	  nTimeOut = co.getTimeOutDisplayBagItemScreenTimer();
  }
  else
	  nTimeOut = 1;

  dm.saveTriColorLightStatus();	//TAR209371 BZ

  SAWLDBScannerDisable();			// Disable the scanner

  // +TAR 355933
  if(dm.m_csCashRecyclerModel == _CASHDISPENSER_SAFEPAY && 
	 dm.CashRecyclerGetEnabled() == TRUE)
  {
	  // If the recycler is already enabled, then keep it enabled
	  // until we either leave this state or the skip bagging context
	  // is displayed.
      DMCashAcceptorEnable(co.csTenderDenominations);	
      DMCoinAcceptorEnable();							
  }
  // -TAR 355933

  // if Scan & Bag sreen has alreay been shown, then set bAlreadyInSnB = true  TAR205214 TK - Fix Screen blinking again
  if (g_bSnBScreenIsShown)
	bAlreadyInSnB = true;

  bScreenIsShown = false;
  csCurContext = _T("BagAndEAS");   // Tar 238987
  ps.SetCMFrame(_T("BagAndEAS"));

  //TAR 418670
  if(nTimeOut==0) //if timer is zero go to timedout immediately
  {
	  TimedOut();
  }

  
  if(co.fAutoRunOn && co.fAutoRunBypassBagging)
  {
      return PSButton1();
  }
  else
  {
	  setAnchorState(BES_BAGANDEAS);    // TAR294703: Set it to anchor state before timed out
      return STATE_NULL;                // stay in SMBagAndEASBase state
  }
}

///////////////////////////////////////
VOID CALLBACK SMBagAndEASBase::EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	mp.sCurr->Escalation2RAP();
}

void SMBagAndEASBase::Escalation2RAP()
{   // RJF 5/28/02 RFC 202655 - separate timer for escalation to RAP
    KillTimer(hw,RAP_ESCALATION);            // ensure timer is dead
    if(csExpectedWts != EMPTY_STRING)
    {
		 dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
             DM_TRICOLORLIGHT_BLINK_1HZ,
             DM_TRICOLORLIGHT_NORMALREQUEST);
	
		cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("BagItemTimerExpires"));
		m_View = DATANEEDED_BAGITEMTIMEREXPIRED;
		CString csSecInfo, csDesc;
	    csDesc.Format(ps.GetPSText(RA_DESCRIPTIONINFO, SCOT_LANGUAGE_PRIMARY), io.d.csDescription, csScaleUnits, _T("0.0"), csScaleUnits, csExpectedWts);
	    if (dm.fStateTakeawayBelt)
	    	csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_BELT_PLACEONBELT, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_TIMEREXPIRED, SCOT_LANGUAGE_PRIMARY));
	    else
	    	csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(MSG_BAGITEM, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_TIMEREXPIRED, SCOT_LANGUAGE_PRIMARY));

	    ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View); 
       	m_bNeedToClearApproval = true; //TAR245215
		ra.RequestEnableAssistMode(true); //Tar 238294
		//BZ RFC 203538 begin: After escalation to RAP, show a Store Login button
		if (co.fOperationsAllowStoreLoginBtnOnBagItemScreen) 		
			STATE(SecOperatorPWState)::Initialize();		//TAR 203538
		//RFC 203538 end	
    }
}

//+SR778
VOID CALLBACK SMBagAndEASBase::SkipBaggingButtonTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	m_pBagAndEAS->HandleSkipBaggingButtonTimedOut();

}
//***************************************************************************************//
// Function:	HandleSkipBaggingButtonTimeOut()				                 //
//                                                                                       //
// Purpose:		SR778 �VHandle EnableSkipBaggingButtonTimer after the timer expired.    //
//      		       Call KillTimer(,) to ensure SKIPBAG_ENABLE timer is dead	     //
//                  									                 //
//                                                                                       //
//                                                                                       //
// Parameters:		None.                                                            //
//                                                                                       //
// Returns:			None                                                             //
//***************************************************************************************//

void SMBagAndEASBase::HandleSkipBaggingButtonTimedOut()
{   
	trace(L6,_T("+SMBagAndEASBase::HandleSkipBaggingButtonTimedOut()"));
    KillTimer(hw,SKIPBAG_ENABLE);            // ensure timer is dead
	
	trace(L6,_T("EnableSkipBaggingButtonTimer Expires"));
	
	g_bSkipBaggingButtonTimerExpires = true;

	COleVariant v;
	v = VARIANT_TRUE;
	if (dm.fStateTakeawayBelt)
		ps.SetButtonText(_T("CMButton1Med"),BTT_BELT_SKIPBELT);     // skip belt button
	else
		ps.SetButtonText(_T("CMButton1Med"),BTT_SKIP_BAGGING);      // skip bagging button
	m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
	
    trace(L6,_T("-SMBagAndEASBase::HandleSkipBaggingButtonTimedOut()"));
}

//***************************************************************************************//
// Function:	StartTimerForSkipBaggingBtn()				                       //
//                                                                                       //
// Purpose:		SR778 �V After the SKIP_BAG_BTTN_ENABLE message is receive start        //
//      				     timer, EnableSkipBaggingButtonTimer configured in     //
//                  scotopts.xxx									     //
//                                                                                       //
//                                                                                       //
// Parameters:		None.                                                            //
//                                                                                       //
// Returns:			None                                                             //
//***************************************************************************************//

void SMBagAndEASBase::StartTimerForSkipBaggingBtn(void)
{
	trace(L6,_T("+SMBagAndEASBase::StartTimerForSkipBaggingBtn()"));

	int SkipBaggingBtnEnableTimer = co.getTimeOutEnableSkipBaggingButtonTimer();
	SetTimer(hw, SKIPBAG_ENABLE, SkipBaggingBtnEnableTimer, &SkipBaggingButtonTimedOut);
	
	trace(L6,_T("Start EnableSkipBaggingButtonTimer=%d"), SkipBaggingBtnEnableTimer);

	trace(L6,_T("-SMBagAndEASBase::StartTimerForSkipBaggingBtn()"));
}
//-SR778

//////////////////////////////////////
SMStateBase *SMBagAndEASBase::TimedOut(void) // timeout
{
	setAnchorState(BES_BAGANDEAS);
	if (nCountForPeriodicBagItemAudio  >= 1)	// this will occur the 3rd time thru the nth time until cleared or press btn 1
	{
        //RFC 210367 FFR: Add Skip Belt Wave
        if (dm.fStateTakeawayBelt)
		{
			if (co.fOperationsAllowSkipBaggingBtn)
				DMSaySecurity( SEC_NONE, ps.GetSoundName(SKIPBELTFORLARGE));
			else
				DMSaySecurity( SEC_NONE, ps.GetSoundName(PLACEITEMONBELT));
		}
        else if (dm.EASDeactivatorPresent() && dm.fStateMotionSensor)
			DMSaySecurity( SEC_NONE, ps.GetSoundName(BAGITEMSENSORMATIC));
		else if (co.fOperationsAllowSkipBaggingBtn)
			DMSaySecurity( SEC_NONE, ps.GetSoundName(SKIPBAGFORLARGE));//RFC 210367 FFR: Add Skip Bag Wave
		else
			DMSaySecurity( SEC_NONE, ps.GetSoundName(BAGITEM));

		nTimeOut = co.getTimeOutPeriodicBagItemAudio();
		return STATE_NULL;                // stay in DeactivateTheftTagBase state
	}
	else if (bScreenIsShown)		// this only occurs the second timeout, after the screen is shown
	{
		trace(L7, _T("+ra.OnTransactionState, Itemization State"));
		ra.OnSetTransactionState(RA_STATE_BAG_ITEM);
		trace(L7, _T("-ra.OnTransactionState"));
		// don't play the wave file until the 'initial' timer has expired
        //RFC 210367 FFR: Add Skip Belt Wave
        if (dm.fStateTakeawayBelt)
		{
			if (co.fOperationsAllowSkipBaggingBtn)
				DMSaySecurity( SEC_NONE, ps.GetSoundName(SKIPBELTFORLARGE));
			else
				DMSaySecurity( SEC_NONE, ps.GetSoundName(PLACEITEMONBELT));
		}
		else if (dm.EASDeactivatorPresent() && dm.fStateMotionSensor)
			DMSaySecurity( SEC_NONE, ps.GetSoundName(BAGITEMSENSORMATIC));
		else if (co.fOperationsAllowSkipBaggingBtn)
			DMSaySecurity( SEC_NONE, ps.GetSoundName(SKIPBAGFORLARGE));//RFC 210367 FFR: Add Skip Bag Wave
		else
			DMSaySecurity( SEC_NONE, ps.GetSoundName(BAGITEM));

		nCountForPeriodicBagItemAudio = 1;		// set to 1 so the 1st else will be hit after the next timeout
		nTimeOut = co.getTimeOutPeriodicBagItemAudio();
		return STATE_NULL;                // stay in DeactivateTheftTagBase state
	}
	else
	{	// this only occurs the first time thru (after the 1 second timeout in initialize)
		bAlreadyInSnB = false;

		ps.SetCMFrame(_T("BagAndEAS"));

        CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMBagAndEASBase::TimedOut"));

		if (dm.fStateTakeawayBelt)
        {
            ps.SetCMLeadthruText(LTT_BELT_PLACEONBELT); // title for TAB
            ps.Message12(MSG_BELT_PLACE_ITEM_ON_BELT, true); // instructions for TAB
        }
		else if(dm.EASDeactivatorPresent() && dm.fStateMotionSensor) 
		{
			ps.SetCMLeadthruText(MSG_BAGITEM);			// instructions for EAS
			ps.Message12(LTT_BAGGING_AREA,true);
		}
		else
		{
			ps.SetCMLeadthruText(MSG_BAGITEM);			// instructions for Non EAS
			ps.Message12(MSG_PLACEITEM_IN_BAGGING, true);
		}
		COleVariant v;
		if (co.fOperationsAllowSkipBaggingBtn &&  co.getTimeOutEnableSkipBaggingButtonTimer() == 0 || 
		(CString(_T("SMProduceFavorites")) == this->csLastNonHelpStateName ) && co.fOperationsAllowMultiPick &&
		co.fOperationsAllowSkipBaggingBtn || co.getTimeOutEnableSkipBaggingButtonTimer()> 0 && co.fOperationsAllowSkipBaggingBtn && 
		g_bSkipBaggingButtonTimerExpires) //RFC 200652
		{
			v = VARIANT_TRUE;
			if (dm.fStateTakeawayBelt)
				ps.SetButtonText(_T("CMButton1Med"),BTT_BELT_SKIPBELT);     // skip belt button
			else
				ps.SetButtonText(_T("CMButton1Med"),BTT_SKIP_BAGGING);      // skip bagging button
		}
		else
		{
			v = VARIANT_FALSE;
		}
		// depending on the skipbagging option make the button visible or non-visible
		m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
		// hide the store mode button 
		v = VARIANT_FALSE;
		m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, v);

		ps.ShowCMTBText(csTBMessage2Scot);	//thp
		nTimeOut = co.getTimeOutInitialBagItemAudio();
		if (nTimeOut==0)	//T.D 05/01/02 +200565 
		{
			trace(L6, _T("InitialBagItemAudio is immediate"));
			//RFC 210367 FFR: Add Skip Belt Wave
			if (dm.fStateTakeawayBelt)
			{
				if (co.fOperationsAllowSkipBaggingBtn)
					DMSaySecurity( SEC_NONE, ps.GetSoundName(SKIPBELTFORLARGE));
				else
					DMSaySecurity( SEC_NONE, ps.GetSoundName(PLACEITEMONBELT));
			}
			else if (dm.EASDeactivatorPresent() && dm.fStateMotionSensor)
				DMSaySecurity( SEC_NONE, ps.GetSoundName(BAGITEMSENSORMATIC));
			else if (co.fOperationsAllowSkipBaggingBtn)
				DMSaySecurity( SEC_NONE, ps.GetSoundName(SKIPBAGFORLARGE));//RFC 210367 FFR: Add Skip Bag Wave
			else
				DMSaySecurity( SEC_NONE, ps.GetSoundName(BAGITEM));

			nCountForPeriodicBagItemAudio = 1;// set to 1 so the 1st else will be hit after the next timeout
			nTimeOut = co.getTimeOutPeriodicBagItemAudio();
		}	//T.D 05/01/02 -200565 
		bScreenIsShown = true;
        // RJF 5/28/02 RFC 202655 - separate timer for escalation to RAP
        csExpectedWts = GetExpectedWts();  // save the expected weights for timeout function
        nEscalationRAPTimeOut = co.getTimeOutBagItemEscalation();    // time is milliseconds
        if (nEscalationRAPTimeOut == 0)  // if 0, immediate escalation
        {
		    trace(L6, _T("BagItemEscalation is immediate"));
            SAEnterSecurityCondition();
	        cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("BagItemTimerExpires"));
		    m_View = DATANEEDED_BAGITEMTIMEREXPIRED;
		    CString csSecInfo, csDesc;
	        csDesc.Format(ps.GetPSText(RA_DESCRIPTIONINFO, SCOT_LANGUAGE_PRIMARY), io.d.csDescription, csScaleUnits, _T("0.0"), csScaleUnits, csExpectedWts);
	        if (dm.fStateTakeawayBelt)
	        	csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_BELT_PLACEONBELT, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_TIMEREXPIRED, SCOT_LANGUAGE_PRIMARY));
	        else
	        	csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(MSG_BAGITEM, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_TIMEREXPIRED, SCOT_LANGUAGE_PRIMARY));

			ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
			ra.RequestEnableAssistMode(true); ra.RequestEnableAssistMode(true); 
			//RFC 210367 FFR: Add Skip Belt Wave
			if (dm.fStateTakeawayBelt)
			{
				if (co.fOperationsAllowSkipBaggingBtn)
					DMSaySecurity( SEC_NONE, ps.GetSoundName(SKIPBELTFORLARGE));
				else
					DMSaySecurity( SEC_NONE, ps.GetSoundName(PLACEITEMONBELT));
			}
	        else if (dm.EASDeactivatorPresent() && dm.fStateMotionSensor)
				DMSaySecurity( SEC_NONE, ps.GetSoundName(BAGITEMSENSORMATIC));
			else if (co.fOperationsAllowSkipBaggingBtn)
				DMSaySecurity( SEC_NONE, ps.GetSoundName(SKIPBAGFORLARGE));//RFC 210367 FFR: Add Skip Bag Wave
			else
				DMSaySecurity( SEC_NONE, ps.GetSoundName(BAGITEM));

			//BZ 06/06/02 RFC 203538 begin: After escalation to RAP, show a Store Login button
			if (co.fOperationsAllowStoreLoginBtnOnBagItemScreen) 
				STATE(SecOperatorPWState)::Initialize();		
        }
        else
            SetTimer(hw, RAP_ESCALATION, nEscalationRAPTimeOut, &EscalationTimedOut);
		ps.ShowCMFrame(false);

		// +TAR 355933
		// If displaying context, then disable the recycler.
		if(dm.m_csCashRecyclerModel == _CASHDISPENSER_SAFEPAY)
		{
			CString csDeposit;
			dm.CashRecyclerStopDeposit(csDeposit);				
		}
		// -TAR 355933

		return STATE_NULL;                // stay in Bag and EAS state
	}
}

///////////////////////////////////
void SMBagAndEASBase::UnInitialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("BagAndEAS") );
  bDoNotResetTimeOutOnDMEvent = false; //Tar324574
  ra.RequestEnableAssistMode(true); //Tar 238294
 
  KillTimer(hw,RAP_ESCALATION);            // rfc 202655 ensure timer is dead

  csExpectedWts = EMPTY_STRING;
  if((nCountForPeriodicBagItemAudio >= 1) || m_bNeedToClearApproval ) //TAR277066
  {
	SAClearSecurityCondition();
	m_bNeedToClearApproval = false; //if ItitialBagItemAudio>BagItemEscalationMiliSecs, RAP is notified before 
	                                //nCountForPeriodicBagItemAudio gets increased. We could leave security not 
	                                //cleared on RAP by pressing skip bagging button. 277066
  }



	// if there is not a EAS motion sensor on the FastLane
	if (!dm.fStateTakeawayBelt) // if take away want to set this to true
	{
	  // then set the re-enable scanner flag to true
	  trace(L2, _T("::UnInitialize - bEASReEnableScanner is set to true."));
	  bEASReEnableScanner = true;
	}
	nCountForPeriodicBagItemAudio = 0;
	// if going to transport item, don't want to interrupt the wave file TAR199671 LPM032002
	if (!dm.fStateTakeawayBelt) 
		DMSaySecurity( SEC_NONE, _T("blank.wav") );			//to interrupt the wave file if it's playing 153796

  // else don't change anything cause we coming back here coming back TAR199671 LPM032002
  bAlreadyInSnB = false;//TAR 205814

  // +TAR 355933
  if(dm.m_csCashRecyclerModel == _CASHDISPENSER_SAFEPAY)
  {
	DMCashAcceptorDisable();
	DMCoinAcceptorDisable();
  }
  // -TAR 355933
}

//////////////////////////////////////
SMStateBase  *SMBagAndEASBase::PSButton1(void) // unbag-able item
{

  KillTimer(hw,RAP_ESCALATION);				// rfc 202655 ensure timer is dead
  DMSaySecurity( SEC_NONE, _T("blank.wav") );	// to interrupt the wave file if it's playing 153796

  if( co.IsSecurityBypassOn() ) // if by chance that it go to skip bagging with securitybypasson
  {
	  trace(L6, _T("Returning to ScanAndBag, IsSecurityBypassOn=%d"), co.IsSecurityBypassOn());
	  return setAndCreateAnchorState(BES_SCANANDBAG);
  }

  // true if zero wt	TAR199154
  //+TAR 384906
  if( fInValidOperatorPassword )
  {
	return STATE(SecOperatorPWState)::PSButton1();
  }
  //-TAR 384906

  
  // RFC 252473
  if ( co.fOperationsShowSkipBaggingOnEReceipt )
  {
	ShowSkipBaggingOnEReceipt();
  } // end RFC 252473
  											// true if zero wt	TAR199154
  g_bMatchedWtWaitingOnEAS = false;

  // If timer is expired already, tell RAP no data needed and press Skip Bagging button, 
  // then clear the timer expire event.
  if((nCountForPeriodicBagItemAudio >= 1) || m_bNeedToClearApproval ) //TAR245215
  {
	SAClearSecurityCondition();
	m_bNeedToClearApproval = false; //if ItitialBagItemAudio>BagItemEscalationMiliSecs, RAP is notified before 
	                                //nCountForPeriodicBagItemAudio gets increased. We could leave security not 
	                                //cleared on RAP by pressing skip bagging button. 245215
  }

  trace(L2, _T("PSButton1 - bEASReEnableScanner is set to true."));
  bEASReEnableScanner = true;
  g_bWtExpected = false;	//We should never expect wt if button is pressed
  g_lNumberOfWLDBEntriesThisItem = -1;//TAR 196992

  //tar 407200
  trace(L6, _T("csLastNonHelpStateName: %s, fOperationsAllowMultiPick = %d, fVoid = %d, bMultiPickSkipBaggingAllowed = %d"),
      csLastNonHelpStateName, co.fOperationsAllowMultiPick, io.d.fVoid, g_bMultiPickSkipBaggingAllowed);

  if( ( CString(_T("SMProduceFavorites")) == this->csLastNonHelpStateName ) && co.fOperationsAllowMultiPick && (io.d.fVoid || g_bMultiPickSkipBaggingAllowed)) // TAR 398289
  {
      g_bMultiPickSkipBaggingAllowed = false;// TAR 398289
	  return setAndCreateAnchorState(BES_SCANANDBAG);
  }
  //tar 407200

  trace(L6, _T("fCoupon = %d"), io.d.fCoupon);

  //TAR 418258
  if((ps.GetPicklistItemsSold() > 0) && io.d.fCoupon)
  {
	  ps.SetPicklistItemsSold(0);
      g_bMultiPickSkipBaggingAllowed = false;
	  return setAndCreateAnchorState(BES_SCANANDBAG);
  }

  // if item has a zero wt,  shouldn't ever reach threshold
   if( g_bZeroWtExpected )	  
		return setAndCreateAnchorState(BES_SCANANDBAG);

  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_SKIP_BAGGING);
  _bstr_t bstrParmList((const _TCHAR *)parmList);
  BSTR bstrResult;
  bstrResult = NULL;
  SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
  SysFreeString(bstrResult);

  //return setAndCreateAnchorState(BES_SCANANDBAG); should should not return to S&B Tar 337080
  trace(L6, _T("SMBagAndEASBase::PSButton1"));
  return STATE_NULL;
}

///////////////////////////////////////
SMStateBase *SMBagAndEASBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
	trace(L2, _T("+SMBagAndEASBase::OnRAPClearSecurityBtn"));
	DMSaySecurity( SEC_NONE, _T("blank.wav") );			//to interrupt the wave file if it's playing 
    // RFC 252473
    if ( co.fOperationsShowSkipBaggingOnEReceipt )
	{
		ShowSkipBaggingOnEReceipt();
	} // end RFC 252473

	CKeyValueParmList parmList;
	if(m_View == DATANEEDED_BAGITEMTIMEREXPIRED)
	{
	    setAnchorState(getNextStateForGoodItem());
	    nCountForPeriodicBagItemAudio = 0;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_ITEM_EXPIRED);
	}
	else
	{
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_APPROVE_WT);
	}

	// resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
	SAClearSecurityCondition();	
	m_bNeedToClearApproval = false; //TAR245215

	//218558 set scanner flag to re-enabled only in state that isn't handling EAS automatically
	trace(L2, _T("bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;

	g_bMatchedWtWaitingOnEAS = false;
	g_bWtExpected = false;	//We should never expect wt if button is pressed on RAP

    _bstr_t bstrParmList((const _TCHAR *)parmList);
	BSTR bstrResult;
    bstrResult = NULL;
    SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
	SysFreeString(bstrResult);

	g_lNumberOfWLDBEntriesThisItem = -1;	// TAR 212831 Reset this value after approve from RAP
	trace(L2, _T("-SMBagAndEASBase::OnRAPClearSecurityBtn"));

  //tar 407200
  trace(L6, _T("csLastNonHelpStateName: %s, fOperationsAllowMultiPick = %d, fVoid = %d, bMultiPickSkipBaggingAllowed = %d"),
      csLastNonHelpStateName, co.fOperationsAllowMultiPick, io.d.fVoid, g_bMultiPickSkipBaggingAllowed);

  //Tar 418598
  if( ( CString(_T("SMProduceFavorites")) == this->csLastNonHelpStateName ) && co.fOperationsAllowMultiPick && (io.d.fVoid || g_bMultiPickSkipBaggingAllowed)) // TAR 398289
  {
      g_bMultiPickSkipBaggingAllowed = false;// TAR 398289
	  return setAndCreateAnchorState(BES_SCANANDBAG);
  }
  //tar 407200

  trace(L6, _T("fCoupon = %d"), io.d.fCoupon);

  //TAR 418258
  if((ps.GetPicklistItemsSold() > 0) && io.d.fCoupon)
  {
	  ps.SetPicklistItemsSold(0);
      g_bMultiPickSkipBaggingAllowed = false;
	  return setAndCreateAnchorState(BES_SCANANDBAG);
  }
  //Tar 418598

	
	//TAR376425 wait until SecurityManager to send ItemOK
    	trace(L2, _T("Return STATE_NULL to wait on SecurityManager"));
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMBagAndEASBase::OnWtIncreaseNotAllowed()	 
{
	DMSaySecurity( SEC_NONE, _T("blank.wav") );

	trace(L6, _T("csLastNonHelpStateName: %s, fOperationsAllowMultiPick = %d"), csLastNonHelpStateName, co.fOperationsAllowMultiPick);

	if( ( CString(_T("SMProduceFavorites")) == this->csLastNonHelpStateName ) && co.fOperationsAllowMultiPick && (secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_UNEX_INCREASE))
	{
		OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR);
		return setAndCreateAnchorState(BES_SCANANDBAG);
	}

  trace(L6, _T("fCoupon = %d"), io.d.fCoupon);

  //TAR 418258
  if((ps.GetPicklistItemsSold() > 0) && io.d.fCoupon)
  {
	  ps.SetPicklistItemsSold(0);
      g_bMultiPickSkipBaggingAllowed = false;
	  return setAndCreateAnchorState(BES_SCANANDBAG);
  }


	// 218558 EAS motion sensor received in the wrong order must behave the same as 
	// other violations. Therefore removing the following code LPM103002
	// Stay in the Bag Item screen if have matched wt but not EAS motion
	//if( (!dm.fStateTakeawayBelt) && (g_bMatchedWtWaitingOnEAS && dm.EASDeactivatorPresent() ) )
	//	return STATE_NULL;

	//TAR221282 chu 112702
	//for TAB, if we are at this state that means we are waiting for good weight, so always return to
	//Mismatch state. We added "&& !dm.fStateTakeawayBelt" in the following line:
	//if (g_bMatchedWtWaitingOnEAS)  
	if (g_bMatchedWtWaitingOnEAS && !dm.fStateTakeawayBelt) //TAR221282 chu 112702
	{
		CREATE_AND_DISPENSE(SecUnExpectedIncrease)(BES_BAGANDEAS, getNextStateForGoodItem());
	}

	if ( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_UNEX_INCREASE )
	{
		CREATE_AND_DISPENSE(SecUnExpectedIncrease)(BES_BAGANDEAS, getNextStateForGoodItem());
	}

	//unmatched weights while on the bag and EAS mean a mismatched wt so return here, else if cleared, return to s&b
	CREATE_AND_DISPENSE(SecMisMatchWeight)(BES_BAGANDEAS, getNextStateForGoodItem());
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMBagAndEASBase::OnWtDecrease()
{
	// don't need to set the anchor state, it is passed as a param in the CREATE_AND_DISPENXE below LPM092502
	DMSaySecurity( SEC_NONE, _T("blank.wav") );

	// 218558 EAS motion sensor received in the wrong order must behave the same as 
	// other violations. Therefore removing the following code LPM103002
	// Stay in the Bag Item screen if having a matched wt but not breaking the EAS
	//if(g_bMatchedWtWaitingOnEAS && dm.EASDeactivatorPresent() )
	//	return STATE_NULL;
	//unexpected increases while on the bag and EAS not allowed, return here if customer fixes, else if cleared, return to s&b
	CREATE_AND_DISPENSE(SecUnexpectedDecrease)(BES_BAGANDEAS, getNextStateForGoodItem());
}

//229084 created a Base function for getNextStateForGoodItem LPM022503

///////////////////////////////////////////
// EAS Motion Sensor detected motion ended
///////////////////////////////////////////
SMStateBase  *SMBagAndEASBase::DMdf_EASNoMotion(void) 
{
	SASendSCOTInput(PRODUCT_TAG_DEACTIVATED, io.csItemCode, 0, 0, 0, 0, 0);
	// If we have a matched wt or back to LGW
	m_bEASHappened = true;

	if (g_bMatchedWtWaitingOnEAS)
	{
		g_bMatchedWtWaitingOnEAS = false;
//USSF START
		USSFHOOK(_T("OnMatchedWt"), nextState = OnMatchedWt(), return nextState;);
		//return OnMatchedWt();
//USSF END
	}

	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMBagAndEASBase::OnBackToLGW(void)
{
	DMSaySecurity( SEC_NONE, _T("blank.wav") );
	trace(L2, _T("SMBagAndEASBase::OnBackToLGW, determining if navigation needed"));

	//If there is WaitForApproval state waiting, clear it
	if (g_pDelay_WaitForApprovalStateChange != NULL)
	{
		CString csStateName = g_pDelay_WaitForApprovalStateChange->GetRuntimeClass()->m_lpszClassName;
		delete g_pDelay_WaitForApprovalStateChange;
		g_pDelay_WaitForApprovalStateChange = NULL;
		trace(L6,_T("Delete the state %s saved in g_pDelay_WaitForApprovalStateChange"),csStateName);
	}
	
	// If EAS is already detected and there is a match wt
	if(	m_bEASHappened && g_bMatchedWtWaitingOnEAS)
	{
		g_bMatchedWtWaitingOnEAS = false;
//USSF START
		USSFHOOK(_T("OnMatchedWt"), nextState = OnMatchedWt(), return nextState;);
		//return OnMatchedWt();
//USSF END
	}
	//TAR 362028 begin - BRC
	else if (g_bOnItemOK) //for TAB
	{
//USSF START
		USSFHOOK(_T("OnMatchedWt"), nextState = OnMatchedWt(), return nextState;);
		//return OnMatchedWt();
//USSF END
	}
	//TAR 362028 end
	
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMBagAndEASBase::OnMatchedWt()
{
	trace(L2, _T("SMBagAndEASBase::OnMatchedWt, returning to anchor state"));
	return SMStateBase::OnMatchedWt();
}
//BZ 06/06/02 RFC 203538 Begin
///////////////////////////////////////
SMStateBase  *SMBagAndEASBase::PSButtonGoBack(void) // Store Login button
{
	m_bLoginProcessing = true;  // TAR 274406
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
					if (co.fStateDualLanguage)
						ps.SetLanguage(m_customerLanguage);  //Set back to customerLanuguage for dual language
					showScreen(false);
					return STATE_NULL;  
				}
		}

    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	promptForOperator();
    return STATE_NULL;	
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMBagAndEASBase::stateAfterPasswordVerify()
{
	trace(L2, _T("+SMBagAndEASBase::stateAfterPasswordVerify"));
	trace(L2, _T("-SMBagAndEASBase::stateAfterPasswordVerify"));
	//do same thing that OnRAPClearSecurityBtn() does
	return OnRAPClearSecurityBtn(_T("Lane"));
}

////////////////////////////////////////////////////////
void SMBagAndEASBase::showScreen(bool passwordVerified)
{	
	COleVariant v;
	if (passwordVerified)
		return;
	
	if (inputMode == PROMPTING_FOR_OPERATOR)
		{
			
			STATE(SecOperatorPWState)::ShowEnterID();  //SR93 Biometrics			
			ps.SetCMLeadthruText( TXT_ENTER_ID );
			ps.AllowInput(MAX_DIGITS_OPERATOR,false,false);
			ps.ShowCMFrame(false);
			return;
		}
		else
		{  
			// TAR 200470 - this problem was discovered as a side effect of the show being slow
			ps.SetCMFrame(_T("BagAndEAS"));
			// the wave and background changes depending on if Belt is installed or not
            if (dm.fStateTakeawayBelt)
            	ps.SetCMBackground(BMP_PLACEITEMONBELT);
            else if (dm.EASDeactivatorPresent() && dm.fStateMotionSensor) 
				ps.SetCMBackground(BMP_BAGITEMANDEAS);
			else
				ps.SetCMBackground(BMP_BAGITEM);			// background bitmap for Non EAS
			// BZ 06/02/02 RFC 203538: Show the Store Login button 			
//			ps.CMButton(_T("ButtonStoreLogIn"),BTT_STORELOGIN, true);
			v = VARIANT_TRUE;
		    m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, v);
		}

	m_bLoginProcessing = false;  // TAR 274406

	if (co.fOperationsAllowSkipBaggingBtn &&  co.getTimeOutEnableSkipBaggingButtonTimer() == 0 || 
		(CString(_T("SMProduceFavorites")) == this->csLastNonHelpStateName ) && co.fOperationsAllowMultiPick &&
		co.fOperationsAllowSkipBaggingBtn || co.getTimeOutEnableSkipBaggingButtonTimer()> 0 && co.fOperationsAllowSkipBaggingBtn && 
		g_bSkipBaggingButtonTimerExpires) //BZ RFC 203538
	{
		v = VARIANT_TRUE;
		if (dm.fStateTakeawayBelt)
			ps.SetButtonText(_T("CMButton1Med"),BTT_BELT_SKIPBELT);     // skip belt button
		else
			ps.SetButtonText(_T("CMButton1Med"),BTT_SKIP_BAGGING);      // skip bagging button
	}
	else
	v = VARIANT_FALSE;
    m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);

    CPSXRedrawLock  csRedrawLock(_T("Display"), _T("SMBagAndEASBase::ShowScreen"));
	if (dm.fStateTakeawayBelt)
    {
        ps.SetCMLeadthruText(LTT_BELT_PLACEONBELT); // title for TAB
        ps.Message12(MSG_BELT_PLACE_ITEM_ON_BELT, true); // instructions for TAB
    }
	else if(dm.EASDeactivatorPresent() && dm.fStateMotionSensor) 
	{
		ps.SetCMLeadthruText(MSG_BAGITEM);			// instructions for EAS
		ps.Message12(LTT_BAGGING_AREA,true);
	}
	else
	{
		ps.SetCMLeadthruText(MSG_BAGITEM);			// instructions for Non EAS
		ps.Message12(MSG_PLACEITEM_IN_BAGGING, true);
	}

	ps.ShowCMTBText(csTBMessage2Scot);	//thp

    DisplayAuxScreen(AUX_SCREEN_DEFAULT, true); //tar 193987
	ps.ShowCMFrame(false);
}

//BZ 06/06/02 RFC 203538 End

// RFC 252473
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMBagAndEASBase::ShowSkipBaggingOnEReceipt()
//
// Purpose:		Displays the "Item not bagged" message on the e-receipt on
//				Lane and RAP when "Skip bagging" button is pressed or approved
//				from RAP or Lane
//
// Parameters:	none
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
void SMBagAndEASBase::ShowSkipBaggingOnEReceipt()
{
	trace(L6, _T("+SMBagAndEASBase::ShowSkipBaggingOnEReceipt"));
	CString csMsgItemEntryId, csMsgItemEntryRapId,csReceiptSrc, csDesc, csDescPrimary;
	csReceiptSrc = _T("CMSMReceipt");
	CString csItemEntryId = ps.GetSelection(_T("CMReceipt")); // Get the item's entry id
	csMsgItemEntryId = csItemEntryId + _T("ShowSkipBagging");
    csMsgItemEntryRapId = csItemEntryId + _T("ShowSkipBaggingRap");
	m_pAppPSX->CreateReceiptItem(csReceiptSrc, csMsgItemEntryId, csItemEntryId);
	COleVariant	vValue;
	// TAR 253984 - different message for belt
	if (dm.fStateTakeawayBelt)
	{
		csDesc = ps.GetPSText(MSG_ITEMNOTONBELT, ps.m_languageUsed);
        csDescPrimary = ps.GetPSText(MSG_ITEMNOTONBELT, SCOT_LANGUAGE_PRIMARY);
	}
	else
	{
		csDesc = ps.GetPSText(MSG_ITEMNOTBAGGED, ps.m_languageUsed);
		csDescPrimary = ps.GetPSText(MSG_ITEMNOTBAGGED, SCOT_LANGUAGE_PRIMARY);
	}
	vValue= csDesc;
	m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csMsgItemEntryId, _T("SUB_ITEM_DESCRIPTION"), vValue);
    m_pAppPSX->CreateReceiptItem(csReceiptSrc, csMsgItemEntryRapId, csItemEntryId);
 //   vValue= csDescPrimary;
//	m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csMsgItemEntryRapId, _T("ITEM_RAP_ONLY_HILITE"), vValue);
//	m_pAppPSX->UpdateReceiptControls(csReceiptSrc); 
	//CString csText;
	// TAR 253984 - different message for belt
	//if (dm.fStateTakeawayBelt)
	//{
//		csText = ps.GetPSText(MSG_ITEMNOTONBELT, SCOT_LANGUAGE_PRIMARY);
	//}
	//else
	//{
	//	csText = ps.GetPSText(MSG_ITEMNOTBAGGED, SCOT_LANGUAGE_PRIMARY);
	//}
	trace(L7, _T("+ra.OnNormalItem, Message=%s"), csDescPrimary);
	//csText.TrimLeft();
	//Tar336741 Display Item not bagged on Rap Quad view
	//ra.OnNormalItem(RAP_RECEIPT_LINE_HIGHLIGHT_SUBITEM + csDescPrimary);
	ra.OnNormalItem(RAP_RECEIPT_LINE_HIGHLIGHT + csDescPrimary);
	//End Tar 336741
	trace(L7, _T("-ra.OnNormalItem"));
	trace(L6, _T("-SMBagAndEASBase::ShowSkipBaggingOnEReceipt"));
}
// end RFC 252473

///////////////////////////////////////////////////////////
SMStateBase * SMBagAndEASBase::DMScanner(void)
{
   if (m_bLoginProcessing)
   {
		return STATE(SecOperatorPWState)::DMScanner(); 
   }
   else
   {
      trace(L2, _T("Ignoring scan arriving in BagAndEAS state"));
      return STATE_NULL;
   }
}

//+TAR 372391
//Fix from Patch E5
//This event is only received when the item is off the first belt but SM hasn't received
//good weight from SmartScale.

///////////////////////////////////////////////////////////
SMStateBase *SMBagAndEASBase::DMTakeawayReadyForItem()
{

  KillTimer(hw,RAP_ESCALATION);				// rfc 202655 ensure timer is dead
  DMSaySecurity( SEC_NONE, _T("blank.wav") );	// to interrupt the wave file if it's playing 153796
										// true if zero wt	TAR199154
  g_bMatchedWtWaitingOnEAS = false;
  g_bReadyForItem = true; //TAR462489  

  // If timer is expired already, tell RAP no data needed and press Skip Bagging button, 
  // then clear the timer expire event.
  if((nCountForPeriodicBagItemAudio >= 1) || m_bNeedToClearApproval ) //TAR245215
  {
	SAClearSecurityCondition();
	m_bNeedToClearApproval = false; //if ItitialBagItemAudio>BagItemEscalationMiliSecs, RAP is notified before 
	                                //nCountForPeriodicBagItemAudio gets increased. We could leave security not 
	                                //cleared on RAP by pressing skip bagging button. 245215
  }

  trace(L2, _T("DMTakeawayReadyForItem - bEASReEnableScanner is set to true."));
  bEASReEnableScanner = true;
  g_bWtExpected = false;	//We should never expect wt if button is pressed
  g_lNumberOfWLDBEntriesThisItem = -1;//TAR 196992

  //tell SM to go to ItemEntry state from WaitScale state  
  SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);

  return STATE_NULL; //TAR387587 - wait for OnItemOK from SM
}
//-TAR 372391

//TAR 412874

bool SMBagAndEASBase::storeMode()
{
	if(co.IsSecurityBypassOn())
	{
		return true;
	}
	else
	{
		return false;
	}
}
//-412874
