#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecNewItemEntry.h"      
#include "SMSecUnexpectedDecrease.h"
#include "SMSecUnexpectedIncrease.h"
#include "SMScaleBroken.h"          // Reset scale


#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecNewItemEntry")


IMPLEMENT_DYNCREATE(SMSecNewItemEntryBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP


//////////////////////////////////////////
SMSecNewItemEntryBase::SMSecNewItemEntryBase(const BEST prevState, const BEST approvedNextState)
{
  m_PrevState = prevState;
  m_NextState = BES_SECNEWITEMENTRY;
  IMPLEMENT_TIMESTAMP
}
//////////////////////////////////////////
SMSecNewItemEntryBase::SMSecNewItemEntryBase()
{
  IMPLEMENT_TIMESTAMP
}
///////////////////////////////////
////////////////////////////////////////
SMStateBase  *SMSecNewItemEntryBase::Deliver(const BEST argPrevState, const BEST argApprovedNextState)
{
  m_PrevState = argPrevState;
  m_NextState = BES_SECNEWITEMENTRY;
  return this;
}
////////////////////////////////////////
bool SMSecNewItemEntryBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, both ints
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMSecNewItemEntryBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver((BEST)DLParamDWORD(dlParameterList),    
                (BEST)DLParamDWORD(dlParameterList));   
        return true;
    }
    else
    {
        trace(L2, _T("+SMSecNewItemEntryBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}
///////////////////////////////////////
SMStateBase  *SMSecNewItemEntryBase::Initialize(void)
{

	ra.RequestEnableAssistMode(false);

	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SecNewItemEntry") );

	//This text msg is the name of the section in scotdataentry.ini.  Don't need to be in SCOTMSGE.DAT
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("NewWeightItem"));
	m_View = DATANEEDED_NEWITEMENTRY;
	CString csSecInfo, csWeight, csDesc;
	csDesc.Format(ps.GetPSText(MSG_NEW_ITEM_THRESHOLD_EXCEEDED, SCOT_LANGUAGE_PRIMARY));
	csWeight.Format(_T("%s %s"), secEventParms.m_csDeltaWeight, csScaleUnits);//TAR 200109 abs value
	csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_WAITFORASSISTANCE, SCOT_LANGUAGE_PRIMARY), csWeight, ps.GetPSText(RA_NEWWEIGHTITEM, SCOT_LANGUAGE_PRIMARY));
	if( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_OUTSTANDING )
	{
		ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
		SAEnterSecurityCondition();
	}
	else if( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_DELAYED )
	{
	  trace(L7, _T("+ra.OnNormalItem, Message = %s"), csSecInfo);
	  ra.OnNormalItem(csSecInfo);
	  trace(L7, _T("-ra.OnNormalItem"));
	}
	return STATE(SecOperatorPWState)::Initialize();
}

////////////////////////////////////////////
void SMSecNewItemEntryBase::showScreen(bool passwordVerified)
{
    ps.SetPSText(PS_WORK1,ps.GetPSText(BTT_STORELOGIN,SCOT_LANGUAGE_PRIMARY));
    ps.SetButtonText(_T("ButtonStoreLogIn"),PS_WORK1);
	nTimeOut = co.getTimeEASDeactivationScreenTimer();

	if (passwordVerified)
		return;


		if (inputMode == PROMPTING_FOR_OPERATOR)
		{
			STATE(SecOperatorPWState)::ShowEnterID();  //SR93 Biometrics support		
			ps.SetCMLeadthruText( TXT_ENTER_ID );
			ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
			ps.ShowCMFrame( false ); 
            
			return;
		}
		else
		{  // Display Wait for Approval....   
			DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));	// play wave here so it doesn't replay on the key pad screen
            // NewUI
			ps.SetCMFrame(_T("SecNewItemEntry"));
			ps.CMButton(_T("ButtonStoreLogIn"), BTT_STORELOGIN, true);
		}


	CString csWork;
	csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
	csWork += _T("\n* ") + ps.GetPSText(TXT_ITEM_WEIGHT_APPROVAL);

	ps.SetCMLeadthruText(LTT_WAITFORASSISTANCE);
    // NewUI

	SAWLDBScannerDisable();			// Disable the scanner
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	ps.Message12(csWork, true);
	trace(L7, _T("+ra.OnTransactionState, Itemization State"));
	ra.OnSetTransactionState(RA_STATE_SECURITY_VIOLATION);
	trace(L7, _T("-ra.OnTransactionState"));
	ps.ShowCMFrame(false);
}
//////////////////////////////////////
void SMSecNewItemEntryBase::UnInitialize(void)
{
	ra.RequestEnableAssistMode(true);
    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	ClearSecurityViolationLight(); //TAR447986
	g_lNumberOfWLDBEntriesThisItem = -1;	// reset this now because we're done with it
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SecNewItemEntry") );
}

