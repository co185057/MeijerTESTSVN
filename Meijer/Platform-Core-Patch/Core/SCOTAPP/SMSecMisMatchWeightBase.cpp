//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecMisMatchWeightBase.CPP
//
// TITLE: Class implementation for Mismatched Weight base state
//
// AUTHOR: Tara Duncan, Thanhhieu Pham, Leila Parker
//
/////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecMisMatchWeight.h" 
#include "SMScaleBroken.h"          // Reset scale
#include "SMSecBagBoxThreshold.h"
#include "SMSecUnexpectedDecrease.h"
#include "MPProcedures.h"			// RFC 233322
#include "SMRAPDataNeeded.h"	    //TAR 256154

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SMSecMisMatchWeight")

PSTEXT nBtn1;
DLLEXPORT extern HWND hw;           // RFC 233322
DLLEXPORT extern MPProcedures mp;   // RFC 233322

IMPLEMENT_DYNCREATE(SMSecMisMatchWeightBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

SMSecMisMatchWeightBase *SMSecMisMatchWeightBase::m_pMisMatch = NULL; //TAR379130/379131
//////////////////////////////////////////
SMSecMisMatchWeightBase::SMSecMisMatchWeightBase(const BEST prevState, const BEST approvedNextState)
: csCurContext(EMPTY_STRING)
{
  m_PrevState = prevState;			// prev state = BES_BAGANDEAS
  m_NextState = approvedNextState;	// next state = BES_SCANANDBAG (BES_TRANSPORTITEM for Takeaway Belt)
  IMPLEMENT_TIMESTAMP
}
///////////////////////////////////////////////////////////////////////////
SMSecMisMatchWeightBase::SMSecMisMatchWeightBase()
: csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
}

static bool bButton1Processed = false; //393994
////////////////////////////////////////
SMStateBase  *SMSecMisMatchWeightBase::Deliver(const BEST argPrevState, const BEST argApprovedNextState)
{
  m_PrevState = argPrevState;
  m_NextState = argApprovedNextState;
  return this;
}
////////////////////////////////////////
bool SMSecMisMatchWeightBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, both ints
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMSecMisMatchWeightBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver((BEST)DLParamDWORD(dlParameterList),    
                (BEST)DLParamDWORD(dlParameterList));   
        return true;
    }
    else
    {
        trace(L2, _T("+SMSecMisMatchWeightBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}
///////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSecMisMatchWeightBase::Initialize(void)
{
	ra.RequestEnableAssistMode(false);  //Tar 238294
    csCurContext = _T("SecMisMatchWeight");  //  Tar 238987
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, csCurContext );

	// SR682-Show/hide Weight Expected, Weight Observed and Number Of Items Sold under Weight Mismatch screen.
	if(co.fOperationsShowWeightInfoOnLane == true)
		ps.ShowCMWeightNumber(g_csExpectedWeight, g_csDeltaWeight, lItems);
	else
		ps.HideCMWeightNumber();

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
		if ( secEventParms.m_bMultiPackItem ) // +TAR 256154
		{
		cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("MultiPackItem"));
	    m_View = DATANEEDED_MULTIPACKITEM;
	    CString csDesc, csSecInfo;
	    csDesc.Format(ps.GetPSText(MSG_MAXIMUM_WEIGHT_ALLOWANCE_EXCEEDED,SCOT_LANGUAGE_PRIMARY));
	    csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), 
		     csDesc, ps.GetPSText(MSG_RA_MULTIPACKITEM, SCOT_LANGUAGE_PRIMARY), 
		     ps.GetPSText(MSG_RA_MULTIPACKITEM, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""));
	    ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
		ra.RequestEnableAssistMode(true);  
		SAEnterSecurityCondition();
		
		} // -TAR 256154
		else{
		//The text here is just the name of the section in scotdataentry.ini.  Don't need to be in SCOTMSGE.DAT
		cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("IncorrectItemWeight"));
		m_View = DATANEEDED_MISMATCHWEIGHT;
		CString csInstruction, csDesc;
		CString csExpectedWts = GetExpectedWts();
		csDesc.Format(ps.GetPSText(RA_DESCRIPTIONINFO, SCOT_LANGUAGE_PRIMARY), io.d.csDescription, csScaleUnits, secEventParms.m_csDeltaWeight, csScaleUnits, csExpectedWts);	// see if need to wait on EAS because returning to the bag and EAS screen LPM022002 TAR096236
		if ( secEventParms.m_bWeight2x )
		{
			csInstruction.Format(ps.GetPSText(RA_WEIGHTINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_DOUBLEWEIGHT, SCOT_LANGUAGE_PRIMARY), csExpectedWts, csScaleUnits, secEventParms.m_csDeltaWeight, csScaleUnits);
		}
		else
		{
			csInstruction.Format(ps.GetPSText(RA_WEIGHTINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTMISMATCH, SCOT_LANGUAGE_PRIMARY), csExpectedWts, csScaleUnits, secEventParms.m_csDeltaWeight, csScaleUnits);
		}
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
	}
	return STATE(SecOperatorPWState)::Initialize();
}

