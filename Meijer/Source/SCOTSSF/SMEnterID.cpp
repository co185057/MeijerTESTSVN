//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterID.CPP
//
// TITLE: Class implementation for EnterID state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMEnterID.h"            // This state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("EnterID")

IMPLEMENT_DYNCREATE(SMEnterID, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMEnterID::SMEnterID()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMEnterID::Initialize(void)
{
    return SMEnterIDBase::Initialize();
}

void SMEnterID::UnInitialize(void)
{
    SMEnterIDBase::UnInitialize();
}

SMStateBase * SMEnterID::PSNumericKey(const long lKey)
{
    return SMEnterIDBase::PSNumericKey(lKey);
}

SMStateBase * SMEnterID::PSClearKey(void)
{
    return SMEnterIDBase::PSClearKey();
}

SMStateBase * SMEnterID::PSEnterKey(void)
{
    return SMEnterIDBase::PSEnterKey();
}

SMStateBase * SMEnterID::PSButtonGoBack(void)
{
    return SMEnterIDBase::PSButtonGoBack();
}
