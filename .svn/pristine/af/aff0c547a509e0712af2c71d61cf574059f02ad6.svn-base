//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOutOfServiceBase.CPP
//
// TITLE: Class implementation for out of service state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"                 // MS MFC always needed first
//               // MS MFC template header
#include "LaneStateData.h"
#ifndef _CPPUNIT
#include <mmsystem.h>
#include "resource.h"
#include "Common.h"                 // Common includes

#include "SMOutOfService2.h"
#include "SMWaitForApproval.h"
#include "SMSmAuthorization.h"
//USSF Start
#include "USSFProcedures.h"
#include "SMUSSFManager.h"
#include "USSFCommon.h"
//USSF End
#include "SMStateBase.h"
#include "GloryOpos.h"
#include "DMProcedures.h"           // DM procedures
#include "ncrpid.h"
#include "DeviceErrorLookUpAPI.h"  //SR672
DLLEXPORT extern DMProcedures dm;   // Global DM object
#define _BILLRECYCLER _T("BILLRECYCLER")
#define _COINRECYCLER _T("COINRECYCLER")
#else  // _CPPUNIT
    #include "FakeCommon.h"
#endif // _CPPUNIT
#include "SMOutOfService.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("OutOfServiceBase")
#ifndef _CPPUNIT
#include "MethodTrace.h"
#endif // _CPPUNIT

DLLEXPORT extern HWND hw;

DEFINE_TIMESTAMP

//IMPLEMENT_STATE(OutOfService)

IMPLEMENT_DYNCREATE(SMOutOfServiceBase, CObject)// MFC Runtime class/object information

#ifndef _CPPUNIT
//////////////////////////////////////////////////////////////////////////////////////////////////
SMOutOfServiceBase::SMOutOfServiceBase(const int nWhy)
: nReason(nWhy), m_bDoLegacyDeviceErrs(false)
{
  IMPLEMENT_TIMESTAMP
		
  nReason = nWhy;
	
  // this state stack initialization code was moved from the base state
  // constructor to prevent that stack from re-initializing upon enter to
  // each state. I believe this is the proper placement for it.
  aStateFifo = 0;	// init the 'pointers'
  uStateFifo = 0;
  for (int i=0;i<MAX_FIFO_STACK;i++)
  {
    sStateFifo[i] = STATE_NULL;
  }
  
  //+ TAR 348623
  if(g_pDelay_TBExpectedStateChange != NULL)	
  {
	  try{
		  delete g_pDelay_TBExpectedStateChange;
	  }
	  catch(...){
		trace(L0,_T("SMOutOfServiceBase::SMOutOfServiceBase - Error deleting the state saved in g_pDelay_TBExpectedStateChange"));
	  }
	g_pDelay_TBExpectedStateChange = NULL;
  }
  if (g_pDelay_WaitForApprovalStateChange != NULL)
  {
	  try{
		delete g_pDelay_WaitForApprovalStateChange;
	  } 
	  catch(...){
	  	trace(L0,_T("SMOutOfServiceBase::SMOutOfServiceBase - Error deleting the state saved in g_pDelay_WaitForApprovalStateChange"));
	  }
	g_pDelay_WaitForApprovalStateChange = NULL;	
  }
  //- TAR 348623
}
#endif // _CPPUNIT

////////////////////////////////////////
SMOutOfServiceBase::SMOutOfServiceBase() : m_bDoLegacyDeviceErrs(false)
{
  IMPLEMENT_TIMESTAMP
  nReason = 1;        // TAR 142611

}

