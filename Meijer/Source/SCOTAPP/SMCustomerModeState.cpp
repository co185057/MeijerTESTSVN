//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCustomerModeState.cpp
//
// TITLE: Base class for Customer Mode states
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"
#include "SMCustomerModeState.h"
#include "SMSystemMessage1.h"
#include "SMSystemMessage4.h"
#include "SMWaitForApproval.h"		// Wait for approval state	TAR122724
#include "sigreceiptdatafile.h"


#define COMP_ID ID_SM               // base state component
#define T_ID _T("CustomerModeState")

bool SMCustomerModeState::issuedSigCapError = false;

IMPLEMENT_DYNCREATE(SMCustomerModeState, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////
SMCustomerModeState::SMCustomerModeState()
{
  IMPLEMENT_TIMESTAMP

    if (!co.IsAttendantModeOn() && !co.IsInAssistMenus() && !fStateAssist) //SR819: SSCOP-366 & 368 
    {
        g_bValidHeadCashier = false;
    }
}

void SMCustomerModeState::TurnOffSigCap(void) 
{
    trace(L6, _T("+TurnOffSigCap()"));
	if (bSigCapInProgress)
	{
		gpSCOTSigCap->EndCapture() ;
		bSigCapInProgress = false ;
        // SR875
        DisplayVerifoneForm(_T("FA_WELC")); //Default form 
    // Nasty 5992 SO bug means I have to disable the Forms interface before
    // calling BeginCapture, and enable it after sigcap is complete (or cancelled).
    // This SHOULD BE TAKEN OUT once the OPOS SO is fixed.  ncr_barn - 2/8/2000

    // Okay, it is now fixed in the 3/9/2000 version of ncr5992.dll
    //if (gpSCOTAuxDisp)
      //gpSCOTAuxDisp->Enable();
	}
    trace(L6, _T("-TurnOffSigCap()"));
}

SMStateBase *SMCustomerModeState::TurnOnSigCap(HWND hDispWnd) 
{
    trace(L6, _T("SMCustomerModeState::TurnOnSigCap(HWND %d)"), hDispWnd);
	if (!bSigCapInProgress)
	{
    // Nasty 5992 SO bug means I have to disable the Forms interface before
    // calling BeginCapture, and enable it after sigcap is complete (or cancelled).
    // This SHOULD BE TAKEN OUT once the OPOS SO is fixed.  ncr_barn - 2/8/2000

    // Okay, it is now fixed in the 3/9/2000 version of ncr5992.dll
    //if (gpSCOTAuxDisp)
    //  gpSCOTAuxDisp->Disable();

        // SR875
        LPCTSTR szCaptureForm = NULL;
        if(IsVerifoneConfigured())
        {
            szCaptureForm = _T("FA_SIGN");
        }
        else
        {
            szCaptureForm = NULL;
        }
        
        if (0 != gpSCOTSigCap->BeginCapture(szCaptureForm, hDispWnd))        // start to capture the signature(enable, setnotify...)
		{
			// (*) SR898 ====================
            DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                               DM_TRICOLORLIGHT_BLINK_1HZ,
                               DM_TRICOLORLIGHT_NORMALREQUEST);

			//begin TAR122724	LPM093099  The system msg templates 1-3 are not to be 
			//  used for the approval needed screen.  Only the templates 4-6 will emulate 
            //  an approval needed screen and then display the appropriate system msg 1-3
            cDataEntry.FromConfigData(co.csDataEntryINIFile, "DeviceFailure");
            m_View = DATANEEDED_DEVICEERROR;
            CString csWork = ps.GetPSText(MSG_SIGCAP_DEVICE_FAILURE, SCOT_LANGUAGE_PRIMARY);
            CString csSecInfo;
            csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),csWork,ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), "", "", "", "");
            ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
            trace(L7, _T("-ra.OnRAPDataNeeded"));
			if (issuedSigCapError)
			{
				CREATE_AND_DISPENSE(SystemMessage1)(NULL,		// char *message
										MSG_SIGCAP_DEVICE_FAILURE,
										PS_NOTEXT,				// no prompt
										0,						// no value
										DMCLASS_SIGCAP,			// DM device
										true);					// DM check health after
			}
			else
			{
				issuedSigCapError = true;
				CREATE_AND_DISPENSE(SystemMessage4)(NULL,		// char *message
										MSG_SIGCAP_DEVICE_FAILURE,
										PS_NOTEXT,				// no prompt
										0,						// no value
										DMCLASS_SIGCAP,			// DM device
										true);					// DM check health after
			}
		}
		else 
		{
			issuedSigCapError = false;
			bSigCapInProgress = true ;

      // Nasty 5992 SO bug means I have to disable the Forms interface before
      // calling BeginCapture, and enable it after sigcap is complete (or cancelled).
      // This SHOULD BE TAKEN OUT once the OPOS SO is fixed.  ncr_barn - 2/8/2000

      // Okay, it is now fixed in the 3/9/2000 version of ncr5992.dll
      //if (gpSCOTAuxDisp)
      //  gpSCOTAuxDisp->Enable();
		}
	}
  
  m_bNeedToDisplayAuxScreen = false; // SIG Cap uses AuxDisp so don't overwrite screen

	return STATE_NULL;
}

// Assist menu states may be run legally from the RAP station
bool SMCustomerModeState::PSRemoteLegalForState()
{
	return (co.IsInAssistMenus() && PSAssistMenuLegalForState());
}

SMStateBase *SMCustomerModeState::PSParse(const MessageElement *me)
{
    return STATE(State)::PSParse(me);
}