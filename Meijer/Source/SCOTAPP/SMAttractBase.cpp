//////////////////////////////////////////////////////////////////////////////////////////////////
//
// TITLE: Class implementation for SMAttractBase State
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"                 // MS MFC always needed first
#include "LaneStateData.h"
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes

#include "SMAttract.h"
#include "SMTakeCard.h"
#include "SMTakeChange.h" //SSCOADK-836
#include "SMScaleBroken.h"
#include "SMOutOfService.h"
#include "SMLaneClosed.h"
#include "SMSecUnexpectedIncrease.h" 
#include "SMWaitForApproval.h"
#include "SMSystemMessage1.h" // TAR 292956
#include "SMPutBagOnScale.h"//SR085

#include "Reporting.h"
#include "MPProcedures.h"           // RFC 276235

#include "DMProcedures.h"
#include "VolumeControl.h"
#else
#include "FakeVolumeControl.h"
#endif //_CPPUNIT

#ifndef _CPPUNIT
#include "DeviceErrorLookUpAPI.h" //SR672

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("AttractBase")
DLLEXPORT extern HWND hw;
DLLEXPORT extern MPProcedures mp;   // RFC 276235

#else  // _CPPUNIT
#include "SMAttractBase.h"
#include "DMTriColorLightConstants.h"
#endif // _CPPUNIT

DEFINE_TIMESTAMP


IMPLEMENT_DYNCREATE(SMAttractBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(Attract)
int SMAttractBase::ms_nNumberOfAutoRunTrans = 0;
long SMAttractBase::ms_nAutoRunTransactionTally = 0;
SMAttractBase *SMAttractBase::m_pAttract = NULL;
bool SMAttractBase::ms_bTransactionCancelled = false;

CString SMAttractBase::m_csLoyaltyCheckType=_T("");

bool SMAttractBase::m_bLanguageButtonClicked = false;
bool SMAttractBase::m_bGiveChange = false;  //SSCOADK-1554
bool SMAttractBase::ms_bInitalizeIncomplete = false;
//////////////////////
// Constructor
//////////////////////
SMAttractBase::SMAttractBase()
: bGoingToInProgress(false), csCurContext(EMPTY_STRING), m_bLangChange(false)
{
  IMPLEMENT_TIMESTAMP
  // We have started the application successfully if we get here, so we can
  // reset the counter used to restart SCOTAPP during error conditions.
  // Nothing we can do with the return code.

#ifndef _CPPUNIT
  AppRestartCount::reset();
  saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
#endif // _CPPUNIT

  m_pAttract = this;
}


////////////////////////////////////
// Initialize
////////////////////////////////////
SMStateBase  *SMAttractBase::Initialize(void)
{
  //+SSCOADK-6740 - additional fix
  if (co.GetfStateDualLanguage() && m_customerLanguage != SCOT_LANGUAGE_PRIMARY)
  { ps.SetLanguage(m_customerLanguage); }
  //-SSCOADK-6740	
	
#ifndef _CPPUNIT
  int i;
  m_TimerResponse = 0;          //RFC 276235
  m_bCashRecyclerStopDeposit = 0;  //reset
  if(AnySMDataNeeded())
  {
    setAnchorState(BES_START);
    return ProcessSMDataNeeded();
  }

  //+SSCOADK-835
  if(nDMCashInserted)
  {
     m_bGiveChange = true; //SSCOADK-1554
     CREATE_AND_DISPENSE(TakeChange)(false);
  }
  //-SSCOADK-835

  m_nDMCashPaid = 0;

 
  g_savedAnchorStateFromAttract = BES_START; //416582

  //+TAR 450342 
  if(ms_bLaneClosedFromRapFlag || m_bIsFromLaneClosed) // adding m_bIsFromLaneClosed - SSCOB-1399
  {
      //trace(L6, _T("Turn on lane light when lane closed from RAP."));
      dm.RemoteRelay(DM_MISC_STATE_ON); //Lane light on
      ms_bLaneClosedFromRapFlag = false;
  }
  //-TAR 450342

  m_bIsFromLaneClosed = false;  //TAR 292436 

  //395064+
  if(dm.fStateTakeawayBelt)
  {
      TABstateBeforeTransport = BES_EMPTY_STATE;
  }
  //395064-


  // +SR683
    if(!co.fStateSecurityDisableNoReset && (co.IsSecurityDisableOn() || co.GetSecurityDisableSubCheckOn()))
    {
        //(+)SSCOADK-849, SSCOADK-850
        //TAR [384803] NOTE: added another if statement so as to not reset security after the reporting state
        if ((this->csLastNonHelpStateName != _T("SMSmReportsMenu")) || (this->csLastNonHelpStateName == _T("SMThankYou")))  //+BEGIN TAR [384803]
        {
            ra.OnEnableSecurity();

            if(g_bIsSecurityDisableSubCheckOnly)
            {
                trace(L6, _T("call exit disable security when g_bIsSecurityDisableSubCheckOnly."));

                co.SetSecurityDisableSubCheck(false);
                SetSecurityConfig(CONFIG_KEY_IS_SECURITYDISABLE_SUB_CHECK_ON, _T("false"));
            }
            else
            {
                trace(L6, _T("call exit disable security when fStateSecurityDisableNoReset."));

                co.SetSecurityDisable(false);

                CKeyValueParmList parmList;
                CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_EXIT_DISABLE_SECURITY_MODE);
                _bstr_t bstrParmList((const _TCHAR *)parmList);
                BSTR bstrResult = NULL;
                bstrResult = NULL;
                SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
                SysFreeString(bstrResult);
            }

            if(csOperator != _T(""))
            { rp.SetOperator(csOperator); }

            rp.ModeChange(rp.m_lAttendantEntryID, SECURITYDISABLED, EXIT);
        }  //-END TAR [384803] by: tl185041
        //(-)SSCOADK-849, SSCOADK-850
    }
    else if(co.fStateSecurityDisableNoReset && co.IsSecurityDisableOn())
    {
        if(!g_bIsSecurityDisableSubCheckOnly)//SSCOADK-849, SSCOADK-850
        {
            CKeyValueParmList parmList;
            CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_ENTER_DISABLE_SECURITY_MODE);
            _bstr_t bstrParmList((const _TCHAR *)parmList);
            BSTR bstrResult = NULL;
            bstrResult = NULL;
            SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
            SysFreeString(bstrResult);  
        }
    }
  // -SR683
    
  //+ TAR 356575 - to ensure that if in Attendant Mode and beginning another trx, Sec Mgr doesn't come out of AssistMode
  if(co.IsSecurityBypassOn())
  {
    bool bAttnd = co.IsAttendantModeOn();
    co.SetAttendantMode(false);
    //bool bSecurityDisable = co.IsSecurityDisableOn();  //SR683
    //co.SetSecurityDisable(false);  //SR683
    
    //TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode"));
    //TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmAssistMode"));
    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_REENTER_ASSISTMODE_FROM_ATTRACT);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    BSTR bstrResult;
    bstrResult = NULL;
    SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
    SysFreeString(bstrResult);
    
    co.SetAttendantMode(bAttnd);
    //co.SetSecurityDisable(bSecurityDisable);  //SR683
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

  // SSCOP-1158
  ReloadThemes();

  // -TAR 309003
  CString csLReceiptFile = LRECEIPT_FILE;
  CString csLReceiptTmpFile = LRECEIPT_TMP_FILE;

  GET_PARTITION_PATH(LRECEIPT_FILE, csLReceiptFile.GetBuffer(_MAX_PATH));
  GET_PARTITION_PATH(LRECEIPT_TMP_FILE, csLReceiptTmpFile.GetBuffer(_MAX_PATH));

  csLReceiptFile.ReleaseBuffer();
  csLReceiptTmpFile.ReleaseBuffer();

  //Log "healthy" message RFC 262343
  eo.Error(INFO,SM_CAT_STARTUP_INIT, SCOTAPP_ATTRACT_SCREEN_ENTRY, (long) (NCRSM_SNMP_ACTION | NCRSM_REMOTE_LOG_ACTION));

  if(dm.m_csCashRecyclerModel.IsEmpty() )
   {
      DMHandleAcceptorHigh();
   }

  // TAR 311471 - Begin
  dm.CashRecyclerResetCounts();
  // TAR 311471 - End
  if (!m_bLanguageButtonClicked)
  {
      custInfo.ClearCustomerInfo(); //TAR 452221
  }

 DisplayHopperSubstitution();

  //TAR 292956 start - if cash acceptor out, go to system message until fixed
  if (co.fTenderCashAllowed)
  {
    //SR672
   if ( BILL_STATUS_TAMPER_OUT == lastDMCashAcceptorCode )
    //casette was out during POS Startup and it is not back yet
    {
      bLaneClosedTamperOut = false;
      ms_bInitalizeIncomplete = true;
      //SR672
      //CString csWork = _T("DeviceErrorCashAcceptor.xml:NCR_EBILLSO_CASS_OUT");
      CString csWork, s2(_T(""));
      CString s1=dm.GetDeviceModelName(DMCLASS_CASHACCEPTOR, 0);
      bool bLocalCheckHealth = false;

      csWork = DMGetDeviceErrorHTML(DMCLASS_CASHACCEPTOR, s1, s2, BILL_STATUS_TAMPER_OUT, EXTENDED_RESULT_ANY, 0, NULL, &bLocalCheckHealth, CONTEXT_ANY, co.csPrimaryLanguageCode );

      CREATE_AND_DISPENSE(SystemMessage1)(csWork,      // char *message
                                          PS_NOTEXT,
                                          PS_NOTEXT,    // no prompt
                                          0,                    // no value
                                          DMCLASS_CASHACCEPTOR, // DM device
                                          true, true);     // DM check health after
    }
  }
  //TAR 292956 end

  // TAR 306397 - Begin
  DMCheckCashCounts(false);     // Ensure that dm.GetCoinsOrBillsVeryLow() returns valid result.
  // TAR 306397 - End

      // Add Glory CM Support
    if(!dm.m_csCashRecyclerModel.IsEmpty())
    {
        // Check cash unbalance status for Glory
        CheckCashDiscrepancyStatus();
    }


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
      ms_bInitalizeIncomplete = true;
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
  io.Reset();   // 228225 - failsafe for when the F&P is pressed and there are no sold items LPM022303
  bIsInFinalization = false; //tar 194770
  EnableHandheldAssistMode(true);

  //TAR200557 if media errors and haven't told RAP, then tell RAP
  if(dm.GetCoinsOrBillsVeryLow() || dm.GetPrinterPaperLow() )
  {
    // (*) SR898 ====================
    DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                       DM_TRICOLORLIGHT_BLINK_1HZ,
                       DM_TRICOLORLIGHT_NORMALREQUEST);  
    // Notify RAP is done in updateStateName, below LPM060502 206311  
  }
  else if(!fRequestForHelp && !isSecurityApprovalRequired())  // tar 242625  // Begin TAR 210017
  {
    // Turn off all lights on just in case because lane light is off at attract
    
    // (*) SR898 ====================
    DMTurnOffTriColorLight();
  }
  // End TAR 210017
  bToldRAPMediaLow = false;

  //+ TAR426302 - should be called ahead for it to be initialized first before calling the showframe. 
  DMCashAcceptorDisable();  // TAR#137272 should be disabled at attract screen
  DMCoinAcceptorDisable();
  
  //Update RAP window with SCOT state and media low status
  UpdateStateName(ps.GetPSText(LTT_ATTRACT, SCOT_LANGUAGE_PRIMARY));
  //bQuietNeedSecApproval = false; //for quiet mode //Tar 224576
  m_lRememberVirtualZero = 9999999;  // 227472 remember the virtual zero in case it was wind. 9999999 means not set. LPM021103 SOMEONE CAN MAKE THIS A #define VIRTZERO_NOTSET = 99999999 LATER

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
      delete [] cDMLastTrack; //SSCOP-4165
      cDMLastTrack = NULL;
      nDMLastTrackLen = 0;
      csDMEncryptedPin = EMPTY_STRING;
  }

  if(fPutSCOTinLC || bLaneClosed)
  {
      bTransactionComplete = false; //TAR 411286
      ms_bInitalizeIncomplete = false;
      ResetAllTransactionValues();      // TAR 283045 - TAR Migrated to 4.0
      RETURNSTATE(LaneClosed)
  }

  if(fStopSCOT)
  {
      fStateFreeze = false;
      fStateAssist = false;
      return STATE_STOP;
  }

  //SSCOP-3552 Removed "ASSERT(!fSAInTransaction);"
  fSAInTransaction = false;
  appBusy.ClearBusyStatus();