#ifndef _CPPUNIT
////////////////////////////////////////
SMStateBase  *SMOutOfServiceBase::Deliver(const int nWhy)
{
  nReason = nWhy;
  // this state stack initialization code was moved from the base state
  // constructor to prevent that stack from re-initializing upon enter to
  // each state. I believe this is the proper placement for it.
  aStateFifo = 0;	// init the 'pointers'
  uStateFifo = 0;
  for (int i=0;i<MAX_FIFO_STACK;i++)
  {
    sStateFifo[i] = STATE_NULL;
  }

  //+ TAR 348623
  if(g_pDelay_TBExpectedStateChange != NULL)	
  {
	  try{
		  delete g_pDelay_TBExpectedStateChange;
	  }
	  catch(...){
		trace(L0,_T("SMOutOfServiceBase::SMOutOfServiceBase - Error deleting the state saved in g_pDelay_TBExpectedStateChange"));
	  }
	g_pDelay_TBExpectedStateChange = NULL;
  }
  if (g_pDelay_WaitForApprovalStateChange != NULL)
  {
	  try{
		delete g_pDelay_WaitForApprovalStateChange;
	  } 
	  catch(...){
	  	trace(L0,_T("SMOutOfServiceBase::SMOutOfServiceBase - Error deleting the state saved in g_pDelay_WaitForApprovalStateChange"));
	  }
	g_pDelay_WaitForApprovalStateChange = NULL;	
  }
  //- TAR 348623

  return this;
}
//////////////////////////////////////////
bool SMOutOfServiceBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be one parameter, a dword
    if (dlParameterList.GetDescriptorCount() == 1)
    {
        trace(L6, _T("+SMOutOfServiceBase::DeliverDescriptorList() - Parameter Count of 1"));
        Deliver(DLParamDWORD(dlParameterList));    // nWhy
        return true;
    }
    else
    {
        trace(L2, _T("+SMOutOfServiceBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMOutOfServiceBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("OutOfService") );	// RFC 330050
  nTimeOut = 0;					    // no timeout

  if (gpSCOTAuxDisp)
    gpSCOTAuxDisp->SetLanguage(SCOT_LANGUAGE_PRIMARY);

  ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);

  ps.SetCMFrame(_T("Startup"));

  CString csWork = _T("");	// reset the message to blank in case a device failed
  ps.Message12(csWork, true);
  ps.ShowCMFrame();
  // reasons for being out of service
  //
  // 1 - load options, at IPL
  // 2 - load options, after IPL
  // 3 - state error, shut down
  // 4 - state stop, stop
  // 5 - state restart, execl() new and stop
	
  switch (nReason)
  {
  case 1:
    // There is currently no such file
    // DMSayPhrase("SCOT Logon Sound");
    setAnchorState(BES_OUTOFSERVICE);
	rp.ModeChange(0, _T("Status"), _T("START"));
    return InitializeAll();

  case 2:
	  {
		  fDoOptions = true;
		  UnInitConfigData();
		  rp.ModeChange(0, _T("Status"), _T("RELOAD OPTIONS"));
		  SMStateBase* pStartState = InitConfigData();
		  fDoOptions = false;
		  if (pStartState)
		  {
			return pStartState;
		  }
		  return STATE_RESTART;	
		  // return InitializeAll();
	  }
	  break;
  case 3:
	  {
		  rp.ModeChange(0, _T("Status"), _T("STATE ERROR"));
		  return UnInitializeAll();
	  }
	  break;
  case 4:
	  {
		  rp.ModeChange(0, _T("Status"), _T("STOP"));
		  return UnInitializeAll();
	  }
	  break;
  case 5:
	  {
		  rp.ModeChange(0, _T("Status"), _T("RESTART"));
		  return UnInitializeAll();
	  }
	  break;
  }

  return createAnchorState();       // no reason to be in this state
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void SMOutOfServiceBase::TBShow2x20(void)
{
  // clean up 2x20 display line
  // put blank between line 1 and 2
  // convert double blanks to single blanks
  //  ps.Prompt(CleanUpMessage(szLastDisplay,true));
}

// TAR 287143 start
//////////////////////////////////////////////////////////////////////////////////////////////////
// caller should provide valid SCOT terminal number
// Security Configuration file is downloaded here, 
// return true on successfull initialization
//////////////////////////////////////////////////////////////////////////////////////////////////
#endif // _CPPUNIT
void SMOutOfServiceBase::SCDownload(int nTerminalNumber)
{
    STATE(State)::SCDownload(nTerminalNumber);
}
// TAR 287143 end

//////////////////////////////////////////////////////////////////////////////////////////////////
// caller should provide valid SCOT terminal number
// Config object is intialized here, 
// bLocal = true   : options files are taken from local disk
// bLocal = false  : options files are taken from host application by calling CopyFileFromServer()
// bLocal defaults to false
// return true on successfull initialization
//////////////////////////////////////////////////////////////////////////////////////////////////
bool SMOutOfServiceBase::COInitialize(int nTerminalNumber, bool bLocal)
{
    bool bRetVal = STATE(State)::COInitialize(nTerminalNumber, bLocal);
    // TAR 445003 - ensure option set immediately for RAP use
    ps.GetPSXObject()->SetTransactionVariable(_T("SecurityDisableAllowed"),
        co.GetfStateSecurityDisableAllowed() ? VARIANT_TRUE : VARIANT_FALSE);

    
    return bRetVal;
    
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// caller should provide valid SCOT terminal number
// Tender option is intialized here, 
// bLocal = true   : tender files are taken from local disk
// bLocal = false  : tender files are taken from host application by calling CopyFileFromServer()
// bLocal defaults to false
// return true on successfull initialization
//////////////////////////////////////////////////////////////////////////////////////////////////
bool SMOutOfServiceBase::SCOTTENDInitialize(int nTerminalNumber, bool bLocal)
{
    return STATE(State)::SCOTTENDInitialize(nTerminalNumber, bLocal);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMOutOfServiceBase::InitializeAll(void)
{
  fStateFreeze = true;
  //ps.Prompt(PS_BLANK);

  CLaneStateData laneStateData(ps.GetPSXObject());
  laneStateData.ResetNextGenDataTrxnVariables();
  laneStateData.SendNextGenUIData(_T("NextGenData"),CString(_T("Loading")));
  laneStateData.SendNextGenUIData(_T("NextGenUIShowSigCapture"), (bool)false);


  ps.SetCMLeadthruText(LTT_OUTOFSERVICE);
  int DeviceClass = -1;					// send to system message screen for new EAS devices
  bool bCkHealth = false;
  ps.Echo(TB_LOADING);
  DMSayPhrase(KEYPADVALID);

  // TAR 452531:  Cache result of registry read.
  m_bDoLegacyDeviceErrs = dm.IsLegacyStartupErrorHandling();


  if (TBInitialize(hw, co.csConfigFilePath)<0) 
  {
    fStateFreeze = false;
    ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_TB_INIT);

    return STATE_RESTART;
  }

  TBLoadOptions();
    //TBLoadApplMessages();							  // load EAMSDESC messages

  ps.Echo(TB_LOADED);

	
  // first do any file maintance add/replace/delete, copyto, copyfrom, restart, reboot
  SMStateBase  *sTemp = ProcessMaintenance();
  if (sTemp!=STATE_NULL)
  {
    fStateFreeze = false;
    return sTemp;
  }
	
  // load configuration options files 000 and 'xxx'
  ps.Echo(CO_LOADING);              // again
  DMSayPhrase(KEYPADVALID);

  int nTermNumber = TBGetTerminalNumber();

    // Set terminal number for report logging object.
  CString csTerminalNumber;
  csTerminalNumber.Format(_T("%d"), nTermNumber);
  trace(L7, _T("+rp.SetTerminalNumber"));
  rp.SetTerminalNumber(csTerminalNumber);
  trace(L7, _T("-rp.SetTerminalNumber<%s>"), csTerminalNumber);

  SCDownload( nTermNumber);  // TAR 287143
  if (!COInitialize(nTermNumber))
  {
    fStateFreeze = false;
    ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_CO_INIT);

    return STATE_RESTART;
  }

  loadOptionsFromConfigObject();

  ps.Echo(CO_LOADED);

  PipeConnect();

  co.InitPickListAssistOptions();

  to.SetTimeToFlush( co.GetmsTraceToFlush() );	//THD Performance RFC
  // RFC 219283 DE/RF Touchscreen Motion sensor enables 7883 scanner motor
  MotionSensorEnd();
  MotionSensorStart();

  if (CheckForDATFile(false) == STATE_ENDED) //TAR 173990 awk.7.01 Checking for existance of .DAT files on system.
  {
		fStateFreeze = false;
	    fStateAssist = false;
		return STATE_STOP;	  
		
  }
  // now finish loading PS allowing copy of new text files and load 50-keyboard
  ps.Echo(PS_LOADING);
  DMSayPhrase(KEYPADVALID);

  // load tender files 000 and 'xxx'
  if (!SCOTTENDInitialize(nTermNumber))
  {
    fStateFreeze = false;
    ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_CO_INIT);

    return STATE_RESTART;
  }
  ps.Echo(CO_LOADED);

  // now finish loading PS allowing copy of new text files and load 50-keyboard
  ps.Echo(PS_LOADING);
  DMSayPhrase(KEYPADVALID);

  // repeat init of PS interface, this time we'll pass file names to copy from 4690
  if (PSInitialize()<0)             // errors handled in called methods
  {
    fStateFreeze = false;
    ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_PS_INIT);

    return STATE_STOP; //tar 286744 (Need a way to abort application for severe errors)
  }

  //USSF Start
  if (co.GetfOperationsEnableUSSF())

  {
		InitializeUSSF();
		// TAR #429050 START
		USSFdisp.StoreSymbols();
		// TAR #429050 STOP
  }
  //USSF End  

  ps.Echo(PS_LOADED);
  TBLoadAssistModeKeyboard(); // note that this function is an empty one, should go away!

  if (custInfo.Initialize() < 0)
  {
      ScotError(RETURN, SM_CAT_STARTUP_INIT, CUSTINFO_INIT);
  }
  
  // Remote Approval initialization
  ps.Echo(RA_LOADING);
  DMSayPhrase(KEYPADVALID);
   if (RAInitialize()<0)             // errors handled in called methods
  {
    fStateFreeze = false;
    ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_RA_INIT);

    return STATE_RESTART;
  }
  ps.Echo(RA_LOADED);

  ShrinkPicklistImages();

  //
  // Load all "straight" OPOS devices before
  // DM so we will know which DM devices NOT to load

  long rc;
  bool bDevErrors = false;

   //SR672
  bool bNewFrameWork = DeviceErrorInitialize();
  if (!bNewFrameWork)
  {
	   return createLevel1Message(_T("Failed to initialize framework for enhanced external device error handling"),          // char *message
                               PS_NOTEXT,         // no canned message
                               PS_NOTEXT,         // no prompt
                               0,                 // no prompt value
							   -1,		  // -1 = no device
                               false,        // whether DM check health needed
							   false);		//display HTML
  }

  ps.Echo(DF_LOADING);
  DMSayPhrase(KEYPADVALID);
  if (gSCOTDevFactory.Initialize() < 0)
  {
    fStateFreeze = false;
    // Next line needs to be changed to an DevFactory INIT
    ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_DF_INIT);

    bDevErrors = true;
  }
  else
  {
    ps.Echo(DF_LOADED);
    ps.Echo(SE_LOADING);
    DMSayPhrase(KEYPADVALID);
    if (gSCOTDevFactory.InitDevice(&gpSCOTEncryptor) < 0)
    {
      fStateFreeze = false;
      ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_SE_INIT);

      bDevErrors = true;
    }
    ps.Echo(SE_LOADED);
    ps.Echo(SC_LOADING);
    DMSayPhrase(KEYPADVALID);
    if (gSCOTDevFactory.InitDevice(&gpSCOTSigCap) < 0)
    {
      fStateFreeze = false;
      ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_SC_INIT);

      bDevErrors = true;
    }
    ps.Echo(SC_LOADED);
    ps.Echo(AD_LOADING);
    DMSayPhrase(KEYPADVALID);
    if (gSCOTDevFactory.InitDevice(&gpSCOTAuxDisp) < 0)
    {
      fStateFreeze = false;
      ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_AD_INIT);

      bDevErrors = true;
    }
    ps.Echo(AD_LOADED);

    //ps.Echo(TAB_LOADING); //SSCOP-2683
    DMSayPhrase(KEYPADVALID);
  }

	CString sModel, sSubModel, csWork2;
	sModel.Empty();
	sSubModel.Empty();
	int lRetCode = EVENTID_ANY;
  if (bDevErrors)
  {
    // if we got an error, first make sure DevFactory is shutdown before we retry
    gSCOTDevFactory.UnInitialize();

	//TAR 236390 Merge TAR 211800 BZ 11/25/02: Still need to send an event to switch app even loading

	//device was failed, otherwise, switch app  won't work.

	CEvent evStart(TRUE, TRUE,_T("NCRSCOTLoading"));

	evStart.SetEvent();

	//TAR 236390 

    gpSCOTTakeawayBelt = NULL; //tar 225991
    gpSCOTAuxDisp = NULL;
    gpSCOTSigCap = NULL;
    gpSCOTEncryptor = NULL;

    CString csWork = ps.GetPSText(MSG_DEVICESERROR);
    csWork += gSCOTDevFactory.GetDevicesInError();
    
	//SR672 - Enhanced device error display
	if(csWork.Find(_T("NCR5992SigCap")) !=-1  || csWork.Find(_T("NCR5945SigCap")) !=-1)  // Pinpad error 3.1 DeviceError
	{
		sModel = dm.GetDeviceModelName(DMCLASS_SIGCAP, 0);
		lRetCode = ERROR_SIGCAP_NO_COMM;
		csWork2 = DMGetDeviceErrorHTML(DMCLASS_SIGCAP, sModel, sSubModel, lRetCode, EXTENDED_RESULT_ANY, 0, NULL, &bCkHealth, 0, co.GetcsPrimaryLanguageCode() );
        DeviceClass = DMCLASS_SIGCAP;
	}
	else if ( csWork.Find(_T("NCR5992Pinpad")) !=-1 || csWork.Find(_T("NCR5945Pinpad")) !=-1)
	{
		lRetCode = ERROR_SIGCAP_NO_COMM;
		sModel = dm.GetDeviceModelName(DMCLASS_ENCRYPTOR, 0);
		csWork2 = DMGetDeviceErrorHTML(DMCLASS_ENCRYPTOR, sModel, sSubModel, lRetCode, EXTENDED_RESULT_ANY, 0, NULL, &bCkHealth, 0, co.GetcsPrimaryLanguageCode() );
        DeviceClass = DMCLASS_ENCRYPTOR;
	}
	else 
	{
		sModel = dm.GetDeviceModelName(DMCLASS_FORMS, 0);
		csWork2 = DMGetDeviceErrorHTML(DMCLASS_ENCRYPTOR, sModel, sSubModel, EVENTID_ANY, EXTENDED_RESULT_ANY, 0, NULL, &bCkHealth, 0, co.GetcsPrimaryLanguageCode() );
        DeviceClass = DMCLASS_FORMS;
	}
	if (!csWork2.IsEmpty() && !csWork.IsEmpty() && DeviceClass == DMCLASS_FORMS)
	{
	    TCHAR sHTML2[_MAX_PATH];
		_tcscpy(sHTML2, csWork2.GetBuffer(_MAX_PATH));
		ParseDeviceErrorHTML(sHTML2, csWork.GetBuffer(_MAX_PATH), _T("%s"));
		csWork2.ReleaseBuffer();
		csWork2 = sHTML2;
		csWork.ReleaseBuffer();
	}
	  
	  return createLevel1Message(csWork2,          // char *message
                               PS_NOTEXT,         // no canned message
                               PS_NOTEXT,         // no prompt
                               0,                 // no prompt value
							   DeviceClass,		  // -1 = no device
                               false,        // whether DM check health needed
							   true);		//display HTML //SR672
  }

  ASSERT(gpSCOTAuxDisp			!= NULL);
  ASSERT(gpSCOTEncryptor		!= NULL);
  ASSERT(gpSCOTSigCap			!= NULL);



	//+SR93 Biometrics
	if (co.GetbOperationsUseFingerPrintDevice())
	  {
		BOOL bFTOpen = FALSE;
		if (!g_bBiometricOpened)
		{
			bFTOpen = FT_Open(getRegisterAction);
			g_bBiometricOpened = true;
			if (bFTOpen == FALSE)
			{
				g_bBiometricACLServer = false;
				g_bBiometricACLDevice = false;
				to.Trace(L6, _T("Failed to open fingerprint device"));
			}
		}
		  
	  }
	//-SR93 Biometrics
	
	//+SR Personalization Digital Receipt    
    try
    {
		if ( !dr.Initialize() )
        {
            to.Trace(L6, _T("Failed to initialize NCRSSCODigitalReceiptGen.ocx ActiveX Control in SCOTApp."));
        }
        else
        {
            to.Trace(L6, _T("Successfully initialized NCRSSCODigitalReceiptGen.ocx ActiveX Control in SCOTApp."));
        }
    }
    catch(CException* e)
	{
        _TCHAR szError[255] = {NULL};
		e->GetErrorMessage(szError, 255);
		to.Trace(L6, _T("Got an exception in creating the ActiveX control. Error text:%s"), szError);
		e->Delete();
	}
	//-SR Personalization Digital Receipt
	
  // device manager initialization
  ps.Echo(DM_LOADING);
  DMSayPhrase(KEYPADVALID);
  m_nDevInitStatus = rc = DMInitialize();

	//+SR93 Biometrics support
	//display biometric device error if it encounter as such..
	if(co.GetbOperationsUseFingerPrintDevice())
	{						
		//Tar 397076 - do not display systemmessage at startup for biometric failure
		if( true == g_bBiometricACLDevice && true == g_bBiometricACLServer )
				to.Trace(L6, _T("Sucessfully opened fingerprint device"));
		else if (!g_bBiometricACLDevice )
				to.Trace(L6, _T("Fingerprint device is disconnected"));
		else if (!g_bBiometricACLServer )
				to.Trace(L6, _T("Fingerprint server is not available"));
		
	}
	//-SR93 Biometrics support

  if (rc<0)
  {
    fStateFreeze = false;
	//TAR 236390 Merge TAR 211800 BZ 11/25/02: Still need to send an event to switch app even loading

	//device was failed, otherwise, switch app  won't work.

	CEvent evStart(TRUE, TRUE,_T("NCRSCOTLoading"));

	evStart.SetEvent();

	//TAR 236390

    if (rc==-3)                     // devices in error
    {
        SMStateBase *pRetState = DMHandleStartupError();
        if(pRetState != STATE_NULL)
        {
            return pRetState;
        }

        // Else, mask error condition and let it be handled in OOS2 state.
        rc = 0;
    }

	if (rc == -1)
	{
		trace(L0, _T("Failed to create Device Manager control, quit loading SCOT application"));
		ScotError(FATAL,SM_CAT_STARTUP_INIT, SCOTAPP_DMINTERFACE_DEVICE_CREATE);
		return STATE_STOP; // FATAL is posting STATE_STOP message to SCOT
	}
    ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_DM_INIT);

    if(rc != 0)                         // RFC 445940.
    {
        return STATE_RESTART;           // other error see error log
    }
  }
  long retErrorCode = DMConfigureScaleDisplay();


  if( retErrorCode > -1)
  {
	fStateFreeze = false;
	CString csWork = ps.GetPSText(retErrorCode);
	
	return createLevel1Message(csWork,    // char *message
						PS_NOTEXT,         // no canned message
						PS_NOTEXT,         // no prompt
						0,                 // no prompt value
						-1,                // no device
						false, false);            // no DM check health needed //SR672
  }

  // TAR 452531:  Moved the scale/scanner disable code that was here to 
  //              SMStateBase::SecMgrPreInitialize.
  
  int test = SecMgrInitialize();
  if (test <= 0)             // errors handled in called methods
  {
    fStateFreeze = false;
	trace(L0, _T("Failed to create Security Manager control, quit loading SCOT application"));
//	ScotError(FATAL,SM_CAT_STARTUP_INIT, SCOTAPP_SEC_MGR_PROCEDURE_INIT_FAILED);
	return STATE_STOP; // FATAL is posting STATE_STOP message to SCOT
  }

  // TAR 452531:  Moved post security manager init code from here to 
  //              SMStateBase::SecMgrPostInitialize().

