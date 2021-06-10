//////////////////////////////////////////////////////////////////////////////////////////////////
//
// TITLE: Class implementation for SMAttractBase State
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMAttract.h"
#include "SMTakeCard.h"
#include "SMScaleBroken.h"
#include "SMOutOfService.h"
#include "SMLaneClosed.h"
#include "SMSecUnexpectedIncrease.h" 
#include "SMWaitForApproval.h"
#include "SMSystemMessage1.h" // TAR 292956
#include "SMPutBagOnScale.h"//SR085

#include "Reporting.h"
#include "MPProcedures.h"			// RFC 276235

#include "DMProcedures.h"
#include "VolumeControl.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("AttractBase")
DLLEXPORT extern HWND hw;
DLLEXPORT extern MPProcedures mp;   // RFC 276235

DEFINE_TIMESTAMP


IMPLEMENT_DYNCREATE(SMAttractBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(Attract)
int SMAttractBase::ms_nNumberOfAutoRunTrans = 0;
long SMAttractBase::ms_nAutoRunTransactionTally = 0;
SMAttractBase *SMAttractBase::m_pAttract = NULL;
//////////////////////
// Constructor
//////////////////////
SMAttractBase::SMAttractBase()
: bGoingToInProgress(false), csCurContext(EMPTY_STRING), m_bLangChange(false),m_bLanguageButtonClicked(false)
{
  IMPLEMENT_TIMESTAMP
  // We have started the application successfully if we get here, so we can
  // reset the counter used to restart SCOTAPP during error conditions.
  // Nothing we can do with the return code.
  AppRestartCount::reset();
  saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
  m_pAttract = this;
}

////////////////////////////////////
// Initialize
////////////////////////////////////
SMStateBase  *SMAttractBase::Initialize(void)
{
  int i;
  m_TimerResponse = 0;			//RFC 276235
  m_bIsFromLaneClosed = false; //TAR 292436
  m_bCashRecyclerStopDeposit = 0;  //reset
  m_bCashChangerFailToDispense = false;
  m_nDMCashPaid = 0;

  g_savedAnchorStateFromAttract = BES_START; //416582

  //395064+
  if(dm.fStateTakeawayBelt)
  {
	  TABstateBeforeTransport = BES_EMPTY_STATE;
  }
  //395064-


  // +SR683
  if (co.IsSecurityDisableOn() && !co.fStateSecurityDisableNoReset)
  {
    //TAR [384803] NOTE: added another if statement so as to not reset security after the reporting state
    if ((this->csLastNonHelpStateName != _T("SMSmReportsMenu")) || (this->csLastNonHelpStateName == _T("SMThankYou")))  //+BEGIN TAR [384803]
	{
	  co.SetSecurityDisable(false);
	  ra.OnEnableSecurity();

	  if(csOperator != _T(""))
	    { rp.SetOperator(csOperator); }

	  rp.ModeChange(rp.m_lAttendantEntryID, SECURITYDISABLED, EXIT);
	}  //-END TAR [384803] by: tl185041
  }
  // -SR683
	
  //+ TAR 356575 - to ensure that if in Attendant Mode and beginning another trx, Sec Mgr doesn't come out of AssistMode
  if(co.IsSecurityBypassOn())
  {
	bool bAttnd = co.IsAttendantModeOn();
	co.SetAttendantMode(false);
	bool bSecurityDisable = co.IsSecurityDisableOn();  //SR683
	co.SetSecurityDisable(false);  //SR683
	
	  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode"));
	  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmAssistMode"));
	
	co.SetAttendantMode(bAttnd);
	co.SetSecurityDisable(bSecurityDisable);  //SR683
  }
  else
  {
	  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("Attract"));
  }
  //- TAR 356575

  // +TAR 309003 - need to set it dynamically since part of Security Mamager's compelete
  // actions, it will clear hold weight at the end of a trsanction. 
  if (co.IsAttendantModeOn() ) // if attendant mode is on - hold security
  {
	CKeyValueParmList parmList;
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_HOLD);
	_bstr_t bstrParmList((const _TCHAR *)parmList);
	BSTR bstrResult;
	bstrResult = NULL;
	SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
	SysFreeString(bstrResult);
  }
  // -TAR 309003
  CString csLReceiptFile = LRECEIPT_FILE;
  CString csLReceiptTmpFile = LRECEIPT_TMP_FILE;

  GET_PARTITION_PATH(LRECEIPT_FILE, csLReceiptFile.GetBuffer(_MAX_PATH));
  GET_PARTITION_PATH(LRECEIPT_TMP_FILE, csLReceiptTmpFile.GetBuffer(_MAX_PATH));

  csLReceiptFile.ReleaseBuffer();
  csLReceiptTmpFile.ReleaseBuffer();

  //Log "healthy" message RFC 262343
  eo.Error(INFO,SM_CAT_STARTUP_INIT, SCOTAPP_ATTRACT_SCREEN_ENTRY, (long) (NCRSM_SNMP_ACTION | NCRSM_REMOTE_LOG_ACTION));

  if(co.fLocaleCashManagementAPPTrackCashCount && dm.m_csCashRecyclerModel.IsEmpty() )
   {
      DMHandleAcceptorHigh();
   }

  // TAR 311471 - Begin
  dm.CashRecyclerResetCounts();
  // TAR 311471 - End
  custInfo.ClearCustomerInfo();
  custInfo.SetPreference(_T("LangSelectionMethod"), _T("None"));
  custInfo.SetPreference(_T("SavePreferences"), _T("?"));

 DisplayHopperSubstitution();

  //TAR 292956 start - if cash acceptor out, go to system message until fixed
  if (co.fTenderCashAllowed)
  {
    if ( DM_CASHACCEPTOR_STATUS_TAMPER_OUT == lastDMCashAcceptorCode )
    //casette was out during POS Startup and it is not back yet
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
  //TAR 292956 end

  // TAR 306397 - Begin
  DMCheckCashCounts(false);		// Ensure that dm.GetCoinsOrBillsVeryLow() returns valid result.
  // TAR 306397 - End
  m_bLangChange = (m_customerLanguage != SCOT_LANGUAGE_PRIMARY); // TAR 262131
  //Tar 417933
  if (m_bLangChange) //417933
	  m_LanguageUsedPreviously = m_customerLanguage;

  //Start tar 256141
  if (co.fOperationsCheckCashChanger)
  {
	 if(dm.GetCoinsOrBillsVeryLow())
     {
		trace(L6, _T("CoinsorBills are low"));
		g_bgCheckCashChanger = true;
      // TAR 279497 start - ensure cashier can understand message
      if (m_bLangChange)
      {
         m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
         ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
      }
      // TAR 279497 end
		CREATE_AND_DISPENSE(WaitForApproval)(false,false,false);

	 }
  }
  g_bgCheckCashChanger = false;
  //end tar 256141
  g_bTBNeedMoreData = false;         // TAR 238061 
  ra.RequestEnableAssistMode(true);  //Tar 218158
  // TAR 200294 NN 041602 - ensure back to normal of DMEvents when interacting with timers
  bDoNotResetTimeOutOnDMEvent = false;  
  nDeviceErrorContext = 0;  //FZ20021010
  m_bMotionSensorEnabledOnAttract = false;   //RFC 265465
  //TAR237079+ CHU/TP 051303
  if ( g_bWtIncAtContextHelpComingFromAttract )
  {
	  SMStateBase *sReturnState = CkStateStackAndReturn();
	  if (sReturnState != STATE_NULL)
		  return sReturnState;   //back to security state
  }
  //TAR237079-

  //Clear stack so we have a clean start in case if we have device failure during loading
  ClearStateStackEOT();
  io.Reset();	// 228225 - failsafe for when the F&P is pressed and there are no sold items LPM022303
  bIsInFinalization = false; //tar 194770
  EnableHandheldAssistMode(true);

  //TAR200557 if media errors and haven't told RAP, then tell RAP
  if(dm.GetCoinsOrBillsVeryLow() || dm.GetPrinterPaperLow() )
  {
	dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
		                DM_TRICOLORLIGHT_BLINK_1HZ,
			            DM_TRICOLORLIGHT_NORMALREQUEST);  
    // Notify RAP is done in updateStateName, below LPM060502 206311  
  }
  else if(!fRequestForHelp && !isSecurityApprovalRequired())  // tar 242625  // Begin TAR 210017
  {
	// Turn off all lights on just in case because lane light is off at attract
	dm.turnOffTriColorLight();
  }
  // End TAR 210017
  bToldRAPMediaLow = false;
  
  //Update RAP window with SCOT state and media low status
  UpdateStateName(ps.GetPSText(LTT_ATTRACT, SCOT_LANGUAGE_PRIMARY));
  //bQuietNeedSecApproval = false; //for quiet mode //Tar 224576
  m_lRememberVirtualZero = 9999999;	 // 227472 remember the virtual zero in case it was wind. 9999999 means not set. LPM021103 SOMEONE CAN MAKE THIS A #define VIRTZERO_NOTSET = 99999999 LATER

  //for Print Last Receipt module
  g_bAllowRemove = true;
  // TAR 227850 - added tracing
  trace(L6, _T("+RemoveRcptFile"));
  if (FileExist(csLReceiptTmpFile))
  {
	  RemoveFile(csLReceiptFile);
	  CFile::Rename(csLReceiptTmpFile, csLReceiptFile);
  }
  trace(L6, _T("-RemoveRcptFile"));	
  //To be sure the scanned item in the previous transaction is remembered
  // the flag "fItemIsWaiting" should be reset to false in the attract screens' 
  // initialize. - to wait for verification
  fItemIsWaiting = false;
  ASSERT(bEASReEnableScanner);
  if (!bEASReEnableScanner)
  {
      trace(L2, _T("::Initialize - bEASReEnableScanner is set to true explicitly"));
      bEASReEnableScanner = true;
  }
  // *thp* To be sure the previous card data track is deleted
  if (cDMLastTrack)
  {
      delete cDMLastTrack;
      cDMLastTrack = NULL;
      nDMLastTrackLen = 0;
      csDMEncryptedPin = EMPTY_STRING;
  }

  if(fPutSCOTinLC || bLaneClosed)
  {
	  bTransactionComplete = false; //TAR 411286
	  ResetAllTransactionValues();	    // TAR 283045 - TAR Migrated to 4.0
	  RETURNSTATE(LaneClosed)
  }

  if(fStopSCOT)
  {
	  fStateFreeze = false;
	  fStateAssist = false;
	  return STATE_STOP;
  }

  ASSERT(!fSAInTransaction);
  fSAInTransaction = false;
  appBusy.ClearBusyStatus();

  if(co.fStateSingleLanguageButtonAtAttract || m_bLangChange)//TAR#440749
  {
	  if(m_bLanguageButtonClicked && m_customerLanguage != SCOT_LANGUAGE_PRIMARY)
		  ps.SetLanguage(m_customerLanguage);
	  else if(this->csLastNonHelpStateName == _T("SMSelectLanguage") && m_customerLanguage != SCOT_LANGUAGE_PRIMARY)
		  ps.SetLanguage(m_customerLanguage);
	  else if( m_bSecViolationFromAttract == true   && m_customerLanguage != SCOT_LANGUAGE_PRIMARY) //383748
	  {
		  m_bSecViolationFromAttract = false;
		  ps.SetLanguage(m_customerLanguage);
	  }
	  else 
	  {
	      m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
		  ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
          ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT); //429239
	  }
  }
  else
  {
	  m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
	  ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
      ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT); //429239
  }

  if (gpSCOTAuxDisp)
	  gpSCOTAuxDisp->SetLanguage(m_customerLanguage);
	  
  //ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT);	//tar 375008    TAR [383748] moved down below just after ps.ShowCMFrame();
  
  setAnchorState(BES_START);
  nTimeOut = co.getTimeOutAttract();
  if (co.IsAttendantModeOn() && co.getTimeOutAttendantModeTimer() !=0 )
  {
    SetTimer(hw, ATTENDANT_MODE, co.getTimeOutAttendantModeTimer()*1000, &AttendantModeTimedOut);
  }

  // only logs if transinprogress flag is true
  // TAR 227850 - changed the trace level to 6
  trace(L6, _T("+rp.TransactionEnd"));
  rp.TransactionEnd();
  trace(L6, _T("-rp.TransactionEnd"));

  CString csAssistTemplateCommand;
  csAssistTemplateCommand.Format(_T("COMMAND=UIInitialize;ASSISTMODEFILENAME=%s;"), (LPCTSTR)co.csTemplateAssistMode);
  ra.OnAddOtherEvent((LPCTSTR)csAssistTemplateCommand);

  // moved Reset to later in initialize HOWEVER
  // !!! ResetAllTransactionValues MUST be called BEFORE DMCheckCashCounts is called!!!
  if(this->csLastNonHelpStateName != _T("SMAttract")) //Tar 224576 Don't reset values if press HELP and then come back to Attract 
  {
	bTransactionComplete = false; //TAR 411286
	ResetAllTransactionValues();	    // ensure all counters. totals, flags are reset
	ps.ShowCMRewardInfoBox(lTrxTotalSavings, lTrxTotalRewardPoints, CustomerMessage);	//tar 373874
  }
  // if we need to load options, do it now
  if (fDoOptions)
  {
	//return ReloadOptions();
	 fDoOptions = false;
	 CREATE_AND_DISPENSE(OutOfService)(2);
  }
  // if need to run EOD, do so
  if (fDoEOD)
  {
	  fDoEOD = false;
	  return PerformEOD();
  }

  DMCheckCashCounts(false);

  	
  if (cDMLastTrack)  
  {// eject any card in MSR
    CREATE_AND_DISPENSE(TakeCard)(false);   // end of trans false
  }
	
  if (lDMScaleWeight<0)
  {
    RETURNSTATE(ScaleBroken)
  }
	
  // de-queue any stacked states from the FIFO queue
  // these are usually system messages of a low priority
  // like paper low
  //  176732 LPM082401 Can't let this happen here because the 
  //  State Stack might have a screen change in it now.
  //SMStateBase  *sExit = PopStateFifoStack();
  //if (sExit!=STATE_NULL)
  //    return sExit;

  DMCashAcceptorDisable();	// TAR#137272 should be disabled at attract screen
  DMCoinAcceptorDisable();

  DMCashAcceptorReject();	// eject any cash not accepted	
  DMCoinAcceptorReject();	// eject any coins not accepted	

  dm.PinPadDisable(); //TAR 121779

