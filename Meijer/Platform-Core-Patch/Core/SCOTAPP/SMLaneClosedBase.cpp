//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMLaneClosedBase.cpp
//
// TITLE: Class implementation for Lane Close state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "USSFCommon.h"
#include "USSFUtility.h"


#include "SMLaneClosed.h"
#include "DMProcedures.h"
#include "SMSystemMessage1.h"
#include "SharedSendScot.h" //dp185016 tar 287778 !this file has been added in
                              //$../development/platform/utility/sendscot/
                              // and will also placed/linked in c:\scot\adk\include
#include "dmx\CashRecyclerWrapper.h"    // Disable of SafePay device.

DLLEXPORT extern DMProcedures dm;   // Global DM object

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("LaneClosedBase")

DLLEXPORT bool bGLOBALFromLaneClose = false; // TAR 195273 true;	// Tar 192351 RF012002

IMPLEMENT_DYNCREATE(SMLaneClosedBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(LaneClosed)


DEFINE_TIMESTAMP

////////////////////////////
SMLaneClosedBase::SMLaneClosedBase()
: csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////
SMStateBase *SMLaneClosedBase::Initialize(void)
{
	USSFutil.DumpSymbolsByElementID();

  //+dp185016 tar 286203
  if (co.IsAttendantModeOn())
  {
    rp.TransactionStart(rp.m_csBusinessDate);
    trace(L6, _T("Exiting Attendant mode"));
  	rp.SetOperator(csOperator);
    co.SetAttendantID(EMPTY_STRING);
    co.SetAttendantMode(false);
	if (dm.fStateTakeawayBelt)
	{
		CKeyValueParmList parmList;
		BSTR bstrResult;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_ASSIST_MODE_STOP);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		bstrResult = NULL;
		SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
	}
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode") );
    rp.ModeChange(rp.m_lAttendantEntryID, ATTENDANT, EXIT);
    DMTakeawayBeltNotify(tabEventAssistModeStop);
	  rp.m_lAttendantEntryID += 1;
    rp.TransactionEnd();
    // ShowTrainingModeOnScreen will update training/attendant text 
    // and show/hide mode indicators. 
    ps.ShowTrainingModeOnScreen(TrainingModeObj.IsTMOn());
  }
  //-dp185016
  if ( !m_bReloadingOptions) //tar 352145
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("LaneClosed") );

	//Begin Tar 209206
  //ra.OnSetTransactionState(RA_STATE_ASSISTNOTOK);
    ra.RequestEnableAssistMode(false);   //TAR 213249
  //End Tar 209206

	csCurContext = _T("LaneClosed");  //TAR279101
	ps.SetCMFrame(csCurContext);

	/******cm150000: Added In Merged Code******/
   // Commented out line for tar 193603
//	setAnchorState(BES_LANECLOSED);
	/******cm150000: End Merged Code***********/
    //Tar 125278
	if( !co.fStateStartupWaitForTB )  //RFC 986	
	    TBLaneClosed(true);     //Enter Lane Closed
  
  if(fPutSCOTinLC)
  {
    //+dp185016 tar 287778
  	TCHAR szSCOTState[100];
    GetPrivateProfileString (SECTION3,KEY80, KEY80, szSCOTState, sizeof(szSCOTState), szIniFilePath);

    CString csWrite = ps.ConvertToEscapeSequence(szSCOTState, _T("\\n"), _T("\n"));
    if(WriteSCOTStatusToFile(csWrite)) 
	  //if(WriteSCOTStatusToFile(_T("Lane Closed"))) 
    //-dp185016
		fPutSCOTinLC = false;
  }

  fStateFreeze = true;						  // stay in this state no matter what!
  nTimeOut = 0;
  bGLOBALFromLaneClose = true; // Tar 192351 RF012002
  m_bIsFromLaneClosed = true;

  dm.saveTriColorLightStatus();
