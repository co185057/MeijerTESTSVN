//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSavePreferencesBase.cpp
//
// TITLE: Class implementation for Save Preferences State
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header
#include <afxpriv.h>                // for USES_CONVERSION
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSavePreferences.h"      


#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SavePreferencesBase")

IMPLEMENT_DYNCREATE(SMSavePreferencesBase, CObject)// MFC Runtime class/object information



DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSavePreferencesBase::SMSavePreferencesBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////
SMStateBase  *SMSavePreferencesBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SavePreferences") );	// RFC 330050
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_PERSONALIZATION_SAVE_PREF, SCOT_LANGUAGE_PRIMARY));

  ps.SetCMFrame(_T("SavePreferences"));

  CString csWork;

  csWork.Format(ps.GetPSText(MSG_PERSONALIZATION_SAVE_PREF), 
                ps.GetPSText(MSG_LANGUAGENAME),
	            //co.language[m_customerLanguage].csName,
                ps.GetPSText(MSG_LOYALTY_CARD));

  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.Message12(csWork,true);

//  DMSayPhrase(????);
  ps.ShowCMFrame();
  return STATE_NULL;                // stay in SavePreferences state
}

//////////////////////////////////////
SMStateBase  *SMSavePreferencesBase::PSButton1(void) // Yes, Save Preferences
{
  ps.HideButtons();
 
  custInfo.SetPreference(_T("SavePreferences"),_T("Y"));

  return createAnchorState();
}

//////////////////////////////////////
SMStateBase  *SMSavePreferencesBase::PSButton2(void) // no, Dont save preferences
{

  custInfo.SetPreference(_T("SavePreferences"),_T("N"));

  return createAnchorState();

}

// + RFC 330050
void SMSavePreferencesBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SavePreferences") );	
}
// - RFC 330050