//TAR 195179 & 191490 start
  //Signature printing maintenance start
	WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    
	CString csSigbmps = TEMP_SIGNATURE_DIRECTORY;
	GET_PARTITION_PATH(TEMP_SIGNATURE_DIRECTORY, csSigbmps.GetBuffer(_MAX_PATH));
	csSigbmps.ReleaseBuffer();

	CString csFullFileName;
	csSigbmps += _T("*.*");
	hFind = FindFirstFile(csSigbmps, &FindFileData);
	rc = 1;
	while ((hFind != INVALID_HANDLE_VALUE) && (rc))
	{
	   if ((FindFileData.cFileName != _T(".")) || (FindFileData.cFileName != _T("..")))
	   {
	   csFullFileName = TEMP_SIGNATURE_DIRECTORY;
	   GET_PARTITION_PATH(TEMP_SIGNATURE_DIRECTORY, csFullFileName.GetBuffer(_MAX_PATH));
	   csFullFileName.ReleaseBuffer();

	   csFullFileName += FindFileData.cFileName;
	   trace(L6,_T("Signature file deleted is: <%s>"), csFullFileName);
	   STATE(State)::SCOTDeleteFile(csFullFileName);   //TAR 348946
	   rc = FindNextFile(hFind, &FindFileData);
	   }
	} 

	FindClose(hFind);
  //Signature printing maintenance end