#endif // _CPPUNIT
  if(co.GetfStateSingleLanguageButtonAtAttract() || m_bLangChange)//TAR#440749
  {
      if(m_bLanguageButtonClicked && m_customerLanguage != SCOT_LANGUAGE_PRIMARY)
          ps.SetLanguage(m_customerLanguage);
      else if(this->csLastNonHelpStateName == _T("SMSelectLanguage") && m_customerLanguage != SCOT_LANGUAGE_PRIMARY)
          ps.SetLanguage(m_customerLanguage);
      // +SSCOP-2756
      //else if( m_bSecViolationFromAttract == true   && m_customerLanguage != SCOT_LANGUAGE_PRIMARY) //383748
      //{
      //  m_bSecViolationFromAttract = false;
      //  ps.SetLanguage(m_customerLanguage);
      //}
      // -SSCOP-2756
      else if (! m_bLangChange || (ms_bTransactionCancelled == true))
      {
          ms_bTransactionCancelled = false;
          m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
          ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
      }
  }
  else
  {
      m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
      ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
  }
#ifndef _CPPUNIT
  ps.SetNextGenUICustomerLanguage(m_customerLanguage);

  if (gpSCOTAuxDisp)
      gpSCOTAuxDisp->SetLanguage(m_customerLanguage);
      
  //ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT);    //tar 375008    TAR [383748] moved down below just after ps.ShowCMFrame();
  
  setAnchorState(BES_START);
  nTimeOut = co.getTimeOutAttract();
  if (co.IsAttendantModeOn() && co.getTimeOutAttendantModeTimer() !=0 )
  {
    SetTimer(hw, ATTENDANT_MODE, co.getTimeOutAttendantModeTimer()*1000, &AttendantModeTimedOut);
  }

#endif // _CPPUNIT
  if (g_bMobileAppEnabled)
  {
    recallList.Reset();
    mobileCart.SendReadyForTransaction();
  }