///////////////////////////////////////////////////////////////////////////
void SMSecMisMatchWeightBase::showScreen(bool passwordVerified)
{

	nTimeOut = co.getTimeOutPeriodicSecurityAudio();	// RFC 233322

	if (passwordVerified)
		return;
    
    CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMSecMisMatchWeightBase::showScreen"));
    
    if (inputMode == PROMPTING_FOR_OPERATOR)
    {
		STATE(SecOperatorPWState)::ShowEnterID();  //SR93 Biometrics support			
        ps.SetCMLeadthruText( TXT_ENTER_ID );
        ps.AllowInput( MAX_DIGITS_OPERATOR, false, false );
        ps.ShowCMFrame( false );   
        
        return;
    }
    else
    {  // Display Wait for Approval....   
        // NewUI
        ps.SetCMFrame(_T("SecMisMatchWeight"));
        
        //RFC232801
        ps.SetCMBackground(BMP_ASSISTANCENEEDEDITEMDETAILS);
		//TAR379130/379131
		if (dm.fStateTakeawayBelt && ( co.getTimeOutDisplayIncreaseMisMatchScreenTimer() !=0 ) )
		{
			SetTimer(hw, SHOWSCREEN, co.getTimeOutDisplayIncreaseMisMatchScreenTimer()*1000, &ShowScreenTimedOut);	
		}
		else if (!co.fOperationsSuppressAudioInMismatch)				
        {
            DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
        }
        COleVariant v = VARIANT_FALSE;;		// hide the store login button
        if(nEscalationRAPTimeOut == 0)		// RFC 233322
        {
            v = VARIANT_TRUE;				// show the store mode button
        }
        //TAR 256154
        if (secEventParms.m_bMultiPackItem)
        {
            v = VARIANT_FALSE;
        }
        m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, v);
    }

	//tar 395320 +
	//only show the instruction if we are about to show the screen
	if (!(dm.fStateTakeawayBelt && (co.getTimeOutDisplayIncreaseMisMatchScreenTimer() !=0 )))
	{
		//RFC 232801 
		CString csWork;
		ps.SetCMLeadthruText(LTT_WAITFORASSISTANCE);
		if ( secEventParms.m_bWeight2x )
		{
			csWork = ps.GetPSText(MSG_CHECKITEMWEIGHTDOUBLE);	
		}
		else
		{
			csWork = ps.GetPSText(MSG_CHECKITEMWEIGHT);	
		}
		ps.Message12(csWork, true);
	}//tar 395320 -
	
	COleVariant v;
	if (dm.fStateTakeawayBelt)
	{
		v = VARIANT_FALSE;
		m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
	}
	else
	{
		if(co.fOperationsAllowContainerBtnForMismatching && !secEventParms.m_bMultiPackItem)
		{
   			nBtn1 = BTT_BAGBOX;
			v = VARIANT_TRUE;
			ps.SetButtonText(_T("CMButton1Med"), nBtn1);
		}
		else
		{
			v = VARIANT_FALSE;
		}
		m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
		// End NewUI
	}
	SAWLDBScannerDisable();			// Disable the scanner
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	trace(L7, _T("+ra.OnTransactionState, Itemization State"));
	ra.OnSetTransactionState(RA_STATE_SECURITY_VIOLATION);
	trace(L7, _T("-ra.OnTransactionState"));
	//TAR379130/379131
	if (dm.fStateTakeawayBelt && (co.getTimeOutDisplayIncreaseMisMatchScreenTimer() !=0 ))
	{
		trace(L6, _T("Do not display the screen"));
	}
	else
	{
		ps.ShowCMFrame(false);
	}
	//ps.ShowCMFrame(false);	
}