//TAR 195179 & 191490 end

  // Hard Totals initialization
  ps.Echo(HT_LOADING);
  DMSayPhrase(KEYPADVALID);
  if (HTInitialize()<0)             // errors handled in called methods
  {
    fStateFreeze = false;
    nReason = 3;                    //TAR126490 When we return to OutOfService, Exit app due to error
    ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_HT_INIT);
    return createLevel1Message(NULL,              // char *message
                               MSG_HARDTOTALS_ERROR,  // no canned message
                               PS_NOTEXT,         // no prompt
                               0,                 // no prompt value
							   DeviceClass,		  // -1 = no device
                               false, false);        // whether DM check health needed //SR672

  }
  ps.Echo(HT_LOADED);

  // user exit code initialization
  ps.Echo(UE_LOADING);
  DMSayPhrase(KEYPADVALID);
	
	//A Albertsons MGG
  if ((co.GetfUserExitAllEnabled()) &&   // if exits enabled and
		(co.GetfUserExit00Enabled()))        //    this exit is enabled
  {
		if (UserExit00()<0)
		{
			fStateFreeze = false;
			ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_UE_INIT);
			return STATE_RESTART;
		}
  }
	//E Albertsons MGG
	
  ps.Echo(UE_LOADED);
	
  if (co.GetfOperationsPrintVersion())
  {
	DMPrintVersion();
  }
