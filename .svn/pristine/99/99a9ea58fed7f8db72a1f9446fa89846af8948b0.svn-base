//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCardMisReadBase.cpp
//
// TITLE: Class implementation for mis-read card State
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "DMProcedures.h"           // DM procedures

#include "SMState.h"                // Base state
#include "SMCardMisRead.h"      
#include "SMTakeCard.h"             // Take card state
#include "SMContinueTrans.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("CardMisReadBase")

DEFINE_TIMESTAMP

//IMPLEMENT_STATE(CardMisRead)		//TAR119740	LPM100199  must return to system msg after call for help

IMPLEMENT_DYNCREATE(SMCardMisReadBase, CObject)// MFC Runtime class/object information

///////////////////////////////////////////////csMsgToDisplay//
SMCardMisReadBase::SMCardMisReadBase(LPTSTR csMsgToDisplay, PSTEXT Title)
: charDisplayMsg(csMsgToDisplay),pDisplayMsg(PS_NOTEXT),
	fTimedOutOnce(false), nTitle(Title), csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
// code comes thru here 1st
SMCardMisReadBase::SMCardMisReadBase()
: csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
  nTitle = TTL_CARD_DECLINED;
  charDisplayMsg = _T("");
  pDisplayMsg = PS_NOTEXT;
  fTimedOutOnce = false;
}


//////////////////////////////////////////////////
// doesn't appear to be used anymore
SMCardMisReadBase::SMCardMisReadBase(const PSTEXT pMsgToDisplay, PSTEXT Title)
: pDisplayMsg(pMsgToDisplay),charDisplayMsg(NULL),
	fTimedOutOnce(false), nTitle(Title), csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
// doesn't appear to be used anymore
SMStateBase  *SMCardMisReadBase::Deliver(LPTSTR csMsgToDisplay, PSTEXT Title)
{
  charDisplayMsg = csMsgToDisplay; pDisplayMsg = PS_NOTEXT;
  fTimedOutOnce = false; nTitle = Title;

  return this;
}

