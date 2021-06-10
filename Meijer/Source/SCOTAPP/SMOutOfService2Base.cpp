//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOutOfService2Base.cpp
//
// TITLE: Class implementation for out of service state, waiting for TS testing
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMOutOfService2.h"     
#include "SMOutOfService.h"         // SSCOP-4297

#include "DMProcedures.h"           // DM procedures

#include "SMSystemMessageStartup.h" // Handling of device errors at startup.
#include "SMInprogress.h"           // SSCOADK-1318
#include <assert.h>

DLLEXPORT extern DMProcedures dm;   // Global DM object

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("OutOfService2Base")

IMPLEMENT_DYNCREATE(SMOutOfService2Base, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(OutOfService2)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMOutOfService2Base::SMOutOfService2Base() : m_bDoLegacyDeviceErrs(false)
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////
SMStateBase  *SMOutOfService2Base::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("OutOfService2") );	// RFC 330050
  setAnchorState(BES_OUTOFSERVICE2);
  fStateFreeze = true;              // stay in this state no matter what!
 
  m_nDMSavedDev = -1;               // RFC 445940 -- Reset.
  trace(L6, _T("m_nDMSavedDev set to %d"), m_nDMSavedDev);

  // TAR 452531:  Cache result of registry read.
  m_bDoLegacyDeviceErrs = dm.IsLegacyStartupErrorHandling();

  // (*) SR898 ====================
  DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                     DM_TRICOLORLIGHT_ON,
                     DM_TRICOLORLIGHT_NORMALREQUEST);

  ps.SetDisplayWMFlag(true); //load W&M information when scot starts - improve screen display

  ps.SetCMFrame(_T("OutOfService2"));

  
//  ps.Echo(TESTING_TB);
  ps.Echo(TB_LOADING);  //RFQ 986 - use better "loading" message
  DMSayPhrase(KEYPADVALID);

  // +RFC 445940:  
  if(m_nDevInitStatus == DMProcedures::DM_DEVICE_CASH_LEVEL_ERROR)
  {
     // Handle return from refilling dispensers.
      assert(dm.m_nStartDeviceErrorDeviceClass != -1);
      if(dm.m_nStartDeviceErrorDeviceClass == -1)
      {
        trace(L6, _T("WARNING:  Invalid device class [%d];  Using class [%d]"),
        dm.m_nStartDeviceErrorDeviceClass, DMCLASS_CASHCHANGER);

        dm.m_nStartDeviceErrorDeviceClass = DMCLASS_CASHCHANGER;
      }

      m_nDevInitStatus = dm.ReloadDevice(dm.m_nStartDeviceErrorDeviceClass);

      if(m_nDevInitStatus == OPOS_SUCCESS)
      {
         // Check all devices again. 
         m_nDevInitStatus = DMInitialize();
      }
  }

  // Check if there were device errors at startup.  If there were, then 
  // proceed to error screen.
  if(m_nDevInitStatus == DMProcedures::DM_DEVICE_ERROR)// Device startup error.
  {
      assert(dm.m_nStartDeviceErrorDeviceClass != -1);

      // Need to preserve this variable because DMHandleStartupError() may
      // erroneously reset it!
      int nStartDeviceErrorDeviceClass = dm.m_nStartDeviceErrorDeviceClass;
      SMStateBase *pRetState = DMHandleStartupError();

      // This might cause problems for degraded mode?
      dm.m_nStartDeviceErrorDeviceClass = nStartDeviceErrorDeviceClass; 

      if(pRetState != STATE_NULL)
      {
          // Prevent any events that come in for this device from being queued.
          m_nDMSavedDev = dm.m_nStartDeviceErrorDeviceClass;
          trace(L6, _T("m_nDMSavedDev set to %d"), m_nDMSavedDev);
          return pRetState;
      }
  }
  // -RFC 445940

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

  // +TAR 452531:  Delay loading of security until devices have loaded.
  int test = SecMgrInitialize();
  if (test <= 0)             // errors handled in called methods
  {
    fStateFreeze = false;
    trace(L0, 
     _T("Failed to initialize Security Manager control, quit loading SSCO."));
    CREATE_AND_DISPENSE(OutOfService)(4);//SSCOP-4297: unload devices before exit
  }
  // -TAR 452531:  Delay loading of security until devices have loaded.

  SynchronizeTime();//SSCOP-2494

  if (co.fOperationsPrintVersion)
  {
	DMPrintVersion();
  }
	
	// we use the nTimeOut as a sequencing control
	// 
	// 30 - initializing, getting to CLOSED state if needed, sign off
	// 29 - Got initial CLOSED, startof test sequence, sign on, wait for ENTERITEM
	// 28 - got ENTERITEM, sign off, wait for CLOSED
	//	1	- got final CLOSED, sequence completed, exit this state

  nTimeOut = 30;                    // wait up to 30 seconds for display change
  ps.ShowCMFrame();
  
  // now we wait for ts 2x20 events or the timer ticks to check our progress
  return TimedOut();                // initiate first display change
}

