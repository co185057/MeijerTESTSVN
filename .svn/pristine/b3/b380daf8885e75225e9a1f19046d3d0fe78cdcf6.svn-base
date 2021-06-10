//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOutOfService2Base.cpp
//
// TITLE: Class implementation for out of service state, waiting for TS testing
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMOutOfService2.h"     

#include "DMProcedures.h"           // DM procedures
DLLEXPORT extern DMProcedures dm;   // Global DM object

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("OutOfService2Base")

IMPLEMENT_DYNCREATE(SMOutOfService2Base, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(OutOfService2)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMOutOfService2Base::SMOutOfService2Base()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////
SMStateBase  *SMOutOfService2Base::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("OutOfService2") );	// RFC 330050
  setAnchorState(BES_OUTOFSERVICE2);
  fStateFreeze = true;              // stay in this state no matter what!
 
  dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                      DM_TRICOLORLIGHT_ON,
                      DM_TRICOLORLIGHT_NORMALREQUEST);

  ps.SetDisplayWMFlag(true); //load W&M information when scot starts - improve screen display

  ps.SetCMFrame(_T("OutOfService2"));

  
//  ps.Echo(TESTING_TB);
  ps.Echo(TB_LOADING);  //RFQ 986 - use better "loading" message
  DMSayPhrase(KEYPADVALID);
	
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
      return STATE_NULL;      	  
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
  dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                      DM_TRICOLORLIGHT_OFF,
                      DM_TRICOLORLIGHT_NORMALREQUEST);
  
  if (co.fStateDualLanguage)
  {
     ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT);
  }
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("OutOfService2") );	// RFC 330050
}
