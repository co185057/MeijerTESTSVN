//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoidTransApprovalBase.CPP
//
// TITLE: Class implementation for Assistance Needed for Voided transaction state
//
// AUTHOR:  Aiqin Hou
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMVoidTransApproval.h"      
#include "SMScaleBroken.h"          // Reset scale
#include "SMSecUnExpectedIncrease.h"
#include "SMSmAbort.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("VoidTransApproval")

IMPLEMENT_DYNCREATE(SMVoidTransApprovalBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMVoidTransApprovalBase::SMVoidTransApprovalBase(const BEST prevState, const BEST approvedNextState)
: Prevstate(prevState),
  Nextstate(approvedNextState)
{
  IMPLEMENT_TIMESTAMP
}
//////////////////////////////////////////
SMVoidTransApprovalBase::SMVoidTransApprovalBase()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMVoidTransApprovalBase::Deliver(const BEST argPrevState, const BEST argApprovedNextState)
{
  Prevstate = argPrevState;
  Nextstate = argApprovedNextState;
  return this;
}
bool SMVoidTransApprovalBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, both ints
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMVoidTransApprovalBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver(DLParamDWORD(dlParameterList),    // Machine Name
                DLParamDWORD(dlParameterList));   // Remote Context
        return true;
    }
    else
    {
        trace(L2, _T("+SMVoidTransApprovalBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}


///////////////////////////////////////
SMStateBase  *SMVoidTransApprovalBase::Initialize(void)
{
    ra.RequestEnableAssistMode(false); //Tar 298100

	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("CancelTransactionGeneric"));
	m_View = DATANEEDED_CANCELLTRANSACTION;
	CString csDesc, csSecInfo, csWork;
	//TD 04/25/02 TAR 202801
	csDesc.Format(ps.GetPSText(LTT_CANCELEDITEMS,SCOT_LANGUAGE_PRIMARY));
	csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_VOID_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(TTL_CANCELEDITEMS, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""));
	ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
	bCancelTransApproval = true; // TAR 298096
	if(!fInMultiSelectPickList)
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ApproveVoidedItem") );
	}
	// Save current light and Turn the red light RED
	SAEnterSecurityCondition();
	return STATE(OperatorPasswordState)::Initialize();
}

////////////////////////////////////////////
void SMVoidTransApprovalBase::showScreen(bool passwordVerified)
{
	CString csWork;
	nTimeOut = co.getTimeEASDeactivationScreenTimer();
	if (passwordVerified)
		return;


		if (inputMode == PROMPTING_FOR_OPERATOR)
		{
			STATE(OperatorPasswordState)::ShowEnterID();  //SR93 Biometrics support			
			ps.SetCMLeadthruText( TXT_ENTER_ID );
			ps.AllowInput( MAX_DIGITS_OPERATOR, false, false );
			ps.ShowCMFrame( false );  

            return;
		}
		else
		{  // Display Assistance Needed for Approval....  
			DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
			ps.SetCMFrame(_T("VoidTransApproval"));
			ps.CMButton(_T("ButtonStoreLogIn"), BTT_STORELOGIN, true);
		}


	ps.SetCMLeadthruText(LTT_WAITFORASSISTANCE);
    // End NewUI

	SAWLDBScannerDisable();			// Disable the scanner
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
	csWork += _T("\n* ") + ps.GetPSText(TXT_VOIDEDITEMAPPROVAL);
    ps.Message12(csWork, true);
	trace(L7, _T("+ra.OnTransactionState, Itemization State"));
	ra.OnSetTransactionState(RA_STATE_WAIT_FOR_APPROVAL);
	trace(L7, _T("-ra.OnTransactionState"));
	ps.ShowCMFrame(false);
}

//////////////////////////////////////
void SMVoidTransApprovalBase::UnInitialize(void)
{
	//TAR214093 CHU If we don't disable it, if the next state is in InProgress
	//Scanner would be enabled.
	SAWLDBScannerDisable();			// Disable the scanner

    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	if(!fInMultiSelectPickList)
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ApproveVoidedItem") );
	}
	if (!bIsFromVoidApproval)  // TAR 298095 - clear the light only if we have approved void
	{
		ClearSecurityViolationLight();
	}

    ra.RequestEnableAssistMode(true); //Tar 298100
}

//////////////////////////////////////
SMStateBase *SMVoidTransApprovalBase::stateAfterPasswordVerify()
{
	trace(L2, _T("::stateAfterPasswordVerify - bEASReEnableScanner is set to true."));
	/*if (io.d.csItemCode != _T("") ) {  g_bOkToUpdate = true;  }

	ps.SetLanguage(m_customerLanguage); //tar 192275

	g_bAllowRemove = false;
  
	//do same thing that OnRAPClearSecurityBtn() does
	return OnRAPClearSecurityBtn(_T("Lane")); */
	RETURNSTATE(SmAbort)
}

