//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMLaneClosedBase.cpp
//
// TITLE: Class implementation for Lane Close state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#ifndef _CPPUNIT

#include "Common.h"                 // Common includes
#include "USSFCommon.h"
#include "USSFUtility.h"
#include "DeviceErrorLookUpAPI.h"  //SR672


#include "SMLaneClosed.h"
#include "DMProcedures.h"
#include "SMSystemMessage1.h"
#include "SharedSendScot.h" //dp185016 tar 287778 !this file has been added in
                              //$../development/platform/utility/sendscot/
                              // and will also placed/linked in c:\scot\adk\include
#include "dmx\CashRecyclerWrapper.h"    // Disable of SafePay device.

DLLEXPORT extern DMProcedures dm;   // Global DM object
#else //_CPPUNIT
#include "SMLaneClosedBase.h"
#endif //_CPPUNIT

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("LaneClosedBase")

#ifndef _CPPUNIT
DLLEXPORT bool bGLOBALFromLaneClose = false; // TAR 195273 true;	// Tar 192351 RF012002
#endif //_CPPUNIT

IMPLEMENT_DYNCREATE(SMLaneClosedBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(LaneClosed)


DEFINE_TIMESTAMP

#ifndef _CPPUNIT
////////////////////////////
SMLaneClosedBase::SMLaneClosedBase()
: csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
}
#endif //_CPPUNIT

//////////////////////////////////////////
SMStateBase *SMLaneClosedBase::Initialize(void)
{
	
#ifndef _CPPUNIT
	USSFutil.DumpSymbolsByElementID();
	
	// TAR 445003
	m_pAppPSX->SetTransactionVariable(_T("SecurityDisableAllowed"),
		co.fStateSecurityDisableAllowed ? VARIANT_TRUE : VARIANT_FALSE);
		
  //sscop-1601 (migrated from 4.5.1 and G2.5 fixes)
  if (co.GetfStateDualLanguage())
  {
	m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
	ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
   	co.csCMCustomerMessage = ps.GetCustomerMessageDefault();
	CustomerMessage = co.csCMCustomerMessage;
	ps.SetTareListToCurrentLanguage();
	if(gpSCOTAuxDisp)
	gpSCOTAuxDisp->SetLanguage(SCOT_LANGUAGE_PRIMARY);
	// + SSCOP-3753  
	if(m_LanguageUsedPreviously != m_customerLanguage)
	{
		m_LanguageUsedPreviously=m_customerLanguage; 
	}
	// - SSCOP-3753
  }
  //sscop-1601

#endif //_CPPUNIT
  //+dp185016 tar 286203
  if (co.IsAttendantModeOn())
  {
#ifndef _CPPUNIT
    rp.TransactionStart(rp.m_csBusinessDate);
    trace(L6, _T("Exiting Attendant mode"));
  	rp.SetOperator(csOperator);
#endif // _CPPUNIT
    co.SetAttendantID(EMPTY_STRING);
    co.SetAttendantMode(false);
    SetAttendantModeStateInSM(false); //SSCOADK-544
    SetNextGenUIMode();

#ifndef _CPPUNIT
	if (dm.fStateTakeawayBelt)
	{
		CKeyValueParmList parmList;
		BSTR bstrResult;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_ASSIST_MODE_STOP);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		bstrResult = NULL;
		SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
		::SysFreeString(bstrResult); //cs185088 mem
	}
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode") );
    rp.ModeChange(rp.m_lAttendantEntryID, ATTENDANT, EXIT);
    DMTakeawayBeltNotify(tabEventAssistModeStop);
	  rp.m_lAttendantEntryID += 1;
    rp.TransactionEnd();
    // ShowTrainingModeOnScreen will update training/attendant text 
    // and show/hide mode indicators. 
    ps.ShowTrainingModeOnScreen(TrainingModeObj.IsTMOn());
#endif // _CPPUNIT
  }
#ifndef _CPPUNIT
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
  
  // (*) SR898 ====================
  DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                     DM_TRICOLORLIGHT_ON,
                     DM_TRICOLORLIGHT_NORMALREQUEST);
  // (*) SR898 ====================
  DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                     DM_TRICOLORLIGHT_OFF,
                     DM_TRICOLORLIGHT_NORMALREQUEST);
  // (*) SR898 ====================
  DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
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
  
  if(dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_SAFEPAY) != -1) //SR672
  {
	  trace(L6, _T("LANE CLOSE: Disabling SafePay device for maintenance."));
	  dm.SetDeviceEnabledRaw(FALSE, DMCLASS_CASHRECYCLER, 0);
  }