#ifndef _CPPUNIT
  tcpConnect.Initialize();
#endif // _CPPUNIT
  InitializeSSCOMobile();

	
  // all initialization is done
  // try a sign on and off sequence
  // this is an async process so we have to return to NT after issuing
  // the sign on and allow TB events to drive the change to our
  // status.  The timeout() handler is called every second
  // to monitor our progress.
	
  fDoOptions = false;
  fStateFreeze = false;


  /******cm150000: Added In Merged Code******/
	trace(L2,_T("Firing StartScot event for SwitchApp."));
	CEvent evStart(TRUE, TRUE,_T("NCRSCOTLoading"));
	evStart.SetEvent();
	trace(L2,_T("Completed InitializeAll()."));
	/******cm150000: End Of Merged Code********/

  // move to after calling SecMgrInitialize()
  dm.fStartUpDevFactoryLoading = false;
  UpdateTransactionCountRegistry();
  RETURNSTATE(OutOfService2)
}

//////////////////////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMOutOfServiceBase::UnInitializeAll(void)
{
    UnInitializeSSCOMobile();
    tcpConnect.UnInitialize();
	USSFutil.DumpSymbolsByElementID();

#ifndef _CPPUNIT
  fStateFreeze = true;
  //Forced Shut Down
  trace(L7, _T("+rp.TransactionEnd"));
  rp.m_bForceEnd = true;
  rp.TransactionEnd();
  if (m_csCurrentReport != EMPTY_STRING)
  {
	 STATE(State)::SCOTDeleteFile(m_csCurrentReport);   //TAR 348946 - Delete Existing Report File
     m_csCurrentReport = EMPTY_STRING;
  }
  m_csRPT = EMPTY_STRING;
  trace(L7, _T("-rp.TransactionEnd Forced<%d>"), rp.m_bForceEnd);
#endif

    CLaneStateData laneStateData(m_pAppPSX);
    laneStateData.SendNextGenUIData(_T("NextGenData"), CString(_T("Unloading")));

#ifndef _CPPUNIT
  try
  {
    //ps.Prompt(PS_BLANK);
    ps.Echo(UE_STOPPING);
    DMSayPhrase(KEYPADVALID);
		
	if ((co.fUserExitAllEnabled) &&   // if exits enabled and
		(co.fUserExit99Enabled))      // this exit is enabled
    {
			UserExit99();
    }
	//BEGIN: TAR123168	LPM101199  BagScale & Scanner disabled while terminal out of service	
	// case 1: above InitializeAll() enables the bagscale & scanner, so they have to be disabled here
	DMScaleDisable();					// disable the scale
	DMScannerDisable();				// disable the scanner	
	//end 123168
  }
  catch(...)
  {
	  TRACE(_T("IN HERE"));
  }
	
  // Hard Totals uninitialize
  try
  {
    ps.Echo(HT_STOPPING);
    DMSayPhrase(KEYPADVALID);
    HTUnInitialize();
  }
  catch(...)
  {
	  TRACE(_T("IN HERE"));
  }

  try
  {
    ps.Echo(MSG_SEC_MGR_ENDED);
    DMSayPhrase(KEYPADVALID);
    SecMgrUnInitialize();
  }
  catch(...)
  {
	  TRACE(_T("Got an exception in SecMgrUnInitialize()"));
  }

  MotionSensorEnd();	  // RFC 219283 DE/RF Touchscreen Motion sensor enables 7883 scanner motor

  try
  {
    ps.Echo(DM_STOPPING);
    DMSayPhrase(KEYPADVALID);
    // Turn off tricolor light
    
    // (*) SR898 ====================
    DMTurnOffTriColorLight();

    dm.RemoteRelay(DM_MISC_STATE_OFF); // turn off lane light
    //DMScaleDisable();      // disable the scale moved to beginning of proc TAR123168
    DMEncryptorDisable();  // disable the encryptor
    DMUnInitialize();
  }
  catch(...)
  {
  }


  try
  {
    ps.Echo(DF_STOPPING);
    //DMSayPhrase(KEYPADVALID); //TAR238811 DM is CLOSED!!!!!
    gSCOTDevFactory.UnInitialize();
  }
  catch(...)
  {
  }

	
  try
  {
    ps.Echo(TB_STOPPING);
    //DMSayPhrase(KEYPADVALID); //TAR238811 DM is CLOSED!!!!!
    TBUnInitialize();
  }
  catch(...)
  {
	  TRACE(_T("In here"));
  }
	
  // Remote Approval uninitialize
  try
  {
    ps.Echo(RA_STOPPING);
    RAUnInitialize();
  }
  catch(...)
  {
	  TRACE(_T("In here"));
  }

  try
  {
	  custInfo.UnInitialize();
  }
  catch(...)
  {
	  TRACE(_T("CCustomerInfo::UnInitialize exception from OutOfService"));
  }
	
  try
  {
    //ps.Echo(PS_STOPPING);
    //DMSayPhrase(KEYPADVALID); //TAR238811 DM is CLOSED!!!!!
    PSUnInitialize();
	//Tar 143309
	if (ps.GetPickQtyList()->GetSize())
       ps.GetPickQtyList()->RemoveAll();
  }
  catch(...)
  {
	  TRACE(_T("IN HERE"));
  }
	  
	//+SR93 Biometrics
	if (co.m_bOperationsUseFingerPrintDevice)
	{
		g_bBiometricOpened = false;
		BOOL bFTClose = FT_Close();
		if (bFTClose == FALSE)
			to.Trace(L6, _T("Failed to close fingerprint device"));
		else
			to.Trace(L6, _T("Sucessfully closed fingerprint device"));

		//Tar 394580
		g_bBiometricACLServer = false;
		g_bBiometricACLDevice = false;

	}
	//-SR93 Biometrics

	//+SR Personalization Digital Receipt    
    try
    {
		dr.UnInitialize();
    }
    catch(CException* e)
	{
        _TCHAR szError[255] = {NULL};
		e->GetErrorMessage(szError, 255);
		to.Trace(L6, _T("Got an exception in destroying the ActiveX control. Error text:%s"), szError);
		e->Delete();
	}
	//-SR Personalization Digital Receipt
	
    //DMSayPhrase(KEYPADVALID); //TAR238811 DM is CLOSED!!!!!
  fStateFreeze = false;
#endif
  return STATE_ENDED;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// Routine to read and process a script file of commands from the 4690
//
//  File name SCOT:SCOTUPDT.DAT resides on the 4690 controller in the SCOT sub-dir
//
//  Commands: general form 'key=p1,p2,p3...p10', blanks may be used in place of = and ,
//            comments start with any non alpha in column 1
//            do not put comments on command lines
//
//      GET=filefrom4690,filetoNT
//      PUT=filefromNT,fileto4690
//      EXE=command,parameter1,parm2,...
//      RESTART
//      STOP
//      REGSET=key1,[key2,...,keyn,]valuename,valuedata  (key0 is HKEY_LOCAL_MACHINE)
//      i.e.  REGSET CURRENT_CONTROL_SET,SERVICES,TSF,PARAMETERS,DISPLAY,1
//
//  Note: Only FAT style 8.3 names can be copied from the 4690 but a ZIPed file
//        may contain long file names to be unzipped.
//
//  Example: Get a file and execute it. It may be a self extracting ZIP file...
//
//      GET=SCOT:NEW9804.EXE,C:\NEW9804.EXE
//      EXE=C:\NEW9804.EXE,-S,-R
//
SMStateBase  *SMOutOfServiceBase::ProcessMaintenance(void)
{
    return STATE_NULL;
}
#ifndef _CPPUNIT

void SMOutOfServiceBase::UnInitialize(void)
{
  // Don't try to access DM light if we're shutting down and
  // DM is already deleted.
  if (dm.IsOpen())
  {
    // (*) SR898 ====================
    DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                       DM_TRICOLORLIGHT_OFF,
                       DM_TRICOLORLIGHT_NORMALREQUEST);
	
	ra.OnNoRAPDataNeeded();		// Clear the ofline message on the RAP lane button.
  }
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("OutOfService") );	// RFC 330050
}