///////////////////////////////////////
SMStateBase  *SMVoidTransApprovalBase::PSButtonGoBack(void) // store login
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
					if (co.fStateDualLanguage)
						ps.SetLanguage(m_customerLanguage);  //TAR 196315: Set back to customerLanuguage for dual language
					showScreen(false);
					return STATE_NULL;  
				}
		}

	//TAR193505-

    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	promptForOperator();
    return STATE_NULL;
}
  
///////////////////////////////////////
SMStateBase *SMVoidTransApprovalBase::TimedOut(void) // timeout
{
	DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
    return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMVoidTransApprovalBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{

	trace(L2, _T("+SMVoidTransApprovalBase::OnRAPClearSecurityBtn"));
    bIsInFinalization = true;// 261142
	//TAR228754 Reset this flag to indicate that we get out Cancel item screens
    bIsFromVoidApproval = false;
	//TAR228754
	bCancelTransApproval = false; // TAR 298096
	// resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
	SAClearSecurityCondition();	
	//218558 set scanner flag to re-enabled only in state that isn't handling EAS automatically
	trace(L2, _T("bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;
	//+ TAR 384891 and TAR 386017
	if(fInMultiSelectPickList && (ps.GetPicklistItemsSold() > 0))
	{
	    ps.SetPicklistItemsSold(0);
	    ps.SetPicklistItemsIncludeVoidItems(0);
	}
	//- TAR 384891 and TAR 386017
    //tar 410858: the following line clears security exception if there is one at VoidTransApproval
    //OverrideSecurityException(lpszAuthorizationType, KEY_EXCEPTION_OPERATION_CLEAR); //410858

	trace(L2, _T("-SMVoidTransApprovalBase::OnRAPClearSecurityBtn"));
	return TBCancelTransaction(true);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMVoidTransApprovalBase::OnWtIncreaseNotAllowed()	 
{
	setAnchorState(BES_VOIDTRANSAPPROVAL);
	return VoidingWtIncrease(BES_VOIDTRANSAPPROVAL, BES_TAKERECEIPT);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMVoidTransApprovalBase::OnWtDecrease()
{
	trace(L2, _T("SMVoidTransApprovalBase::OnWtDecrease")); //Tar198284 more trace
	return VoidingWtDecrease();

}


//+TAR 292855  Modify ClearSecurityViolationLight() for this state such that it won't restore trilights 
// to last saved state since the transaction is already cancelled.   
///////////////////////////////////////////////////////////////////
void SMVoidTransApprovalBase::ClearSecurityViolationLight(void)
{
		trace(L2, _T("+SMVoidTransApprovalBase::ClearSecurityViolationLight"));
		
		// Turn off red security light.
		dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
					DM_TRICOLORLIGHT_OFF,
				DM_TRICOLORLIGHT_NORMALREQUEST);
		dm.turnOffTriColorLight();
	

		if (dm.GetCoinsOrBillsVeryLow() || dm.GetPrinterPaperLow())
		{
			dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
								DM_TRICOLORLIGHT_BLINK_1HZ,
								DM_TRICOLORLIGHT_NORMALREQUEST);
		}
		else
		{
			// If inside the transaction, turn on the green light
			if(fInSignedOn || fInSignedOff || fStateAssist || fSAInTransaction)
				dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
								 DM_TRICOLORLIGHT_ON,
								 DM_TRICOLORLIGHT_NORMALREQUEST);
		}

		trace(L2, _T("-SMVoidTransApprovalBase::ClearSecurityViolationLight"));
}
//-TAR 292855 

SMStateBase *SMVoidTransApprovalBase::HandleCancelTrasaction(CString csBttID)
{
    if(m_sButton == _T("cYes"))
	{
		return OnRAPClearSecurityBtn(_T("RAP"));
	}
	else if(m_sButton == _T("cNo"))
	{
	    trace(L6, _T("ra.OnNoRAPDataNeeded()"));
        ra.OnNoRAPDataNeeded();

		//TAR228754 Reset this flag to indicate that we get out Cancel item screens
		bIsFromVoidApproval = false;
		//TAR228754
		bEASReEnableScanner = true; //TAR286414: reenable the scanner
		SASendSCOTInput( TRANSACTION_CONTINUE );
		ra.RequestEnableAssistMode(true);
	    if(getAnchorState() == BES_VOIDITEM)
		{
			return setAndCreateAnchorState(BES_SCANANDBAG);
		}
		else
		{
			return createAnchorState();  // scan and bag or coupon entry
		}
	}

	// +TAR 391593
	ASSERT(FALSE);
	return STATE_NULL;
	// -TAR 391593
}
