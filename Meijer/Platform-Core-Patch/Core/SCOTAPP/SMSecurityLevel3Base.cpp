//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecurityLevel3Base.cpp
//
// TITLE: Implementation File for Level 3 Security agent State.
//
// Notes: This state is used for manager's override confirmation only
//
//
// AUTHOR:  DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>								  // MS MFC always needed first

#include "Common.h"								  // MGV common includes

#include "SMState.h"							  // MGV base state
#include "SMSecurityLevel3.h"       // MGV security level msg 3 state
#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;             // global DM object

#define COMP_ID ID_SM							  // base state component
#define T_ID _T("SecurityLevel3Base")

IMPLEMENT_DYNCREATE(SMSecurityLevel3Base, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////////////////////////////
SMSecurityLevel3Base::SMSecurityLevel3Base( CString csBar, CString csPop )
: csBarMsg(csBar), csPopUpMsg(csPop)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSecurityLevel3Base::SMSecurityLevel3Base()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSecurityLevel3Base::Deliver( CString csBar, CString csPop )
{
  csBarMsg = csBar; csPopUpMsg = csPop;

  return this;
}

bool SMSecurityLevel3Base::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, both strings
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMSecurityLevel3Base::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver(DLParamCString(dlParameterList),    // csBar
                DLParamCString(dlParameterList));   // csPop
        return true;
    }
    else
    {
        trace(L2, _T("+SMSecurityLevel3Base::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}


///////////////////////////////////////////
SMStateBase  *SMSecurityLevel3Base::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SecurityLevel3") );	// RFC 330050
  dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                      DM_TRICOLORLIGHT_BLINK_1HZ,
                      DM_TRICOLORLIGHT_NORMALREQUEST);

  return STATE(OperatorPasswordState)::Initialize();
}

////////////////////////////////////////////////////////
void SMSecurityLevel3Base::showScreen(bool passwordVerified)
{
  ps.SetPSText(PS_WORK1,csBarMsg);
  PSTEXT button2 = ( passwordVerified ?  BTT_CONTINUE : PS_NOTEXT);
#ifndef NewUI
  ps.SetCMFrame(DEFAULT_FRAME);
#endif
  ps.SetCMBackground(BMP_ERRMSGLEVEL3);
  ps.SetCMMascotImage(BMP_MASCOT_ERRMSGLEVEL3);
  ps.SetCMLeadthruText(PS_WORK1);
#ifdef NewUI
  ps.SetButtonText(_T("CMButton1"), PS_NOTEXT);
  ps.SetButtonText(_T("CMButton2"), PS_NOTEXT);
  ps.SetButtonText(_T("CMButton3"), button2);
  ps.SetButtonText(_T("CMButton4"), PS_NOTEXT);
#else
  ps.SetCMButton1Text(PS_NOTEXT);
  ps.SetCMButton2Text(PS_NOTEXT);
  ps.SetCMButton3Text(button2);
  ps.SetCMButton4Text(PS_NOTEXT);
#endif
  if (passwordVerified)
	  ps.SetCMGobackButtonVisible(false);
  else
	  ps.SetCMGobackButtonVisible(true);
 
  ps.ShowCMFrame();
  ps.ShowCMTotal(false ,true);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  if (_tcsclen(csPopUpMsg)>0)
  {
	ps.Message12(csPopUpMsg, true);
  }
}

//////////////////////////////////////////
// Button 3 - Continue
SMStateBase  *SMSecurityLevel3Base::PSButton3(void)
{
  return STATE_RETURN;
}

////////////////////////////////////////////
// Cancel
SMStateBase  *SMSecurityLevel3Base::PSCancelKey(void)
{
  if (parentClassCurrentlyHandlingCancelKey())
    return STATE(OperatorPasswordState)::PSCancelKey();
  else
  {
    DMSayPhrase(KEYPADVALID);
    return PSButton3();
  }
}
///////////////////////////////////////////
SMStateBase  *SMSecurityLevel3Base::PSButtonGoBack(void)// cancel
{
  return PSButton3();			   // usually same as cancelkey
}

//////////////////////////////////////////////////////////////
SMStateBase *SMSecurityLevel3Base::OnWtIncreaseNotAllowed() //tar 197857
{
	return VoidingWtIncrease(BES_SECURITYLEVEL3, BES_SECURITYLEVEL3);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecurityLevel3Base::OnWtDecrease() //tar 197857
{
	return ThresholdViolWtDecrease(BES_SECURITYLEVEL3, BES_SECURITYLEVEL3);
}

// + RFC 330050
void SMSecurityLevel3Base::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SecurityLevel3") );	
}
// - RFC 330050