// call this to do a selective uninitialization required to unload options
void SMOutOfServiceBase::UnInitConfigData()
{
	// uninit start for load options
	fStateFreeze = true;
	try
	{
	//ps.Prompt(PS_BLANK);
	ps.Echo(UE_STOPPING);
	DMSayPhrase(KEYPADVALID);
	if ((co.fUserExitAllEnabled) &&   // if exits enabled and
		(co.fUserExit99Enabled))      // this exit is enabled
	{
			UserExit99();
	}
			
	}
	catch(...)
	{
	}
/*
	try
	{
		ps.Echo(DM_STOPPING);
		DMSayPhrase("KeyPadValid");
		// Turn off tricolor light
		dm.turnOffTriColorLight();
		dm.RemoteRelay(DM_MISC_STATE_OFF); // turn off lane light
		DMScaleDisable();      // disable the scale
		DMEncryptorDisable();  // disable the encryptor
		DMUnInitialize();
	}
	catch(...)
	{
	}

*/
	DMSayPhrase(KEYPADVALID);
	fStateFreeze = false;
	// uninit end for load options
}

SMStateBase * SMOutOfServiceBase::DMHandleStartupError(void)   // RFC 445940
{
    // If configured to use the old-style legacy device startup err handling.
    if(m_bDoLegacyDeviceErrs)
    {
        return SMCustomerModeState::DMHandleStartupError();
    }

    // Else, bypass startup device errors.
    return STATE_NULL;
}

// If delaying error handling, then we don't want to try to configure the 
// scale display if there is a potential problem with the device.
long SMOutOfServiceBase::DMConfigureScaleDisplay(void)
{
    if(m_bDoLegacyDeviceErrs)
    {
        return SMCustomerModeState::DMConfigureScaleDisplay();
    }

    return -1;
}

long SMOutOfServiceBase::DMPrintVersion(void)                // RFC 445940
{
    // If configured to use the old-style legacy device startup err handling.
    if(m_bDoLegacyDeviceErrs)
    {
        return SMCustomerModeState::DMPrintVersion();
    }

    // Else, this will be done in out-of-service 2.
    return OPOS_SUCCESS;
}

/**
 * TAR 452531.
 * Delay loading of security manager until after the devices have loaded.
 */
long SMOutOfServiceBase::SecMgrInitialize(void)
{
    TRACE_METHOD(_T("SMOutOfServiceBase::SecMgrInitialize"));

    // If configured to use the old-style device startup error handling.
    if(m_bDoLegacyDeviceErrs)
    {
        // Otherwise, initialize security as we normally do.
        SecMgrPreInitialize();
        long lRc = SMCustomerModeState::SecMgrInitialize();

        if(lRc > 0)  // Only do post if init succeeded.
        {
            SecMgrPostInitialize();
        }

        return lRc;
    }

    if(m_nDevInitStatus < 0)        // There is a device error.
    {
        trace(L6, 
           _T("Defer load of Security until all device errors are corrected."));
    }
    return 1;
}