//////////////////////////////////////
SMStateBase *SMSecNewItemEntryBase::stateAfterPasswordVerify()
{
	g_lRAPApprovedUpdate = true;	 // Use RAPimportanceTally after new item weight is approved from lane (the same way works as for RAP approval)
	// TAR 196227 & 233017- Should reset the approval counter here not in the UnInitialize()
	g_lWLDBNotApprovedThisTrans = 0;
	// do same thing as OnRAPClearSecurityBtn() does
	return OnRAPClearSecurityBtn(_T("Lane"));
}

///////////////////////////////////////
SMStateBase *SMSecNewItemEntryBase::TimedOut(void) // timeout
{
	DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
    return STATE_NULL;
}

///////////////////////////////////////
SMStateBase  *SMSecNewItemEntryBase::PSButtonGoBack(void) // store login
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
					showScreen(false);
					return STATE_NULL;  
				}
		}

	//TAR193505-

    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	promptForOperator();
    return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecNewItemEntryBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    // NewSec
    OverrideSecurityException(lpszAuthorizationType, KEY_EXCEPTION_OPERATION_CLEAR);

	return ThresholdViolRAPClearSecBtn(m_PrevState);
}


///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecNewItemEntryBase::OnWtIncreaseNotAllowed()	 
{
	//Tar 202266 BZ & AB 04/21/02 +  Update weight display on RAP for wt changes
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("NewWeightItem"));
	m_View = DATANEEDED_NEWITEMENTRY;
	CString csSecInfo, csWeight, csDesc;
	csDesc.Format(ps.GetPSText(RA_DESCRIPTIONINFO, SCOT_LANGUAGE_PRIMARY), io.d.csDescription, csScaleUnits, secEventParms.m_csDeltaWeight, csScaleUnits, _T("0.0"));
	csWeight.Format(_T("%s %s"), secEventParms.m_csDeltaWeight, csScaleUnits);//TAR 200109 abs value
	csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_WAITFORASSISTANCE, SCOT_LANGUAGE_PRIMARY), csWeight, ps.GetPSText(RA_NEWWEIGHTITEM, SCOT_LANGUAGE_PRIMARY));
	ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
	return STATE(SecOperatorPWState)::Initialize();
	//Tar 202266 - 
}
/*
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecNewItemEntryBase::OnWtDecrease()	 
{
	return ThresholdViolWtDecrease(m_PrevState, BES_SECNEWITEMENTRY);
}
*/

//Tar 204711 start
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecNewItemEntryBase::OnWtDecrease()	 
{
	return ThresholdViolWtDecrease(m_PrevState, BES_SECNEWITEMENTRY);
}


///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecNewItemEntryBase::OnBackToLGW(void)
{
	if( !ra.GetRAPDataPendingCount() )
	{
		//TAR 390026
		BEST nextState;
		if( dm.fStateTakeawayBelt )
		{
			nextState = BES_TRANSPORTITEM;
		}
		else
		{
			nextState = BES_SCANANDBAG;
		}

		return setAndCreateAnchorState(nextState);
		//TAR 390026
	}
	else
	{
	trace(L2, _T("SMSecNewItemEntryBase::OnBackToLGW, returning to anchor state"));
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("NewWeightItem"));
	m_View = DATANEEDED_NEWITEMENTRY;
	CString csSecInfo, csWeight, csDesc;
	csDesc.Format(ps.GetPSText(RA_DESCRIPTIONINFO, SCOT_LANGUAGE_PRIMARY), io.d.csDescription, csScaleUnits, secEventParms.m_csDeltaWeight, csScaleUnits, _T("0.0"));
	csWeight.Format(_T("%s %s"), FmtWtToString(abs(g_lCTW - g_lLGW)), csScaleUnits);//TAR 200109 abs value
	csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_WAITFORASSISTANCE, SCOT_LANGUAGE_PRIMARY), csWeight, ps.GetPSText(RA_NEWWEIGHTITEM, SCOT_LANGUAGE_PRIMARY));
	ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
	return STATE(SecOperatorPWState)::Initialize();
}
}

//Tar 204711 end
