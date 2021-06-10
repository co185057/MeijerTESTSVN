//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRequestSigBase.cpp
//
// TITLE: Class impliementation for requesting a signature from the customer
//
//
// AUTHOR:    Yafu Wang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMRequestSig.h"     // MGV this state
#include "SMAcknowledgeSig.h"     // MGV this state
#include "SMInProgress.h"           // MGV in progress state
#include "SMSystemMessage1.h"
#include "SMWaitForApproval.h"
#include "SMTakeCard.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("RequestSigBase")


IMPLEMENT_DYNCREATE(SMRequestSigBase, SMCustomerModeState)// MFC Runtime class/object information

//IMPLEMENT_STATE(RequestSig)


DEFINE_TIMESTAMP

////////////////////////////////////////
SMRequestSigBase::SMRequestSigBase()
{
  IMPLEMENT_TIMESTAMP
  m_bSigStarted = FALSE;
}

/////////////////////////////////////////////
SMStateBase  *SMRequestSigBase::Initialize(void)
{

    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("RequestSig") );

	// CANNOT enter Remote Assist Mode at this state
	// ra.OnSetTransactionState(RA_STATE_ASSISTNOTOK);
    ra.RequestEnableAssistMode(false);               //Tar213249
    m_pAppPSX->SetConfigProperty(_T("SigCap"), _T(""), UI::PROPERTYPICTURE, _T("") );
	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_REQSIG, SCOT_LANGUAGE_PRIMARY));
  // There is a bug in the 5992 right now where if we enable sigcap, the
  // MSR sends an error event.  So to work around this, we will disable
  // the MSR on this screen.  However, in the general case, we probably STILL
  // want the MSR DISabled on this screen as we are past any card-entry anyway.
  // ncr_barn - 01/27/2000

	//Tar 153983
	HWND hControlWnd ;

	ps.GetControlHandle(hControlWnd); //*** NewUI ***//

	m_bSigStarted = FALSE;

	//TAR257097+
	SMStateBase * checkState = TurnOnSigCap(hControlWnd) ; 
	if (checkState != STATE_NULL) //Could be SystemMessage if there is an error inside of TurnOnSigCap()
	{
		return checkState;
	}

    //TAR267271+
    if (!cDMLastTrack && !waitingForCardRemoval) 
    {
	  DMMSRDisable();// if no card inserted, disable, no problems
    }
    else if(!co.fTenderIsMotorizedMSR)	// we scanned in a card and we're using a swipe reader
    {
	  DMMSRDisable();// Disable reader to prevent "Can't swipe card now" msgs
    }
    //TAR267271- 	
    
    setAnchorState(BES_REQUESTSIG);  //TAR#132072 -- set anchor state for go back
	fFromRequestSignature = true;	 //TAR#140156 -- if go to help, must disable timeout there too
	//nTimeOut = 0;					 //TAR#140156 -- turn timeout feature off  5/9/00 LMP and YW
	nTimeOut = co.getTimeOutRequestSig();   //configurable timeout - thp

	// TAR 208517 RF 071902 Do not reset the timeout when DMEvent is fired 
    bDoNotResetTimeOutOnDMEvent = true;  // Don't let DMEvents affect timers
  
	ps.SetCMFrame(_T("RequestSig"));
	ps.ShowChangeDueAmountPaid(lChangeDue, lTotalPaid); //Tar #153290
	ps.Message12(MSG_SIGN_SIGNATURE, true);

	if (gpSCOTEncryptor->GetProfileName().CompareNoCase(_T("NCR5992Pinpad.1")) == 0)
	{
		// 5992 pinpad encryptor device
		DMSayPhrase(SIGNSIGNATUREPAD_OKWITHPEN) ;
	}
	else
	{
		DMSayPhrase(SIGNSIGNATUREPAD);
	}
	ps.ShowCMFrame(false);
	return STATE_NULL;
}

SMStateBase *SMRequestSigBase::TimedOut(void) // timeout
{
	//TAR#140156 -- when timeout occurs and return from store mode will cause sigcap data corrupted.  5/9/00 LMP and YW
	//	TurnOffSigCap() ;
	//	return SMStateBase::TimedOut() ;
	//return STATE_NULL;
	// thp - turn of SigCap and go back to Request Sig base
	TurnOffSigCap() ;
    bDoNotResetTimeOutOnDMEvent = false; // TAR 208517 RF 071902 

	// TAR 318246 - if the Topaz or other sigcap device is not working 
	//				we never get the signal from the SO that allows us 
	//				to display the Ok button and finish.  
	//				Neither can the SO detect the Topaz is not working. 
	//				Neither is there a help button or go back button 
	//				because these would get the TB out of sync.  
	//				AssistMode and Suspend transaction are disabled, 
	//				so there is literally no way out of the transaction.  
	//				We can't just restart requestsig.  We'll remain stuck.  
	// RETURNSTATE(RequestSig);

   	m_bSigStarted = TRUE;  // This lets us know NOT to turn of sig cap in UnInit()
    RETURNSTATE(AcknowledgeSig) 
}

SMStateBase  *SMRequestSigBase::PSButtonHelp(void)
{
	TurnOffSigCap() ;
	return SMStateBase::PSButtonHelp();
}


SMStateBase *SMRequestSigBase::DMSigcapStarted()
{
	  //TAR 175143
   	  m_bSigStarted = TRUE;  // This lets us know NOT to turn of sig cap in UnInit()
	  RETURNSTATE(AcknowledgeSig) 

}