// NewUI

  //255048{
  CWnd *pWnd = CWnd::FindWindow(NULL, _T("NCR SCOT"));  
  if(pWnd)
  {
    trace(L0, _T("BringWindowToTop()"));
	BringWindowToTop(pWnd->m_hWnd);
  }
  //255048}
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("Attract") );

  ps.SetCMFrame(_T("Attract"));
  ps.SetAttractScreenMessage(ps.GetPSText(MSG_VIDEOATATTRACT));
  COleVariant v;
  CString csButtonText[SCOT_LANGUAGE_MAX];
  int iButtonID[SCOT_LANGUAGE_MAX];
  for (i=0; i< SCOT_LANGUAGE_MAX; i++)
  {
      csButtonText[i] = EMPTY_STRING;  
      iButtonID[i] = 0;
  }
  if (co.m_iNumberOfLanguagesSupported < 3) 
  {
      ps.SetCMFrame(_T("Attract"));
      csCurContext = _T("Attract");
  }
  else
  {
      if(co.fStateSingleLanguageButtonAtAttract)
	  {	
		  ps.SetCMFrame(_T("Attract"));
		  csCurContext = _T("Attract");
	  }
	  else
	  {
		ps.SetCMFrame(_T("AttractMultiLanguage"));
		csCurContext = _T("AttractMultiLanguage");
	  }

  }
  ps.SetAttractScreenMessage(ps.GetPSText(MSG_VIDEOATATTRACT));

  if (co.fStateDualLanguage && !co.fStateSingleLanguageButtonAtAttract)
  {
	  // For 3.1.1 Change, language name for button 1 and 2 can be text or message ID corresponding in message file
      CString csButtonName;
	  // check to see if language name is text or number.  If it's a number, get the text message corresponding to this number

      for (i=0; i < SCOT_LANGUAGE_MAX; i++)
      {
          if (i < co.m_iNumberOfLanguagesSupported)
          {
              // If there is a language defined for this entry
              csButtonText[i] = co.language[i].csName;
	          if(_istdigit(*csButtonText[i]))
	          {
		          iButtonID[i] = _ttoi(co.language[i].csName);
		          csButtonText[i] = ps.GetPSText(iButtonID[i], (SCOT_LANGUAGE_TYPE) i);
	          }
              ps.SetPSText(PS_WORK1,csButtonText[i]);
	          v = VARIANT_TRUE;
              switch (i)
              {
                  // Set appropriate button text and button visible
                  case SCOT_LANGUAGE_PRIMARY:
                      if (co.m_iNumberOfLanguagesSupported < 3) 
                      {
                          csButtonName = (_T("CMButton1Lg"));
                          m_pAppPSX->SetConfigProperty(_T("CMButton1Lg"), csCurContext, UI::PROPERTYVISIBLE, v);
                          ps.CMButtonFlash(_T("CMButton1Lg"),true);   
                      }
                      else
                      {
                          csButtonName = (_T("CMButton1MidiList"));
                          m_pAppPSX->SetConfigProperty(_T("CMButton1MidiList"), csCurContext, UI::PROPERTYVISIBLE, v);
                          ps.CMButtonFlash(_T("CMButton1MidiList"),true);   
                      }
                      break;
                  case SCOT_LANGUAGE_SECONDARY:
                      if (co.m_iNumberOfLanguagesSupported < 3) 
                      {
                          csButtonName = (_T("CMButton2Lg"));
                          m_pAppPSX->SetConfigProperty(_T("CMButton2Lg"), csCurContext, UI::PROPERTYVISIBLE, v);
                          ps.CMButtonFlash(_T("CMButton2Lg"),true);   
                      }
                      else
                      {
                          csButtonName = (_T("CMButton2MidiList"));
                          m_pAppPSX->SetConfigProperty(_T("CMButton2MidiList"), csCurContext, UI::PROPERTYVISIBLE, v);
                          ps.CMButtonFlash(_T("CMButton2MidiList"),true);   
                      }
                      break;
                  case SCOT_LANGUAGE_THIRD:
                      csButtonName = (_T("CMButton3MidiList"));
                      m_pAppPSX->SetConfigProperty(_T("CMButton3MidiList"), csCurContext, UI::PROPERTYVISIBLE, v);
                      ps.CMButtonFlash(_T("CMButton3MidiList"),true);   
                      break;
                  case SCOT_LANGUAGE_FOURTH:
                      csButtonName = (_T("CMButton4MidiList"));
                      m_pAppPSX->SetConfigProperty(_T("CMButton4MidiList"), csCurContext, UI::PROPERTYVISIBLE, v);
                      ps.CMButtonFlash(_T("CMButton4MidiList"),true);   
                      break;
                  case SCOT_LANGUAGE_FIFTH:
                      csButtonName = (_T("CMButton5MidiList"));
                      m_pAppPSX->SetConfigProperty(_T("CMButton5MidiList"), csCurContext, UI::PROPERTYVISIBLE, v);
                      ps.CMButtonFlash(_T("CMButton5MidiList"),true);   
                      break;
                  case SCOT_LANGUAGE_SIXTH:
                      csButtonName = (_T("CMButton6MidiList"));
                      m_pAppPSX->SetConfigProperty(_T("CMButton6MidiList"), csCurContext, UI::PROPERTYVISIBLE, v);
                      ps.CMButtonFlash(_T("CMButton6MidiList"),true);   
                      break;
                  default:
                      ASSERT( FALSE );
                      csButtonName = (_T("CMButton1Lg"));  // Shouldn't ever get here
                      break;
              }
              if (csButtonText[i] != _T(""))
              {
                ps.SetButtonText(csButtonName,PS_WORK1);
              }
          }

          else if (co.m_iNumberOfLanguagesSupported > 2)
          {
              // Make the other buttons invisible if we're using the multi language screen
	          v = VARIANT_FALSE;
              switch (i)
              {
                  case SCOT_LANGUAGE_SECONDARY:
                      // I don't think this condition should happen but it's coded for
                      ASSERT( FALSE );
                      m_pAppPSX->SetConfigProperty(_T("CMButton2MidiList"), csCurContext, UI::PROPERTYVISIBLE, v);
                      break;
                  // It is logically impossible to get into here with language equal to 3
                  case SCOT_LANGUAGE_FOURTH:
                      m_pAppPSX->SetConfigProperty(_T("CMButton4MidiList"), csCurContext, UI::PROPERTYVISIBLE, v);
                      break;
                  case SCOT_LANGUAGE_FIFTH:
                      m_pAppPSX->SetConfigProperty(_T("CMButton5MidiList"), csCurContext, UI::PROPERTYVISIBLE, v);
                      break;
                  case SCOT_LANGUAGE_SIXTH:
                      m_pAppPSX->SetConfigProperty(_T("CMButton6MidiList"), csCurContext, UI::PROPERTYVISIBLE, v);
                      break;
                  default:
                      ASSERT( FALSE );
              }
          }
          else if (co.m_iNumberOfLanguagesSupported == 1)
          {
              // I don't think this condition should happen but it's coded for
              ASSERT( FALSE );
              m_pAppPSX->SetConfigProperty(_T("CMButton2Lg"), csCurContext, UI::PROPERTYVISIBLE, v);
          }


      }
	  v = VARIANT_TRUE;
  }
  else
  { 
  	  ps.SetButtonText(_T("CMButton1Lg"),BTT_START);
	  v = VARIANT_FALSE;
      m_pAppPSX->SetConfigProperty(_T("CMButton2Lg"), csCurContext, UI::PROPERTYVISIBLE, v);
      ps.CMButtonFlash(_T("CMButton1Lg"),true);   
  }

  if(co.fStateSingleLanguageButtonAtAttract)
  {
	  if(co.fStateDualLanguage && co.m_iNumberOfLanguagesSupported == 2)
	  {
		  /*if(m_customerLanguage == SCOT_LANGUAGE_PRIMARY)
			  m_pAppPSX->SetConfigProperty(_T("CMButton8MidiList"),csCurContext, UI::PROPERTYTEXTFORMAT, ps.GetPSText(BTT_LANGUAGE, SCOT_LANGUAGE_SECONDARY));
			  //ps.SetButtonText(_T("CMButton8MidiList"),ps.GetPSText(BTT_LANGUAGE, SCOT_LANGUAGE_SECONDARY));
		  else
			  //ps.SetButtonText(_T("CMButton8MidiList"),ps.GetPSText(BTT_LANGUAGE, SCOT_LANGUAGE_PRIMARY));
		      m_pAppPSX->SetConfigProperty(_T("CMButton8MidiList"),csCurContext, UI::PROPERTYTEXTFORMAT, ps.GetPSText(BTT_LANGUAGE, SCOT_LANGUAGE_PRIMARY));
		  */
		 int i, iButtonID;
		 CString csButtonText;
		 
		 if(m_customerLanguage == SCOT_LANGUAGE_PRIMARY)
		 {
			 i = SCOT_LANGUAGE_SECONDARY;
		 }
		 else if(m_customerLanguage == SCOT_LANGUAGE_SECONDARY)
		 {
			 i = SCOT_LANGUAGE_PRIMARY;
		 }
		 else
		 {
			 ASSERT(FALSE); // shouldn't enter here
		 }
		 
		 csButtonText = co.language[i].csName;
		 if(_istdigit(*csButtonText))
		 {
			 iButtonID = _ttoi(co.language[i].csName);
			 csButtonText = ps.GetPSText(iButtonID, (SCOT_LANGUAGE_TYPE)i);
		 }
		 ps.SetPSText(PS_WORK1, csButtonText);
		 ps.SetButtonText(_T("CMButton8Global"),PS_WORK1);
		 m_pAppPSX->SetConfigProperty(_T("CMButton8Global"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
	  }
	  else if(co.m_iNumberOfLanguagesSupported > 2)
	  {
		  m_pAppPSX->SetConfigProperty(_T("CMButton8Global"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
		  m_pAppPSX->SetConfigProperty(_T("CMButton8Global"),csCurContext, UI::PROPERTYTEXTFORMAT, ps.GetPSText(BTT_LANGUAGE, SCOT_LANGUAGE_PRIMARY));
	  }
	  else
		  m_pAppPSX->SetConfigProperty(_T("CMButton8Global"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
	  
		  //ps.SetButtonText(_T("CMButton8MidiList"),ps.GetPSText(BTT_LANGUAGE, SCOT_LANGUAGE_PRIMARY));
	  ps.SetButtonText(_T("ButtonHelp"), BTT_HELP);
	  ps.SetLeadthruText(LTT_ATTRACT);
  }
  else
	  m_pAppPSX->SetConfigProperty(_T("CMButton8Global"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
  
  //SR085 +
  if(co.fOperationsCustomerBagAllowed)
  {
	  //display the "I brought my own bag" button
	  trace(L0, _T("SMAttract Initialize: Customer Bag Allowed"));
	  CString csButtonName;
	  COleVariant v;
	  v = VARIANT_TRUE;
	  csButtonName = (_T("CMButton2Lg"));
      m_pAppPSX->SetConfigProperty(_T("CMButton2Lg"), csCurContext, UI::PROPERTYVISIBLE, v);
      ps.CMButtonFlash(_T("CMButton2Lg"),true);
	  ps.CMButton(csButtonName, BTT_OWNBAG, true);
  }
  else
  {
	  //do nothing - not making the button invisible as it might be being made visible by the language setting above
	  trace(L0, _T("SMAttract Initialize: Customer Bag NOT Allowed"));
  }
  //SR085 -
  
  //Need to restore POS Message in RAP by sending it over again
  trace(L7, _T("+ra.OnNormalItem, Message=%s"), csTBMessage2Scot);
  ra.OnNormalItem(_T("[Alert]") + ps.GetPSText(RA_POSMESSAGE, SCOT_LANGUAGE_PRIMARY) + _T(" ") + csTBMessage2Scot);
  trace(L7, _T("-ra.OnNormalItem"));


  // BeginCodeWLDBSA11
  SAWLDBScannerEnable();	// enable the scanner	moved to after the showframe 123168
  // EndCodeWLDBSA11
  DMScaleEnable();         // enable the scale TAR123168	LPM101199  Scanner disabled while terminal out of service
  CString csAttractText;

  if (co.fStateDualLanguage)  // TAR 241836 - check for dual language first 
  {
     csAttractText = ps.GetPSText(MSG_ATTRACT_DUAL_LANGUAGE);
  }
  else if (tb.GetTransactionLimit() > 0)
  {
	 GetTrxLimitString(MSG_ATTRACT, csAttractText);
  }
  else
  {
	 csAttractText = ps.GetPSText(MSG_ATTRACT_NO_LIMIT);
  }
  ps.Message12(_T(""), false); //dp185016 tar 287707

  //TAR 219971 & 220074 BZ 11/20/02 Reset the RC of DisplayAuxScreen called from
  //previously transaction    also tar 227543
  g_lDisplayFormRC = 0;	
  //TAR 219971 & 220074 end

  // TAR 244789 This hack is required because of several things.  First, MSREnabled for the
  // encryptor is really to tell the 5945 to load the form rather than displaying the screen. So
  // the change here is so that the display willl cause the table load to occur instead of the
  // display.  The MSR Enable has to be done after the display aux screen for reasons listed below.
  // When the MSR is enabled, it will display the prompt from the table and overwrite the display, 
  // which is why we have to make sure the prompt is loaded into the table.
  if (gpSCOTEncryptor)
  {
	gpSCOTEncryptor->SetMSREnabled(true);
  }

  // Display the Auxilary attract screen
  if(co.fStateSingleLanguageButtonAtAttract)
  {
	  //CString str = ps.GetCurrentContext();
    //m_pAppPSX->SetConfigProperty(_T("CMButton8MidiList"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_TRUE);
    DisplayAuxScreen(AUX_SCREEN_ATTRACT, true);
  }
  else if ((co.fStateDualLanguage) && (co.m_iNumberOfLanguagesSupported < 3))

  {
	  // For 3.1.1 Change, language name for button 1 and 2 can be text or message ID corresponding in message file
	  // check to see if language name is text or number.  If it's a number, get the text message corresponding to this number
    DisplayAuxScreen(AUX_SCREEN_ATTRACT_LANG, 
                     true, //TAR208962 CHU 091602 changed from false to true so it always refresh
                     2, 
                     csButtonText[0],		//TAR 244298
                     csButtonText[1]);	//TAR 244298
  }
  else
  {
    DisplayAuxScreen(AUX_SCREEN_ATTRACT, true);
  }


  // TAR 244789 - According to OPOS Group, for 5945, displaying the aux screen disables the
  // MSR.  So the change is to enable the MSR after displaying the message.
  DMMSREnable();            // enable the card reader, set track, turn on MEI

  // Let's check on memory usage at the beginning of each transaction
  traceMem(L6);

  //TAR239862 fRequestForHelp = false;
  fSSWtFromAttract = false;		// TAR #123172 only used if scan scale wt increase from attract screen LPM101499	

  //+4.2 provide subtle on-screen volume control
  //check if ShowVolumeControl in 'Scotopts.dat' is set to "Y". 
  if (co.IsShowVolumeControl() )
  {
	m_pAppPSX->SetConfigProperty(_T("ButtonVolumeControl"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);
    if(co.GetVolumeControlTimer()>0)
		ps.CreateVolCtlThread();      // TAR 351654
	else
		ps.CreateVolCtlThread(false);   // TAR 404826
  }else
  {
	  m_pAppPSX->SetConfigProperty(_T("ButtonVolumeControl"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
  }
  //-4.2 provide subtle on-screen volume control
    if(co.fStateSingleLanguageButtonAtAttract && co.GetSingleLanguageButtonAtAttractTimer() > 0 &&
	  co.m_iNumberOfLanguagesSupported > 1)
  {
	  int n = co.GetSingleLanguageButtonAtAttractTimer();
	  SetTimer(hw,LANGUAGE_CONTROL, co.GetSingleLanguageButtonAtAttractTimer()*1000, &BackToPrimaryLanguageTimedOut);
  }

  ps.ShowCMFrame();
  
  //ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT);	//TAR 383748

  ps.Message12(csAttractText, false); //dp185016 tar 287707


// SR683: Show the security disable button on RAP when option allows
	m_pAppPSX->SetTransactionVariable(_T("SecurityDisableAllowed"),
		co.fStateSecurityDisableAllowed ? VARIANT_TRUE : VARIANT_FALSE);

  if(co.fAutoRunOn || co.fAutoRunAssistModeOn)
  {
      ms_nAutoRunTransactionTally++;
	  //+RFC375580
      if ( co.nAutoRunNumberOfTransactionsToRun == 0) 
	  {
		  ms_nNumberOfAutoRunTrans = -1;//prevent it from being 0
          trace(L6, _T("SCOT is running in infinite Autorun. Transaction Tally = %d"), ms_nAutoRunTransactionTally );

	  }//-RFC375580
      if(co.nAutoRunNumberOfTransactionsToRun > ms_nNumberOfAutoRunTrans)
      {
          ++ms_nNumberOfAutoRunTrans;
          nTimeOut = co.nAutoRunAttractDelay > 0 ? co.nAutoRunAttractDelay : 1;

		  //TAR 406698
		  if(co.fAutoRunRandomTimer)
		  {
			  //+RFC375570
			  if (nTimeOut > 1) 
			  {
				  nTimeOut = 1 + ( rand() % co.nAutoRunAttractDelay );

			  }//-RFC375570
		  }//-406698

		  trace(L6, _T("AutoRunAttractDelay = %d, transaction tally %d of %d"), nTimeOut, ms_nAutoRunTransactionTally, co.nAutoRunNumberOfTransactionsToRun );
          return STATE_NULL;
      }
      else
      {
          co.fAutoRunOn = false; // must re-start APP or re-load Options in order to restart AutoRun
          co.fAutoRunAssistModeOn = false;
          ms_nNumberOfAutoRunTrans = 0;
      }
  }
  bInitializeToTimeOut = true;
  if (m_bLangChange)
  {
	  /* 383748 
   	  if(m_bLanguageButtonClicked && m_customerLanguage != SCOT_LANGUAGE_PRIMARY)
		ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT);
	  else if(this->csLastNonHelpStateName != _T("SMThankYou"))
	     ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT);
		 */
	  m_bLangChange = false;
  }
  m_bLanguageButtonClicked = false;
  //RFC 265465 start
  if ( m_MotionSensorOnAttractInterval > 0 )
  {
	  m_bMotionSensorEnabledOnAttract = true;
  }
  //RFC 265465 end

  //SR742+ display mode on RAP e-receipt while in degraded mode
  if ( co.IsDegradedModeOn() && dm.GetDegradedModeType() != DEGRADEDMODE_OFF )
  {
	  CString csRAPReceiptLine;
	  if ( dm.GetDegradedModeType() ==  DEGRADEDMODE_CASH_ONLY )
		  csRAPReceiptLine = ps.GetPSText(RAP_MSG_CASHONLYMODE,SCOT_LANGUAGE_PRIMARY);		
	  else if ( dm.GetDegradedModeType() ==  DEGRADEDMODE_CARD_ONLY )
		  csRAPReceiptLine = ps.GetPSText(RAP_MSG_CARDONLYMODE,SCOT_LANGUAGE_PRIMARY);
	  else
	  {
		  ASSERT(FALSE);
		  trace(L6, _T("SMAttractBase:%d is invalid degraded mode type."), dm.GetDegradedModeType());	
	  }

	  csRAPReceiptLine = RAP_RECEIPT_LINE_SUMMARY + csRAPReceiptLine;
	  trace(L7, _T("+ra.OnNormalItem, Message=%s"), csRAPReceiptLine);
	  ra.OnNormalItem(csRAPReceiptLine);
	  trace(L7, _T("-ra.OnNormalItem"));
  }
  //SR742-

  return TimedOut();     // stay in attract state, but refresh the display
}

//////////////////////////////////
// TimedOut
//////////////////////////////////
SMStateBase  *SMAttractBase::TimedOut(void)
{
  /*if (bQuietNeedSecApproval && !bInitializeToTimeOut) //for quiet mode - aiqin //Tar 224576 if come here because of timer expired
  { 
		bQuietNeedSecApproval = false; //no longer need to approve an increase because the timer expired
		nTimeOut = co.getTimeOutAttract();
  }*/
  bInitializeToTimeOut = false; //Tar 224576
  //return PopStateFifoStack();			  // see if any pending messages
  //  176732 LPM082401 This is dangerous because the 
  //  State Stack might have a screen change in it now.
  if(co.fAutoRunOn || co.fAutoRunAssistModeOn)
  {
      return PSButton1();
  }
  else
  {
      return STATE_NULL;
  }
}

//////////////////////////////////
// UnInitialize
//////////////////////////////////
void SMAttractBase::UnInitialize(void)
{
  KillTimer(hw,ATTENDANT_MODE);
  m_bMotionSensorEnabledOnAttract = false;   //RFC 265465
  g_bAllowRemove = false;
  if(m_TimerResponse != 0) //RFC 276235
  {
	  KillTimer(hw, SCANSCALEATTRACT);
	  m_TimerResponse = 0;
  
  }
  KillTimer(hw,LANGUAGE_CONTROL);
  m_bLanguageButtonClicked = false;
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("Attract") );

  if (m_bLangChange)
  {
     ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT);
     m_LanguageUsedPreviously=m_customerLanguage;
  }
     
  // 206331 LPM lane light not blinking after going into media status even if printer is low
  if (dm.GetPrinterPaperLow())
  {
	dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
		                DM_TRICOLORLIGHT_BLINK_1HZ,
			            DM_TRICOLORLIGHT_NORMALREQUEST);  
  }
  else if(!isSecurityApprovalRequired())
  {
	dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
					    DM_TRICOLORLIGHT_ON,
					    DM_TRICOLORLIGHT_NORMALREQUEST);
  }
  SAWLDBScannerDisable();						  // disable the scanner

  if (!cDMLastTrack && !waitingForCardRemoval)
  {
	DMMSRDisable();	// if no card inserted, disable, no problems
  }
  else if(!co.fTenderIsMotorizedMSR)		// we scanned in a card and we're using a swipe reader
  {
	DMMSRDisable();					// Disable reader to prevent "Can't swipe card now" msgs
  }
  // TAR 199152 & 199010 - stop playing AVI and hide the window if large vedio was played at init
  if (!co.getMoviePlayOnAttract()->GetLength())
	ps.HideVideoLarge();	// end TAR 199152 & 199010

  DMMSRLight(DM_MISC_STATE_OFF);	  // turn off MEI, card or no card
  if (co.fStateDualLanguage)
  {
      for (int i=0; i < co.m_iNumberOfLanguagesSupported; i++)
      {
          switch (i)
          {
              // Set appropriate button text and button visible
              case SCOT_LANGUAGE_PRIMARY:
                  if (co.m_iNumberOfLanguagesSupported < 3) 
                  {
                      ps.CMButtonFlash(_T("CMButton1Lg"), false);
                  }
                  else
                  {
                      ps.CMButtonFlash(_T("CMButton1MidiList"),false);   
                  }
                  break;
              case SCOT_LANGUAGE_SECONDARY:
                  if (co.m_iNumberOfLanguagesSupported < 3) 
                  {
                      ps.CMButtonFlash(_T("CMButton2Lg"),false);   
                  }
                  else
                  {
                      ps.CMButtonFlash(_T("CMButton2MidiList"),false);   
                  }
                  break;
              case SCOT_LANGUAGE_THIRD:
                  ps.CMButtonFlash(_T("CMButton3MidiList"),false);   
                  break;
              case SCOT_LANGUAGE_FOURTH:
                  ps.CMButtonFlash(_T("CMButton4MidiList"),false);   
                  break;
              case SCOT_LANGUAGE_FIFTH:
                 ps.CMButtonFlash(_T("CMButton5MidiList"),false);   
                  break;
              case SCOT_LANGUAGE_SIXTH:
                  ps.CMButtonFlash(_T("CMButton6MidiList"),false);   
                  break;
              default:
                  ASSERT( FALSE );
          }
      }
  }
  else
  {
      ps.CMButtonFlash(_T("CMButton1Lg"), false);
  }
  /*
  ps.CMButtonFlash(_T("CMButton1Lg"), false);
  if (co.fStateDualLanguage)
    ps.CMButtonFlash(_T("CMButton2Lg"), false);
  */

  //+4.2 provide subtle on-screen volume control
    if (co.IsShowVolumeControl() )
  {
	if ( co.GetVolumeControlTimer() > 0)
	{
		KillTimer(hw, VOLUME_CONTROL);
	}
  }
	//-4.2 provide subtle on-screen volume control
}




///////////////////////////////////
// Start Button, or primary language button pressed
///////////////////////////////////
SMStateBase  *SMAttractBase::PSButton1(void)
{
	CString csButtonText;
	int iButtonID;

	SCOT_LANGUAGE_TYPE customerLanguage; 
	customerLanguage = ps.GetLanguage();

	
	if(!co.fStateDualLanguage)
	{  
		csButtonText = ps.GetPSText(MSG_LANGUAGENAME);
		customerLanguage = SCOT_LANGUAGE_PRIMARY; 

	}
	else
	{
		if(!co.fStateSingleLanguageButtonAtAttract)
		{
			csButtonText = co.language[0].csName;
			if(_istdigit(csButtonText[0]))
			{
				iButtonID = _ttoi(co.language[0].csName);
				csButtonText = ps.GetPSText(iButtonID, (SCOT_LANGUAGE_TYPE) 0);
				
			}
			customerLanguage = SCOT_LANGUAGE_PRIMARY;
			
		}
		else
		{
			//csButtonText = co.language[0].csName;//ps.GetPSText(MSG_LANGUAGENAME);
			csButtonText = co.language[customerLanguage].csName;//ps.GetPSText(MSG_LANGUAGENAME);
			
		}
	}

	rp.SetLanguageName(csButtonText);
	return transition(START_BUTTON, customerLanguage); 
	
	/*if(!co.fStateDualLanguage)
	{  
		csButtonText = ps.GetPSText(MSG_LANGUAGENAME);
		rp.SetLanguageName(csButtonText);
		return transition(START_BUTTON, SCOT_LANGUAGE_PRIMARY); 

	}
	else
	{
		if(!co.fStateSingleLanguageButtonAtAttract)
		{
			csButtonText = co.language[0].csName;
			if(_istdigit(csButtonText[0]))
			{
				iButtonID = _ttoi(co.language[0].csName);
				csButtonText = ps.GetPSText(iButtonID, (SCOT_LANGUAGE_TYPE) 0);
				
			}
			rp.SetLanguageName(csButtonText);
			return transition(START_BUTTON, SCOT_LANGUAGE_PRIMARY); 
		}
		else
		{
			//csButtonText = co.language[0].csName;//ps.GetPSText(MSG_LANGUAGENAME);
			csButtonText = co.language[customerLanguage].csName;//ps.GetPSText(MSG_LANGUAGENAME);
			rp.SetLanguageName(csButtonText);
			return transition(START_BUTTON, customerLanguage); 
		}
	}*/
	
}

///////////////////////////////////
// Secondary language Button or /*SR085*/ "I brought my own bag" button pressed
///////////////////////////////////
SMStateBase  *SMAttractBase::PSButton2(void)
{
  if(!co.fOperationsCustomerBagAllowed)//SR085 - secondary language button pressed
  {
	  ASSERT(co.fStateDualLanguage);  

	  CString csButtonText;
		int iButtonID;
		
		csButtonText = co.language[1].csName;
		if(_istdigit(csButtonText[0]))
		{
			iButtonID = _ttoi(co.language[1].csName);
			csButtonText = ps.GetPSText(iButtonID, (SCOT_LANGUAGE_TYPE) 1);
		}
		rp.SetLanguageName(csButtonText);
	  return transition(START_BUTTON, SCOT_LANGUAGE_SECONDARY); 
  }
  //SR085 +
  else // "I brought my own bag" button pressed
  {
	  //display the new state
	  trace(L6, _T("+SMAttractBase: I brought my own bag bttn pressed"));
	  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("Attract"));

	  //Reset quiet mode flag
	  if(bQuietNeedSecApproval)
	  {
		  bQuietNeedSecApproval = false;
	  }

	  SCOT_LANGUAGE_TYPE customerLanguage; 
	  customerLanguage = ps.GetLanguage();
	  if (co.fStateDualLanguage)
	{
      m_customerLanguage = customerLanguage;
      ps.SetLanguage(customerLanguage);
	  co.csCMCustomerMessage = ps.GetPSText(co.nCMCustomerMessage);
	  CustomerMessage = co.csCMCustomerMessage;

	  ps.SetTareListToCurrentLanguage();
	  if(gpSCOTAuxDisp)
      gpSCOTAuxDisp->SetLanguage(customerLanguage);

      if(m_LanguageUsedPreviously != m_customerLanguage)
      {
        ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT);
        m_LanguageUsedPreviously=m_customerLanguage;        
      }
    }
      if(!co.IsSecurityBypassOn())
      {
        CKeyValueParmList parmList;
        CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_CUST_HAS_BAG);
        _bstr_t bstrParmList((const _TCHAR *)parmList);
        BSTR bstrResult;
        bstrResult = NULL;
        SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
        SysFreeString(bstrResult);
      }

	  if(!fSAInTransaction)
		  SASendSCOTInput( ITEMIZATION );

	  //395551+
	  setAnchorState(BES_PUTBAGONSCALE);
	  //395551-
	  g_savedAnchorStateFromAttract = BES_PUTBAGONSCALE; //416582

      // +TAR 424892
	  //return TBStart();
      if (co.fStateDualLanguage)
          return TBStart(co.language[m_customerLanguage].szCode); 
      else
          return TBStart();
      // -TAR 424892

	  //395551+
	  //return setAndCreateAnchorState(BES_PUTBAGONSCALE);
	  //395551-
  }
  //SR085 -
}

///////////////////////////////////
// PSButton3 - Third language Button pressed
///////////////////////////////////
SMStateBase  *SMAttractBase::PSButton3(void)
{
	 CString csButtonText;
	int iButtonID;
	
	csButtonText = co.language[2].csName;
	if(_istdigit(csButtonText[0]))
	{
		iButtonID = _ttoi(co.language[2].csName);
		csButtonText = ps.GetPSText(iButtonID, (SCOT_LANGUAGE_TYPE) 2);
	}
	rp.SetLanguageName(csButtonText);
  return transition(START_BUTTON, SCOT_LANGUAGE_THIRD);  

//  return STATE_NULL;				  // no FTU in this release
}

///////////////////////////////////
// PSButton4 - Fourth language Button pressed
///////////////////////////////////
SMStateBase  *SMAttractBase::PSButton4(void)
{
	CString csButtonText;
	int iButtonID;
	
	csButtonText = co.language[3].csName;
	if(_istdigit(csButtonText[0]))
	{
		iButtonID = _ttoi(co.language[3].csName);
		csButtonText = ps.GetPSText(iButtonID, (SCOT_LANGUAGE_TYPE) 3);
	}
	rp.SetLanguageName(csButtonText);
  return transition(START_BUTTON, SCOT_LANGUAGE_FOURTH);  

}

///////////////////////////////////
// PSButton5 - Fifth language Button pressed
///////////////////////////////////
SMStateBase  *SMAttractBase::PSButton5(void)
{
	CString csButtonText;
	int iButtonID;
	
	csButtonText = co.language[4].csName;
	if(_istdigit(csButtonText[0]))
	{
		iButtonID = _ttoi(co.language[4].csName);
		csButtonText = ps.GetPSText(iButtonID, (SCOT_LANGUAGE_TYPE) 4);
	}
	rp.SetLanguageName(csButtonText);
  return transition(START_BUTTON, SCOT_LANGUAGE_FIFTH);  

}

///////////////////////////////////
// PSButton6 - Sixth language Button pressed
///////////////////////////////////
SMStateBase  *SMAttractBase::PSButton6(void)
{
	CString csButtonText;
	int iButtonID;
	
	csButtonText = co.language[5].csName;
	if(_istdigit(csButtonText[0]))
	{
		iButtonID = _ttoi(co.language[5].csName);
		csButtonText = ps.GetPSText(iButtonID, (SCOT_LANGUAGE_TYPE) 5);
	}
	rp.SetLanguageName(csButtonText);
  return transition(START_BUTTON, SCOT_LANGUAGE_SIXTH);  

}

SMStateBase  *SMAttractBase::PSButton8(void)
{
	m_bLanguageButtonClicked = true;

	if(co.m_iNumberOfLanguagesSupported > 2)
	{
		// Tell security that we are exiting form scan& bag state
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("Attract") );

		return setAndCreateAnchorState(BES_LANGUAGESELECT);
	}
	else if(co.m_iNumberOfLanguagesSupported == 2)
	{
	
		if(m_customerLanguage == SCOT_LANGUAGE_PRIMARY)
		{
			m_customerLanguage = SCOT_LANGUAGE_SECONDARY;
		}
		else
		{
			m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
		}
        SetPreferredLanguage(m_customerLanguage, _T("Button") );
		//ChangeLanguage(m_customerLanguage);
		Initialize(); // refresh the screen
		ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT); //429239

		return STATE_NULL;
	}
	else
	{
		ASSERT(FALSE); // shouldn't enter here
		return STATE_NULL;
	}
}


/////////////////////////////////////
// DMScanner - something was scanned
/////////////////////////////////////
SMStateBase  *SMAttractBase::DMScanner(void)
{
  //begin TAR#377242
  CString csButtonText;
  csButtonText = ps.GetPSText(MSG_LANGUAGENAME);
  rp.SetLanguageName(csButtonText);
  //end TAR#377242
  //begin TAR#121479
  SASendSCOTInput( ITEMIZATION );	// security needs to know we scanned an item quickly.
  csItemSent = csDMLastBarCode;
  fItemIsWaiting = true;            // to know if the item was sold from the attract screen or not.  sell item after signon in S&B
  SASendSCOTInput( UPC_SCANNED, csDMLastBarLabel );
  //end TAR#121479	LPM092499

  if ( bQuietNeedSecApproval )  //TAR234536
  {
	  bQuietNeedSecApproval = false;
	  m_bScanOnAttractAtQuietMode = true;
  }

  if (co.fStatePaintScanAndBagOnInitialScan)
  {
     PaintScanAndBagScreen();  // TAR 262131 Provide immediate feedback
  }
  if(!co.fStateSingleLanguageButtonAtAttract)
	  return transition(SCAN_ITEM, SCOT_LANGUAGE_PRIMARY);
  else
      return transition(SCAN_ITEM, m_customerLanguage);
}

//////////////////////////////////////
// DMCardReader - a card was swiped
//////////////////////////////////////
SMStateBase  *SMAttractBase::DMCardReader(void)
{  
//+TAR 275219
if (co.fStatePaintScanAndBagOnInitialScan) 
 {
  PaintScanAndBagScreen();  // Provide immediate feedback
}
//-TAR 275219
  return transition(SWIPE_CARD, SCOT_LANGUAGE_PRIMARY);
}

///////////////////////////////////////////
// DMScale - something was put on the scale
///////////////////////////////////////////
SMStateBase  *SMAttractBase::DMScale(void)
{
  trace(L6, _T("+SMStateBase  *SMAttractBase::DMScale(void)"));
  int ntimeoutvalue = co.getTimeOutScannerScaleOnAttract() ;
  SMStateBase *sExit = SMStateBase::DMScale();
  if (fDMScaleZeroed)
  {
	  //RFC 276235 start
	  if (lDMScaleWeight > 0 && m_TimerResponse == 0)
	  {
		  m_TimerResponse = SetTimer(hw, SCANSCALEATTRACT, ntimeoutvalue, &ScanScaleAttractTimedOut);
		  trace(L6, _T("::DMScale-Start Timer from weight increase"));
	  }
	  if ((lDMScaleWeight == 0) && (m_TimerResponse != 0))
	  {
		  KillTimer(hw, SCANSCALEATTRACT);
		  m_TimerResponse = 0;
		  
	  }
	  else if ((lDMScaleWeight > 0) && (m_TimerResponse != 0)) 
	  {
		  KillTimer(hw, SCANSCALEATTRACT);
		  m_TimerResponse = 0;
		  if (lDMScaleWeight != 0)
		  {
			  //Now restart Timer
			  m_TimerResponse = SetTimer(hw, SCANSCALEATTRACT, ntimeoutvalue, &ScanScaleAttractTimedOut);
		  }
	  }

	  
	  if (lDMScaleWeight > 0)
	  {
		  fSSWtFromAttract = true;	// TAR123172 only used if scan scale wt increase from attract screen LPM101499
		  fDMScaleZeroed = false;
	  }
  }
  trace(L6, _T("-SMStateBase  *SMAttractBase::DMScale(void)"));
  return sExit;
}

/////////////////////////////////////////////////////
// transition - proceed to next state
/////////////////////////////////////////////////////
SMStateBase * SMAttractBase::transition(TRANSITION_EVENT event, SCOT_LANGUAGE_TYPE languageUsed)
{
  m_bCancellingCashTender = false; //TAR 323590

  m_bMotionSensorEnabledOnAttract = false;   //RFC 265465
    
  if (event == START_BUTTON)
      SetPreferredLanguage(languageUsed, _T("Button") );
    
  if (bQuietNeedSecApproval) //for quiet mode - aiqin
  {
	  if (event == START_BUTTON)
      {
		  setAnchorState(BES_SCANANDBAG);
      }
	  else if (event == SCAN_ITEM)
      {
		  setAnchorState(BES_BAGANDEAS);
      }
  }
  
  // enable the sensormatic EAS deactivator while in a transaction
  dm.EASDeactivatorEnable( EASEnable );

  //NewSec
  if(!fSAInTransaction)
	  SASendSCOTInput( ITEMIZATION );
	
  // Move to a transitioning state where the application waits for a sign on
  // from terminal services. look for TS display event
  bGoingToInProgress = true;
  if (co.fStateDualLanguage)
  {
      m_customerLanguage = languageUsed;
      ps.SetLanguage(languageUsed);
	  co.csCMCustomerMessage = ps.GetPSText(co.nCMCustomerMessage);
	  CustomerMessage = co.csCMCustomerMessage;

	  ps.SetTareListToCurrentLanguage();
	  if(gpSCOTAuxDisp)
      gpSCOTAuxDisp->SetLanguage(languageUsed);
      SMStateBase *retVal = TBStart(co.language[languageUsed].szCode);

      if(m_LanguageUsedPreviously != m_customerLanguage)
      {
        ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT);
        m_LanguageUsedPreviously=m_customerLanguage;        
      }  
      return retVal;
  }
  else
  {
      return TBStart();
  }
}

