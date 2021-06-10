//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterPassword.CPP
//
// TITLE: Class implementation for Enter Password state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMEnterPassword.h"            // This state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("EnterPassword")

IMPLEMENT_DYNCREATE(SMEnterPassword, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMEnterPassword::SMEnterPassword()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMEnterPassword::Initialize(void)
{
    return SMEnterPasswordBase::Initialize();
}

void SMEnterPassword::UnInitialize(void)
{
    SMEnterPasswordBase::UnInitialize();
}

SMStateBase * SMEnterPassword::PSNumericKey(const long lKey)
{
    return SMEnterPasswordBase::PSNumericKey(lKey);
}

SMStateBase * SMEnterPassword::PSClearKey(void)
{
    return SMEnterPasswordBase::PSClearKey();
}

SMStateBase * SMEnterPassword::PSEnterKey(void)
{
    return SMEnterPasswordBase::PSEnterKey();
}

SMStateBase * SMEnterPassword::PSButtonGoBack(void)
{
    return SMEnterPasswordBase::PSButtonGoBack();
}
