//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmEnterBirthdateBase.CPP 
//
// TITLE: Class implementation for store mode Enter Birthdate state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSmEnterBirthdate.h"     // This state
#include "SMSmAuthorization.h"      // MGV auth state
#include "SMSmRestrictedItems.h" 

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmEnterBirthdateBase")

IMPLEMENT_DYNCREATE(SMSmEnterBirthdateBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmEnterBirthdate)

DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmEnterBirthdateBase::SMSmEnterBirthdateBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmEnterBirthdateBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmEnterBirthdate") );	// RFC 330050
    csBirthDate = _T("");                 // Birthdate input by user
    // NewUI
	ps.SetFrame(_T("SmEnterBirthdate"));
    // NewUI
	
	CString csWork; 

	if (co.fLocaleDateSettingNotFromRegionalSetting)
	{
		csWork = ps.GetPSText(LTT_BIRTHDATE) +  _T("\n") + ps.GetPSText(LTT_MMDDYY);
		ps.SetLeadthruText(csWork,_T(""));
	}
	else
	{
		csWork = ps.GetPSText(LTT_BIRTHDATE) + _T("\n") + ps.DateFormat();
		ps.SetLeadthruText(csWork , _T(""));
	}

    ps.ShowSMMNumericKeypad(true);
    // allow the EchoInput() func to accept up to max digits
    // BeginCodeLocale
    int nNoTotalDigit = 4; // By definition: 2 digits for month and for days
    if (co.nLocaleNoCenturyDigit==0) nNoTotalDigit += 2;
    if (co.nLocaleNoCenturyDigit==1) nNoTotalDigit += 4;  
     ps.AllowInput(nNoTotalDigit,false,false);
    // EndCodeLocale
	ps.ShowFrame();
    return STATE_NULL;                // stay in SmEnterBirthdateBase state
}

void SMSmEnterBirthdateBase::UnInitialize()
{
  ps.PSHideSMMNumericKeypad();
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmEnterBirthdate") );	// RFC 330050
}

/////////////////////////////////////////////
// PSButton8 - Cancel
/////////////////////////////////////////////
SMStateBase  *SMSmEnterBirthdateBase::PSButton8(void)
{
  //ps.PSHideSMMNumericKeypad();
  RETURNSTATE(SmRestrictedItems)
}

/////////////////////////////////////////////
// PSClearKey - Clear all input (on the pinpad)
//////////////////////////////////////////////
SMStateBase * SMSmEnterBirthdateBase::PSClearKey()
{
  PSClearAllInput();
  return STATE_NULL;
}


//////////////////////////////////////////////
// PSEnterKey
//////////////////////////////////////////////
SMStateBase  *SMSmEnterBirthdateBase::PSEnterKey(void)
{
  SMStateBase *tempState = VerifyBirthDate();
  PSClearAllInput();
  return tempState;

}


