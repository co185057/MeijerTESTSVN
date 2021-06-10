//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMUnDeActivatedItemApprovalBase.cpp
//
// TITLE: Class implementation for Assistance Needed for UnDeActivated item state
//
// AUTHOR:  Anurag Mishra
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                              // MS MFC always needed first
#include <afxtempl.h>                            // MS MFC template header

#include "Common.h"                              // Common includes
#include "SMState.h"                             // Base state
#include "SMUnDeActivatedItemApprovalBase.h"
#include "SMScaleBroken.h"                       // Reset scale

#include "SMSecUnExpectedIncrease.h"
#include "SMScanAndBag.h"

#define COMP_ID ID_SM                            // Base state component
#define T_ID    _T("UnDeActivatedItemApproval")

IMPLEMENT_DYNCREATE(SMUnDeActivatedItemApprovalBase, CObject) // MFC Runtime class/object information

DEFINE_TIMESTAMP


//////////////////////////////////////////
SMUnDeActivatedItemApprovalBase::SMUnDeActivatedItemApprovalBase(const BEST prevState, const BEST approvedNextState)
: Prevstate(prevState),
  Nextstate(approvedNextState)
{
  IMPLEMENT_TIMESTAMP
}
//////////////////////////////////////////
SMUnDeActivatedItemApprovalBase::SMUnDeActivatedItemApprovalBase()
{
 IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMUnDeActivatedItemApprovalBase::Deliver(const BEST argPrevState, const BEST argApprovedNextState)
{
  Prevstate = argPrevState;
  Nextstate = argApprovedNextState;
  return this;
}
////////////////////////////////////////
bool SMUnDeActivatedItemApprovalBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, both ints
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMUnDeActivatedItemApprovalBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver(DLParamDWORD(dlParameterList),    // Machine Name
                DLParamDWORD(dlParameterList));   // Remote Context
        return true;
    }
    else
    {
        trace(L2, _T("+SMUnDeActivatedItemApprovalBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}
///////////////////////////////////////
SMStateBase  *SMUnDeActivatedItemApprovalBase::Initialize(void)
{
    m_tempView = -1;
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("UnDeActivatedItemApproval") );
    // Increment the number of Undeactivated item count
	fUnDeActivatedItem = true;
	// Save current light and Turn the red light RED
	SAEnterSecurityCondition();

    trace(L7, _T("+ra.OnRAPDataNeeded, undeactivated item on belt\n"));
	CString sDescription;
	sDescription = ps.GetPSText(RA_TC_UNDEACTIVATEDITEM, SCOT_LANGUAGE_PRIMARY);
	// send a DataNeeded Approval to RAP
    cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("UnDeActivatedItemOnTABeltDetected"));
	// Save the previous view, for example if happened at price needed intervention
	// It will save m_tempView as DATANEEDED_PRICEPROMPT 
	m_tempView = m_View;
    m_View = DATANEEDED_UNDEACTIVATEDITEM;
    CString csSecInfo;
    csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),
	   sDescription, //Description
	   ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY),    //Button
	   getSCOTStateName(),
	   co.IsSecurityDisableOn()==true ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""),
	   ps.GetPSText(RA_THIRDLINE_UNDEACTIVATED, SCOT_LANGUAGE_PRIMARY),  //Status line
	   _T(""));
    ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
    trace(L7, _T("-ra.OnRAPDataNeeded, undeactivated item on belt\n"));

	return STATE(OperatorPasswordState)::Initialize();
}

////////////////////////////////////////////
void SMUnDeActivatedItemApprovalBase::showScreen(bool passwordVerified)
{
	CString csWork;
	nTimeOut = co.getTimeEASDeactivationScreenTimer();
	if (passwordVerified)
		return;


		if (inputMode == PROMPTING_FOR_OPERATOR)
		{
			STATE(OperatorPasswordState)::ShowEnterID();	 //SR93 Biometrics support		
			ps.SetCMLeadthruText( TXT_ENTER_ID );			
			ps.AllowInput( MAX_DIGITS_OPERATOR, false, false );
			ps.ShowCMFrame( false );  

            return;
		}
		else
		{  // Display Wait for Approval....
			DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
			ps.SetCMFrame(_T("UnDeActivatedItemApproval"));
			ps.CMButton(_T("ButtonStoreLogIn"), BTT_STORELOGIN, true);
		}

	ps.SetCMLeadthruText(LTT_WAITFORASSISTANCE);
    // End NewUI

	SAWLDBScannerDisable();			// Disable the scanner
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
	csWork += _T("\n* ") + ps.GetPSText(RA_THIRDLINE_UNDEACTIVATED);
    ps.Message12(csWork, true);
	trace(L7, _T("+ra.OnTransactionState, Itemization State"));
	ra.OnSetTransactionState(RA_STATE_WAIT_FOR_APPROVAL);
	trace(L7, _T("-ra.OnTransactionState"));
	ps.ShowCMFrame(false);
}