#endif // _CPPUNIT
  return STATE(OperatorPasswordState)::Initialize();
}

#ifndef _CPPUNIT
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

  if(dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_SAFEPAY) != -1) //SR672
  {
 	  trace(L6, _T("LANE CLOSE: Enabling SafePay device."));
	  dm.SetDeviceEnabledRaw(TRUE, DMCLASS_CASHRECYCLER, 0);
  }

  CString sModelName = dm.GetDeviceModelName(DMCLASS_CASHACCEPTOR, 0);

  //TAR 451512
  long rc = 0;
  if (sModelName.Find(_T("BNR")) !=-1 || sModelName.Find(_T("GSR50")) !=-1) //SSCOADK-5916
  {
	  rc = DMCheckHealth(DMCLASS_CASHACCEPTOR);
	  if (rc) //CheckHealth fail
	  {
		long lCode = dm.GetCashAcceptorDeviceStatus();
		mo.PostDM(DM_ERROR, DMCLASS_CASHACCEPTOR, 0, lCode, NULL);
	  }
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
        ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
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
		
		ps.AllowInput(0,false,false);
        // (*) SR898 ====================
        DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                           DM_TRICOLORLIGHT_OFF,
                           DM_TRICOLORLIGHT_NORMALREQUEST);
  		
        //then turn red light back on
		
        // (*) SR898 ====================
        DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
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
                               false,false);            // no DM check health needed  //SR672
#else

  //TAR268138 if cash acceptor is out, we stay here, until it is fixed
  if (co.fTenderCashAllowed)
  {
	  //SR672
	  //if (lastDMCashAcceptorCode == DM_CASHACCEPTOR_STATUS_TAMPER_OUT || bLaneClosedTamperOut) //casette was out during lane close and it is not back yet  // Tar 279371
    if (lastDMCashAcceptorCode == BILL_STATUS_TAMPER_OUT || bLaneClosedTamperOut) //casette was out during lane close and it is not back yet  // Tar 279371
    
	  {
		  bLaneClosedTamperOut = false;
		  //SR672
        //CString csWork = _T("DeviceErrorCashAcceptor.xml:NCR_EBILLSO_CASS_OUT");
		CString csWork, s2(_T(""));
		CString s1=dm.GetDeviceModelName(DMCLASS_CASHACCEPTOR, 0);
		bool bLocalCheckHealth = false;
	    
		csWork = DMGetDeviceErrorHTML(DMCLASS_CASHACCEPTOR, s1, s2, BILL_STATUS_TAMPER_OUT, EXTENDED_RESULT_ANY, 0, NULL, &bLocalCheckHealth, CONTEXT_ANY, co.csPrimaryLanguageCode );
	  CREATE_AND_DISPENSE(SystemMessage1)(csWork,      // char *message
                                    PS_NOTEXT,
                                    PS_NOTEXT,	// no prompt
                                    0,					// no value
                                    DMCLASS_CASHACCEPTOR, // DM device
                                    true,     // DM check health after
									true);   //display HTML

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
       //TAR458686+
		if (ms_bLaneClosedFromRapFlag)
		{
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmAuthorization") );
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAuthorization") );
		}
		//458686-
	    return STATE_RETURN;
	}	
	//-RFC 986

#endif
}

///////////////////////////////////////
SMStateBase *SMLaneClosedBase::PSButton1(void) // Store Log In
{
  // (*) SR898 ====================
  DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
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

#endif // _CPPUNIT

//TAR#445712
SMStateBase * SMLaneClosedBase::DMScanner(void)
{
    trace(L6, _T("SMLaneClosedBase::DMScanner - Disabling scanner"));
	SAWLDBScannerDisable();
    SMStateBase* retState = STATE_NULL;
    #ifndef _CPPUNIT
    retState = SMOperatorPasswordStateBase::DMScanner();
    #endif //_CPPUNIT
    if (fInValidOperatorPassword)
    {
        SAWLDBScannerEnable();
    }
    return retState;
}
//TAR#445712
