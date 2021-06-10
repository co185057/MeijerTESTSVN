//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMInProgressBase.cpp
//
// Description: The InProgressBase state is used when the transition from a
//        state to the next state might take some time.  InProgressBase is a
//        "chameleon" state with respect to Customer Mode, Store Mode,
//        and Help Mode - it becomes the mode of the creating state and
//        therefore does not inherit from the above classes.
//
//
// Author:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////
 
#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMInProgress.h"           // MGV this state
#include "SMSystemMessage1.h" 
#include "SMScanAndBag.h"			// 410299	
#include "SMFinish.h"               // 410299


#define COMP_ID ID_SM               // base state component
#define T_ID _T("InProgressBase")


IMPLEMENT_DYNCREATE(SMInProgressBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(InProgress)

DEFINE_TIMESTAMP

////////////////////////////////////////////////
SMInProgressBase::SMInProgressBase(const bool storeMode,
                                    const bool helpMode,
                                    const int timeout)
                                    : fStoreMode(storeMode),
                                    fHelpMode(helpMode)
{
  IMPLEMENT_TIMESTAMP
		nTimeOut = timeout;
}

////////////////////////////////////////
SMInProgressBase::SMInProgressBase()
{
  IMPLEMENT_TIMESTAMP
  fAMNeedEnable = false;  // save the state of Assistmode button at RAP
}

////////////////////////////////////////
SMStateBase  *SMInProgressBase::Deliver(const bool storeMode,
                           const bool helpMode,
                           const int timeout)
{
  fStoreMode = storeMode;
  fHelpMode = helpMode;
  nTimeOut = timeout;
  fAMNeedEnable = false;  // TAR246532
  return this;
}

/////////////////////////////////////////
bool SMInProgressBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    
    // There should be three parameters, two bools passed as word and an int
    if (dlParameterList.GetDescriptorCount() == 3)
    {
        trace(L6, _T("+SMInProgressBase::DeliverDescriptorList() - Parameter Count of 3"));
        Deliver((bool)DLParamWORD(dlParameterList),    // storeMode
                (bool)DLParamWORD(dlParameterList),    // helpMode
                (int)DLParamDWORD(dlParameterList));   // timeout
        return true;
    }
    else
    {
        trace(L2, _T("+SMInProgressBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}

//////////////////////////////////////
SMStateBase  *SMInProgressBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("InProgress") );	// RFC 330050
	m_bNeedToDisplayAuxScreen = false; // DON'T display aux screens in this state!
	//+TAR 350775 
	CString csPreviousStateName = this->csLastNonHelpStateName;
	csPreviousStateName.MakeLower();
	if ( fSAInStoreMode && ( csPreviousStateName.Find( _T("smsmdataentry") ) > -1 ) && \
		fItemIsWaiting )
	{
		ra.OnExitStoreMode();
	}
	//-TAR 350775 
	// +TAR246532
	if (ra.GetRequestEnableAssistMode() && !ps.RemoteMode())  
	{
		// not in remote mode, currently AM is enabled to access it at RAP
		// disable access to AM at RAP while in this state and enable it on exit

        fAMNeedEnable = true; // save the state of Assistmode button at RAP and restore it on exit of this state
		ra.RequestEnableAssistMode(false);	 
	}
	// -TAR246532
	return STATE_NULL;
}

////////////////////////////////////
SMStateBase  *SMInProgressBase::TimedOut(void) // timeout
{
  trace(L6, _T(" !!! Timedout in SMInProgress while waiting to hear from TB !!!, potential out-of-synch situation betweeen TB and SCOT"));

  // RFC 262343 - start
  if (fTenderPending) //If we time out while in a tender
  {
      ScotError(RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_TENDER_TIMEOUT);
      fTenderPending = false;       
      if (cDMLastTrack)
      {
          delete cDMLastTrack;
          cDMLastTrack = NULL;
      }
  }
  else
  {
      ScotError(RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_TB_OUT_OF_SYNC);
  }
  // RFC 262343 - end

  // +207606 LPM062002  Scanner not re-enabled if the TB doesn't ever reply 
  trace(L2, _T("::TimedOut - bEASReEnableScanner is set to true."));
  bEASReEnableScanner = true;
  // -207606
  //TAR251537+
  if ( getAnchorState() == BES_CARDPROCESSING ) 
  {
	  if (bIsInFinalization )
	  {
		trace(L2, _T("::TimedOut - Changing Anchor State to Finish."));
		setAndCreateAnchorState(BES_FINISH);
	  }
	  else
	  {
		trace(L2, _T("::TimedOut - Changing Anchor State to ScanAndBag."));
		setAndCreateAnchorState(BES_SCANANDBAG);
	  }
  }
  //TAR252537-	  
  
  return createAnchorState();
}

//////////////////////////////////
SMStateBase  *SMInProgressBase::DMScanner(void) // scanned an item
{
  trace(L3, _T("Probable race condition, scanned code: %s, Previous input sent to Sec: %d, fItemIsWaiting: %d"), csDMLastBarCode, nSAPrevSCOTInputID, fItemIsWaiting); 
  if (!fItemIsWaiting)
  {
	csItemSent = csDMLastBarCode;
	fItemIsWaiting = true;            // to know if the item was sold from the attract screen or not.  sell item after signon in S&B
	if (nSAPrevSCOTInputID != UPC_SCANNED) 
	{
	  trace(L3, _T("Sending Security event ID: 1 to fix race condition."));		// 
	  SASendSCOTInput( UPC_SCANNED, csDMLastBarLabel); //security needs to know we scanned an item quickly.  //TAR#121479
	}
  }
  return STATE_NULL;
}

////////////////////////////////////////
// trap this message just so base doesn't give an error msg,
// nothing else to do here
SMStateBase  *SMInProgressBase::DMCardReader(void)
{
  return STATE_NULL;
}


///////////////////////////////////////////////////////////////////////////
SMStateBase *SMInProgressBase::OnBackToLGW(void)
{
	// TAB re-enable scanner and clear Wait For Approval State
	TABReEnableScannerAndClearWaitForApprovalState();
	return STATE_NULL;
}


//TAR199913+
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMInProgressBase::OnWtIncreaseNotAllowed()	 
{
	if (bIsInFinalization )   //We are at finialization stage
		return FinalizationWtIncrease(BES_STATERETURN);	
	else
		return SMStateBase::OnWtIncreaseNotAllowed();
		

}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMInProgressBase::OnWtDecrease()
{

	if (bIsInFinalization)   //We are at finialization stage
	{
		if(!g_bIsForgiveUnexpectedDecreaseDuringTendering && !(lBalanceDue < 0.01))
			return FinalizationWtDecreaseNotAllowed(BES_STATERETURN);
		else
			return STATE_NULL;
	}
	else
		return SMStateBase::OnWtDecrease();
}

//TAR199913-

//////////////////////////////////////////////////////////////////////////
void SMInProgressBase::UnInitialize()
{
	if (fAMNeedEnable)  
	{
        fAMNeedEnable = false; 
		ra.RequestEnableAssistMode(true); // save the state of Assistmode button at RAP
	}
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("InProgress") );	// RFC 330050
}

bool SMInProgressBase::PSRemoteLegalForState() 
{
	return true;
}


//TAR222852 CHU 052903
//If the bill/coin is accepted in this state, we will reject it
////////////////////////////////
SMStateBase *SMInProgressBase::DMAcceptor(void)
{
  long rc;
  trace(L6,_T("SMInProgressBase::DMAcceptor; can't accept coin/bill in this state."));
 
  rc = DMCashAcceptorReject();  // Coin Acceptor Modifications
  if (rc < 0)			//  most likely out of change
  {
	return DMCashChangerError(nDMLastBillEvent);
  }
  else if (nDMLastBillEvent > 0)
  {
    trace(L6, _T("Reject bill amount: %d"), nDMCashInserted);
    nDMCashInserted -= nDMLastBillEvent;
    nDMLastBillEvent = 0;
	
  }

  rc = DMCoinAcceptorReject();  // Coin Acceptor Modifications

  if (rc < 0)			// Tar 214729 RJF 101002 most likely out of change
  {
	return DMCashChangerError(nDMLastCoinEvent);
  }
  else if (nDMLastCoinEvent > 0)
  {
	 trace(L6, _T("Reject coin amount: %d"), nDMCashInserted);
    nDMCashInserted -= nDMLastCoinEvent;
    nDMLastCoinEvent = 0;
  }
  
  if (!Is5XHardware())  // Tar 403042
  {
	  //+RFC 330995
	  CString csWork = _T("DeviceErrorCashChanger.xml:DM_CASHCHANGER_STATUS_DISPENSE");
	  CREATE_AND_DISPENSE(SystemMessage1)(csWork,	// no char *message	
						PS_NOTEXT,	// canned message
						PS_NOTEXT,  // no prompt
						0,          // no prompt value
						-1,         // no device
						false);     // no DM check health needed 
	  //-RFC 330995
  }

  return STATE_NULL;    
}


//+ TAR 391316
SMStateBase *SMInProgressBase::PSButton1(void)     
{
	trace(L6, _T("SMInProgressBase::PSButton1 - ignore PSButton1 button press"));
	return STATE_NULL; 
}
//- TAR 391316


// TAR 246429
SMStateBase *SMInProgressBase::PSButtonHelp()
{
    trace(L6,_T("SMInProgressBase::PSButtonHelp - ignore help button press"));
	return STATE_NULL;    
}
// TAR 246429

//405090+
SMStateBase *SMInProgressBase::DMParse(const MessageElement *me)
{
    trace(L6, _T("SMInProgressBase::DMParse"));
        
    if( (me->dmInfo.Msg == DM_DATA) &&
        (me->dmInfo.Class == DMCLASS_MSR) )
    {
	    trace(L6,_T("SMInProgressBase::DMParse; can't accept any swipe in this state."));
		return STATE_NULL;

	}

	#ifdef _BUILDLIB_
	return SMStateBase::DMParse(me);
	#else
	return SMState::DMParse(me);
	#endif

}
//405090-

//410299+
SMStateBase *SMInProgressBase::SASecMgrParse( const MessageElement *me)
{

    int nEvent = me->secmgrInfo.nEvt;
	trace(L6, _T("SMInProgressBase::SASecMgrParse nEvent = %d"), nEvent);
    LPTSTR szParms;
    szParms = new TCHAR [_tcslen(me->secmgrInfo.szParms) + sizeof(TCHAR)];
    _tcscpy( szParms, me->secmgrInfo.szParms );
    
	SecurityControlEventParms secControlEventParms;
	if (_tcslen(szParms) != 0)
	{
		secControlEventParms.ParseKeys(szParms);
	}
	delete [] szParms;

    if ( (nEvent == SmOnSecurityControl) && 
		 (secControlEventParms.m_csOperation == KEY_OSC_OPERATION_GOTO_SCANANDBAG ) &&
		 ( (nTBLastRealState == TB_LOYALTYCARD )
		   || (TB_OVERRIDE_SECURITY_WITH_NOWEIGHT == io.d.lRequireSecBagging )
		   || (TB_OVERRIDE_SECURITY_WITH_YES != io.d.lRequireSecBagging && io.d.lExtPrice < 0 )) )//tar413251
    {
		trace(L6, _T("KEY_OSC_OPERATION_GOTO_SCANANDBAG is received"));
        delete [] me->secmgrInfo.szParms;
		if (getAnchorState() == BES_FINISH)
		{
			RETURNSTATE(Finish)
		}
		RETURNSTATE(ScanAndBag)
    
	} 

    #ifdef _BUILDLIB_
	return SMStateBase::SASecMgrParse(me);
	#else
	return SMState::SASecMgrParse(me);
	#endif 
}
//410299-