////////////////////////////////////////
// code comes thru here 2nd
SMStateBase  *SMCardMisReadBase::Deliver(PSTEXT pMsgToDisplay, PSTEXT Title)
{
  charDisplayMsg = NULL;pDisplayMsg = pMsgToDisplay;	// ps text msg
  fTimedOutOnce = false; nTitle = Title;

  return this;
}
////////////////////////////////////////
bool SMCardMisReadBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    CDescriptor firstDescriptor;
    // There should be two parameters, both ints
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMCardMisReadBase::DeliverDescriptorList() - Parameter Count of 2"));
        firstDescriptor = dlParameterList.GetLastDescriptor();
        if (firstDescriptor.GetType() == DESCRIPTOR_CSTRING)
        {
            trace(L6, _T("+SMCardMisReadBase::DeliverDescriptorList() - First Deliver Type"));
            Deliver((LPTSTR)(LPCTSTR)firstDescriptor.GetCString(),  // csMsgToDisplay  
                    (PSTEXT)DLParamDWORD(dlParameterList));        // Title
        }
        else
        {
            trace(L6, _T("+SMCardMisReadBase::DeliverDescriptorList() - Second Deliver Type"));
            Deliver((PSTEXT)firstDescriptor.GetDWORD(),    // pMsgToDisplay  
                     (PSTEXT)DLParamDWORD(dlParameterList));        // Title
        }

        return true;
    }
    else if (dlParameterList.GetDescriptorCount() == 1)
    {
        trace(L6, _T("+SMCardMisReadBase::DeliverDescriptorList() - Parameter Count of 1"));
        firstDescriptor = dlParameterList.GetLastDescriptor();
        if (firstDescriptor.GetType() == DESCRIPTOR_CSTRING)
        {
            trace(L6, _T("+SMCardMisReadBase::DeliverDescriptorList() - First Deliver Type"));
            Deliver((LPTSTR)(LPCTSTR)firstDescriptor.GetCString());  // csMsgToDisplay  
        }
        else
        {
            trace(L6, _T("+SMCardMisReadBase::DeliverDescriptorList() - Second Deliver Type"));
            Deliver((PSTEXT)firstDescriptor.GetDWORD());    // pMsgToDisplay  
        }

        return true;
    }
    else
    {
        trace(L2, _T("+SMCardMisReadBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}


///////////////////////////////////////
SMStateBase  *SMCardMisReadBase::Initialize(void)
{
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("CardMisRead") );
	//Update RAP window with SCOT state
	trace(L6, _T("In Finalization = %d."), bIsInFinalization);
	UpdateStateName(ps.GetPSText(nTitle, SCOT_LANGUAGE_PRIMARY));
    static BEST nPrevGoToState = BES_FINISH;// TAR119740	LPM100199  must return to this state after call for help
	static int nPreviousTitle = 0;			
	static int nPrevDisplayMsg = 0;	
	
	fTimedOut = false; //SSCOP-481

	// when the help btn is pressed (or system msg interrupts, etc), the return state is this one
	// so none of the msgs are sent in, they are expected to be remembered
    if (getAnchorState() == BES_CARDMISREAD)//if last anchor state was this one - help btn pressed or system msg interrupted, etc
    {
		nTenderButtonIndex = SMStateBase::nSavedTenderButtonIndex; // // SSCOP-1324 get the previous tender button index
        nGoToState = nPrevGoToState;		//need to return to correct state when leave here
    }
    else if ( isAnySoldVoidedItems() &&
        ( nTBLastState != TB_NEEDMOREDATA &&
        nTBLastState != TB_UNKNOWN &&
        nTBLastState != TB_IGNORE &&
        nTBLastState != TB_TOTAL))// else, got here from normal xactn flow, AND we have sold items,   
    {
		//+TAR319581 Migrate Fix 3.2 to 4.0
		if ( fInSignedOn || fInSignedOff ) //TAR319581: If TB_CLOSED has been returned, don't set nGoToState to BES_FINISH 
		{
        nGoToState = BES_FINISH;			// when we leave here, must goto Finiah and try to pay
        nPrevGoToState = nGoToState;		// incase re-enter this state, set previous to Finish again
	    nPreviousTitle = nPrevDisplayMsg = 0;// reset title
		}
		else
		{
		nGoToState = getAnchorState();  //TB_CLOSED has returned, go back to anchor state
        nPrevGoToState = nGoToState;	
	    nPreviousTitle = nPrevDisplayMsg = 0;
		}
		//-TAR319581
	}
    else
    {
        if ((IsTenderStarted() || bIsInFinalization) 
            && getAnchorState() == BES_CARDPROCESSING)
        {
            nGoToState = BES_FINISH;
            nPrevGoToState = nGoToState;
            nPreviousTitle = nPrevDisplayMsg = 0;
        }
        else
        {
            nGoToState = getAnchorState();      // otherwise just return to the anchor state - Attract or Scan&Bag
            nPrevGoToState = nGoToState;        // incase re-enter, set prev state to anchor state
            nPreviousTitle = nPrevDisplayMsg = 0;// reset title
        }
    }
    setAnchorState(BES_CARDMISREAD);		

	ps.SetCMFrame(_T("CardMisRead"));
	// NewUI
	COleVariant v;
	if ( co.fTenderIsMotorizedMSR )			// if Motorized Card Reader
	{
		if ( g_bTBCaptureDeclinedCard )	
		{	// if supposed to capture the declined card
			DMMSRCapture();					// then capture it

			v=VARIANT_TRUE;
			ps.CMButtonFlash(_T("CMButton1Med"), true);

			nTimeOut = co.getTimeEASDeactivationScreenTimer();	// enable timeout feature
		}	
		else
		{
			DMMSREject();					// eject card	

			v=VARIANT_FALSE;

			nTimeOut = co.getTimeOutTakeCard();	// use the timeout feature
		}
		m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
	}
	else
	{
		v=VARIANT_TRUE;
		m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
		ps.CMButtonFlash(_T("CMButton1Med"), true);

		DMMSREnable();   //For 2.0 leave MSR enabled so users can swipe card again
		// If card swiped & is a bad card, throw bad track data away
		if ( cDMLastTrack )
		{
			delete [] cDMLastTrack; //SSCOP-4165
			cDMLastTrack = NULL;
			nDMLastTrackLen = 0;
			csDMEncryptedPin = EMPTY_STRING;
		} 
		nTimeOut = co.getTimeEASDeactivationScreenTimer();	// enable timeout feature
	}
	if (nTitle <= 0)						// if title is not defined, *which cannot happen*
		nTitle = nPreviousTitle;			// then set it to the previous title
	else						
		nPreviousTitle = nTitle;			// else the title is defined to save prev for re-enter sitch

    //sscoadk-6740: move showcmframe earlier. ShowCMFrame is where to set the language if scotapp gets here from store mode. 
    ps.ShowCMFrame();  //here, tha language is set if it gets here from store mode
    if(fTenderDeclined || co.fTenderIsMotorizedMSR)
	{
		DMSayPhrase(CARDNOTACCEPTED);
	}
	else
	{
		DMSayPhrase(SWIPECARDAGAIN);
	}

	ps.SetCMLeadthruText(nTitle);
    csCurContext = _T("CardMisRead");       // Tar 238987

    //sscoadk-6740: move showcmframe earlier. ShowCMFrame is where to set the language if scotapp gets here from store mode. 
    ps.ShowCMFrame();  //here, tha language is set if it gets here from store mode
    if(fTenderDeclined || co.fTenderIsMotorizedMSR)
	{
		DMSayPhrase(CARDNOTACCEPTED);
	}
	else
	{
		DMSayPhrase(SWIPECARDAGAIN);
	}

	ps.SetCMLeadthruText(nTitle);
    csCurContext = _T("CardMisRead");       // Tar 238987

	PSTEXT psTempMsg;
	if(co.fTenderCashAllowed)
		psTempMsg = MSG_CANTREADCARDSWIPE;			// not really swipe specific - generic msg
	else
		psTempMsg = MSG_CANTREADCARDSWIPENOCASH;		// not really swipe specific - generic msg

	// 119740 upon re entrance, variables need to be saved
	CString  csCurrentDisplayMsg;	
	if (!nPrevDisplayMsg)						// if a previous msg doesn't exist
	{
	  if (charDisplayMsg)						// then if there is a passed in msg
		  csCurrentDisplayMsg = charDisplayMsg;	// use the char passed in
	  else if(pDisplayMsg)						// or if there is a passed in PS txt 
	  {
		  csCurrentDisplayMsg = ps.GetPSText(pDisplayMsg);	// use the PS txt passed in
		  nPrevDisplayMsg = pDisplayMsg;					// and save off for re-enrance
	  }
	  else if (psTempMsg)						// else if none of the above, use the cash allowed msg
	  {
		 csCurrentDisplayMsg = ps.GetPSText(psTempMsg);
		 nPrevDisplayMsg = psTempMsg;
	  }
	}	
	else										// if a previous msg does exist
		csCurrentDisplayMsg = ps.GetPSText(nPrevDisplayMsg);	// use that one

	if ( co.fOperationsPartialIntegratedPINPad			//if PIP
	 && !co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bIntegratedMSR) // and not integrated MSR
	{
		if ( !co.IsDegradedModeOn() 
            || co.IsDegradedModeOn() && dm.GetDegradedModeType() == DEGRADEDMODE_OFF)  //sscoadk-6042
            csCurrentDisplayMsg = ps.GetPSText(MSG_CARDDECLINED_PIP);		
	}
	CString csTempMsg = csCurrentDisplayMsg;		// set us the informational txt
	CString csDeclinedReasonPrefix = ps.GetPSText(MSG_FALSE_TENDER_REASON);

	if (!csErrDescription.IsEmpty())		// an error description will exist for misread 
		csTempMsg = csTempMsg + csDeclinedReasonPrefix  + csErrDescription;

  //+dp185016 tar 278014, Message12 will output csTempMsg on screen even before ShowCMFrame
	//ps.Message12(csTempMsg, true);		// display the final msg
	ps.Message12(_T(""), true); // clear the instructiontextarea first
  //-dp185016 tar 278014

	ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	DisplayAuxScreen(AUX_SCREEN_CARD_MISREAD, true);		// Display the Auxilary card misread screen
	//ps.ShowCMFrame();
	ps.Message12(csTempMsg, true);		//tar 278014, we set instructiontextarea to csTempMsg here

	SMStateBase::nSavedTenderButtonIndex = nTenderButtonIndex; // SSCOP-1324 saved the tender button index in case help btn pressed or system msg interrupted, etc

    //++TAR 257931
    //Can't leave these variables set in case a Suspend happens next
    nTenderType = TB_TENDER_NONE;  
	nTenderButtonIndex = -1;       
    //--TAR 257931
	
	return STATE_NULL;
}

///////////////////////////////////
void SMCardMisReadBase::UnInitialize(void)
{
	if (!cDMLastTrack && !waitingForCardRemoval)
	{
		DMMSRDisable();	    // if no card inserted, disable, no problems
	}
    else if(!co.fTenderIsMotorizedMSR)      // we scanned in a card and we're using a swipe reader
    {
        DMMSRDisable();                // Disable reader to prevent "Can't swipe card now" msgs
        waitingForCardRemoval = false; // TAR 302923
    }
  	bBadCard = false;   //TAR229353 2/27/03 Since we are leaving, we reset this back to false
	//waitingForCardRemoval = false; // tar 260141
	g_bTBCaptureDeclinedCard = false;
	if(!fTimedOut)//SSCOP-481
	{
	    csErrDescription = EMPTY_STRING;
	}
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("CardMisRead") );
}

///////////////////////////////////////
SMStateBase  *SMCardMisReadBase::DMCardRemoved(void)
{
    waitingForCardRemoval = false; // tar 260141
  //119740	the anchor state is card misread.  must return to the state before that.
  return setAndCreateAnchorState(nGoToState);
}

///////////////////////////////////////
SMStateBase  *SMCardMisReadBase::DMCardReader(void)
{
  //Motorized MSR is now being used 
  //ASSERT(!dm.MSRIsMotorized()); // to get to this point it must be a swipe reader

  if (nGoToState == BES_START)   //if we started on Attract screen
  {
   //copy of Attract state code to start a transaction, since we're moving directly from 
   //this state to Welcome screen.
      SASendSCOTInput( ITEMIZATION );
	
      if (co.GetfStateDualLanguage())
      {   //use the primary language as default when user scans a card to start a trans
          m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
          ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
          ps.SetTareListToCurrentLanguage();
		  if(gpSCOTAuxDisp)
          gpSCOTAuxDisp->SetLanguage(SCOT_LANGUAGE_PRIMARY);
          return TBStart(co.Getlanguage(SCOT_LANGUAGE_PRIMARY).szCode);
      }
      else
      {
          return TBStart();
      }
  }
  else  //transaction already started
  {
      return setAndCreateAnchorState(nGoToState);
  }
}

/////////////////////////////////////////////////////
// This state allows 2 timeouts before capturing card.
SMStateBase  *SMCardMisReadBase::TimedOut(void) // timeout
{

	// only get here is motorized MSR and if card is misread
	if (co.fTenderIsMotorizedMSR)	// using ATM style MSR (insertable)
	{
		if (fTimedOutOnce)			// upon 2nd timeout - 1st time thru will be false
		{
			DMMSRCapture();			// capture the card because it has not been taken
			return DMCardRemoved();	// return to previous state
		}
		else						// upon 1st timeout in this state
		{	
			fTimedOutOnce = true;	// now set to true so it will capture the card
			DMSayPhrase(TAKECARD);	// instructions
			if(!bIsTransactionStarted)  //TAR 275219 
			{
				return STATE_NULL;  // stay in this state, wait for card removal or else capture
			}
			else
			{
				fTimedOut = true; //SSCOP-481
				RETURNSTATE(ContinueTrans)
			}
		}
	}
	else							
	{	// if swipe or tender declined motorized
		if(!bIsTransactionStarted)  //TAR 275219 
		{
			return setAndCreateAnchorState(nGoToState); 
		}
		else
		{
			fTimedOut = true; //SSCOP-481
			RETURNSTATE(ContinueTrans)
		}
	}
}

/////////////////////////////////////////////////////
SMStateBase  *SMCardMisReadBase::PSButton1(void)
{
  SetTenderStarted(false);
  return setAndCreateAnchorState(nGoToState);
}

/////////////////////////////////////////////////////
SMStateBase *SMCardMisReadBase::PSEnterKey(void)
{
  return PSButton1();
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMCardMisReadBase::OnWtIncreaseNotAllowed()	 
{
	if (bIsInFinalization )   //We are at finialization stage
		return FinalizationWtIncrease(BES_STATERETURN);	
	else
		return STATE(State)::OnWtIncreaseNotAllowed();
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMCardMisReadBase::OnWtDecrease()
{
	if (bIsInFinalization)   //if we are at a finialization screen
	{
		if(!g_bIsForgiveUnexpectedDecreaseDuringTendering)
			return FinalizationWtDecreaseNotAllowed(BES_STATERETURN);
		else
			return STATE_NULL;
	}
	else
		return STATE(State)::OnWtDecrease();
}

///////////////////////////////////////////////////////////////////////////
// SSCOP-481 - adding override for ButtonHelp
SMStateBase *SMCardMisReadBase::PSButtonHelp()
{
	fTimedOut = true; //SSCOP-481
	return STATE(State)::PSButtonHelp();
}