//TAR#121479 moved checkCashAcceptorCassette to Finish base	LPM092999

///////////////////////////////////////
SMStateBase  * SMAttractBase::SAEndOfTransaction(void)
{
	bTransactionComplete = false; //TAR 411286
	ResetAllTransactionValues();	//better way to clear

	// remote approval:  sending info to rap with no security items at all causes 
	// erroneous state handle message in log and assert failures from RAP
	SCOTDO_INFO securityInfo;
	ra.GetSecurityInfo(&securityInfo);
	if(securityInfo.bApprovalRequired)  
	{
	  trace(L7, _T("+ra.OnSecurityApproval"));
	  ra.OnSecurityApproval();
	  trace(L7, _T("-ra.OnSecurityApproval"));
	}
	trace(L7, _T("+ra.OnTransactionEnd"));
	ra.OnTransactionEnd();
	trace(L7, _T("-ra.OnTransactionEnd"));
	
	// turn off lane light errors
  dm.turnOffTriColorLight();
  return STATE_NULL; // No need to create Attract while on Attract
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMAttractBase::OnWtDecrease(void)
{
	// TAR 257709 - moved the code that was here to SMStateSABase::AttractWtDecrease()
	// function.
	m_bSecViolationFromAttract = true; //383748
	AttractWtDecrease(); 
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMAttractBase::OnWtIncreaseNotAllowed() 
{
/*		trace (L6, _T("Increase at Attract while Quiet Mode = Y, ignoring wt for now"));
		CString csInstruction, csDesc;
		csDesc.Format(ps.GetPSText(RA_UNEXPECTEDINCREASEINFO, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits);
		csInstruction.Format(ps.GetPSText(RA_WEIGHTINCREASEINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTINCREASE, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits, _T(""));
		ra.OnNormalItem(csInstruction);
		bQuietNeedSecApproval = true; // need to get approval for the increase when start btn or scan item
//		nTimeOut = co.getTimeOutAttractQuietModeTimer();
		return STATE_NULL;*/
		//if not from Bag&EAS, then the wt is UnExpected
	nTimeOut = SMStateBase::nTimeOut;
	m_bSecViolationFromAttract = true; //383748

	CREATE_AND_DISPENSE(SecUnExpectedIncrease)(BES_START, BES_START);
}

///////////////////////////////////////////////////////////////////////////
// NewUI - adding the handler for help in attract to insure that the 
// Help context will always be in primary language 
SMStateBase *SMAttractBase::PSButtonHelp()
{
	csCurContext = _T("ContextHelp");
	static const TCHAR s_XMLSTRINGID[] = _T("StringId");
		
	CString strValue( _T("") );
	CStringArray strControlNameArray;
	if( PSX_SUCCESS == m_pAppPSX->GetControlNameList( strControlNameArray, csCurContext ) )
	{
		for( int i = 0; i < strControlNameArray.GetSize(); i++ )
		{
			if( PSX_SUCCESS == m_pAppPSX->GetCustomDataVar( s_XMLSTRINGID, strValue, strControlNameArray[i], csCurContext ) )
			{
				if( strValue.GetLength() )
				{
					m_pAppPSX->SetConfigProperty( strControlNameArray[i], csCurContext, UI::PROPERTYTEXTFORMAT, ps.ConvertToEscapeSequence( ps.GetPSText( _ttoi( strValue ) ), _T("\\n"), _T("\n") ) );
				}
			}
		}
	}
	
	return SMStateBase::PSButtonHelp();
}

//  TAR 247053 We will ignore back ups that come in while in attract mode
//  If it truly is a backup, the customer isn't in front of the display
//  anyway.  If there is still a backup when the first item is scanned, it
//  will get picked up at that point in time.  This is to avoid the problem
//  where a backup signalled during attract mode and cleared during attract
//  mode can cause the next customer to see a backed up item state after the
//  first item they scan.  DEM
SMStateBase* SMAttractBase::DMTakeawayBagAreaBackup(bool isBackedUp)
{
    return STATE_NULL;
}

// TAR 262131 - Paint Scan & Bag screen immediately, to give impression of quick response
void SMAttractBase::PaintScanAndBagScreen()
{
  g_bSnBScreenIsShown = true; //TAR195100 CMal/Chu021402 
  if (co.fCMRewardInfo)
  {
    ps.SetCMFrame(_T("ScanAndBagWithReward")); //*** NewUI ***//
    csCurContext = _T("ScanAndBagWithReward");
  }
  else
  {
    ps.SetCMFrame(_T("ScanAndBag"));  //*** NewUI ***//
    csCurContext = _T("ScanAndBag");
  }
  
  COleVariant v = VARIANT_FALSE;
  m_pAppPSX->SetConfigProperty(_T("LeadthruText"), csCurContext, UI::PROPERTYVISIBLE, v);
  ps.SetLeadthruText(LTT_SCANPROCESSING);
  v = VARIANT_TRUE;
  m_pAppPSX->SetConfigProperty(_T("LeadthruText"), csCurContext, UI::PROPERTYVISIBLE, v);

  ps.ShowCMTotal(true ,false);
  m_pAppPSX->SetConfigProperty( _T("ScanBagMessageModalBackGround"), csCurContext,UI::PROPERTYVISIBLE, VARIANT_FALSE );
  m_pAppPSX->SetConfigProperty( _T("ScanBagTextArea"),csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );
  v = _T("");
  m_pAppPSX->SetConfigProperty(_T("ScanBagTextArea"), csCurContext, UI::PROPERTYTEXTFORMAT, v);
  m_pAppPSX->SetConfigProperty( _T("ScanBagAVIMessageModalBackGround"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE );
  m_pAppPSX->SetConfigProperty( _T("ScanBagVideo"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE );
  //TAR 355761 -start
  int iButtonID;
  CString csButtonText;
  if(co.fStateDualLanguage && 
	  (co.fStateAllowLanguageSelectionDuringTransaction ||
	  (custInfo.GetOption(_T("PersonalizeLanguage")) == "Y")))
  {
	  if(co.m_iNumberOfLanguagesSupported > 2)
	  {
		  csButtonText = ps.GetPSText(BTT_LANGUAGE, SCOT_LANGUAGE_PRIMARY);
	  }
	  else if(co.m_iNumberOfLanguagesSupported == 2)
	  {
          int i = SCOT_LANGUAGE_PRIMARY; //tar 389302/388448
         int iButtonID;
         
         if(m_customerLanguage == SCOT_LANGUAGE_PRIMARY)
         {
             i = SCOT_LANGUAGE_SECONDARY;
         }
         else if(m_customerLanguage != SCOT_LANGUAGE_SECONDARY)//tar 389302/388448
         {
             //fix TAR388448+
             trace(L2, _T("WARNING: Unexpected value %d for m_customerLanguage;reset to primary language"), m_customerLanguage); 
             m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
             //fix TAR388448-
              //i = SCOT_LANGUAGE_PRIMARY;
         }

         //Get the language name text for button
         csButtonText = co.language[i].csName;
         if(_istdigit(*csButtonText))
         {
             iButtonID = _ttoi(co.language[i].csName);
             csButtonText = ps.GetPSText(iButtonID, (SCOT_LANGUAGE_TYPE)i);
         }
         ps.SetPSText(PS_WORK1, csButtonText);
      }
      else
      {
         ASSERT(FALSE); // shouldn't enter here because at least 2 languages should be supported
      }

     ps.SetButtonText(_T("CMButton5MidiList"),PS_WORK1);
     //make visible the CMButton5MidiList
     m_pAppPSX->SetConfigProperty(_T("CMButton5MidiList"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
  }
  //TAR 355761 -end

  if(co.fStateDualLanguage)
  {
      if(m_LanguageUsedPreviously != m_customerLanguage)
      {
        ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT);           
        m_LanguageUsedPreviously=m_customerLanguage;
      }
  }
  ps.ShowCMFrame();
}
// TAR 262131 - end

VOID CALLBACK SMAttractBase::AttendantModeTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	m_pAttract->HandleAttendantModeTimedOut();
    
}

void SMAttractBase::HandleAttendantModeTimedOut()
{
	trace(L2, _T("+SMAttractBase::AttendantModeTimeOut=%d"), co.getTimeOutAttendantModeTimer());
	KillTimer(hw,ATTENDANT_MODE);

	if (co.IsAttendantModeOn() && !fInSignedOn && !fInSignedOff && !fSAInTransaction)
    {
        if (co.IsRemoteScannerConnected())
        {
            dm.ScannerRemoteAccess(false);
            co.SetRemoteScannerConnected(false);
        }
		rp.TransactionStart(rp.m_csBusinessDate);
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
        TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("Attract") );
		rp.ModeChange(rp.m_lAttendantEntryID, ATTENDANT, EXIT);
		rp.TransactionEnd();
		ps.ShowTrainingModeOnScreen(TrainingModeObj.IsTMOn());
		UpdateStateName(ps.GetPSText(LTT_ATTRACT, SCOT_LANGUAGE_PRIMARY));
    }
}
VOID CALLBACK SMAttractBase::ScanScaleAttractTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	trace(L6, _T("+SMAttractBase::ScanScaleAttractTimedOut(void)"));
	mp.sCurr->ScanScaleAttract();
	trace(L6, _T("-SMAttractBase::ScanScaleAttractTimedOut(void)"));
}

void SMAttractBase::ScanScaleAttract()
{

	
	//Once Timer has expired, we can now say the appropriate audio
	if(lDMScaleWeight != 0)
	{
		 KillTimer(hw, SCANSCALEATTRACT);
		 m_TimerResponse = 0;
		 if(co.fStateDualLanguage)
		 {
			 SMStateBase::DMSayPhrase(SELECTLANGUAGE);
		 }
		 else
		 {
			 DMSayPhrase(TOUCHSTART);
		 }

	 }
	trace(L6, _T("::ScanScaleattract-Audio was played"));

}
//******************************************************
//Method : PSButton7
//Purpose: Change the volume level
//Parameters:  
//Return Values: 
//******************************************************
SMStateBase  *SMAttractBase::PSButton7()
{
    int nRC;
	bool bRC=KillTimer(hw, VOLUME_CONTROL);
    trace(L6, _T("-SMAttractBase::PSButton7 - KillTimer = %d"), bRC);
	CVolumeControl VolumeControl(SETVOLUME);
	//+TAR#343251 
	if ( co.GetVolumeControlTimer() > 0)
	{
		//set the timeout
		nRC = SetTimer(hw, VOLUME_CONTROL, co.GetVolumeControlTimer()*1000, &VolumeControlTimedOut);	
        trace(L6, _T("-SMAttractBase::PSButton7 - Volume control timer started nRC= %d"), nRC);

	}
	//-TAR#343251
	return STATE_NULL;
}

//******************************************************
//Method : VolumeControlTimedOut
//Purpose: This is called after VolumeControlTimer is reached.
//Parameters:  
//Return Values: 
//******************************************************
VOID CALLBACK SMAttractBase::VolumeControlTimedOut ( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	trace(L6, _T("+SMAttractBase::VolumeControlTimedOut "));
	m_pAttract->HandleVolumeControlTimedOut();
	trace(L6, _T("-SMAttractBase::VolumeControlTimedOut ")); 
    
}
//******************************************************
//Method : HandleVolumeControlTimedOut
//Purpose: 
//Parameters:  
//Return Values: 
//******************************************************
void SMAttractBase::HandleVolumeControlTimedOut()
{
	trace(L6, _T("+SMAttractBase::HandleVolumeControlTimedOut"));
	KillTimer(hw, VOLUME_CONTROL);
	CVolumeControl VolumeControl(SETDEFAULT);
	if ( co.GetVolumeControlTimer() > 0)
	{
		//set the timeout
		SetTimer(hw, VOLUME_CONTROL, co.GetVolumeControlTimer()*1000, &VolumeControlTimedOut);	
	}
	trace(L6, _T("-SMAttractBase::HandleVolumeControlTimedOut"));
}

//RFQ SR681
VOID CALLBACK SMAttractBase::BackToPrimaryLanguageTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	trace(L6, _T("+SMAttractBase::SingleLanguageButtonAtAttractTimedOut"));
	m_pAttract->HandleBackToPrimaryLanguageTimedOut();
	trace(L6, _T("-SMAttractBase::SingleLanguageButtonAtAttractTimedOut"));
}

void SMAttractBase::HandleBackToPrimaryLanguageTimedOut()
{
	KillTimer(hw, LANGUAGE_CONTROL);
	if ( m_customerLanguage != SCOT_LANGUAGE_PRIMARY)
	{
		m_LanguageUsedPreviously = m_customerLanguage; //417933
		m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
		ps.SetLanguage(m_customerLanguage);
		Initialize();
	}

	if ( co.GetSingleLanguageButtonAtAttractTimer() > 0)
	{
		//set the timeout
		SetTimer(hw,LANGUAGE_CONTROL, co.GetSingleLanguageButtonAtAttractTimer()*1000, &BackToPrimaryLanguageTimedOut);
	}
}
//end RFQ SR681

void SMAttractBase::DisplayHopperSubstitution(void)
{
	if (co.fOperationsHopperSubstitutionIntervention)
 {
  bool bEmpty = false;
  bool bChuteJam = false;
  if (IsHopperSubstitutionUsed(bEmpty, bChuteJam) ) 
  {
	  if (nHopperTrxCount >= co.nOperationsHopperFixLaterTrxLimit)
	  {
		  lUnapprovedHopperSubstitution++;
		  lRegularYellowNeedingApproval++;
		  dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
								DM_TRICOLORLIGHT_ON,
								DM_TRICOLORLIGHT_NORMALREQUEST);

		  CString csInstruction;
		  CString csAnchorStateName = getSCOTStateName();
		  
		  CString sHopperStatusLine = getHopperStatusString();
		  if (!sHopperStatusLine.IsEmpty())
				sHopperStatusLine = sHopperStatusLine + _T("\n");
	
		  csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), sHopperStatusLine, ps.GetPSText(BUTTON_RA_HOPPER_ERROR, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, _T(""), ps.GetPSText(BUTTON_RA_HOPPER_ERROR, SCOT_LANGUAGE_PRIMARY));
		  trace(L7, _T("+ra.HopperSubstitution, Message=%s"), csInstruction);			
		  ra.OnHopperSubstitution(csInstruction);
		  trace(L7, _T("-ra.HopperSubstitution"));
	  }
	  
  }
 }

}
