//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeLoyaltyCardBase.cpp 
//
// TITLE: Class impliementation file for Take Card State
//
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/core/SCOTAPP/SMTakeLoyaltyCardBase.cpp 3     10/30/08 4:18p Sm133076 $
//
// AUTHOR:  SCOT CORE TEAM, NCR Future Retail, ATLANTA
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // common includes

#include "SMState.h"                // base state
#include "SMTakeLoyaltyCard.h"      // take Card state
#include "SMSecUnExpectedIncrease.h"      

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("TakeLoyaltyCardBase")


IMPLEMENT_DYNCREATE(SMTakeLoyaltyCardBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(TakeLoyaltyCard)

DEFINE_TIMESTAMP

///////////////////////////////////////
SMTakeLoyaltyCardBase::SMTakeLoyaltyCardBase(TBLOYALTYCARDTYPE LoyaltyCardType)
: fTimedOutOnce(false), bUnexpectedIncHappened(false),fCardType(LoyaltyCardType)
{
	IMPLEMENT_TIMESTAMP
}


////////////////////////////////////////
SMStateBase  *SMTakeLoyaltyCardBase::Deliver(TBLOYALTYCARDTYPE LoyaltyCardType)
{
  fTimedOutOnce = false; fCardType = LoyaltyCardType;bUnexpectedIncHappened = false;

  return this;
}

/////////////////////////////////////////
bool SMTakeLoyaltyCardBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    
    // There should be zero or one parameter, a loyalty card type enum passed as a long
    if (dlParameterList.GetDescriptorCount() == 0)
    {
        trace(L6, _T("+SMTakeLoyaltyCardBase::DeliverDescriptorList() - Parameter Count of 0"));
        Deliver();
        return true;
    }
    else if (dlParameterList.GetDescriptorCount() == 1)
    {
        trace(L6, _T("+SMTakeLoyaltyCardBase::DeliverDescriptorList() - Parameter Count of 1"));
        Deliver((TBLOYALTYCARDTYPE)DLParamDWORD(dlParameterList));  // Loyalty Card Type
        return true;
    }
    else
    {
        trace(L2, _T("+SMTakeLoyaltyCardBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}

////////////////////////////////////
SMStateBase *SMTakeLoyaltyCardBase::Initialize(void)
{

	if (!co.fTenderIsMotorizedMSR)
	{
		if (cDMLastTrack)
		{
			delete cDMLastTrack;
			cDMLastTrack = NULL;
			nDMLastTrackLen = 0;
			csDMEncryptedPin = EMPTY_STRING;
		}
        if (fLoyaltyCardTrx && (fCardType != LC_INVALID)) 
		{
			io.d.fDetails = true;
			io.nLoyaltyCardScanned = 1;
		}
		//TAR246871+
		//For swipped loyalty card at finalization states, say a phrase even though we are leaving this state
		if (bIsInFinalization)
		{
			if (fCardType == LC_INVALID)
			{
				DMSayPhrase(TAKEINVALIDLOYALTYCARD);
			}
			else
			{
				DMSayPhrase(FREQSHOPACCT);
			}
		}
		//TAR246871-
		return DMCardRemoved();	// already taken
	}
	else
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("TakeLoyaltyCard") );

	// must Enable() device before calling Eject() so that the Enable() while in Eject() 
		// is ignored. Otherwise, the Enable() is processed too late and captures the card.  LPM100202 215897
		DMMSREnable();					// enable the card reader, set track, turn on MEI
	}

	// There's no data because the card isn't there, or is there
	// but couldn't be read.
	if (!cDMLastTrack && !waitingForCardRemoval)
		return DMCardRemoved();	// already taken

	PSTEXT nTakeCardMsgID = PS_BLANK;

	switch(fCardType)
	{
		case LC_VALID:
			if (!isAnySoldVoidedItems())	// at the trx beginning
			{
				// display in the message popup that customer gets discount automatically in this trx
				nTakeCardMsgID = MSG_LC_AUTO_DISCOUNT;
				SASendSCOTInput( ITEMIZATION );
			}
			else
			{
				// customer gets discount check by mail, loyalty card information is saved
				nTakeCardMsgID = MSG_LC_MAIL_DISCOUNT;
			}
			break;
		case LC_INSERTED_BEFORE:
			// saved card information when it was inserted before, please take card ??
			nTakeCardMsgID = MSG_LC_INSERTED_BEFORE;
			break;
		case LC_INVALID:
			if (!isAnySoldVoidedItems())	// at the trx beginning
			{
				SASendSCOTInput( ITEMIZATION );
			}
			nTakeCardMsgID = MSG_LC_INVALID;
			break;
		default:
			ASSERT(0);
			break;
	}

// NewUI
    ps.SetCMFrame(_T("TakeLoyaltyCard"));
// NewUI
	
	ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	ps.Message12(nTakeCardMsgID, true);	// true: message does not erase bitmap if any ,  // false : message erases bitmap if any

	// MSR should be enabled, light on, change it to blinking
	DMMSRLight(DM_MISC_STATE_ONE_HZ);

	// entry into this method is assuming MSR is still enabled.
	nTimeOut = co.getTimeOutTakeCard();
	DMMSREject();					  // eject card
	if (fCardType == LC_INVALID)
	{
		DMSayPhrase(TAKEINVALIDLOYALTYCARD);
	}
	else
	{
		DMSayPhrase(TAKELOYALTYCARD);
	}
	//waitingForCardRemoval = true; //tar 260141
    ps.ShowCMFrame();
	return STATE_NULL;
}


/////////////////////////////////////////////////////
// This state allows 2 timeouts before taking action.
SMStateBase *SMTakeLoyaltyCardBase::TimedOut(void) // timeout
{
  if (fTimedOutOnce)
  {
    // This is the second timeout in this state
    // capture the card because it has not been taken
    DMMSRCapture();
	ScotError(INFO, SM_CAT_INTERNAL_ERROR, SCOTAPP_CARD_PURGED,_T("Card purged - not removed by customer before timeout"));

    return DMCardRemoved();
  }
  else
  {
    fTimedOutOnce = true;
    DMSayPhrase(TAKELOYALTYCARD);
    return STATE_NULL;
  }
}

///////////////////////////////////
void SMTakeLoyaltyCardBase::UnInitialize(void)
{
	if (!cDMLastTrack)
	{
		DMMSRDisable();	// if no card inserted, disable, no problems
	}
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("TakeLoyaltyCard") );
}

///////////////////////////////////////
SMStateBase *SMTakeLoyaltyCardBase::DMCardRemoved(void)
{
	waitingForCardRemoval = false;
	//Swipe loyalty card at select payment type screen should not return to S&B
	//TAR256942+
	if (bIsInFinalization)    //TAR245485
		setAnchorState(BES_FINISH);
	else 
		setAnchorState(BES_SCANANDBAG);
	// if unexpected inc happened while here then go to that state after card removed
	// otherwise transition accordingly
	if (bUnexpectedIncHappened)
	{
		bUnexpectedIncHappened = false;
		trace(L6, _T("SMTakeLoyaltyCardBase::DMCardRemoved, returning to SecUnExpectedIncrease"));
		CREATE_AND_DISPENSE(SecUnExpectedIncrease)(getAnchorState(), getAnchorState());
	}
	else
	{
		trace(L6, _T("SMTakeLoyaltyCardBase::DMCardRemoved, returning to anchor state"));
		return setAndCreateAnchorState(getAnchorState());
	}
//TAR256942-

}
//TAR256942+
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMTakeLoyaltyCardBase::OnBackToLGW(void)
{
    //We shouldn't get this event from this state since setholdweight is true.
	//The only way to get here is in this race condition:
	//The event is fired in Inprogress state and parsed in this state.
	//This handler should be same as SMInProgressBase::OnBackToLGW(void)
	trace(L6, _T("SMTakeLoyaltyCardBase::OnBackToLGW, returning STATE_NULL"));
	bUnexpectedIncHappened = false;
	// TAB re-enable scanner and clear Wait For Approval State
	TABReEnableScannerAndClearWaitForApprovalState();
	return STATE_NULL;
}

SMStateBase* SMTakeLoyaltyCardBase::OnWtIncreaseNotAllowed()
{
	bUnexpectedIncHappened = true;
	return STATE_NULL;
}

//TAR256942-
