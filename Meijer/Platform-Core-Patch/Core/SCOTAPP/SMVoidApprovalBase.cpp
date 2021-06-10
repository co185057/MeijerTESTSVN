//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoidApprovalBase.CPP
//
// TITLE: Class implementation for Assistance Needed for Voided item state
//
// AUTHOR:  Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMVoidApproval.h"      
#include "SMScaleBroken.h"          // Reset scale
#include "SMSecUnExpectedIncrease.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("VoidApproval")

IMPLEMENT_DYNCREATE(SMVoidApprovalBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMVoidApprovalBase::SMVoidApprovalBase(const BEST prevState, const BEST approvedNextState)
: Prevstate(prevState),
  Nextstate(approvedNextState)
{
  IMPLEMENT_TIMESTAMP
}
//////////////////////////////////////////
SMVoidApprovalBase::SMVoidApprovalBase()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMVoidApprovalBase::Deliver(const BEST argPrevState, const BEST argApprovedNextState)
{
  Prevstate = argPrevState;
  Nextstate = argApprovedNextState;
  return this;
}
////////////////////////////////////////
bool SMVoidApprovalBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, both ints
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMVoidApprovalBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver((BEST)DLParamDWORD(dlParameterList),    
                (BEST)DLParamDWORD(dlParameterList));   
        return true;
    }
    else
    {
        trace(L2, _T("+SMVoidApprovalBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}

///////////////////////////////////////
SMStateBase  *SMVoidApprovalBase::Initialize(void)
{
    ra.RequestEnableAssistMode(false); //Tar 298100

	//The text here is just the name of the section in scotdataentry.ini.  Don't need to be in SCOTMSGE.DAT
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("ApproveVoidedItem"));
	//m_View = DATANEEDED_APPROVEVOIDEDITEM;
	//+RFC 275377
	if ( nTBLastState == TB_TENDERVOIDED )  
	{  
		m_View = DATANEEDED_CANCELLTRANSACTION;  
	}  
	else 
	{  
		m_View = DATANEEDED_APPROVEVOIDEDITEM;
	}  
	//-RFC 275377
	CString csDesc, csSecInfo;
	//TD 04/25/02 TAR 202800
	csDesc.Format(ps.GetPSText(TXT_RA_VOIDED_ITEM_APPROVAL,SCOT_LANGUAGE_PRIMARY) + _T(" %s"), io.d.csDescription);
	csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_VOID_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_VOIDEDITEM, SCOT_LANGUAGE_PRIMARY), co.IsSecurityDisableOn()==true ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), _T(""));
	ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
	// Save current light and Turn the red light RED
	SAEnterSecurityCondition();
	// + TAR 296488
    if (io.lRestCode==3)
	{
		CKeyValueParmList parmList;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_IV_OPERATION, KEY_IV_OPERATION_BEGIN);
		_bstr_t bstrParmList((const _TCHAR *)parmList); 
		SecMgr.VoidItem(lSecMgrCookie, bstrParmList);
	}
	// - TAR 296488
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ApproveVoidedItem") );
	return STATE(OperatorPasswordState)::Initialize();
}

////////////////////////////////////////////
void SMVoidApprovalBase::showScreen(bool passwordVerified)
{
	CString csWork;
	nTimeOut = co.getTimeEASDeactivationScreenTimer();

	if (passwordVerified)
		return;


		if (inputMode == PROMPTING_FOR_OPERATOR)
		{
			STATE(OperatorPasswordState)::ShowEnterID();	//SR93 Biometrics support					
			ps.SetCMLeadthruText( TXT_ENTER_ID );
			ps.AllowInput( MAX_DIGITS_OPERATOR, false, false );
			ps.ShowCMFrame( false );   

			return;
		}
		else
		{  // Display Wait for Approval....   
			DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
			ps.SetCMFrame(_T("VoidApproval"));
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
void SMVoidApprovalBase::UnInitialize(void)
{
    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ApproveVoidedItem") );
    // +TAR 269208
    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IV_OPERATION, KEY_IV_OPERATION_END);
    _bstr_t bstrParmList((const _TCHAR *)parmList); 
    SecMgr.VoidItem(lSecMgrCookie, bstrParmList);
    // -TAR 269208
	if (!bIsFromVoidApproval) // TAR 298095 - clear the light only if we have approved void
	{
		ClearSecurityViolationLight();
	}
   	SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);//TAR367249
    ra.RequestEnableAssistMode(true); //TAR298100
}

//////////////////////////////////////
SMStateBase *SMVoidApprovalBase::stateAfterPasswordVerify()
{
	trace(L2, _T("::stateAfterPasswordVerify - bEASReEnableScanner is set to true."));
	if (io.d.csItemCode != _T("") ) {  g_bOkToUpdate = true;  }

	ps.SetLanguage(m_customerLanguage); //tar 192275

	g_bAllowRemove = false;
  
	//do same thing that OnRAPClearSecurityBtn() does
	return OnRAPClearSecurityBtn(_T("Lane")); 
}

///////////////////////////////////////
SMStateBase  *SMVoidApprovalBase::PSButtonGoBack(void) // store login
{
    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	// TAR196321 Added 193505 to this state LPM022202
	//+TAR193505: At EnterID screen, "go back" should be back to SMSecUnExpectedIncreaseBase screen

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
	//-TAR193505

    promptForOperator();
	return STATE_NULL;
}
  
///////////////////////////////////////
SMStateBase *SMVoidApprovalBase::TimedOut(void) // timeout
{
	DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
    return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMVoidApprovalBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
	trace(L2, _T("+SMVoidApprovalBase::OnRAPClearSecurityBtn"));

	//TAR228754 Reset this flag to indicate that we get out Cancel item screens
	bIsFromVoidApproval = false;
	//TAR228754
	// resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
	SAClearSecurityCondition();	
	//218558 set scanner flag to re-enabled only in state that isn't handling EAS automatically
	trace(L2, _T("bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;

    //tar 410858: the following line clears security exception if there is one at VoidApproval
    //OverrideSecurityException(lpszAuthorizationType, KEY_EXCEPTION_OPERATION_CLEAR); //tar 410858

	if(Nextstate == BES_STATERETURN)
		return STATE_RETURN;

	//Tar 412129
	if(Nextstate == BES_VOIDITEM)
	{
		return setAndCreateAnchorState(BES_SCANANDBAG);
	}
	
    trace(L2, _T("-SMVoidApprovalBase::OnRAPClearSecurityBtn"));
    return setAndCreateAnchorState(Nextstate);  //tar 193667
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMVoidApprovalBase::OnWtIncreaseNotAllowed()	 
{
	setAnchorState(BES_VOIDAPPROVAL);
	return VoidingWtIncrease(Prevstate, Nextstate);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMVoidApprovalBase::OnWtDecrease()
{
	return VoidingWtDecrease();
}