#ifndef _CPPUNIT

  // only logs if transinprogress flag is true
  // TAR 227850 - changed the trace level to 6
 if (!rp.IsInterventionEndInProgress() || co.IsAttendantModeOn())  //429111 && 449618 
  {
    trace(L6, _T("+rp.TransactionEnd"));
    rp.TransactionEnd();
    trace(L6, _T("-rp.TransactionEnd"));
  }

  CString csAssistTemplateCommand;
  csAssistTemplateCommand.Format(_T("COMMAND=UIInitialize;ASSISTMODEFILENAME=%s;"), (LPCTSTR)co.csTemplateAssistMode);
  ra.OnAddOtherEvent((LPCTSTR)csAssistTemplateCommand);

  // moved Reset to later in initialize HOWEVER
  // !!! ResetAllTransactionValues MUST be called BEFORE DMCheckCashCounts is called!!!
  if((this->csLastNonHelpStateName != _T("SMAttract")) //Tar 224576 Don't reset values if press HELP and then come back to Attract 
      || ms_bInitalizeIncomplete  || m_bGiveChange  )
  {
    bTransactionComplete = false; //TAR 411286
    m_bGiveChange = false; //SSCOADK-1554
    ms_bInitalizeIncomplete = false;
    ResetAllTransactionValues();        // ensure all counters. totals, flags are reset
  }

  ps.ShowCMRewardInfoBox(lTrxTotalSavings, lTrxTotalRewardPoints, CustomerMessage);   //tar 373874 //SSCOADK-7435

  // if we need to load options, do it now
  if (fDoOptions)
  {
     fDoOptions = false;
     return ReloadOptions();
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

 // commented them out for TAR426302
 // DMCashAcceptorDisable();    // TAR#137272 should be disabled at attract screen
 // DMCoinAcceptorDisable();

  DMCashAcceptorReject();   // eject any cash not accepted  
  DMCoinAcceptorReject();   // eject any coins not accepted 

  dm.PinPadDisable(); //TAR 121779

  if(co.IsSecurityBypassOn())
  {
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("Attract") );
  }

  CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMAttractBase::Initialize"));
  CString csTheme = custInfo.GetOption(_T("LaneTheme")); //SR814.5 SR815.6 SR851 Personalization and Security
  m_pAppPSX->SetTheme(csTheme); //SR815 - Personalization
  SetPersonalThemeUsed(csTheme);//SR814.5 SR815.6 SR851 Personalization and Security  
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
  if (co.GetNumberOfLanguagesSupported() < 3) 
  {
      ps.SetCMFrame(_T("Attract"));
      csCurContext = _T("Attract");
  }
  else
  {
      if(co.GetfStateSingleLanguageButtonAtAttract())
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

  if (co.GetfStateDualLanguage() && !co.GetfStateSingleLanguageButtonAtAttract())
  {
      // For 3.1.1 Change, language name for button 1 and 2 can be text or message ID corresponding in message file
      CString csButtonName;
      // check to see if language name is text or number.  If it's a number, get the text message corresponding to this number

      for (i=0; i < SCOT_LANGUAGE_MAX; i++)
      {
          if (i < co.GetNumberOfLanguagesSupported())
          {
              // If there is a language defined for this entry
              csButtonText[i] = co.Getlanguage(i).csName;
              if(_istdigit(*csButtonText[i]))
              {
                  iButtonID[i] = _ttoi(co.Getlanguage(i).csName);
                  csButtonText[i] = ps.GetPSText(iButtonID[i], (SCOT_LANGUAGE_TYPE) i);
              }
              ps.SetPSText(PS_WORK1,csButtonText[i]);
              v = VARIANT_TRUE;
              switch (i)
              {
                  // Set appropriate button text and button visible
                  case SCOT_LANGUAGE_PRIMARY:
                      if (co.GetNumberOfLanguagesSupported() < 3) 
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
                      if (co.GetNumberOfLanguagesSupported() < 3) 
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

          else if (co.GetNumberOfLanguagesSupported() > 2)
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
          else if (co.GetNumberOfLanguagesSupported() == 1)
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
      if(!co.fOperationsCustomerBagAllowed) //SSCOADK-2241
      {
          v = VARIANT_FALSE;
          m_pAppPSX->SetConfigProperty(_T("CMButton2Lg"), csCurContext, UI::PROPERTYVISIBLE, v);
      }
      ps.CMButtonFlash(_T("CMButton1Lg"),true);   
  }

  if(co.GetfStateSingleLanguageButtonAtAttract())
  {
      if(co.GetfStateDualLanguage() && co.GetNumberOfLanguagesSupported() == 2)
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
         
         csButtonText = co.Getlanguage(i).csName;
         if(_istdigit(*csButtonText))
         {
             iButtonID = _ttoi(co.Getlanguage(i).csName);
             csButtonText = ps.GetPSText(iButtonID, (SCOT_LANGUAGE_TYPE)i);
         }
         ps.SetPSText(PS_WORK1, csButtonText);
         ps.SetButtonText(_T("CMButton8Global"),PS_WORK1);
         m_pAppPSX->SetConfigProperty(_T("CMButton8Global"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
      }
      else if(co.GetNumberOfLanguagesSupported() > 2)
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
  SAWLDBScannerEnable();    // enable the scanner   moved to after the showframe 123168
  // EndCodeWLDBSA11
  DMScaleEnable();         // enable the scale TAR123168    LPM101199  Scanner disabled while terminal out of service
  CString csAttractText;

  if (co.GetfStateDualLanguage())  // TAR 241836 - check for dual language first 
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
  if(co.GetfStateSingleLanguageButtonAtAttract())
  {
      //CString str = ps.GetCurrentContext();
    //m_pAppPSX->SetConfigProperty(_T("CMButton8MidiList"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_TRUE);
    DisplayAuxScreen(AUX_SCREEN_ATTRACT, true);
  }
  else if ((co.GetfStateDualLanguage()) && (co.GetNumberOfLanguagesSupported() < 3))

  {
      // For 3.1.1 Change, language name for button 1 and 2 can be text or message ID corresponding in message file
      // check to see if language name is text or number.  If it's a number, get the text message corresponding to this number
    DisplayAuxScreen(AUX_SCREEN_ATTRACT_LANG, 
                     true, //TAR208962 CHU 091602 changed from false to true so it always refresh
                     2, 
                     csButtonText[0],       //TAR 244298
                     csButtonText[1]);  //TAR 244298
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
  fSSWtFromAttract = false;     // TAR #123172 only used if scan scale wt increase from attract screen LPM101499    
#endif
  //+4.2 provide subtle on-screen volume control
  //check if ShowVolumeControl in 'Scotopts.dat' is set to "Y". 
  if (co.IsShowVolumeControl() )
  {
    CLaneStateData laneStateData(m_pAppPSX);
    laneStateData.SendNextGenUIData(_T("NextGenUIVolumeControlIncLevel"), (long)co.GetVolumeControlIncrementLevel());
#ifndef _CPPUNIT
    m_pAppPSX->SetConfigProperty(_T("ButtonVolumeControl"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);
    if(co.GetVolumeControlTimer()>0)
        ps.CreateVolCtlThread();      // TAR 351654
    else
        ps.CreateVolCtlThread(false);   // TAR 404826
#endif
  }
#ifndef _CPPUNIT
  else
  {
      m_pAppPSX->SetConfigProperty(_T("ButtonVolumeControl"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
  }
  //-4.2 provide subtle on-screen volume control
    if(co.GetfStateSingleLanguageButtonAtAttract() && co.GetSingleLanguageButtonAtAttractTimer() > 0 &&
      co.GetNumberOfLanguagesSupported() > 1)
  {
      int n = co.GetSingleLanguageButtonAtAttractTimer();
      SetTimer(hw,LANGUAGE_CONTROL, co.GetSingleLanguageButtonAtAttractTimer()*1000, &BackToPrimaryLanguageTimedOut);
  }

  ps.ShowCMFrame();

  //SSCOADK-5989 + 
  //When duallanguage, weights and measures and scale configured, need to run DMConfigureScaleDisplay to reset  
  //weights and measures (W&M) info and reset scale unit. Need to do it for two known reasons:
  //1) HandleBackToPrimaryLanguageTimedOut() calls Initialize() after setting back to primary language
  //2) After using a non primary language and return to Attract have to reset W&M back to primary
  if (co.fStateDualLanguage && co.fStateDisplayWeightsAndMeasures && dm.IsDeviceConfigured(DMCLASS_SCALE))
  {
          // If healthy, then set scale display.
          long retErrorCode = DMConfigureScaleDisplay();
          if( retErrorCode > -1)
          {
	        fStateFreeze = false;
	        CString csWork = ps.GetPSText(retErrorCode);
	
	        return SMCustomerModeState::createLevel1Message(
                        csWork,  // char *message
						PS_NOTEXT,         // no canned message
						PS_NOTEXT,         // no prompt
						0,                 // no prompt value
						-1,                // no device
						false, false);     // no DM check health needed //SR672
          }
   }
   //SSCOADK-5989 -
  
  //ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT);    //TAR 383748

  ps.Message12(csAttractText, false); //dp185016 tar 287707


// SR683: Show the security disable button on RAP when option allows
    m_pAppPSX->SetTransactionVariable(_T("SecurityDisableAllowed"),
        co.fStateSecurityDisableAllowed ? VARIANT_TRUE : VARIANT_FALSE);

  if(co.GetfAutoRunOn() || co.GetfAutoRunAssistModeOn())
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
          co.SetfAutoRunOn(false); // must re-start APP or re-load Options in order to restart AutoRun
          co.SetfAutoRunAssistModeOn(false);
          ms_nNumberOfAutoRunTrans = 0;
      }
  }
  bInitializeToTimeOut = true;
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
  // SR875
  DisplayVerifoneForm(_T("WELCOME"));

  return TimedOut();     // stay in attract state, but refresh the display
#else //CPPUNIT
  return STATE_NULL;
#endif //_CPPUNIT
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
  //return PopStateFifoStack();           // see if any pending messages
  //  176732 LPM082401 This is dangerous because the 
  //  State Stack might have a screen change in it now.
  if(co.GetfAutoRunOn() || co.GetfAutoRunAssistModeOn())
  {
      return PSButton1();
  }
  else
  {
      return STATE_NULL;
  }
}

#ifndef _CPPUNIT
//////////////////////////////////
// UnInitialize
//////////////////////////////////
void SMAttractBase::UnInitialize(void)
{
  mobileCart.NotReadyForTransaction();
  KillTimer(hw,ATTENDANT_MODE);
  // +SSCOP-3809
  if (!m_csLoyaltyCheckType.IsEmpty())
  {
      KillTimer(hw, LOYALTY_LOOKUP);
      trace(L6, _T("SMAttractBase::UnInitialize Killed Timer LOYALTY_LOOKUP"));
      m_csLoyaltyCheckType = _T("");
  }// -SSCOP-3809

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
     m_LanguageUsedPreviously=m_customerLanguage;
     m_bLangChange = false; //SSCOP-2756
  }
     
  // 206331 LPM lane light not blinking after going into media status even if printer is low
  if (dm.GetPrinterPaperLow())
  {
    // (*) SR898 ====================
    DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                       DM_TRICOLORLIGHT_BLINK_1HZ,
                       DM_TRICOLORLIGHT_NORMALREQUEST);  
  }
  else if(!isSecurityApprovalRequired())
  {
    // (*) SR898 ====================
    DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                       DM_TRICOLORLIGHT_ON,
                       DM_TRICOLORLIGHT_NORMALREQUEST);
  }
  SAWLDBScannerDisable();                         // disable the scanner

  if (!cDMLastTrack && !waitingForCardRemoval)
  {
    DMMSRDisable(); // if no card inserted, disable, no problems
  }
  else if(!co.fTenderIsMotorizedMSR)        // we scanned in a card and we're using a swipe reader
  {
    DMMSRDisable();                 // Disable reader to prevent "Can't swipe card now" msgs
  }
  // TAR 199152 & 199010 - stop playing AVI and hide the window if large vedio was played at init
  if (!co.getMoviePlayOnAttract()->GetLength())
    ps.HideVideoLarge();    // end TAR 199152 & 199010

  DMMSRLight(DM_MISC_STATE_OFF);      // turn off MEI, card or no card
  if (co.GetfStateDualLanguage())
  {
      for (int i=0; i < co.GetNumberOfLanguagesSupported(); i++)
      {
          switch (i)
          {
              // Set appropriate button text and button visible
              case SCOT_LANGUAGE_PRIMARY:
                  if (co.GetNumberOfLanguagesSupported() < 3) 
                  {
                      ps.CMButtonFlash(_T("CMButton1Lg"), false);
                  }
                  else
                  {
                      ps.CMButtonFlash(_T("CMButton1MidiList"),false);   
                  }
                  break;
              case SCOT_LANGUAGE_SECONDARY:
                  if (co.GetNumberOfLanguagesSupported() < 3) 
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
#endif //_CPPUNIT



///////////////////////////////////
// Start Button, or primary language button pressed
///////////////////////////////////
SMStateBase  *SMAttractBase::PSButton1(void)
{
    CString csButtonText;
    int iButtonID;

    SCOT_LANGUAGE_TYPE customerLanguage; 
    customerLanguage = ps.GetLanguage();

    
    if(!co.GetfStateDualLanguage())
    {  
        csButtonText = ps.GetPSText(MSG_LANGUAGENAME);
        customerLanguage = SCOT_LANGUAGE_PRIMARY; 

    }
    else
    {
        if(!co.GetfStateSingleLanguageButtonAtAttract())
        {
            csButtonText = co.Getlanguage(0).csName;
            if(_istdigit(csButtonText[0]))
            {
                iButtonID = _ttoi(co.Getlanguage(0).csName);
                csButtonText = ps.GetPSText(iButtonID, (SCOT_LANGUAGE_TYPE) 0);
                
            }
            customerLanguage = SCOT_LANGUAGE_PRIMARY;
            
        }
        else
        {
            csButtonText = co.Getlanguage(customerLanguage).csName;
            if(_istdigit(csButtonText[0]))
            {
                iButtonID = _ttoi(co.Getlanguage(customerLanguage).csName);
                csButtonText = ps.GetPSText(iButtonID, (SCOT_LANGUAGE_TYPE) customerLanguage);
                
            }
            //csButtonText = co.language[0].csName;//ps.GetPSText(MSG_LANGUAGENAME);
            //csButtonText = co.Getlanguage(customerLanguage).csName;//ps.GetPSText(MSG_LANGUAGENAME);            
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

#ifndef _CPPUNIT
///////////////////////////////////
// Secondary language Button or /*SR085*/ "I brought my own bag" button pressed
///////////////////////////////////
SMStateBase  *SMAttractBase::PSButton2(void)
{
  if(!co.fOperationsCustomerBagAllowed)//SR085 - secondary language button pressed
  {
      ASSERT(co.GetfStateDualLanguage());  

      CString csButtonText;
        int iButtonID;
        
        csButtonText = co.Getlanguage(1).csName;
        if(_istdigit(csButtonText[0]))
        {
            iButtonID = _ttoi(co.Getlanguage(1).csName);
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
      if (co.GetfStateDualLanguage())
    {
      m_customerLanguage = customerLanguage;
      ps.SetLanguage(customerLanguage);
      co.csCMCustomerMessage = ps.GetCustomerMessageDefault(); //SSCOB-1860
      CustomerMessage = co.csCMCustomerMessage;

      ps.SetTareListToCurrentLanguage();
      if(gpSCOTAuxDisp)
      gpSCOTAuxDisp->SetLanguage(customerLanguage);

      if(m_LanguageUsedPreviously != m_customerLanguage)
      {
        m_LanguageUsedPreviously=m_customerLanguage;        
      }
    }

      if(!fSAInTransaction)
          SASendSCOTInput( ITEMIZATION );

      //395551+
      setAnchorState(BES_PUTBAGONSCALE);
      //395551-
      g_savedAnchorStateFromAttract = BES_PUTBAGONSCALE; //416582

      //TAR425221+
      //return TBStart();
      if (co.GetfStateDualLanguage())
          return TBStart(co.Getlanguage(m_customerLanguage).szCode); 
      else
          return TBStart();
      //TAR425221-

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
    
    csButtonText = co.Getlanguage(2).csName;
    if(_istdigit(csButtonText[0]))
    {
        iButtonID = _ttoi(co.Getlanguage(2).csName);
        csButtonText = ps.GetPSText(iButtonID, (SCOT_LANGUAGE_TYPE) 2);
    }
    rp.SetLanguageName(csButtonText);
  return transition(START_BUTTON, SCOT_LANGUAGE_THIRD);  

//  return STATE_NULL;                // no FTU in this release
}

///////////////////////////////////
// PSButton4 - Fourth language Button pressed
///////////////////////////////////
SMStateBase  *SMAttractBase::PSButton4(void)
{
    CString csButtonText;
    int iButtonID;
    
    csButtonText = co.Getlanguage(3).csName;
    if(_istdigit(csButtonText[0]))
    {
        iButtonID = _ttoi(co.Getlanguage(3).csName);
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
    
    csButtonText = co.Getlanguage(4).csName;
    if(_istdigit(csButtonText[0]))
    {
        iButtonID = _ttoi(co.Getlanguage(4).csName);
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
    
    csButtonText = co.Getlanguage(5).csName;
    if(_istdigit(csButtonText[0]))
    {
        iButtonID = _ttoi(co.Getlanguage(5).csName);
        csButtonText = ps.GetPSText(iButtonID, (SCOT_LANGUAGE_TYPE) 5);
    }
    rp.SetLanguageName(csButtonText);
  return transition(START_BUTTON, SCOT_LANGUAGE_SIXTH);  

}

SMStateBase  *SMAttractBase::PSButton8(void)
{
    m_bLanguageButtonClicked = true;

    if(co.GetNumberOfLanguagesSupported() > 2)
    {
        // Tell security that we are exiting form scan& bag state
        TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("Attract") );

        return setAndCreateAnchorState(BES_LANGUAGESELECT);
    }
    else if(co.GetNumberOfLanguagesSupported() == 2)
    {
    
        if(m_customerLanguage == SCOT_LANGUAGE_PRIMARY)
        {
            m_customerLanguage = SCOT_LANGUAGE_SECONDARY;
        }
        else
        {
            m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
        }
        
        //SSCOADK-5989 + 
      if (co.fStateDualLanguage && co.fStateDisplayWeightsAndMeasures 
          && dm.IsDeviceConfigured(DMCLASS_SCALE))
      {
        // If healthy, then set scale display for weights and measures 
        long retErrorCode = DMConfigureScaleDisplay();
        if( retErrorCode > -1)
        {
	        fStateFreeze = false;
	        CString csWork = ps.GetPSText(retErrorCode);

	        return SMCustomerModeState::createLevel1Message(
                        csWork,  // char *message
				        PS_NOTEXT,         // no canned message
				        PS_NOTEXT,         // no prompt
				        0,                 // no prompt value
				        -1,                // no device
				        false, false);     // no DM check health needed //SR672
        }
      }
        //SSCOADK-5989 -

        SetPreferredLanguage(m_customerLanguage, _T("Button") );

        // cs185088 TAR 437249+
        ps.SetLanguage(m_customerLanguage);
        ps.SetTareListToCurrentLanguage();
        if(gpSCOTAuxDisp)
            gpSCOTAuxDisp->SetLanguage(m_customerLanguage);

        //+SSCOP-963
        CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMAttractBase::PSButton8"));
        //-SSCOP-963
        if(m_LanguageUsedPreviously != m_customerLanguage)
          {
            m_LanguageUsedPreviously=m_customerLanguage;  
          }
        // cs185088 TAR 437249-

        //ChangeLanguage(m_customerLanguage);

        TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("Attract"));   //SSCOADK-980
        return Initialize(); // refresh the screen
        
        //return STATE_NULL;
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

  bool bLookupStarted = false;
  SMStateBase* retState = STATE_NULL;
   
  if (m_csLoyaltyCheckType.IsEmpty())
  {
      bLookupStarted = LoyaltyCheck(_T("Scan"), csDMLastBarLabel);
  }
  if(bLookupStarted)
  {
      return STATE_NULL;
  }

  //begin TAR#377242
  CString csButtonText;
  csButtonText = ps.GetPSText(MSG_LANGUAGENAME);
  rp.SetLanguageName(csButtonText);
  //end TAR#377242
  //begin TAR#121479
  //SASendSCOTInput( ITEMIZATION ); // security needs to know we scanned an item quickly.
  csItemSent = csDMLastBarCode;
  fItemIsWaiting = true;            // to know if the item was sold from the attract screen or not.  sell item after signon in S&B
  SASendSCOTInput( UPC_SCANNED, csDMLastBarLabel );
  //end TAR#121479  LPM092499

  if ( bQuietNeedSecApproval )  //TAR234536
  {
      bQuietNeedSecApproval = false;
      m_bScanOnAttractAtQuietMode = true;
  }

  if (co.fStatePaintScanAndBagOnInitialScan)
  {
     PaintScanAndBagScreen();  // TAR 262131 Provide immediate feedback
  }

  //m_CustomerLanguage may have been set by personalization 
  return transition(SCAN_ITEM, m_customerLanguage);
}

//////////////////////////////////////
// DMCardReader - a card was swiped
//////////////////////////////////////
SMStateBase  *SMAttractBase::DMCardReader(void)
{  
  bool bLookupStarted = false;
  SMStateBase* retState = STATE_NULL;
  
  if (m_csLoyaltyCheckType.IsEmpty())
  {
      bLookupStarted = LoyaltyCheck(_T("Swipe"), GetAccountDataFromMSRSwipe(cDMLastTrack));
  }
  if(bLookupStarted)
  {
      return STATE_NULL;
  }

  // TAR 442211 +
  CString csButtonText;
  csButtonText = ps.GetPSText(MSG_LANGUAGENAME);
  rp.SetLanguageName(csButtonText);
  // TAR 442211 -

  //+TAR 275219
  if (co.fStatePaintScanAndBagOnInitialScan) 
  {
      PaintScanAndBagScreen();  // Provide immediate feedback
  }
  //-TAR 275219
  //m_CustomerLanguage may have been set by personalization
  return transition(SWIPE_CARD, m_customerLanguage);
}
#endif //_CPPUNIT

///////////////////////////////////////////
// DMScale - something was put on the scale
///////////////////////////////////////////
SMStateBase  *SMAttractBase::DMScale(void)
{
  trace(L6, _T("+SMStateBase  *SMAttractBase::DMScale(void)"));
  int ntimeoutvalue = co.getTimeOutScannerScaleOnAttract() ;
  SMStateBase *sExit = STATE(State)::DMScale();
  if (fDMScaleZeroed)
  {
#ifndef _CPPUNIT
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
#endif //_CPPUNIT

      if (lDMScaleWeight > 0)
      {
          fSSWtFromAttract = true;  // TAR123172 only used if scan scale wt increase from attract screen LPM101499
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
  if (g_bMobileAppEnabled && !g_bInRecallMode)
    mobileCart.NotReadyForTransaction();

#ifndef _CPPUNIT
  m_bCancellingCashTender = false; //TAR 323590
  ps.SetNextGenUICustomerLanguage(languageUsed);

  m_bMotionSensorEnabledOnAttract = false;   //RFC 265465
    
  if (event == START_BUTTON)
  {
      if(co.GetfStateDualLanguage() && !co.GetfStateSingleLanguageButtonAtAttract())
      {
          SetPreferredLanguage(languageUsed, _T("Button") );
      }
  }
    
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
  /*if(!fSAInTransaction)
      SASendSCOTInput( ITEMIZATION );*/
    
  // Move to a transitioning state where the application waits for a sign on
  // from terminal services. look for TS display event
  bGoingToInProgress = true;
  if (co.GetfStateDualLanguage())
  {
      m_customerLanguage = languageUsed;
	  //SSCOADK-6266
      // If healthy, then set scale display.
      long retErrorCode = DMConfigureScaleDisplay();
      if( retErrorCode > -1)
      {
        fStateFreeze = false;
	    CString csWork = ps.GetPSText(retErrorCode);
	
	    return SMCustomerModeState::createLevel1Message(
            csWork,  // char *message
		    PS_NOTEXT,         // no canned message
			PS_NOTEXT,         // no prompt
			0,                 // no prompt value
			-1,                // no device
			false, false);     // no DM check health needed //SR672
      }
      //SSCOADK-6266 -
      ps.SetLanguage(languageUsed);
      co.csCMCustomerMessage = ps.GetCustomerMessageDefault(); //SSCOB-1860
      CustomerMessage = co.csCMCustomerMessage;

      ps.SetTareListToCurrentLanguage();
      if(gpSCOTAuxDisp)
      gpSCOTAuxDisp->SetLanguage(languageUsed);
      SMStateBase *retVal = TBStart(co.Getlanguage(languageUsed).szCode);

      if(m_LanguageUsedPreviously != m_customerLanguage)
      {                                                       //is already on the initialize function that controls are assigned with primary language                                                        
        m_LanguageUsedPreviously=m_customerLanguage;        
      }  
      return retVal;
  }
  else
  {
      return TBStart();
  }
#else
  return STATE_NULL;
#endif //_CPPUNIT
}

#ifndef _CPPUNIT

//TAR#121479 moved checkCashAcceptorCassette to Finish base LPM092999

///////////////////////////////////////
SMStateBase  * SMAttractBase::SAEndOfTransaction(void)
{
    bTransactionComplete = false; //TAR 411286
    ResetAllTransactionValues();    //better way to clear

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
    
    // (*) SR898 ====================
    DMTurnOffTriColorLight();
    
    return STATE_NULL; // No need to create Attract while on Attract
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMAttractBase::OnWtDecrease(void)
{
    // TAR 257709 - moved the code that was here to SMStateSABase::AttractWtDecrease()
    // function.
    //m_bSecViolationFromAttract = true; //383748 // SSCOP-2756
    AttractWtDecrease(); 
    return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMAttractBase::OnWtIncreaseNotAllowed() 
{
/*      trace (L6, _T("Increase at Attract while Quiet Mode = Y, ignoring wt for now"));
        CString csInstruction, csDesc;
        csDesc.Format(ps.GetPSText(RA_UNEXPECTEDINCREASEINFO, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits);
        csInstruction.Format(ps.GetPSText(RA_WEIGHTINCREASEINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTINCREASE, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits, _T(""));
        ra.OnNormalItem(csInstruction);
        bQuietNeedSecApproval = true; // need to get approval for the increase when start btn or scan item
//      nTimeOut = co.getTimeOutAttractQuietModeTimer();
        return STATE_NULL;*/
        //if not from Bag&EAS, then the wt is UnExpected
    nTimeOut = SMStateBase::nTimeOut;
    //m_bSecViolationFromAttract = true; //383748 // SSCOP-2756

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
    
    return STATE(State)::PSButtonHelp();
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

#endif // _CPPUNIT
// TAR 262131 - Paint Scan & Bag screen immediately, to give impression of quick response
void SMAttractBase::PaintScanAndBagScreen()
{
#ifndef _CPPUNIT
  CLaneStateData laneStateData(m_pAppPSX);
  laneStateData.SendLaneStateData(_T("ScanAndBag"), _T(""), false, false);
  //laneStateData.SendNextGenUIData(_T("NextGenUIApplicationState"), (CString) laneStateData.INTRANSACTION);

  g_bSnBScreenIsShown = true; //TAR195100 CMal/Chu021402 
  bAlreadyInSnB = true;

  if( co.fOperationsDisplayQuickPickItems )
  {
        if ( co.fCMRewardInfo)
        {
          ps.SetCMFrame(_T("QuickPickItemsWithReward"));
          csCurContext = _T("QuickPickItemsWithReward");
        }
        else
        {
          ps.SetCMFrame(_T("QuickPickItems"));
          csCurContext = _T("QuickPickItems");
        }
  }
  else
  {
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
  }

  // SR880 ++
  if(co.fOperationsCustomerBagAllowedInScanAndBag)
  {
      m_pAppPSX->SetConfigProperty(_T("CMButton1LookUp"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
      m_pAppPSX->SetConfigProperty(_T("CMButton2KeyInCode"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
      m_pAppPSX->SetConfigProperty(_T("CMButton1LookUpIn3ButtonRow"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
      m_pAppPSX->SetConfigProperty(_T("CMButton2KeyInCodeIn3ButtonRow"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
      m_pAppPSX->SetConfigProperty(_T("CMButton6OwnBag"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
  }
  else
  {
      m_pAppPSX->SetConfigProperty(_T("CMButton1LookUp"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
      m_pAppPSX->SetConfigProperty(_T("CMButton2KeyInCode"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
      m_pAppPSX->SetConfigProperty(_T("CMButton1LookUpIn3ButtonRow"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
      m_pAppPSX->SetConfigProperty(_T("CMButton2KeyInCodeIn3ButtonRow"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
      m_pAppPSX->SetConfigProperty(_T("CMButton6OwnBag"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
  }
  // SR880 --
  
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
  CString csButtonText;
  if(co.GetfStateDualLanguage() && 
      (co.fStateAllowLanguageSelectionDuringTransaction ||
      (custInfo.GetOption(_T("PersonalizeLanguage")) == "Y")))
  {
      if(co.GetNumberOfLanguagesSupported() > 2)
      {
          // TAR 437633 +
          //csButtonText = ps.GetPSText(BTT_LANGUAGE, SCOT_LANGUAGE_PRIMARY);
          ps.SetPSText(PS_WORK1, ps.GetPSText(BTT_LANGUAGE, SCOT_LANGUAGE_PRIMARY));
          // TAR 437633 -
      }
      else if(co.GetNumberOfLanguagesSupported() == 2)
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
         csButtonText = co.Getlanguage(i).csName;
         if(_istdigit(*csButtonText))
         {
             iButtonID = _ttoi(co.Getlanguage(i).csName);
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

  if(co.GetfStateDualLanguage())
  {
      if(m_LanguageUsedPreviously != m_customerLanguage)
      {
        m_LanguageUsedPreviously=m_customerLanguage;
      }
  }
#endif //_CPPUNIT
  ps.ShowCMFrame();
}
#ifndef _CPPUNIT
// TAR 262131 - end

VOID CALLBACK SMAttractBase::AttendantModeTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    m_pAttract->HandleAttendantModeTimedOut();
    
}
#endif //_CPPUNIT

void SMAttractBase::HandleAttendantModeTimedOut()
{
#ifndef _CPPUNIT
    trace(L2, _T("+SMAttractBase::AttendantModeTimeOut=%d"), co.getTimeOutAttendantModeTimer());
    KillTimer(hw,ATTENDANT_MODE);
#endif //_CPPUNIT
    if (co.IsAttendantModeOn() && !fInSignedOn && !fInSignedOff && !fSAInTransaction)
    {
        if (co.IsRemoteScannerConnected())
        {
#ifndef _CPPUNIT
            dm.ScannerRemoteAccess(false);
#endif //_CPPUNIT
            co.SetRemoteScannerConnected(false);
        }
#ifndef _CPPUNIT
        rp.TransactionStart(rp.m_csBusinessDate);
#endif //_CPPUNIT
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
            ::SysFreeString(bstrResult); // cs185088 mem
        }
        TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode") );
        TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("Attract") );
        rp.ModeChange(rp.m_lAttendantEntryID, ATTENDANT, EXIT);
        rp.TransactionEnd();
        ps.ShowTrainingModeOnScreen(TrainingModeObj.IsTMOn());
        UpdateStateName(ps.GetPSText(LTT_ATTRACT, SCOT_LANGUAGE_PRIMARY));
#endif //_CPPUNIT
    }
}
#ifndef _CPPUNIT
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
         if(co.GetfStateDualLanguage())
         {
             STATE(State)::DMSayPhrase(SELECTLANGUAGE);
         }
         else
         {
             DMSayPhrase(TOUCHSTART);
         }

     }
    trace(L6, _T("::ScanScaleattract-Audio was played"));

}
#endif //_CPPUNIT

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
    if (nPSLastParm > 0)
        CVolumeControl VolumeControl(SETVOLUMELEVEL, 0, ((nPSLastParm > 100) ? 100 : nPSLastParm));
    else if (nPSLastParm == -1) // if -1
        CVolumeControl VolumeControl(SETVOLUMELEVEL, 0, 0);
    else
        CVolumeControl VolumeControl(SETVOLUME);

#ifndef _CPPUNIT
    custInfo.SetDataString(("VolumeChangeMethod"),_T("Button"));
    //+TAR#343251 
    if ( co.GetVolumeControlTimer() > 0)
    {
        //set the timeout
        nRC = SetTimer(hw, VOLUME_CONTROL, co.GetVolumeControlTimer()*1000, &VolumeControlTimedOut);    
        trace(L6, _T("-SMAttractBase::PSButton7 - Volume control timer started nRC= %d"), nRC);

    }
    //-TAR#343251
#endif //_CPPUNIT
    return STATE_NULL;
}

#ifndef _CPPUNIT
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
    trace(L7, _T("+SMAttractBase::SingleLanguageButtonAtAttractTimedOut"));
    m_pAttract->HandleBackToPrimaryLanguageTimedOut();
    trace(L7, _T("-SMAttractBase::SingleLanguageButtonAtAttractTimedOut"));
}

void SMAttractBase::HandleBackToPrimaryLanguageTimedOut()
{
    KillTimer(hw, LANGUAGE_CONTROL);
    if ( m_customerLanguage != SCOT_LANGUAGE_PRIMARY)
    {
        CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMAttractBase::HandleBackToPrimaryLanguageTimedOut"));//+SSCOP-963
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
          
          // (*) SR898 ====================
          DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
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

//429111
SMStateBase *SMAttractBase::RAParse(MessageElement *me)
{
    trace(L5,_T("SMAttractBase::RAParse"));
    trace(L4,_T("SMAttractBase::RAParse: me->raInfo.msg = %d on thread 0x%08.8x"), me->raInfo.msg, GetCurrentThreadId());
     
    if (me->raInfo.msg == WMRA_SEND_INTERVENTION_APPROVED)
    {
        trace(L6, _T("SMAttractBase::RAParse, +rp.TransactionEnd"));
        rp.TransactionEnd();
        trace(L6, _T("SMAttractBase::RAParse, -rp.TransactionEnd"));
    }
    
    return STATE(State)::RAParse(me);   


}

bool SMAttractBase::LoyaltyCheck(CString csLookupType, CString csLookupData)
{
    bool bLookupStarted = false;
    trace(L6, _T("+SMAttractBase::LoyaltyCheck"));

    long lLoyaltyCheckTimer = custInfo.VerifyAndGetLookupTimer(csLookupType, csLookupData);
    if (lLoyaltyCheckTimer > 0)
    {
        // +SSCOP-3809
        UINT_PTR  uiRet;
        uiRet = SetTimer(hw, LOYALTY_LOOKUP, lLoyaltyCheckTimer, &LoyaltyCheckTimedOut);
        trace(L6, _T("  SetTimer returned %d"),uiRet);
        if(uiRet==NULL)
        {
            trace(L6, _T("  GetLastError = %d"), GetLastError());
        }// -SSCOP-3809

        m_csLoyaltyCheckType = csLookupType;
        bLookupStarted = true;
    }
    trace(L6, _T("-SMAttractBase::LoyaltyCheck, returned %d"),bLookupStarted);
    return bLookupStarted;
}

VOID CALLBACK SMAttractBase::LoyaltyCheckTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    trace(L6, _T("+SMAttractBase::LoyaltyCheckTimedOut"));
    m_pAttract->HandleLoyaltyCheckTimedOut();
    trace(L6, _T("-SMAttractBase::HandleLoyaltyCheckTimedOut"));
}

void SMAttractBase::HandleLoyaltyCheckTimedOut()
{
    mo.PostCU(LOOKUP_TIMEOUT, _T("")); 
}

SMStateBase *SMAttractBase::OnLookupComplete (long byteCount)
{
    trace(L6, _T("+SMAttractBase::OnLookupComplete byteCount = %d"), byteCount);
    KillTimer(hw, LOYALTY_LOOKUP);
    SCOT_LANGUAGE_TYPE scotLangTemp = m_customerLanguage;
    SMStateBase* retState = STATE_NULL;
    if (byteCount >= 0) //Lookup completed
    {
        //+ Personalization age prompt
        if(custInfo.GetOption(_T("PersonalizedAgePrompt")).CompareNoCase(_T("y"))==0)
        {
            COleVariant v = custInfo.GetPreference(_T("Birthdate"));
            CString csDOB = v.bstrVal;
            trace(L6, _T("GetPreference-Birthdate = %s"), csDOB);
            if (csDOB.GetLength() > 0)
            {
                ps.GetPSXObject()->SetTransactionVariable( _T("AgePrompt"), v );
            }
        }
        //- Personalization age prompt
        //+SR815.5 Personalized Volume
        if ( co.IsShowVolumeControl() )
        {
            CVolumeControl *pVolumeControl = new CVolumeControl();
            try
            {
                int nCurrentVolume = pVolumeControl->GetVolumeLevel();
                int nDefaultVolume = pVolumeControl->GetDefaultVolume();
                CString csVolume = custInfo.GetPreference(_T("VolumeLevel")); // SSCOP-2657
                if ( !IsPersonalizedVolumeChange()) // SSCOP-2657
                {
                    if (!csVolume.IsEmpty()) // SSCOP-2657
                    {
                        int nPersonalizedVolume = _ttoi( csVolume ); // SSCOP-2657
                        if ( nCurrentVolume != nPersonalizedVolume ) // SSCOP-2657
                        {
                            CVolumeControl VolumeControl(SETVOLUMELEVEL, 0, nPersonalizedVolume);
                            trace(L6,_T("SMAttractBase::OnLookupComplete - Personalized volume level is set successfully. Personalized Volume Level: %d"), nPersonalizedVolume);
                        }
                        else
                        {
                            trace(L6,_T("SMAttractBase::OnLookupComplete - Personalized Volume level is the same as the current level. Current Volume Level: %d, Personalized Volume Level: %d"), nCurrentVolume, nPersonalizedVolume);
                        }
                    }
                    else
                    {
                        trace(L6,_T("SMAttractBase::OnLookupComplete - Personalized Volume was not set, using the current volume level. Current Volume Level: %d, Default Volume Level: %d"), nCurrentVolume, nDefaultVolume);
                    }
                }
                else
                {
                    trace(L6,_T("SMAttractBase::OnLookupComplete - Personalized Volume level not used, customer has already selected a volume level. Current Volume Level: %d, Default Volume Level: %d"), nCurrentVolume, nDefaultVolume);
                }
            }
            catch( CException *e )
            {
                _TCHAR szError[255] = {NULL};
                e->GetErrorMessage(szError, 255);
                trace(L6, _T("SMAttractBase::OnLookupComplete - Got an exception! Error text: %s"), szError);
                e->Delete();
            }

            delete pVolumeControl;
            pVolumeControl = NULL;
        }
        else
        {
            trace(L6,_T("SMAttractBase::OnLookupComplete - Store volume setting overrides personalize volume setting."));
        }
        //-SR815.5 Personalized Volume
        if (scotLangTemp != GetPreferredLanguage() && custInfo.GetOption(_T("PersonalizeLanguage")) == "Y") //SSCOP-2495

        {
            m_customerLanguage = GetPreferredLanguage(); //SSCOP-2495
            ps.SetLanguage(m_customerLanguage);
        }

        //+SR814.5 SR815.6 SR851 Personalization and Security
        SetPersonalTheme(_T("SMAttractBase::OnLookupComplete"));
        /*
        CString csTheme = custInfo.GetPreference(_T("Theme"));
        if (csTheme.GetLength())
        {
            CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMAttractBase::OnLookupComplete"));
            m_pAppPSX->SetTheme(csTheme);
            CString csTestTheme;
            m_pAppPSX->GetTheme(csTestTheme);
            if (csTestTheme != csTheme)
            {
                m_pAppPSX->SetTheme(custInfo.GetOption(_T("LaneTheme")));
            }
            
        }
        else
        {
            m_pAppPSX->SetTheme(custInfo.GetOption(_T("LaneTheme")));
        }
        */
        //-SR814.5 SR815.6 SR851 Personalization and Security
    }
    
    if (m_csLoyaltyCheckType == _T("Swipe"))
    {
        retState = DMCardReader();
    }
    else if (m_csLoyaltyCheckType == _T("Scan"))
    {
        retState = DMScanner();
    } 
    else
    {
        bAlreadyInSnB = false;
        retState = transition(START_BUTTON, m_customerLanguage);
    }
    m_csLoyaltyCheckType = _T("");
    trace(L6, _T("-SMAttractBase::OnLookupComplete"));
    return retState;
}
#endif // _CPPUNIT
// (+) SR898 ====================
void SMAttractBase::DMTurnOffTriColorLight()
{
    // turn off tricolor light before setting it to other states
    /*#ifdef _CPPUNIT
        FakeSMStateBase::DMTurnOffTriColorLight( );
    #else
        SMStateBase::DMTurnOffTriColorLight( );
    #endif*/
    STATE(State)::DMTurnOffTriColorLight( );

    if(co.GetfOperationsInvertLightStates() && BES_START == getAnchorState())
    {
        trace(L7,_T("Tri-Color Light Override is ENABLED"));
        trace(L7,_T("Turning On the Green Tri-Color Light"));

        dm.setTriColorLight( DM_TRICOLORLIGHT_GREEN, DM_TRICOLORLIGHT_ON, DM_TRICOLORLIGHT_NORMALREQUEST );
    }   
    else
    {
        trace(L7,_T("Tri-Color Light State is in Normal Mode..., SR898 Override is DISABLED"));
    }
}
// (-) SR898 ====================
#ifndef _CPPUNIT
// + SSCOP-1158 =================
void SMAttractBase::ReloadThemes()
{
    // If ms_bThemeReload flag is true then do reload of theme list
    if(ms_bThemeReload)
    {
        // Reload is done by first unloading theme list then loading it
        trace(L6,_T("SMAttractBase::ReloadThemes Reloading Themes"));
        m_pAppPSX->UnloadThemes();
        m_pAppPSX->LoadThemes();
        ms_bThemeReload = false;
    }
    return;
}

#endif // _CPPUNIT

//+SSCOP-1236
SMStateBase *SMAttractBase::PSCmdButton(CString csContext, CString csControl, CString csCommand)
{
    trace(L6,_T("+SMAttractBase::PSCmdButton(%s, %s, %s)"), csContext, csControl, csCommand);

    if ( csCommand.IsEmpty() )
    {
        DMSayPhrase(KEYPADINVALID);
    }
    else
    {
        ps.SetLanguage(m_customerLanguage);

        trace(L6,_T("PSCmdButton sending \"%s\" to TB"), csCommand);
        SendTBCommand(TBCMD_CMDSTRING, csCommand);
    }
    trace(L6,_T("-SMAttractBase::PSCmdButton()"));
    return STATE_NULL;                // discard any input
}
//-SSCOP-1236


#ifndef _CPPUNIT

// Add Glory CM Support
void SMAttractBase::CheckCashDiscrepancyStatus(void)
{
    trace(L7, _T("+SMAttractBase::CheckCashDiscrepancyStatus"));

    if(dm.m_csCashRecyclerModel.IsEmpty())
    {
       return;  // Nothing to do.
    }

    CString csRAPInfo, csSecInfo;
    if(co.ShowRapInfoWhenAbnormalBalance())
    {
        if(GetCashDiscrepancyStatus())
        {
            trace(L6, _T("Cash unbalance detected in SMAttract."));
            csRAPInfo = ps.GetPSText(MSG_CASH_ABNORMAL_BALANCE, SCOT_LANGUAGE_PRIMARY);
        }
    }

    if(!csRAPInfo.IsEmpty())
    {
        csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),
            csRAPInfo, _T(""),_T(""));

        ra.OnNormalItem(csSecInfo);     // Display the info to RAP
    }

    trace(L7, _T("-SMAttractBase::CheckCashDiscrepancyStatus"));
}
#endif // _CPPUNIT

SMStateBase* SMAttractBase::ReloadOptions(void)
{
    mobileCart.NotReadyForTransaction();
    SMStateBase* pStartState = SMCUSTOMERMODESTATE::ReloadOptions();

    if ( pStartState == STATE_NULL )
    {
        RETURNSTATE(Attract)
    }
    else
    {
        return pStartState;
    }
}

SMStateBase *SMAttractBase::OnMobileTransactionStart(CString csTransactionInfo)
{
    SMStateBase *retState = STATE_NULL;

    bool bIsQBItemList = (csTransactionInfo.Find(QBFMT_001) == 0);
    if ( bIsQBItemList )
    {
        bool bAlreadyInRecall = g_bInRecallMode;
        if (!g_bInRecallMode && bIsQBItemList && recallList.LoadQBBarcodes(csTransactionInfo))
        {
            mp.LogTransition((CString)_T("SMStartMobileReplay"), nTBLastRealState); 
            g_bInRecallMode = true;
            g_bEnteringRecallMode = true;
            PaintScanAndBagScreen();
            ps.SetCMFrame(_T("Processing"));
            ps.SetCMLeadthruText(TTL_LOADING_TRANSACTION);
            ps.Message12(MSG_LOADING_TRANSACTION, true);
            ps.ShowCMTBText(csTBMessage2Scot);  
            ps.CMButton(_T("ButtonHelp"),BTT_HELP,false);
            ps.ShowFrame();
        }
        if ( !bAlreadyInRecall && g_bInRecallMode && recallList.ParseNextItem(false))
        {
            
            bool bLookupStarted = false;
            CString csItem = recallList.GetScanDataLabel();
            if (m_csLoyaltyCheckType.IsEmpty())
                bLookupStarted = LoyaltyCheck(_T("Scan"), csItem);
            if(bLookupStarted)
                return STATE_NULL;
        }
        
        CString csButtonText;
        csButtonText = ps.GetPSText(MSG_LANGUAGENAME);
        rp.SetLanguageName(csButtonText);
        if ( bQuietNeedSecApproval )  //TAR234536
        {
            bQuietNeedSecApproval = false;
            m_bScanOnAttractAtQuietMode = true;
        }
        return transition(SCAN_ITEM, m_customerLanguage);
    }
    else if (csTransactionInfo.Find(MOBILE_RESCAN_REQUIRED) == 0)
    {
        retState = GenerateRAPDataNeeded(_T(" "), _T(""), co.GetcsDataEntryINIFile(), _T("MobileTransactionAudit"));
    }
    else
    {
        trace(L6,_T("Ignoring Message that is not an Item list"));
        return STATE_NULL;  //Ignore messages that do not match format;
    }
    return retState;
}

SMStateBase *SMAttractBase::CUParse(MessageElement *me)
{/*
    int nEvent = me->cuInfo.Msg;
    CString csStateName;
    SMStateBase *retState = STATE_NULL;

    trace(L6,_T("+SMAttractBase::CUParse"));
    switch(nEvent)
    {
    case MOBILECART_SALESTARTED:
        {
            LPTSTR szParms = NULL;
            if( me->cuInfo.szParms )
            {
                szParms = new TCHAR [_tcslen(me->cuInfo.szParms) + sizeof(TCHAR)];
                _tcscpy( szParms, me->cuInfo.szParms );
                delete [] me->cuInfo.szParms;
            }
             
            if (szParms != NULL)
            {
                retState = OnMobileTransactionStart(szParms); 
                delete [] szParms;
            }
            trace(L6,_T("-SMAttractBase::CUParse"));
            break;
        }
    case MOBILECART_RESCANREQUIRED:
        {
            retState = GenerateRAPDataNeeded(ps.GetPSText(TTL_CUSTOMER_AUDIT, SCOT_LANGUAGE_PRIMARY),
                _T(""), co.GetcsDataEntryINIFile(), _T("MobileTransactionAudit"));
            break;
        }       
    default:
        retState = STATE(State)::CUParse(me);
    }

    
    trace(L6,_T("-SMAttractBase::CUParse"));
  return retState;*/
    return STATE(State)::CUParse(me);
}