//  dm.saveLaneLightStatus();
  dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                      DM_TRICOLORLIGHT_ON,
                      DM_TRICOLORLIGHT_NORMALREQUEST);
  dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                      DM_TRICOLORLIGHT_OFF,
                      DM_TRICOLORLIGHT_NORMALREQUEST);
  dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
                      DM_TRICOLORLIGHT_OFF,
                      DM_TRICOLORLIGHT_NORMALREQUEST);
  dm.RemoteRelay(DM_MISC_STATE_OFF); // Lane light off

  //awk.2.6.2 start TAR192509
  trace(L7, _T("+ra.OnRAPDataNeeded, Lane Close\n"));
  ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + ps.GetPSText(MSG_RA_LANE_CLOSED, SCOT_LANGUAGE_PRIMARY));
  // send a DataNeeded Approval
  cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("CloseLane"));
  m_View = DATANEEDED_CLOSELANE;
  CString csSecInfo;
  csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),
	  ps.GetPSText(LTT_LANECLOSED, SCOT_LANGUAGE_PRIMARY), //Description
	  ps.GetPSText(RA_BUTTON_LANECLOSED, SCOT_LANGUAGE_PRIMARY), //Button
	  ps.GetPSText(RA_FIRSTLINE_LANECLOSED, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""), _T("")); //Status line
  ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
  trace(L7, _T("-ra.OnRAPDataNeeded, Lane Close\n"));
  //awk.2.6.2 end TAR192509

  appBusy.ClearBusyStatus();
  // Display the Auxilary lane closed screen
  DisplayAuxScreen(AUX_SCREEN_LANE_CLOSED, true);

  // SafePay cash recycler.  Disable the device so that SafePay utilities can 
  // access it (in case of an error, etc.)
  if(dm.m_csCashRecyclerModel == _CASHDISPENSER_SAFEPAY)
  {
	  trace(L6, _T("LANE CLOSE: Disabling SafePay device for maintenance."));
	  dm.SetDeviceEnabledRaw(FALSE, DMCLASS_CASHRECYCLER, 0);
  }
  return STATE(OperatorPasswordState)::Initialize();
}

/////////////////////////////////////
void SMLaneClosedBase::UnInitialize(void)
{
	CCashRecyclerWrapper *pDevice = NULL;

  if ( !m_bReloadingOptions) //tar 352145
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("LaneClosed") );

  //Tar 143172 don't issue lane closed = false unless they press button 1 and successfully log in LPM061300
  //TBLaneClosed(false);   //Leave Lane Closed
  fStateFreeze = false;
  bGLOBALFromLaneClose = false; // Tar 192351 RF012002
  dm.restoreTriColorLight();
//  dm.restoreLaneLight();

  if(dm.m_csCashRecyclerModel == _CASHDISPENSER_SAFEPAY)
  {
 	  trace(L6, _T("LANE CLOSE: Enabling SafePay device."));
	  dm.SetDeviceEnabledRaw(TRUE, DMCLASS_CASHRECYCLER, 0);
  }
}

////////////////////////////////////////////////////
void SMLaneClosedBase::showScreen(bool passwordVerified)
{

  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;

  // This state does not need to reshow if the password
  // was verified, since we'll be leaving now
  if (passwordVerified)
  {
    TBLaneClosed(false);   //TAR# 143172 tell TB to leave lane closed LPM061300
    trace(L6, _T("On NO RAP DATA NEEDED, CLEAR LANE CLOSED VIEW"));
    ra.OnNoRAPDataNeeded();
	SynchronizeTime();
    return;
  }


     if (inputMode == PROMPTING_FOR_OPERATOR)
     { 
		saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
		//+SR93 Biometrics support
		STATE(OperatorPasswordState)::ShowEnterID();	
		//-SR93 Biometrics support

		ps.SetCMLeadthruText(TXT_ENTER_ID);
        ps.AllowInput(MAX_DIGITS_OPERATOR,false,false);
		ps.ShowCMTBText(csTBMessage2Scot);	//thp
        ps.ShowCMFrame(false);
		return;
     }
     else
     {  // Display initial Lane Closed screen.
        // NewUI
        ps.SetCMFrame(_T("LaneClosed"));
		ps.Echo(_T(""));	//TAR # 247620
		ps.SetButtonText(_T("CMButton1StoreLogIn"), BTT_STORELOGIN);
        // End NewUI
		//tar 174918 turn red light off to stop page resend timer
		dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                      DM_TRICOLORLIGHT_OFF,
                      DM_TRICOLORLIGHT_NORMALREQUEST);
  		//then turn red light back on
		dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                      DM_TRICOLORLIGHT_ON,
                      DM_TRICOLORLIGHT_NORMALREQUEST);
     }

  ps.SetCMLeadthruText(LTT_LANECLOSED);
  SAWLDBScannerDisable();	
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  ps.ShowCMFrame(false); 
  // End NewUI
 
  // grey out help button
