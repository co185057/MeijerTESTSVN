//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMHostOffLineBase.cpp
//
// TITLE: Class implementation for host offline state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMHostOffLine.h"
#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;   // Global DM object

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("HostOffLineBase")

IMPLEMENT_DYNCREATE(SMHostOffLineBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(HostOffLine)


DEFINE_TIMESTAMP

////////////////////////////
SMHostOffLineBase::SMHostOffLineBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////
SMStateBase *SMHostOffLineBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("HostOffLine") );	// RFC 330050
  ra.RequestEnableAssistMode(false);   
   
  fStateFreeze = true;		// stay in this state
  nTimeOut = 0;
  
  bIsFromHostOffline = true;

  dm.saveTriColorLightStatus();
  dm.saveLaneLightStatus();
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

  ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + ps.GetPSText(MSG_RA_HOSTOFFLINE, SCOT_LANGUAGE_PRIMARY));

  // send a DataNeeded Approval
  trace(L7, _T("+ra.OnRAPDataNeeded, Host offline\n"));
  cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("HostOffline"));
  m_View = DATANEEDED_HOSTOFFLINE;
  CString csSecInfo;
  csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),
	  ps.GetPSText(MSG_RA_HOSTOFFLINE, SCOT_LANGUAGE_PRIMARY), //Description
	  ps.GetPSText(RA_BUTTON_LANECLOSED, SCOT_LANGUAGE_PRIMARY), //Button
	  ps.GetPSText(RA_FIRSTLINE_LANECLOSED, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""), _T("")); //Status line
  ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
  trace(L7, _T("-ra.OnRAPDataNeeded, Host offline\n"));

  // Display the Auxilary lane closed screen
  DisplayAuxScreen(AUX_SCREEN_LANE_CLOSED, true);

  
  SMStateBase *ret = STATE(OperatorPasswordState)::Initialize();
  if (co.IsLoginBypassOn() && IsLoginBypassAllowed())
     showScreen(false);

  return ret;
}

/////////////////////////////////////
void SMHostOffLineBase::UnInitialize(void)
{
  fStateFreeze = false;
  bIsFromHostOffline = false;
  ra.OnNoRAPDataNeeded();
  dm.restoreTriColorLight();
  dm.restoreLaneLight();
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("HostOffLine") );	// RFC 330050
}

////////////////////////////////////////////////////
void SMHostOffLineBase::showScreen(bool passwordVerified)
{
	DMScannerDisable();

    saMsgFuncPtr = ps.SecurityMessageModal;

    ps.SetCMFrame(_T("HostOffline"));
 
	//turn red light off to stop page resend timer
	dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                  DM_TRICOLORLIGHT_OFF,
                  DM_TRICOLORLIGHT_NORMALREQUEST);
  	//then turn red light back on
	dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                  DM_TRICOLORLIGHT_ON,
                  DM_TRICOLORLIGHT_NORMALREQUEST);
 
    ps.SetCMLeadthruText(LTT_LANECLOSED);
    ps.ShowCMTBText(csTBMessage2Scot);	
    ps.ShowCMFrame(false);
}

//////////////////////////////////////////////////////////////
SMStateBase *SMHostOffLineBase::OnWtIncreaseNotAllowed()
{
	return VoidingWtIncrease(BES_HOSTOFFLINE, BES_HOSTOFFLINE);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMHostOffLineBase::OnWtDecrease() 
{
	return ThresholdViolWtDecrease(BES_HOSTOFFLINE, BES_HOSTOFFLINE);
}

////////////////////////////////////////////////////////////////
SMStateBase *SMHostOffLineBase::OnMatchedWt()
{
	// +218558 EAS motion sensor received in the wrong order must behave differently LPM103002
	DMSaySecurity( SEC_NONE, _T("blank.wav") );

	g_bWtExpected = false;	//We should never expect wt if we got a matched wt.
	g_lNumberOfWLDBEntriesThisItem = -1;	// reset the number of WLDB entries

    if (dm.fStateTakeawayBelt)
        return setAndCreateAnchorState(BES_TRANSPORTITEM);

	//219883 should not use g_bMatchedWtWaitingOnEAS for this LPM121002
	if (!m_bEASHappened)	// matched wt and the EAS event did not happen yet
	{
		trace(L2, _T("SMHostOffLineBase::OnMatchedWt, waiting for EAS motion sensor, returning to BagAndEAS (Bag Item)"));
		return setAndCreateAnchorState(BES_BAGANDEAS);
	}

	trace(L2, _T("bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;
	//-218558 End
	trace(L2, _T("SMHostOffLineBase::OnMatchedWt, returning to ScanAndBag"));
    return setAndCreateAnchorState(BES_SCANANDBAG);
}