SMStateBase *SMRequestSigBase::DMSigcapEnded()
{
    COleVariant vReturn;

	TurnOffSigCap() ;
	
  if (gpSCOTSigCap->m_bHaveValidSig)
  {
    BSTR pData= NULL;
  
  
	  gpSCOTSigCap->Get5991Format(&pData);  // Get 5991 format for either RAP or S-Receipt file
	  
  
	if (co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bVerifySignature
				  || (co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nVerifySignatureOverAmount != -1 && co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nVerifySignatureOverAmount  <= lLastTender) ) // if verify, saving signature and printing receipt will be delayed to confirmSig
	  {	  
        /*
		  trace(L7, _T("+ra.OnSigCapData"));
		  // awk.4.23.2 TAR200003 START-Added delayed intervention for signature capture.
		  CString csInstruction;
		  //Tar 203306 AH 05/14/02 
		  if (cDMLastTrack==NULL)
		  {
			  //The second field in the following  string format should not be NULL
			  csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), 
			    _T(" "),
		        ps.GetPSText(RA_LANEBTT_SIGCAPTURE, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_SIGNCONFIRM, SCOT_LANGUAGE_PRIMARY), _T(""), 
			    ps.GetPSText(RA_THIRDLINE_SIGCAPTURE, SCOT_LANGUAGE_PRIMARY));
		  }
		  else
		  {
			  csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), 
				CustomerName(cDMLastTrack),
				ps.GetPSText(RA_LANEBTT_SIGCAPTURE, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_SIGNCONFIRM, SCOT_LANGUAGE_PRIMARY), _T(""), 
				ps.GetPSText(RA_THIRDLINE_SIGCAPTURE, SCOT_LANGUAGE_PRIMARY));
		  }
		  //Tar 203306 end
	      
		  ra.OnSigCapData(csInstruction, (LPCTSTR) pData, (DWORD)(SysStringByteLen(pData)));
		  //// awk.4.23.2 TAR200003 END
		  trace(L7, _T("-ra.OnSigCapData"));
		  SysFreeString(pData);
		  //DMMSREject();  //TAR#128476 moved to wait for approval, should not eject card until after store personal has logged in LPM122199
          //		  CREATE_AND_DISPENSE(WaitForApproval) (false,false, true) ;
          */
          m_bSigStarted = TRUE;  // This lets us know NOT to turn of sig cap in UnInit()
          SMAcknowledgeSig::bAlreadyApproved = true;
          RETURNSTATE(AcknowledgeSig)
	  }
	  else  // if do not verify, print receipt and take card
	  {
		      //Send signature data to TB
	  if(co.fOperationsSendSignatureDataToTB)
		  TBSendSigData(CustomerName(cDMLastTrack),(LPCTSTR) pData,(DWORD)(SysStringByteLen(pData)));

		  trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_TENDER_COMPLETE, SCOT_LANGUAGE_PRIMARY));
		  ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + 
                            ps.GetPSText(RA_TENDER_COMPLETE, SCOT_LANGUAGE_PRIMARY));
		  trace(L7, _T("-ra.OnNormalItem"));
		  
          EndOfTransactionPrint(); //SR697

    	  // +TAR 390646
          if (co.m_bOperationsPrintSignatureReceipt)
          {
              SaveSigReceiptData(pData);
          }
	      // -TAR 390646

		  //return TBEnterTender(TB_TENDER_CREDIT,lBalanceDue);
      SysFreeString(pData);
	    CREATE_AND_DISPENSE(TakeCard)(true) ; //TAR#132072 -- Proceed SigCap after tendering
	  }
  }
  else
  {
    RETURNSTATE(RequestSig)
  }
}

void SMRequestSigBase::UnInitialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("RequestSig") );

  if (!m_bSigStarted)
    TurnOffSigCap() ;
  // CAN enter Remote Assist Mode after leaving this state
  //ra.OnSetTransactionState(RA_STATE_ASSISTOK);
    ra.RequestEnableAssistMode(true);   //TAR213249

}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMRequestSigBase::OnWtIncreaseNotAllowed()	 
{
	return VoidingWtIncrease(BES_REQUESTSIG,BES_REQUESTSIG); //TAR 374645	
}
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMRequestSigBase::OnWtDecrease()
{
        //Remove lines for TAR 374645
	//if(!g_bIsForgiveUnexpectedDecreaseDuringTendering && !(lBalanceDue < 0.01))
	//	return FinalizationWtDecreaseNotAllowed(BES_STATERETURN);
	//else
	//	return STATE_NULL;
	return ThresholdViolWtDecrease(BES_REQUESTSIG,BES_REQUESTSIG);
}


//Tar199449
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMRequestSigBase::OnBackToLGW(void)
{
	trace(L2, _T("+SMRequestSigBase::OnBackToLGW."));

	//If there is WaitForApproval state waiting, clear it
	if (g_pDelay_WaitForApprovalStateChange != NULL)
	{
		CString csStateName = g_pDelay_WaitForApprovalStateChange->GetRuntimeClass()->m_lpszClassName;
		delete g_pDelay_WaitForApprovalStateChange;
		g_pDelay_WaitForApprovalStateChange = NULL;
		trace(L6,_T("Delete the state %s saved in g_pDelay_WaitForApprovalStateChange"),csStateName);
	}

	trace(L2, _T("-SMRequestSigBase::OnBackToLGW."));


	//TAR199449: We want to stay at this state instead of go to AnchorState.
	//If we go to AnchorState, it is going to interrupt signing the signature and 
	//ask to sign signature again.
	return STATE_NULL;
	
}