///////////////////////////////////////////////////////////////////////////
void SMSecMisMatchWeightBase::UnInitialize(void)
{
	bButton1Processed = false; //393994
	ra.RequestEnableAssistMode(true);  //Tar 238294
	// RFC 233322 RJF 041603 ensure that the RAP escalation timer has stopped
    KillTimer(hw,RAP_ESCALATION);  
    KillTimer(hw,MISMATCH); // 3.2 patch B - RFC 275449    
	KillTimer(hw, SHOWSCREEN);//TAR379130/379131

    // NewUI removed PSNotification
	DMSaySecurity( SEC_NONE, _T("blank.wav") );			//to interrupt the wave file if it's playing 153796
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, csCurContext );
}

///////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSecMisMatchWeightBase::PSButton1(void) //Bag/Box container
{
	//393994+: this button can be clicked more than one time quickly
	//only the first click should be processed:
    if ( bButton1Processed )
	{
		trace(L6, _T("SMSecMisMatchWeightBase Bag/Box btn pressed but is ignored"));
		return STATE_NULL;
	}//393994-
	// Tar 392821
	if (fInValidOperatorPassword)  
	{	return STATE(SecOperatorPWState)::PSButton1();	}
		
	trace(L6, _T("+SMSecMisMatchWeightBase Bag/Box btn pressed"));
	DMSaySecurity( SEC_NONE, _T("blank.wav") );			// to interrupt the wave file 

	CKeyValueParmList parmList;
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_BAG_BOX);
	_bstr_t bstrParmList((const _TCHAR *)parmList);
	BSTR bstrResult;
	bstrResult = NULL;
	SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
	SysFreeString(bstrResult);
	bButton1Processed = true; //393994
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSecMisMatchWeightBase::PSButtonGoBack(void) // store login
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
// Method:		void SMSecMisMatchWeightBase::EscalationTimedOut
//
// Purpose:		handles passing timeout event to Escalation2RAP using the current state pointer
//
// Parameters:	none
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
VOID CALLBACK SMSecMisMatchWeightBase::EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	mp.sCurr->Escalation2RAP();
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMSecMisMatchWeightBase::Escalation2RAP
//
// Purpose:		2nd timeout routine handling the delayed security notification
//
// Parameters:	none
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
void SMSecMisMatchWeightBase::Escalation2RAP()
{   // RFC 233322 - separate timer for escalation to RAP
    KillTimer(hw,RAP_ESCALATION);       // ensure that the RAP escalation timer has stopped
	//TAR240514 CHU reset this so if we come back from EnterPWD or EnterID we show
	//"store login" button.
	nEscalationRAPTimeOut = 0;
	if ( secEventParms.m_bMultiPackItem ) // +TAR 256154
    {
		cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("MultiPackItem"));
	    m_View = DATANEEDED_MULTIPACKITEM;
	    CString csDesc, csSecInfo;
	    csDesc.Format(ps.GetPSText(MSG_MAXIMUM_WEIGHT_ALLOWANCE_EXCEEDED,SCOT_LANGUAGE_PRIMARY));
	    csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), 
		     csDesc, ps.GetPSText(MSG_RA_MULTIPACKITEM, SCOT_LANGUAGE_PRIMARY), 
		     ps.GetPSText(MSG_RA_MULTIPACKITEM, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""));
	    ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
		ra.RequestEnableAssistMode(true);  
		SAEnterSecurityCondition();
    } // - TAR 256154
	else{
	//The text here is just the name of the section in scotdataentry.ini.  Don't need to be in SCOTMSGE.DAT
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("IncorrectItemWeight"));
	m_View = DATANEEDED_MISMATCHWEIGHT;
	CString csInstruction, csDesc;
	CString csExpectedWts = GetExpectedWts();
	csDesc.Format(ps.GetPSText(RA_DESCRIPTIONINFO, SCOT_LANGUAGE_PRIMARY), io.d.csDescription, csScaleUnits, secEventParms.m_csDeltaWeight, csScaleUnits, csExpectedWts);	// see if need to wait on EAS because returning to the bag and EAS screen LPM022002 TAR096236
	if ( secEventParms.m_bWeight2x )
	{
		csInstruction.Format(ps.GetPSText(RA_WEIGHTINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_DOUBLEWEIGHT, SCOT_LANGUAGE_PRIMARY), csExpectedWts, csScaleUnits, secEventParms.m_csDeltaWeight, csScaleUnits);
	}
	else
	{
		csInstruction.Format(ps.GetPSText(RA_WEIGHTINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTMISMATCH, SCOT_LANGUAGE_PRIMARY), csExpectedWts, csScaleUnits, secEventParms.m_csDeltaWeight, csScaleUnits);
	}
	ra.RequestEnableAssistMode(true);  //Tar 238294

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
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecMisMatchWeightBase::TimedOut(void) // timeout
{
	if ( secEventParms.m_bMultiPackItem ) // +TAR 256154
    {

       trace(L2, _T("bEASReEnableScanner is set to true."));
	   bEASReEnableScanner = true;
	   CString csTemp, sDescription;
	   csTemp = ps.GetPSText(MSG_MAXIMUM_WEIGHT_ALLOWANCE_EXCEEDED, SCOT_LANGUAGE_PRIMARY) + _T(" ");
       CREATE_AND_DISPENSE(RAPDataNeeded)(csTemp,DATANEEDED_MULTIPACKITEM);
	   
	} // -TAR 256154
	if (!co.fOperationsSuppressAudioInMismatch)

	{
		//RFC232801
		DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
	}
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecMisMatchWeightBase::stateAfterPasswordVerify()
{
	//TAR229622 030303 CHU
	//After password verified, scanner is left enabled. It is possible to get a scan
	//inside of here. The scan would put ScotApp and TB, or ScotApp and TAB out of sync
	//So we disable the scanner now
	SAWLDBScannerDisable();

	//218558 set scanner flag to re-enabled only in state that isn't handling EAS automatically
	trace(L2, _T("bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;
	g_bWtExpected = false;	//We should never expect wt if button is pressed
	if (io.d.csItemCode != _T("") ) {  g_bOkToUpdate = true;  }
	//Tar 200873 BZ & AB 04/12/02: Use RAPimportanceTally to update WLDB
	g_lRAPApprovedUpdate = true;	

	LogWLDBWeightMismatch(g_csActualWeight, g_csExpectedWeight, g_lTolerance);

    //TAR234536 We don't want to updateWLDB since approved weight could include the
	//weight increased during Attract
	if ( !m_bScanOnAttractAtQuietMode ) 
    {
        // NewSec - for scanned items clear and update, but for keyed in or lookup 
	    // items just clear
	    if (io.cScanType != 0)
	    {
		    OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR_WITH_UPDATE);
	    }
	    else 
	    {
		    OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR);
	    }
    }

	// resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
	SAClearSecurityCondition();		// must call this before setting the g_bWtExpected back to false!!! LPM091202 213936

	ps.SetLanguage(m_customerLanguage); //tar 192275
	lSARedNeedingApproval = false;   // TAR 197015 Clear alert flag.
	//if(dm.fStateTakeawayBelt)	// is on a take away system
	//	ss.SetLastGoodWeight(g_lBeginningTransLGW);
	//else

	//TAR234536+
	if ( m_bScanOnAttractAtQuietMode )  
	{
		m_bScanOnAttractAtQuietMode = false;
	}
	//TAR234536-

	g_lNumberOfWLDBEntriesThisItem = -1;	// reset the number of WLDB entries

	//TAR331553+
	//return setAndCreateAnchorState(m_NextState);	// go back to state after approval - ScanAndBag
	return STATE_NULL;    //wait SecurityManager to fire SmOnItemOK event
	//TAR331553-
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecMisMatchWeightBase::OnBackToLGW(void)
{
	// TAB re-enable scanner and clear Wait For Approval State
	TABReEnableScannerAndClearWaitForApprovalState();

	//If get here, must have come from the bag&eas screen, so we are still waiting on a weight
	// see if need to wait on EAS because returning to the bag and EAS screen LPM022002 TAR096236
	// so if the EAS event has not been received, 
	//if (!bEASReEnableScanner)				// this is set to true once the EAS event is received
	//	g_bMatchedWtWaitingOnEAS = true;	// set the waiting on EAS back to true when leaving this state.

	//CHU+ 050703 for TAB: OnBackToLGW could mean: LGW has virtual zero or LGW has the item weight
	//If LGW has virtual zero: example, put wrong item on belt first, then remove it. At this 
	//moment, LGW has virtual zero since nothing on the belt. For this case, we don't want to flush
	//the belt and don't want to tell TAB this is a good weight
/*  long tempBOTLGWt1 = g_lBeginningTransLGW + co.nSmartScaleSecScaleEOTTolerance; // beginning of trans wt + tolerance
 long tempBOTLGWt2 = g_lBeginningTransLGW - co.nSmartScaleSecScaleEOTTolerance; // beginning of trans wt - tolerance

	if (  dm.fStateTakeawayBelt  && (g_lCTW <= tempBOTLGWt1) && (g_lCTW >= tempBOTLGWt2)) 
	{
		trace(L6, _T("SMSecMisMatchWeightBase::SAClearSecurityCondition - RAP view = %d"), m_View);
		ClearSecurityViolationLight();//End of Tar 209034 & 209045

		//Clear RAP Approval 
		trace(L2, _T("SMSecMisMatchWeightBase: Calling ra.OnNoRAPDataNeeded()"));
		ra.OnNoRAPDataNeeded();  
		
		return createState(m_PrevState);

	}*/
	//CHU-


	// resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
    SAClearSecurityCondition();	

	// if matched waiting on EAS event, then SAClearSecurityCondition just reset bEasReEnableScanner, need to reset it back... LPM100402 BZ 214953
	//if (g_bMatchedWtWaitingOnEAS)
	//	bEASReEnableScanner = false;

	//TAR 356460 BEGIN - BRC
	if(g_bOnItemOK)
	{
		return OnMatchedWt();
	}
	//TAR 356460 END

	return createState(m_PrevState);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecMisMatchWeightBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
	//why don't we just call the state after password in the current state? LPM   
	trace(L2, _T("+SMStateBase::OnRAPClearSecurityBtn"));

	DMSaySecurity( SEC_NONE, _T("blank.wav") );			//to interrupt the wave file if it's playing 
	//218558 set scanner flag to re-enabled only in state that isn't handling EAS automatically
	trace(L2, _T("bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;
	g_bWtExpected = false;			// We should never expect wt if button is pressed

    // NewSec
    OverrideSecurityException(lpszAuthorizationType, KEY_EXCEPTION_OPERATION_CLEAR);

	// resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
    SAClearSecurityCondition();		// must call this before setting the g_bWtExpected back to false!!! LPM091202 213936


	trace(L2, _T("-SMStateBase::OnRAPClearSecurityBtn"));
	return setAndCreateAnchorState(m_NextState);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecMisMatchWeightBase::OnWtIncreaseNotAllowed()	 
{
	// SR682-Refresh Weight Observed in the event of a weight change.
	if(co.fOperationsShowWeightInfoOnLane == true)
		ps.ShowCMWeightNumber(g_csExpectedWeight, g_csDeltaWeight, lItems);

	if ( secEventParms.m_bMultiPackItem ) //TAR 256154
	{
		 //do not show the bag/box and store login button 
	   COleVariant v;
	   v = VARIANT_FALSE;
	   m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
       m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, v);
   	   
	   cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("MultiPackItem"));
	   m_View = DATANEEDED_MULTIPACKITEM;
	   CString csDesc, csSecInfo;
	   csDesc.Format(ps.GetPSText(MSG_MAXIMUM_WEIGHT_ALLOWANCE_EXCEEDED,SCOT_LANGUAGE_PRIMARY));
	   csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), 
		     csDesc, ps.GetPSText(MSG_RA_MULTIPACKITEM, SCOT_LANGUAGE_PRIMARY), 
		     ps.GetPSText(MSG_RA_MULTIPACKITEM, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""));
	   ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
	   ra.RequestEnableAssistMode(true);  
	   return STATE_NULL;

	}	//TAR 256154 end 
	//The text here is just the name of the section in scotdataentry.ini.  Don't need to be in SCOTMSGE.DAT
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("IncorrectItemWeight"));
	m_View = DATANEEDED_MISMATCHWEIGHT;
	CString csInstruction, csDesc;
	CString csExpectedWts = GetExpectedWts();
	csDesc.Format(ps.GetPSText(RA_DESCRIPTIONINFO, SCOT_LANGUAGE_PRIMARY), io.d.csDescription, csScaleUnits, secEventParms.m_csDeltaWeight, csScaleUnits, csExpectedWts);
	CString csWork;
	if ( secEventParms.m_bWeight2x )
	{
		csWork = ps.GetPSText(MSG_CHECKITEMWEIGHTDOUBLE);	
		csInstruction.Format(ps.GetPSText(RA_WEIGHTINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_DOUBLEWEIGHT, SCOT_LANGUAGE_PRIMARY), csExpectedWts, csScaleUnits, secEventParms.m_csDeltaWeight, csScaleUnits);
	}
	else
	{
		csWork = ps.GetPSText(MSG_CHECKITEMWEIGHT);	
		csInstruction.Format(ps.GetPSText(RA_WEIGHTINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTMISMATCH, SCOT_LANGUAGE_PRIMARY), csExpectedWts, csScaleUnits, secEventParms.m_csDeltaWeight, csScaleUnits);
	}
	ps.Message12(csWork, true);
	ra.OnRAPDataNeeded(csInstruction, cDataEntry.ToString(), m_View);	//Tar 200873 04/19/02: Update RAP display window with weight change
	ra.RequestEnableAssistMode(true); //Tar 238294
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecMisMatchWeightBase::OnMatchedWt()
{
	// 218558 Added override function.  EAS motion sensor received in the wrong order must behave differently LPM103002
	if (!m_bEASHappened )	// the matched wt IS waiting on an EAS event
	{
		trace(L2, _T("SMSecMisMatchWeightBase::OnMatchedWt, waiting for EAS motion sensor, returning to BagAndEAS (Bag Item)"));
		return setAndCreateAnchorState(BES_BAGANDEAS);
	}
	else
	{
		trace(L2, _T("SMSecMisMatchWeightBase::OnMatchedWt, Calling base behavior."));
		return SMStateBase::OnMatchedWt();
	}
}

//TAR379130/379131+
VOID CALLBACK SMSecMisMatchWeightBase::ShowScreenTimedOut ( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	trace(L6, _T("+SMSecMisMatchWeightBase::ShowScreenTimedOut "));
	m_pMisMatch->HandleShowScreenTimedOut();
	trace(L6, _T("-SMSecMisMatchWeightBase::ShowScreenTimedOutt ")); 
}

void SMSecMisMatchWeightBase::HandleShowScreenTimedOut()
{
	trace(L6, _T("+SMSecMisMatchWeightBase::HandleShowScreenTimedOut"));
	KillTimer(hw, SHOWSCREEN);
	
	//tar395320 +
	if (dm.fStateTakeawayBelt && (co.getTimeOutDisplayIncreaseMisMatchScreenTimer() !=0 ))
	{
		trace(L6, _T("DisplayIncreaseMisMatchScreenTimer != 0.. delayed display of screen at TAB"));
		CString csWork;
		if ( secEventParms.m_bWeight2x )
		{
			csWork = ps.GetPSText(MSG_CHECKITEMWEIGHTDOUBLE);	
		}
		else
		{
			csWork = ps.GetPSText(MSG_CHECKITEMWEIGHT);	
		}
		ps.Message12(csWork, true);
	}//tar395320 -

	if (!co.fOperationsSuppressAudioInMismatch)				
    {

		SMStateBase::DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
		//SMStateBase::DMSayPhrase(SELECTLANGUAGE);
		trace(L6, _T("+SMSecMisMatchWeightBase::HandleShowScreenTimedOut, DMSaySecurity"));
    }
	trace(L6, _T("Display the screen"));
	ps.ShowCMFrame(false);
	trace(L6, _T("-SMSecMisMatchWeightBase::HandleShowScreenTimedOut"));
}
//TAR379130/379131-