//  ps.CMButton(IDC_BUTTONHELP,BTT_HELP,false);
}

/////////////////////////////////////////
SMStateBase *SMLaneClosedBase::PSButtonHelp(void)
{
  // can't get help on help
  DMSayPhrase(BTTNINVALID);
  return STATE_NULL;
}

///////////////////////////////////////
SMStateBase *SMLaneClosedBase::PSButtonGoBack(void) // exit help
{

    if (inputMode == PROMPTING_FOR_PASSWORD)
      inputMode = PROMPTING_FOR_OPERATOR;
    else
      inputMode = SHOW_WAIT_FOR_APPROVAL;
	showScreen(false);
  return STATE_NULL;
}


/////////////////////////////////////////////////////
SMStateBase *SMLaneClosedBase::stateAfterPasswordVerify(void)
{
  // Apparently MGV implemented something where if media is low when
  // leaving the Lane Closed state, it would display the media low and 
  // keep you in this state until you fix it.  According to Brian Cooper,
  // we shouldn't do this.  TAR # 120638
#if 0
		CString csList = DMCheckCashCounts(false);
		if (csList==_T(""))
			return STATE_RETURN;

	  return createLevel1Message(csList,            // char *message
                               PS_NOTEXT,         // no canned message
                               PS_NOTEXT,         // no prompt
                               0,                 // no prompt value
                               -1,                // no device
                               false);            // no DM check health needed
#else

  //TAR268138 if cash acceptor is out, we stay here, until it is fixed
  if (co.fTenderCashAllowed)
  {
	  if (lastDMCashAcceptorCode == DM_CASHACCEPTOR_STATUS_TAMPER_OUT || bLaneClosedTamperOut) //casette was out during lane close and it is not back yet  // Tar 279371
    {
		  bLaneClosedTamperOut = false;
      CString csWork = _T("DeviceErrorCashAcceptor.xml:NCR_EBILLSO_CASS_OUT");
	  CREATE_AND_DISPENSE(SystemMessage1)(csWork,      // char *message
                                    PS_NOTEXT,
                                    PS_NOTEXT,	// no prompt
                                    0,					// no value
                                    DMCLASS_CASHACCEPTOR, // DM device
                                    true);     // DM check health after

	}		
  }
   //TAR268138-
  
	//+RFC 986
    if (co.fStateStartupWaitForTB)
	{
       fStateFreeze = false;
       return STATE_NULL;
	} 
    else
	{
       return STATE_RETURN;
	}	
	//-RFC 986

#endif
}

///////////////////////////////////////
SMStateBase *SMLaneClosedBase::PSButton1(void) // Store Log In
{
  dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                      DM_TRICOLORLIGHT_BLINK_1HZ,
                      DM_TRICOLORLIGHT_NORMALREQUEST);

  promptForOperator();
  return STATE_NULL;
}

////////////////////////////////////////
SMStateBase *SMLaneClosedBase::PSCancelKey(void)
{
//TAR#117318 Cancel should be disabled
//if (parentClassCurrentlyHandlingCancelKey())
//    return STATE(OperatorPasswordState)::PSCancelKey();
//  else
//  {
//    DMSayPhrase("KeyPadValid");
//    return PSButton3();
//  }
    return STATE_NULL;
}

//////////////////////////////////////////////////////////////
SMStateBase *SMLaneClosedBase::OnWtIncreaseNotAllowed() //tar 197857
{
	return VoidingWtIncrease(BES_LANECLOSED, BES_LANECLOSED);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMLaneClosedBase::OnWtDecrease() //tar 197857
{
	return ThresholdViolWtDecrease(BES_LANECLOSED, BES_LANECLOSED);
}
