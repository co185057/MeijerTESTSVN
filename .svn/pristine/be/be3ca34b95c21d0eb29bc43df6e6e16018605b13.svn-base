//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMSelectCardType.CPP
//
// TITLE:    Class implementation for Select Card Type State
//
// $Header:
//
// AUTHOR:   Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMEnterAlphaNumericInfo.h"       // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SMEnterAlphaNumericInfo")

IMPLEMENT_DYNCREATE(SMEnterAlphaNumericInfo, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMEnterAlphaNumericInfo::SMEnterAlphaNumericInfo()
{
  IMPLEMENT_TIMESTAMP 
}

SMStateBase * SMEnterAlphaNumericInfo::Initialize(void)
{
    return SMEnterAlphaNumericInfoBase::Initialize();
}

void SMEnterAlphaNumericInfo::UnInitialize(void)
{
    SMEnterAlphaNumericInfoBase::UnInitialize();
}

bool SMEnterAlphaNumericInfo::PSAssistMenuLegalForState()
{
    return SMEnterAlphaNumericInfoBase::PSAssistMenuLegalForState();
}

SMStateBase * SMEnterAlphaNumericInfo::PSButton1(void)
{
    return SMEnterAlphaNumericInfoBase::PSButton1();
}

SMStateBase * SMEnterAlphaNumericInfo::PSNumericKey(const long lKey)
{
    return SMEnterAlphaNumericInfoBase::PSNumericKey(lKey);
}

SMStateBase * SMEnterAlphaNumericInfo::PSAlphaNumericKey(const long lKey)
{
    return SMEnterAlphaNumericInfoBase::PSAlphaNumericKey(lKey);
}

SMStateBase * SMEnterAlphaNumericInfo::PSAlphaKey(const long lKey)
{
    return SMEnterAlphaNumericInfoBase::PSAlphaKey(lKey);
}

SMStateBase * SMEnterAlphaNumericInfo::PSEnterKey(void)
{
    return SMEnterAlphaNumericInfoBase::PSEnterKey();
}

SMStateBase * SMEnterAlphaNumericInfo::PSClearKey(void)
{
    return SMEnterAlphaNumericInfoBase::PSClearKey();
}

SMStateBase * SMEnterAlphaNumericInfo::PSSpaceKey(void)
{
    return SMEnterAlphaNumericInfoBase::PSSpaceKey();
}

SMStateBase * SMEnterAlphaNumericInfo::DMScanner(void)
{
    return SMEnterAlphaNumericInfoBase::DMScanner();
}

SMStateBase * SMEnterAlphaNumericInfo::PSButtonGoBack(void)
{
    return SMEnterAlphaNumericInfoBase::PSButtonGoBack();
}
