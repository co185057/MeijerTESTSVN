//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSavePreferencesBase.cpp
//
// TITLE: Class implementation for Save Preferences State
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#include <afxpriv.h>                // for USES_CONVERSION
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSavePreferences.h"      
#include "VolumeControl.h"          // SR815.5 Personalized Volume 

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
  
	//+SR815.5 Personalized Volume, added new preference messages.
    UpdateStateName(ps.GetPSText(LTT_SAVE_PREFERENCES, SCOT_LANGUAGE_PRIMARY));

    ps.SetCMFrame(_T("SavePreferences"));
    ps.SetLeadthruText(LTT_SAVE_PREFERENCES);
    CString csMsg;
    if( IsPersonalizedLanguageChange() && IsPersonalizedVolumeChange() )
    {
        csMsg = ps.GetPSText(MSG_SAVE_PREFERENCES) + MsgPreferredLanguage() + MsgPreferredVolume();
    }
    else if ( IsPersonalizedLanguageChange() )
    {
        csMsg = ps.GetPSText(MSG_SAVE_PREFERENCES) + MsgPreferredLanguage();
    }
    else if ( IsPersonalizedVolumeChange() )
    {
        csMsg = ps.GetPSText(MSG_SAVE_PREFERENCES) + MsgPreferredVolume();
    }
    else
    {
        csMsg = (_T(""));
        trace(L6, _T("SMSavePreferencesBase::Initialize() - Preferences are not modified."));
    }
    ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
    ps.Message12(csMsg,true);
    //-SR815.5 Personalized Volume, added new preference messages.

//  DMSayPhrase(????);
  ps.ShowCMFrame();
  return STATE_NULL;                // stay in SavePreferences state
}

//////////////////////////////////////
SMStateBase  *SMSavePreferencesBase::PSButton1(void) // Yes, Save Preferences
{
  ps.HideButtons();
 
  custInfo.SetDataString(_T("SavePreferences"),_T("Y"));

  return createAnchorState();
}

//////////////////////////////////////
SMStateBase  *SMSavePreferencesBase::PSButton2(void) // no, Dont save preferences
{

  custInfo.SetDataString(_T("SavePreferences"),_T("N"));

  return createAnchorState();

}

// + RFC 330050
void SMSavePreferencesBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SavePreferences") );	
}
// - RFC 330050

//+SR815.5 Personalized Volume 
CString SMSavePreferencesBase::MsgPreferredLanguage()
{
    CString csMsgLang;

    csMsgLang.Format( ps.GetPSText(MSG_PREFERRED_LANGUAGE), ps.GetPSText(MSG_LANGUAGENAME) );

    return csMsgLang;
}

CString SMSavePreferencesBase::MsgPreferredVolume()
{
    CString csMsgVol;
    int nCurrentVolume, nMaxVolume;
    int nVolumePercent; //tar 454494


    try
    {
        CVolumeControl *pVolumeControl = new CVolumeControl();        
        nCurrentVolume = pVolumeControl->GetVolumeLevel();
        nMaxVolume = pVolumeControl->GetMaxVolumeLevel();
        nVolumePercent = (nCurrentVolume * 100)/nMaxVolume;   //tar 454494

        delete pVolumeControl;
        pVolumeControl = NULL;
    }
    catch( CException *e )
    {
        _TCHAR szError[255] = {NULL};
		e->GetErrorMessage(szError, 255);
		trace(L6, _T("SMStateBase::IsPersonalizedVolumeChange - Got an exception! Error text: %s"), szError);
		e->Delete();
    }
    csMsgVol.Format( ps.GetPSText(MSG_PREFERRED_VOLUME), nVolumePercent);

    return csMsgVol;
}
//-SR815.5 Personalized Volume 
