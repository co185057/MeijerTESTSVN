//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOutOfService2.cpp
//
// TITLE: Class implementation for out of service state, waiting for TS testing
//
//
// AUTHOR:    Peter Denhoed, Updated DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMOutOfService2.h"        // MGV this state
#include "SMLaneClosed.h"
#include "SMSmAuthorization.h"
#include "TransbrokerSSF.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("OutOfService2")

IMPLEMENT_DYNCREATE(SMOutOfService2, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

#define MAX_POS_ATTEMPTS 300

//////////////////////////////////////////
SMOutOfService2::SMOutOfService2()
{
  IMPLEMENT_TIMESTAMP
	  fServerStarted = false;
}

SMStateBase * SMOutOfService2::Initialize(void)
{
    setAnchorState(BES_OUTOFSERVICE2);
	fStateFreeze = true;              // stay in this state no matter what!
	
	dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
		DM_TRICOLORLIGHT_ON,
		DM_TRICOLORLIGHT_NORMALREQUEST);
	
	ps.SetDisplayWMFlag(true); //load W&M information when scot starts - improve screen display
	
	ps.SetCMFrame(_T("OutOfService2"));
	
	DMScannerDisable();    //POS215486 
	//ps.Echo(TESTING_TB);
	ps.Echo(TB_LOADING);  //RFQ 986 - use better "loading" message
	DMSayPhrase(KEYPADVALID);
	
	nTimeOut = 2;                    
	ps.ShowCMFrame();
	
	LoadMeijerBiometricOptions();
	LoadEverseenOptions();	//CRD 313028
	
	// now we wait for ts 2x20 events or the timer ticks to check our progress
	return SMOutOfService2Base::Initialize();                // initiate first display change
}

void SMOutOfService2::UnInitialize(void)
{
    SMOutOfService2Base::UnInitialize();
}

SMStateBase  *SMOutOfService2::TimedOut()
{
	trace(L6, _T("*_* SMOutOfService2::TimedOut") );
	//determine if POS is ready to go if return to lane close. else stay here
	static int iPOSCount = 0; 

 	
	if(iPOSCount++ >= MAX_POS_ATTEMPTS)
	{
        fStateFreeze = false;
		return  createLevel1Message(
								  ps.GetPSText(9092, SCOT_LANGUAGE_PRIMARY),           // char *message
								  PS_NOTEXT,        // no canned message
								  PS_NOTEXT,        // no prompt
								  0,                // no value
								  -1,               // device reporting the error or -1
								  false,			// T/F need to issue CheckHealth after operator clears problem
								  false);    // SSCOI-45873       
           
	}

	CString csStr;	
	long lPosErr = SendTBCommand(TBCMD_GET_POS_ERROR_NUM, &csStr);

	trace(L6, _T("*_* SMOutOfService2::TimedOut() lPosErr = %d"), lPosErr );
	if(lPosErr)
    {   
		fStateFreeze = false;
        return  createLevel1Message(
								  csStr,            // char *message
								  PS_NOTEXT,        // no canned message
								  PS_NOTEXT,        // no prompt
								  0,                // no value
								  -1,               // device reporting the error or -1
								  false,			// T/F need to issue CheckHealth after operator clears problem
								  false);			// SSCOI-45873
    }

    if(SendTBCommand(TBCMD_POS_SIGNEDIN, NULL) == TB_SUCCESS_IMMEDIATE)
	{
		dm.RemoteRelay(DM_MISC_STATE_ON);	// turn on lane light
		fStateFreeze = false;

        //SMarks. this may seem odd to return to authorization instead of
        //attract but this is what the base naturally does when we exit from
        //lane closed.  The system goes back to authorization and returns psbutton8
        //return to shopping.  Changing the SSF override to operate more like the
        //base.
        m_bIsFromLaneClosed = true;
        setAnchorState(BES_START);
        RETURNSTATE(SmAuthorization);
		//RETURNSTATE(Attract);
	}

    //pos13750 Move the start of RemoteServer in here so that it will execute no matter what
    // if this is the first time connecting - start the server
	if( !fServerStarted )
	{
		fServerStarted = SUCCEEDED(ps.StartRemoteServer());
        trace(L6, _T("Start Remote Server") );
	}
    //e pos13750 Move the start of RemoteServer in here so that it will execute no matter what

	if(SendTBCommand(TBCMD_POS_READY, NULL) == TB_SUCCESS_IMMEDIATE)
	{
        // pos13750 - Remove the call to startRemoteServer from here
        /*
		// if this is the first time connecting - start the server
		if( !fServerStarted )
		{
			fServerStarted = SUCCEEDED(ps.StartRemoteServer());
		}*/
        // e - pos13750 - Remove the call to startRemoteServer from here
		trace(L6, _T("*_* POS Ready"));
		fStateFreeze = false;
		RETURNSTATE(LaneClosed);
	}
	else
	{
		//return SMOutOfService2Base::TimedOut();
		return STATE_NULL;
	}
}

long SMOutOfService2::DMConfigureScaleDisplay(void)
{
    return SMOutOfService2Base::DMConfigureScaleDisplay();
}

void SMOutOfService2::TBShow2x20(void)
{
    SMOutOfService2Base::TBShow2x20();
}

long SMOutOfService2::SecMgrInitialize(void)
{
    return SMOutOfService2Base::SecMgrInitialize();
}

SMStateBase * SMOutOfService2::createLevel1Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth,
                                           bool displayHTML)
{
    return SMOutOfService2Base::createLevel1Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML);
}

SMStateBase * SMOutOfService2::DMHandleStartupError(void)
{
    return SMOutOfService2Base::DMHandleStartupError();
}

long SMOutOfService2::DMPrintVersion(void)
{
    return SMOutOfService2Base::DMPrintVersion();
}
