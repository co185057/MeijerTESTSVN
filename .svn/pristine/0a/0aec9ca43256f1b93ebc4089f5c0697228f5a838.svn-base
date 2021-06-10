//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOutOfServiceBase.CPP
//
// TITLE: Class implementation for out of service state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header
#include <mmsystem.h>
#include "resource.h"
#include "Common.h"                 // Common includes

#include "SMOutOfService.h"
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
DLLEXPORT extern DMProcedures dm;   // Global DM object
#define _BILLRECYCLER _T("BILLRECYCLER")
#define _COINRECYCLER _T("COINRECYCLER")

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("OutOfServiceBase")

DLLEXPORT extern HWND hw;

DEFINE_TIMESTAMP

//IMPLEMENT_STATE(OutOfService)

IMPLEMENT_DYNCREATE(SMOutOfServiceBase, CObject)// MFC Runtime class/object information

//////////////////////////////////////////////////////////////////////////////////////////////////
SMOutOfServiceBase::SMOutOfServiceBase(const int nWhy)
: nReason(nWhy)
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

////////////////////////////////////////
SMOutOfServiceBase::SMOutOfServiceBase()
{
  IMPLEMENT_TIMESTAMP
  nReason = 1;        // TAR 142611
}

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
void SMOutOfServiceBase::SCDownload(int nTerminalNumber)
{
  SMStateBase::SCDownload(nTerminalNumber);
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
	return SMStateBase::COInitialize(nTerminalNumber, bLocal);
/*
	if (!bLocal)
	{
		// get files from server to local path
		CString csOptionFile, csExt, csFileName;
		CString csDestPath = co.csConfigFilePath+"\\";
		TBRC nTBCallStatus;

		// get generic SCOT option file from server
		// Here SCOT App assumes that TB knows the config file path
		// since SCOT passes the path as an argument to TB on TBInitialize() 
		csOptionFile = SCOTOPTS;
		csExt = ".000";
		csFileName = csOptionFile + csExt;

		nTBCallStatus = CopyFileFromServer(csFileName,csDestPath+csFileName,false);	// destination path is null
		ASSERT(nTBCallStatus == TB_SUCCESS);
		if (nTBCallStatus != TB_SUCCESS)
		{
			return false;
		}

		// copy terminal specific option file from server
		csExt.Format(".%03d", nTerminalNumber);
		csFileName = csOptionFile + csExt;
		nTBCallStatus = CopyFileFromServer(csFileName,csDestPath+csFileName,false);	// destination path is null
		ASSERT(nTBCallStatus == TB_SUCCESS);
		if (nTBCallStatus != TB_SUCCESS)
		{
			return false;
		}
		// do a local options initialize now
		return COInitialize(nTerminalNumber, true);
	}
	else
	{
		int nRetValue = -1;
		nRetValue = co.Initialize(nTerminalNumber);
		bool bRetStatus = (nRetValue >= 0);
		return bRetStatus;
	}
*/
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
	return SMStateBase::SCOTTENDInitialize(nTerminalNumber, bLocal);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMOutOfServiceBase::InitializeAll(void)
{
  fStateFreeze = true;
  //ps.Prompt(PS_BLANK);
  ps.SetCMLeadthruText(LTT_OUTOFSERVICE);
  int DeviceClass = -1;					// send to system message screen for new EAS devices
  bool bCkHealth = false;
  ps.Echo(TB_LOADING);
  DMSayPhrase(KEYPADVALID);

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
  ps.Echo(CO_LOADED);

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
  if (co.fOperationsEnableUSSF)

  {
		InitializeUSSF();
		if (myDisplayShop != NULL)
		{
			myDisplayShop -> StoreSymbols();
		}
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

    ps.Echo(TAB_LOADING);
    DMSayPhrase(KEYPADVALID);
  }

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
    csWork += _T("\n\n");
    csWork += gSCOTDevFactory.GetDevicesInError();
    
	if(csWork.Find(_T("NCR5992SigCap")) !=-1 || csWork.Find(_T("NCR5992Pinpad")) !=-1 || csWork.Find(_T("NCR5945")) !=-1)  // Pinpad error 3.1 DeviceError
	{
		csWork = _T("DeviceErrorSigCap.xml:ERROR_SIGCAP_NO_COMM");

		if( _PINPAD_3X4X == dm.m_csPinpadRelease )
		{
		  csWork = _T("DeviceErrorSigCap.xml:ERROR_SIGCAP_NO_COMM_3X4X");	// Tar 310198
		}

		DeviceClass = DMCLASS_MSR;
	}

	  return createLevel1Message(csWork,          // char *message
                               PS_NOTEXT,         // no canned message
                               PS_NOTEXT,         // no prompt
                               0,                 // no prompt value
							   DeviceClass,		  // -1 = no device
                               bCkHealth);        // whether DM check health needed
  }

  ASSERT(gpSCOTAuxDisp			!= NULL);
  ASSERT(gpSCOTEncryptor		!= NULL);
  ASSERT(gpSCOTSigCap			!= NULL);



	//+SR93 Biometrics
	if (co.m_bOperationsUseFingerPrintDevice)
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
	
  // device manager initialization
  ps.Echo(DM_LOADING);
  DMSayPhrase(KEYPADVALID);
  rc = DMInitialize();

	//+SR93 Biometrics support
	//display biometric device error if it encounter as such..
	if(co.m_bOperationsUseFingerPrintDevice)
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
	  CString csWork;
      if (dm.m_nStartDeviceErrorDeviceClass > -1)  
	  {
        // tar 277605//no ResultCodeExtended for DMCLASS_MISC and DMCLASS_MOTION_SENSOR
		if(	dm.m_nStartDeviceErrorDeviceClass == DMCLASS_MISC ) // tar 277605
			csWork = _T("DeviceErrorMisc.xml:NCR_TRILIGHT_ERROR");
        else if( dm.m_nStartDeviceErrorDeviceClass == DMCLASS_MOTION_SENSOR ) // tar 277605
			csWork = _T("DeviceErrorMotionSensor.xml:NCR_DIGITAL_INTERFACE_ERROR");
		else if(dm.m_nStartDeviceErrorDeviceClass == DMCLASS_CASHRECYCLER )
		{
			// Note that the input parameter to CashRecyclerGetErrorCode
			// is not used for the SafePay device.
			// The hard-coded arguments below are from existing Glory recycler
			// code.  0x80 specifies the Glory note recycler while 0x01
			// specifies the Glory coin recycler.
			 long lBillErrorCode = dm.CashRecyclerGetErrorCode(0x80);
			 CString csTempErrorCode;
			 if(lBillErrorCode)
			  {
					//Bill Hardware errors
					csTempErrorCode.Format(_T("%d"),lBillErrorCode);

					if(dm.m_csCashRecyclerModel == _CASHDISPENSER_GLORY)
					{
						csTempErrorCode = csTempErrorCode.Left(2); // xml file only use first 2 digits
					  	csWork = GetDeviceErrorMnemonic(dm.m_nStartDeviceErrorDeviceClass, 
							_BILLRECYCLER, 0, _ttol(csTempErrorCode), 
							nDeviceErrorContext );
					}
					else if(dm.m_csCashRecyclerModel == _CASHDISPENSER_SAFEPAY)
					{
						// SafePay - If an extended result code was read on init,
						// then use it.
						if(dm.m_nStartDeviceErrorResultCodeExtended)
						{
							lBillErrorCode = dm.m_nStartDeviceErrorResultCodeExtended;
						}

					  	csWork = GetDeviceErrorMnemonic(dm.m_nStartDeviceErrorDeviceClass, 
							dm.m_csCashRecyclerModel, 0, lBillErrorCode, 
							nDeviceErrorContext );
					}
					m_nCashRecyclerHardwareErrorCode = lBillErrorCode;

			  }
			  else 
			  {
				  long lCoinErrorCode = dm.CashRecyclerGetErrorCode(0x01);
				  if(lCoinErrorCode)
				  {
					//Coin Hardware errors
					csTempErrorCode.Format(_T("%d"),lCoinErrorCode);
					csTempErrorCode = csTempErrorCode.Left(2); // xml file only use first 2 digits
				  	csWork = GetDeviceErrorMnemonic(dm.m_nStartDeviceErrorDeviceClass, _COINRECYCLER, 0,  _ttol(csTempErrorCode), nDeviceErrorContext );
					m_nCashRecyclerHardwareErrorCode = lCoinErrorCode;
				  }
				  else
				  {
					csWork = GetDeviceErrorMnemonic(dm.m_nStartDeviceErrorDeviceClass, _T(""), CHAN_STATUS_JAM, 0, nDeviceErrorContext );
				  }
			  }
		
			DeviceClass = dm.m_nStartDeviceErrorDeviceClass; 
		}
		else
		{	
			//App starts, Cash Change Error or Scanner Error
			csWork = GetDeviceErrorMnemonic(dm.m_nStartDeviceErrorDeviceClass, dm.GetDeviceName(dm.m_nStartDeviceErrorDeviceClass, 0), EVENTID_ANY, dm.m_nStartDeviceErrorResultCodeExtended, 0 );
			dm.m_nStartDeviceErrorResultCodeExtended = 0;  //Reset
			DeviceClass = dm.m_nStartDeviceErrorDeviceClass; 
		}
		
		//SR742 - Don't reset device error covered on this feature on startup while in degraded mode.
		if ( ! ( co.IsDegradedModeOn() 
			&& ( IsCashDeviceError (dm.m_nStartDeviceErrorDeviceClass)  
			|| IsCardDeviceError (dm.m_nStartDeviceErrorDeviceClass ) ) ) )
		{
			dm.m_nStartDeviceErrorDeviceClass = -1;
		}

	  }
      
	  if(csWork.IsEmpty() || csWork.Find(_T("DEVICEERRORUNKNOWN")) != -1)  //Not handling by the extended result code
	  {
         csWork = ps.GetPSText(MSG_DEVICESERROR); // code from below replaced here
         csWork += DMGetDevicesInError(); // code from below replaced here

         if(csWork.Find(_T("Signature Capture"))!=-1)  //It is sig cap device error
		 {
			DeviceClass = DMCLASS_SIGCAP;
		 }		 else if(csWork.Find(_T("Scale"))!=-1)  //It is Scale device error
		 {
			DeviceClass = DMCLASS_SCALE;
		 }
		 else if(csWork.Find(_T("OnTrak"))!=-1)  //It is MISC device error
		 {
			DeviceClass = DMCLASS_MISC;
		 }
		 else if(csWork.Find(_T("ZT1000"))!=-1)  //It is CASHACCEPTOR device error
		 {
			DeviceClass = DMCLASS_CASHACCEPTOR;
		 }
		 else if(csWork.Find(_T("5945"))!=-1)  //It is MSR device error
		 {
			DeviceClass = DMCLASS_MSR;
		 }
		 else if(csWork.Find(_T("Journal"))!=-1)  //It is JOURNAL device error
		 {
			DeviceClass = DMCLASS_JOURNAL;
		 }


	  }
	  return createLevel1Message(csWork,            // char *message
                               PS_NOTEXT,         // no canned message
                               PS_NOTEXT,         // no prompt
                               0,                 // no prompt value
							   DeviceClass,		  // -1 = no device
                               bCkHealth);        // whether DM check health needed
    }
	if (rc == -1)
	{
		trace(L0, _T("Failed to create Device Manager control, quit loading SCOT application"));
		ScotError(FATAL,SM_CAT_STARTUP_INIT, SCOTAPP_DMINTERFACE_DEVICE_CREATE);
		return STATE_STOP; // FATAL is posting STATE_STOP message to SCOT
	}
    ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_DM_INIT);
    return STATE_RESTART;           // other error see error log
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
						false);            // no DM check health needed
  }
  //BEGIN: TAR123168	LPM101199  BagScale & Scanner disabled while terminal out of service	
  // DMInitialize() enables the bagscale & scanner, so they have to be disabled here
  DMScaleDisable();					// disable the scale
  DMScannerDisable();				// disable the scanner	
  //end 123168

  //DMScaleEnable();       // enable the scale moved to attract base TAR123268
  DMEncryptorEnable();     // enable the encryptor
  // Lane light off
  dm.RemoteRelay(DM_MISC_STATE_OFF);
  // Red light on
  dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                      DM_TRICOLORLIGHT_ON,
                      DM_TRICOLORLIGHT_NORMALREQUEST);
  ps.Echo(DM_LOADED);
	
  // load Weight Info Object
  ps.Echo(WTINFO_LOADING);
  DMSayPhrase(KEYPADVALID);
  // NewSec
  ps.Echo(MSG_SEC_MGR_LOADING);
  DMSayPhrase(KEYPADVALID);


  if (SecMgrInitialize()<= 0)             // errors handled in called methods
  {
    fStateFreeze = false;
	trace(L0, _T("Failed to create Security Manager control, quit loading SCOT application"));
//	ScotError(FATAL,SM_CAT_STARTUP_INIT, SCOTAPP_SEC_MGR_PROCEDURE_INIT_FAILED);
	return STATE_STOP; // FATAL is posting STATE_STOP message to SCOT
  }
  // set the value of ScaleMetric option in the Security Manager
  if (co.fOperationsScaleMetric)
  {
	SetSecurityConfig( CONFIG_KEY_SS_METRIC_SCALE, _T("true"));
  }
  else
  {
	SetSecurityConfig(CONFIG_KEY_SS_METRIC_SCALE, _T("false"));
  }

  if (co.fOperationsShowItemRemovedButtonOnRemoveItemViolationScreen)
  {
	SetSecurityConfig( CONFIG_KEY_REMOVE_ITEM_VIOLATION_AUTO_ADVANCE, _T("true"));
  }
  else
  {
	SetSecurityConfig(CONFIG_KEY_REMOVE_ITEM_VIOLATION_AUTO_ADVANCE, _T("false"));
  } 

  g_bDelayedSecIntervention = GetSecurityConfig(CONFIG_KEY_DELAY_EXCEPTIONS);
  g_bSubstCkPriceEmbeddedWeights = GetSecurityConfig(CONFIG_KEY_SUBST_CK_PRICE_EMBEDDED_WT);
  g_bIsForgiveUnexpectedDecreaseDuringTendering = GetSecurityConfig(CONFIG_KEY_FORGIVE_UNEX_DEC_DURING_TENDERING);
  //sr714+
  if( GetSecurityConfigValue(CONFIG_KEY_TARE_WT_1) != _T("") )
	{
		g_csTareWt1 = GetSecurityConfigValue(CONFIG_KEY_TARE_WT_1); 
	}
	if( GetSecurityConfigValue(CONFIG_KEY_TARE_WT_2) != _T("") )
	{
		g_csTareWt2 = GetSecurityConfigValue(CONFIG_KEY_TARE_WT_2); 
	}
	if( GetSecurityConfigValue(CONFIG_KEY_TARE_WT_3) != _T("") )
	{
		g_csTareWt3 = GetSecurityConfigValue(CONFIG_KEY_TARE_WT_3); 
	}
	//sr714-


  trace(L6, _T("SubstCkPriceEmbeddedWeights items = %d."), g_bSubstCkPriceEmbeddedWeights);

  //Check for TAB is available or not
  CString csDevices = SecMgr.GetDevices();
  if (csDevices.Find(VALUE_DEVICE_TAKEAWAY_BELT) != -1)
  {
	  dm.fStateTakeawayBelt = true;
	  trace(L6, _T("!!!!!!!!!!!!!!!This is TAB system.!!!!!!!!!!!!"));	
  }
  else
  {
	  trace(L6, _T("!!!!!!!!!!!!!!!This is non-TAB system.!!!!!!!!!!!!"));
  }

  co.InitCustomerBagAllowed(dm.fStateTakeawayBelt); // TAR 393731

  ps.Echo(MSG_SEC_MGR_LOADED); 

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
	   SMStateBase::SCOTDeleteFile(csFullFileName);   //TAR 348946
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
                               bCkHealth);        // whether DM check health needed

  }
  ps.Echo(HT_LOADED);

  // user exit code initialization
  ps.Echo(UE_LOADING);
  DMSayPhrase(KEYPADVALID);
	
	//A Albertsons MGG
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
	//E Albertsons MGG
	
  ps.Echo(UE_LOADED);
	
  if (co.fOperationsPrintVersion)
  {
	DMPrintVersion();
  }
	
	
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

  SynchronizeTime();
  dm.fStartUpDevFactoryLoading = false;
  UpdateTransactionCountRegistry();
  RETURNSTATE(OutOfService2)
}