// call this to do a selective initialization required to reload options
SMStateBase* SMOutOfServiceBase::InitConfigData()
{
    ps.SetCMLeadthruText(LTT_OUTOFSERVICE);

	fStateFreeze = true;
	//ps.Prompt(PS_BLANK);

    //let Transaction Broker reload its options first
    ps.Echo(TB_LOADING);
    DMSayPhrase(KEYPADVALID);
    TBLoadOptions();
    ps.Echo(TB_LOADED);

	// load configuration options files 000 and 'xxx'
	ps.Echo(CO_LOADING);              // again
	DMSayPhrase(KEYPADVALID);

	int nTermNumber = TBGetTerminalNumber();
  SCDownload( nTermNumber);  // TAR 287143
	if (!COInitialize(nTermNumber))
	{
		fStateFreeze = false;
		ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_CO_INIT);
		return STATE_RESTART;
	}
    loadOptionsFromConfigObject();
	ps.Echo(CO_LOADED);

	// RFC 219283 DE/RF Touchscreen Motion sensor enables 7883 scanner motor
	MotionSensorEnd();
	MotionSensorStart();

	// now finish loading PS allowing copy of new text files and load 50-keyboard
	ps.Echo(PS_LOADING);
	DMSayPhrase(KEYPADVALID);

	// load tender files 000 and 'xxx'
	if (!SCOTTENDInitialize(nTermNumber))
	{
		fStateFreeze = false;
		ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_CO_INIT);
		return STATE_RESTART;
	}
	ps.Echo(CO_LOADED);

	// now finish loading PS allowing copy of new text files and load 50-keyboard
	ps.Echo(PS_LOADING);
	DMSayPhrase(KEYPADVALID);

	//if (fDoOptions)
	//	ps.UnInitializeConfigData();

	if (PSInitialize()<0)             // errors handled in called methods
	{
		fStateFreeze = false;
		ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_PS_INIT);
		return STATE_RESTART;
	}

	//USSF Start
	USSFdisp.StoreSymbols();	// TAR #429050
	//USSF End  

	ps.Echo(PS_LOADED);

      
/*
	// device manager initialization
	ps.Echo(DM_LOADING);
	DMSayPhrase("KeyPadValid");
	long rc = DMInitialize();
	if (rc<0)
	{
		fStateFreeze = false;
		if (rc==-3)                     // devices in error
		{
		  CString csWork = ps.GetPSText(MSG_DEVICESERROR);
		  csWork += DMGetDevicesInError();
		  return createLevel1Message(csWork,            // char *message
								   PS_NOTEXT,         // no canned message
								   PS_NOTEXT,         // no prompt
								   0,                 // no prompt value
								   -1,                // no device
								   false);            // no DM check health needed
		}
		ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_DM_INIT);
		return STATE_RESTART;           // other error see error log
	}

	DMScaleEnable();         // enable the scale
	DMEncryptorEnable();     // enable the encryptor
	// Lane light off
	dm.RemoteRelay(DM_MISC_STATE_OFF);
	// Red light on
	dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
					  DM_TRICOLORLIGHT_ON,
					  DM_TRICOLORLIGHT_NORMALREQUEST);
	ps.Prompt(DM_LOADED);

*/

	// start of TAR 380418 
	// since we no longer call DMInitialize in this function,
	// we still need to call these two functions
	// (assumes dm is already initialized).
	trace(L6, _T("CashLow options reloaded - recheck for CoinDispenserLowSensor"));
	DMCheckForCoinDispenserLowSensor();
	DMCheckCashCounts(true);
	// end of TAR 380418
	// user exit code initialization
	ps.Echo(UE_LOADING);
	DMSayPhrase(KEYPADVALID);

	if ((co.fUserExitAllEnabled) &&   // if exits enabled and
		(co.fUserExit00Enabled))        //    this exit is enabled
	{
		if (UserExit00()<0)
		{
			fStateFreeze = false;
			ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_UE_INIT);
			return STATE_RESTART;
		}
	}

	ps.Echo(UE_LOADED);

	if (co.fOperationsPrintVersion)
	{
		DMPrintVersion();
	}

    InitializeSSCOMobile();

	// all initialization is done
	// try a sign on and off sequence
	// this is an async process so we have to return to NT after issuing
	// the sign on and allow TB events to drive the change to our
	// status.  The timeout() handler is called every second
	// to monitor our progress.

	fDoOptions = false;
	fStateFreeze = false;

	ps.Echo(BTT_OK);
	RETURNSTATE(OutOfService2)
}

// RFC 219283 ( 218445) DE/RF 
// Touchscreen Motion sensor enables 7883 scanner motor 
// only executes if scotopt option is > 0
// default setting for feature is off
int SMOutOfServiceBase::MotionSensorThreadEntry(void *p)
{
   ((SMOutOfServiceBase*)p)->MotionSensorThread();
   return 0;
}

// RFC 219283 (& 218445) DE/RF 
// Touchscreen Motion sensor enables 7883 scanner motor 
// only executes if scotopt option is > 0
// default setting for feature is off
void SMOutOfServiceBase::MotionSensorThread()
{
   CTime currentTime = NULL; 
   CTime nextTime = CTime::GetCurrentTime();
   CTime nextTimeOnAttract = CTime::GetCurrentTime(); //RFC 265465
   CTime m_lastTimeMotionSensorEnabledScanner = NULL;
   CTime m_lastTimeMotionSensorOnAttract = NULL;  //RFC 265465
   trace(L6, _T("+SMOutOfServiceBase::MotionSensorThread()  TID:0x%08.8x"), GetCurrentThreadId());
   CEvent MotionEvent(FALSE,                  // initially not signaled
					   FALSE,                   // auto reset
					   _T("MotionSensorEvent"));   // name of event

   while (m_bMotionSensorThreadRun)
   {	// wait for MotionSensorEvent
	   if(WaitForSingleObject(MotionEvent,2000) == WAIT_OBJECT_0)
	   {	// now double check that this state allows scanner to be enabled	   
      if(dm.IsScannerEnabled())
		  {
			  currentTime = CTime::GetCurrentTime();
			  // if we have never posted a motionevent or 
			  // if the time since the last post is > configured interval
			  // post a notification message
			  if((currentTime > nextTime) || (m_lastTimeMotionSensorEnabledScanner == NULL))
			  {
				  mo.PostGP(GP_MOTIONEVENT);
				  trace(L7, _T(" SMOutOfServiceBase::MotionSensorThread - Motion Was Sensed."));
				  m_lastTimeMotionSensorEnabledScanner = CTime::GetCurrentTime();
				  nextTime = m_lastTimeMotionSensorEnabledScanner + CTimeSpan( 0, 0, 0, m_MotionEnablesScannerInterval);    // 45 second interval
			  }
		  }
		  if (m_bMotionSensorEnabledOnAttract) //RFC 265465
		  {
			  currentTime = CTime::GetCurrentTime();
  			
			  if((currentTime > nextTimeOnAttract) || (m_lastTimeMotionSensorOnAttract == NULL))
			  {
				  mo.PostGP(GP_MOTIONONATTRACT);
				  trace(L7, _T(" SMOutOfServiceBase::MotionSensorThread - Motion Was Sensed on Attract Screen."));
				  m_lastTimeMotionSensorOnAttract = CTime::GetCurrentTime();
				  nextTimeOnAttract = m_lastTimeMotionSensorOnAttract + CTimeSpan( 0, 0, 0, m_MotionSensorOnAttractInterval);    
			  }
		  }
	   }
	   Sleep(250);
   }
   trace(L6, _T("-SMOutOfServiceBase::MotionSensorThread()"));
}


