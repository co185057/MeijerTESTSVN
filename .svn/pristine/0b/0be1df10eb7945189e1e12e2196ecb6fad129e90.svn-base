//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateCU.CPP
//
// TITLE: CU SMState functions
//
//
// AUTHOR:  Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"
#include "PSProcedures.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmStateCU")

CCustomerInfo myCustInfo;
DLLEXPORT CCustomerInfo &custInfo = myCustInfo;

SMStateBase *SMState::CUParse(MessageElement *me)
{
    return SMStateBase::CUParse(me);
}

SCOT_LANGUAGE_TYPE SMState::GetPreferredLanguage(bool bOrigValue)
{
    return SMStateBase::GetPreferredLanguage(bOrigValue);
}

void SMState::SetPreferredLanguage(SCOT_LANGUAGE_TYPE langChosen, CString csSelectionMethod)
{
    SMStateBase::SetPreferredLanguage(langChosen, csSelectionMethod);
}

bool SMState::NeedPreferenceUpdatePermission()
{
    return SMStateBase::NeedPreferenceUpdatePermission();
}

void SMState::PreferencesReceived()
{
    SMStateBase::PreferencesReceived();
}

bool SMState::LoyaltyCheck(CString csLookupType, CString csLookupData)
{
    return SMStateBase::LoyaltyCheck(csLookupType, csLookupData);
}

bool SMState::IsPersonalizedLanguageChange()
{
    return SMStateBase::IsPersonalizedLanguageChange();
}

bool SMState::IsPersonalizedVolumeChange()
{
    return SMStateBase::IsPersonalizedVolumeChange();
}

void SMState::PreferenceUpdate()
{
    SMStateBase::PreferenceUpdate();
}

void SMState::SetPersonalTheme(CString csForRedrawLock)
{
    SMStateBase::SetPersonalTheme(csForRedrawLock);
}

SMStateBase *SMState::OnPLAValidation()
{
    return SMStateBase::OnPLAValidation();
}

SMStateBase *SMState::OnPLALookUp()
{
    return SMStateBase::OnPLALookUp();
}

SMStateBase *SMState::OnPLAHandInView()
{
    return SMStateBase::OnPLAHandInView();
}

SMStateBase *SMState::OnPLAHandInViewAfterRetry()
{
    return SMStateBase::OnPLAHandInViewAfterRetry();
}

SMStateBase *SMState::OnPLATransition()
{
    return SMStateBase::OnPLATransition();
}

SMStateBase *SMState::OnLookupComplete (long byteCount)
{
    return SMStateBase::OnLookupComplete ( byteCount);
}

SMStateBase *SMState::OnPersonalizationOptionsReceived()
{
    return SMStateBase::OnPersonalizationOptionsReceived();
}

//#ifdef _SSCOMOBILE
SMStateBase *SMState::OnMobileTransactionStart(CString csTransactionInfo)
{
    return SMStateBase::OnMobileTransactionStart(csTransactionInfo);
}
//#endif // _SSCOMOBILE