//////////////////////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMOutOfServiceBase::UnInitializeAll(void)
{
	USSFutil.DumpSymbolsByElementID();

  fStateFreeze = true;
  //Forced Shut Down
  trace(L7, _T("+rp.TransactionEnd"));
  rp.m_bForceEnd = true;
  rp.TransactionEnd();
  if (m_csCurrentReport != EMPTY_STRING)
  {
	 SMStateBase::SCOTDeleteFile(m_csCurrentReport);   //TAR 348946 - Delete Existing Report File
     m_csCurrentReport = EMPTY_STRING;
  }
  m_csRPT = EMPTY_STRING;
  trace(L7, _T("-rp.TransactionEnd Forced<%d>"), rp.m_bForceEnd);

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
    dm.turnOffTriColorLight();

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

    //DMSayPhrase(KEYPADVALID); //TAR238811 DM is CLOSED!!!!!
  fStateFreeze = false;
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
 /* _TCHAR szFile1[_MAX_PATH];
  _TCHAR szFile2[_MAX_PATH];
  CString csToken[10];
  BOOL fStart = false;
  BOOL fStop = false;
	
  // copy the 4690 controller maintenance script file name SCOTUPDT.DAT
  _tcscpy(szFile1,SCOT4690R SCOTUPDT ".DAT");
  _tmakepath(szFile2,SCOTDRIVE,SCOTPATH,SCOTUPDT,"DAT");
	
  // copy the file SCOTUPDT.DAT if it is different from our local copy
  // return:
  //        -1 error, read or control data error, file not copied
  //        -2 warning, same file data/time/size, file not copied
  //        other -ve rc means file error from 4690 like 0x80104010 not found
  //        other >=0, number of bytes copied
  if (CopyFileFrom4690(szFile1,szFile2,false)>0)
  {
    trace(L4,"Processing updates from file <%s>",szFile1);
    ps.Prompt(SCOTUPDT);
		
    try
    {
      CString csWork;
			
      CStdioFile csfIn(szFile2,CFile::modeRead);
			
      // read a line from SCOTUPDT.DAT
      while (csfIn.ReadString(csWork))
      {
        csWork.TrimLeft();
        csWork.TrimRight();
				
        if (!_istalpha(*csWork)) continue;
				
        trace(L6,"   Update command line <%s>",csWork);
        ps.Echo(csWork);
				
        // parse each line...  key=p1,p2,p3...p10
        int nTokens = 0;
				
        int i = csWork.Find('=');
        if (i<0) i = csWork.Find(' ');
        if (i<0) i = csWork.GetLength();
				
        while (i>0)
        {
          csToken[nTokens] = csWork.Left(i); // get token
          csToken[nTokens].TrimLeft();
          csToken[nTokens].TrimRight();
					
          nTokens++;
          if (nTokens==10) break;   // max 10 tokens
          csWork = csWork.Mid(i+1); // remove token from input
	        csWork.TrimLeft();
  				
          i = csWork.Find(',');
          if (i<0) i = csWork.Find(' ');
          if (i<0) i = csWork.GetLength();
        }
				
        if (nTokens==0) continue;   // no tokens
				
        // execute each line GET, PUT, EXE etc
				
        csToken[0].MakeUpper();
				
        if (csToken[0]=="GET" && nTokens>=3)
        {
          trace(L8,"      Copy file from 4690 to NT <%s> to <%s>",csToken[1],csToken[2]);
          CopyFileFrom4690(csToken[1],csToken[2],true);	// force a copy from 4690
          continue;
        }
				
        if (csToken[0]=="PUT" && nTokens>=3)
        {
          trace(L8,"      Copy file from NT to 4690 <%s> to <%s>",csToken[1],csToken[2]);
          CopyFileTo4690(csToken[1],csToken[2]); // force a copy to 4690
          continue;
        }
				
        if (csToken[0]=="EXE" && nTokens>=2)
        {
          trace(L8,"      Execute <%s>",csToken[1]);
          CString csParms;
          for (int i=2;i<nTokens;i++)
          {
            trace(L8,"         Parm %d <%s>",i-1,csToken[i]);
            csParms += csToken[i];
            csParms += " ";
          }
          HINSTANCE rc = ShellExecute(
            hw,                     // handle to parent window
            NULL,                   // pointer to string that specifies operation to perform
            csToken[1],             // pointer to filename or folder name string
            csParms,                // pointer to string that specifies executable-file parameters
            "C:\\",                 // pointer to string that specifies default directory
            SW_HIDE);               // whether file is shown when opened
					
          if ((const int)rc<=32)
          {
            ScotError(RETURN,"EXE %s failed rc=%d",csToken[1],(const int)rc);
          }
          continue;
        }
				
        if (csToken[0]=="STOP" && nTokens>=1)
        {
          trace(L8,"      Will STOP");
          fStop = true;
          continue;
        }
				
        if (csToken[0]=="RESTART" && nTokens>=1)
        {
          trace(L8,"      Will RESTART");
          fStart = true;
          continue;
        }
				
        if (csToken[0]=="REGSET" && nTokens>=4)
        {
          HKEY hKey = HKEY_LOCAL_MACHINE;
          long rc;
          int nSubKey;
					
          for (nSubKey=1;nSubKey<nTokens-2;nSubKey++) // open or create subsequent keys
          {
            DWORD disp;
            rc = RegCreateKeyEx(
              hKey,								  // handle of open key
              csToken[nSubKey],// address of name of subkey to open
              0,									  // reserved
              "",									  // address of class string
              REG_OPTION_NON_VOLATILE, // options
              KEY_ALL_ACCESS,// security access mask
              NULL,								  // address of key security structure
              &hKey,							  // address of buffer for opened handle
              &disp);							  // address of disposition value buffer
            if (rc!=ERROR_SUCCESS) break;
          }
					
          if (rc!=ERROR_SUCCESS)
          {
            continue;
          }
					
          const int l = csToken[nTokens-1].GetLength() + 1;
          const _TUchar *u = (_TUchar *)csToken[nTokens-1].GetBuffer(l);
					
          rc = RegSetValueEx(
            hKey,									  // handle of key to set value for
            csToken[nTokens-2],// address of value to set
            0,										  // reserved
            REG_SZ,								  // flag for value type
            u,										  // address of value data
            l);										  // size of value data
					
          continue;
        }
      }
			
      trace(L8,"   End of update file");
			
      csfIn.Close();
    }
    catch(CFileException *e)
    {
      eo.FileError(RETURN,"Process Maintenance",*e);
      return STATE_NULL;
    }
		
    trace(L4,"Updates completed from file <%s>",szFile1);
  }
	
  if (fStop)
  {
    ScotError(RETURN,"Maint - STOP");
    return STATE_STOP;
  }
	
  if (fStart)
  {
		ScotError(RETURN,"Maint - RESTART");
		return STATE_RESTART;
  }
*/	
  return STATE_NULL;
}

void SMOutOfServiceBase::UnInitialize(void)
{
  // Don't try to access DM light if we're shutting down and
  // DM is already deleted.
  if (dm.IsOpen())
  {
    dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
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
	if (myDisplayShop != NULL)
	{
		myDisplayShop -> StoreSymbols();
	}
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


// RFC 219283 (& 218445) DE/RF 
// Touchscreen Motion sensor enables 7883 scanner motor 
void SMOutOfServiceBase::MotionSensorStart()
{
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
}

// RFC 219283 (& 218445) DE/RF 
// Touchscreen Motion sensor enables 7883 scanner motor 
void SMOutOfServiceBase::MotionSensorEnd()
{
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
}

// Create/update the transaction count registry key
void SMOutOfServiceBase::UpdateTransactionCountRegistry()
{
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
}