#endif //_CPPUNIT
// RFC 219283 (& 218445) DE/RF 
// Touchscreen Motion sensor enables 7883 scanner motor 
void SMOutOfServiceBase::MotionSensorStart()
{
#ifndef _CPPUNIT
   trace(L6, _T("+SMOutOfServiceBase::MotionSensorStart()"));
   m_MotionSensorOnAttractInterval = co.getTimeOutMotionSensorOnAttract();		//RFC265465 Fixed Bm185018
   m_MotionEnablesScannerInterval = co.getTimeOutTouchScreenEnablesScanner();
   trace(L0, _T(" SMOutOfServiceBase::MotionSensorStart MotionEnablesScannerInterval is %ld"),m_MotionEnablesScannerInterval); 

   if (!((m_MotionEnablesScannerInterval > 0) || (m_MotionSensorOnAttractInterval > 0)))  //RFC 265465
   {
	   trace(L0, _T(" SMOutOfServiceBase::MotionSensorStart -- MotionSensorThread will not be started"));
	   return;
   }

   if (m_hMotionSensorThread == NULL)
   {
      m_bMotionSensorThreadRun = true;
      m_hMotionSensorThread = AfxBeginThread((AFX_THREADPROC) MotionSensorThreadEntry, (LPVOID)this,0 ,0, NULL); 
      trace(L6, _T(" SMOutOfServiceBase::MotionSensorStart -- fired up the thread. Handle: 0x%08.8x"), 
         m_hMotionSensorThread);
   }
   else
   {
       trace(L6, _T(" SMOutOfServiceBase::MotionSensorStart - m_hMotionSensorThread already started"));
   }
   trace(L6, _T("-SMOutOfServiceBase::MotionSensorStart()"));
#endif //_CPPUNIT
}

// RFC 219283 (& 218445) DE/RF 
// Touchscreen Motion sensor enables 7883 scanner motor 
void SMOutOfServiceBase::MotionSensorEnd()
{
#ifndef _CPPUNIT
   trace(L6, _T("+SMOutOfServiceBase::MotionSensorEnd()"));
   if (m_hMotionSensorThread != NULL)
   {
      trace(L6, _T(" SMOutOfServiceBase::MotionSensorEnd -- setting the thread stop flag..."));
      m_bMotionSensorThreadRun = false;
      if( WaitForSingleObject(m_hMotionSensorThread->m_hThread, INFINITE) == WAIT_FAILED)
	  {
		LONG nLastError = GetLastError();
        trace(L6, _T(" SMOutOfServiceBase::MotionSensorEnd -- WaitForSingleObject error %d."),nLastError);
	  }
      trace(L6, _T(" SMOutOfServiceBase::MotionSensorEnd -- thread stopped."));
      m_hMotionSensorThread = NULL;
   }
   else
   {
      trace(L6, _T(" SMOutOfServiceBase::MotionSensorEnd -- thread not running."));
   }
   trace(L6, _T("-SMOutOfServiceBase::MotionSensorEnd()"));
#endif //_CPPUNIT
}

// Create/update the transaction count registry key
void SMOutOfServiceBase::UpdateTransactionCountRegistry()
{
#ifndef _CPPUNIT
  HKEY  hKey;
  DWORD         disposition;
  unsigned long dataLength = sizeof(DWORD);
  long lTrxCount = 0;

  // If the entry does not exist, it will create one.
  if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\TransactionCount"), 
       0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,NULL,&hKey, &disposition) == ERROR_SUCCESS)
  {
	  CTime ctCurrentTime = CTime::GetCurrentTime();
	  int nCurrentDay = ctCurrentTime.GetDay();
	  int nRegistryDay = 0;
	  if (disposition == REG_CREATED_NEW_KEY)
	  {
	  // Write a string value to the registry.
		RegSetValueEx(hKey, _T("TransactionCount"), 0L, REG_DWORD, (LPBYTE)&lTrxCount, sizeof(DWORD));
		RegSetValueEx(hKey, _T("TransactionDate"),0L, REG_DWORD, (LPBYTE)&nCurrentDay, sizeof(DWORD));
	  }
	  else if (disposition == REG_OPENED_EXISTING_KEY)
	  {
		 if(RegQueryValueEx(hKey, _T("TransactionDate"), NULL, NULL, (LPBYTE)&nRegistryDay, &dataLength) == ERROR_SUCCESS)
		 {
			 if (nRegistryDay != nCurrentDay)
			 {
				 RegSetValueEx(hKey, _T("TransactionDate"), 0, REG_DWORD, (LPBYTE)&nCurrentDay, sizeof(DWORD));
				 RegSetValueEx(hKey, _T("TransactionCount"), 0, REG_DWORD, (LPBYTE)&lTrxCount, sizeof(DWORD));
			 }
			 else if(RegQueryValueEx(hKey, _T("TransactionCount"), NULL, NULL, (LPBYTE)&lTrxCount, &dataLength) == ERROR_SUCCESS)
			{
				COleVariant v;
				v = lTrxCount;
				ps.GetPSXObject()->SetTransactionVariable( _T("TransactionCount"), v );
			}
		 }
	  }
  }
  RegCloseKey(hKey);
#endif //_CPPUNIT
}