///////////////////////////////////////
SMStateBase  *SMOutOfService2Base::TimedOut(void)
{
  //if (nTimeOut>1)                   // still waiting for tests to complete
  //{
    //TBRefreshState();	// force redisplay to drive TBShow2x20 function below
    //return STATE_NULL;
  //} KSo

	
	// all done, exit this state
  dm.RemoteRelay(DM_MISC_STATE_ON);	// turn on lane light
  fStateFreeze = false;             // ok to change to Attract
  // let's trace mem usage at startup
  traceMem(L6);

  // Start the remote console service
  ps.StartRemoteServer();
  //Tar 243113 {+
  SMStateBase* pRet = this->CkStateStackAndReturn(); 

  if(pRet != STATE_NULL)
  {
	  //This is needed to handle the condition where the TB_NEEDLANECLOSED received in 
	  // a SystmeMessageState and is has not been processed.
	  return pRet;
  }
  //Tar 243113 -}
  //+RFC 986  
  setAnchorState(BES_START); 
  fPutSCOTinLC = co.fStateStartupToLaneClosed;
   
  if (co.fStateStartupWaitForTB)
  {
	  //bLaneClosed flag value must be opposite the true/false value issued for
      // TBLaneClosed,  or the call will be blocked.
      bLaneClosed = !co.fStateStartupToLaneClosed;  //Make sure LaneClosed is issued
      TBLaneClosed(co.fStateStartupToLaneClosed);
      nTimeOut = 0;
      CREATE_AND_DISPENSE(InProgress)(storeMode(),helpMode(),nTimeOut);
  }
  else
  {      
	  return createAnchorState(); 
  }
  //-RFC 986
}

//////////////////////////////////////
void SMOutOfService2Base::TBShow2x20(void)
{
  switch (nTBLastState)
  {
  case TB_CLOSED:                   // either initially signed of or just signed off
    // the initial timeout is 30, but it is changed to 29 after sign on
    if (nTimeOut==28)               // here after just signed off
    {
      nTimeOut = 1;                 // tests have completed
      break;
    }
    // still need to sign on
    //ps.Prompt(TESTING_TB);
    ps.Echo(TEST_SIGNON);
    DMSayPhrase(KEYPADVALID);
    TBStart();
    nTimeOut = 29;                  // start of sequence, signing on now
    trace(L8,_T("Wait SignOn"));
    break;
  case TB_READY:                    // just signed on
    //ps.Prompt(TESTING_TB);
    ps.Echo(TEST_SIGNOFF);
    DMSayPhrase(KEYPADVALID);
		if (nTimeOut==29)			// in sequence, just did sign on
		{
			nTimeOut = 28;          // sign on has completed, now signoff
		}
    TBFinish();
    trace(L8,_T("Wait SignOff"));
    break;
  }
	
  ps.Prompt(CleanUpMessage(csLastDisplay,true));
}

void SMOutOfService2Base::UnInitialize()
{
  // (*) SR898 ====================
  DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                     DM_TRICOLORLIGHT_OFF,
                     DM_TRICOLORLIGHT_NORMALREQUEST);
  
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("OutOfService2") );	// RFC 330050
}

SMStateBase* SMOutOfService2Base::createLevel1Message( LPCTSTR szMsg,
                                            PSTEXT pstMsg,
                                            PSTEXT prompt,
                                            long   value,
                                            long   device,
                                            bool   checkHealth,
                                            bool displayHTML)
{
    if(m_bDoLegacyDeviceErrs)
    {
        return SMCustomerModeState::createLevel1Message(szMsg,
                                                        pstMsg,
                                                        prompt,
                                                        value,
                                                        device,
                                                        checkHealth,
                                                        displayHTML);
    }

    CREATE_AND_DISPENSE(SystemMessageStartup)(szMsg,
                                              pstMsg,
                                              prompt,
                                              value,
                                              device,
                                              checkHealth, 
                                              displayHTML);
}

SMStateBase * SMOutOfService2Base::DMHandleStartupError(void)   // RFC 445940
{
    // If configured to use the old-style legacy device startup err handling.
    // then don't do anything here.
    if(m_bDoLegacyDeviceErrs)
    {
        return STATE_NULL;
    }

    // Else, handle device startup errors the new way.
    return SMCustomerModeState::DMHandleStartupError();
}

long SMOutOfService2Base::DMConfigureScaleDisplay(void)
{
    // If configured to use the old-style legacy device startup err handling.
    // then don't do anything here.
    if(m_bDoLegacyDeviceErrs)
    {
        return -1;
    }

    // If security has already been initialized,  If security failed to load
    // then we would not be here.
    if(SecMgr.SecMgrICreated)
    {
        return -1;
    }

    return SMCustomerModeState::DMConfigureScaleDisplay();
}

long SMOutOfService2Base::DMPrintVersion(void)                 // RFC 445940
{
    // If configured to use the old-style legacy device startup err handling.
    // then don't do anything here.
    if(m_bDoLegacyDeviceErrs)
    {
        return OPOS_SUCCESS;
    }

    // Else, handle device startup errors the new way.
    return SMCustomerModeState::DMPrintVersion();
}

/**
 * TAR 452531.
 * Delay loading of security manager until after the devices have loaded.
 */
long SMOutOfService2Base::SecMgrInitialize(void)
{
    // If configured to use the old-style legacy device startup err handling.
    // then don't do anything here.  
    if(m_bDoLegacyDeviceErrs)
    {
        return 1;
    }

    // If security has already been loaded, then don't do anything.
    if(SecMgr.SecMgrICreated)
    {
        trace(L4, _T("Security is already up.  Bypassing security init."));
        trace(L4, _T("> [SecMgrICreated=%d; SecMgrPCreated=%d]"), 
                     SecMgr.SecMgrICreated, SecMgr.SecMgrPCreated); 
        return 1;
    }

    // Otherwise, initialize security as we normally do.
    SecMgrPreInitialize();
    long lRc = SMCustomerModeState::SecMgrInitialize();

    if(lRc > 0)  // Only do post if init succeeded.
    {
        SecMgrPostInitialize();
    }

    return lRc;
}