//////////////////////////////////////
void SMUnDeActivatedItemApprovalBase::UnInitialize(void)
{
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("UnDeActivatedItemApproval") );
    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing
	ClearSecurityViolationLight();
	// Restore the saved view back.
	m_View = m_tempView;

}

//////////////////////////////////////
SMStateBase *SMUnDeActivatedItemApprovalBase::stateAfterPasswordVerify()
{
	trace(L2, _T("::stateAfterPasswordVerify - bEASReEnableScanner is set to true."));	
	//do same thing that OnRAPClearSecurityBtn() does
	return OnRAPClearSecurityBtn(_T("Lane"));
}

///////////////////////////////////////
SMStateBase  *SMUnDeActivatedItemApprovalBase::PSButtonGoBack(void) // store login
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
					//+SR93 Biometric support
					if (co.m_bOperationsUseFingerPrintDevice)
					{
						FT_Abort();
					}
					//-SR93 Biometric support

					inputMode = SHOW_WAIT_FOR_APPROVAL;
					showScreen(false);
					return STATE_NULL;
				}
		}
	//-TAR193505

    promptForOperator();
	return STATE_NULL;
}

///////////////////////////////////////
SMStateBase *SMUnDeActivatedItemApprovalBase::TimedOut(void) // timeout
{
	DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
    return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMUnDeActivatedItemApprovalBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
	trace(L2, _T("+SMUnDeActivatedItemApprovalBase::OnRAPClearSecurityBtn"));

	fUnDeActivatedItem = false;
	// resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
	SAClearSecurityCondition();	

	//218558 set scanner flag to re-enabled only in state that isn't handling EAS automatically
	trace(L2, _T("bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;

	if(g_bOnItemOK)
	{
		g_bOnItemOK = false;
		if(g_bOnScannerEnable)
		{
			trace(L2, _T("SMUnDeActivatedItemApprovalBase::OnRAPClearSecurityBtn() - Got Item OK and scanner enable message"));
			g_bOnScannerEnable = false;
			CKeyValueParmList parmList;
			BSTR bstrResult = NULL;
			parmList.Clear();
   			CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_FLUSH_ITEM);
			_bstr_t bstrParmList((const _TCHAR *)parmList);
			bstrResult = NULL;
			SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
			return setAndCreateAnchorState(BES_SCANANDBAG);
		}
		else
		{
			trace(L2, _T("SMUnDeActivatedItemApprovalBase::OnRAPClearSecurityBtn() - Got Item OK"));
			return setAndCreateAnchorState(BES_TRANSPORTITEM);
		}
	}

	trace(L2, _T("SMUnDeActivatedItemApprovalBase::OnRAPClearSecurityBtn() - Not get Item OK and scanner enable message"));
    // This should not have been changed.
	//return setAndCreateAnchorState(Nextstate);
	// Based on Feng's comment during code review - STATE_RETURN is not used for non help states LPM040402
	return STATE_RETURN; 
}

///////////////////////////////////////////////////////////////////////////
SMStateBase* SMUnDeActivatedItemApprovalBase::OnUnDeActivatedTheftTag(void)
{
    return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMUnDeActivatedItemApprovalBase::OnMatchedWt()
{

	//If there is WaitForApproval state waiting, clear it
	if (g_pDelay_WaitForApprovalStateChange != NULL)
	{
		CString csStateName = g_pDelay_WaitForApprovalStateChange->GetRuntimeClass()->m_lpszClassName;
		delete g_pDelay_WaitForApprovalStateChange;
		g_pDelay_WaitForApprovalStateChange = NULL;
		trace(L6,_T("Delete the state %s saved in g_pDelay_WaitForApprovalStateChange"),csStateName);
	}
	if(dm.fStateTakeawayBelt)	// is on a take away system, always re-enable the scanner
	{
		trace(L2, _T("bEASReEnableScanner is set to true."));
		bEASReEnableScanner = true;
		if (g_bTBReturnedITEMSOLDforSA)	// if the TB has returned ITEM_SOLD 
		{
			setAnchorState(BES_TRANSPORTITEM);
		}
	}
	else
		setAnchorState(BES_SCANANDBAG);

	return STATE_NULL;
}
