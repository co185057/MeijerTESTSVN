//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMEnterPin.CPP
//
// TITLE:    Class implementation for entering PIN state
//
//
// AUTHOR:   Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMEnterPin.h"             // MGV this state
#define COMP_ID ID_SM               // base state component
#define T_ID    _T("EnterPin")

IMPLEMENT_DYNCREATE(SMEnterPin, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////
SMEnterPin::SMEnterPin(const TBTENDERTYPE nTyp, const bool fCshBck)
: SMEnterPinBase(nTyp, fCshBck)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMEnterPin::SMEnterPin()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMEnterPin::Deliver(const TBTENDERTYPE nTyp, const bool fCshBck)
{
  SMEnterPinBase::Deliver(nTyp, fCshBck);

  return this;
}

bool SMEnterPin::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMEnterPinBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMEnterPin::Initialize(void)
{
    return SMEnterPinBase::Initialize();
}

void SMEnterPin::UnInitialize(void)
{
    SMEnterPinBase::UnInitialize();
}

SMStateBase * SMEnterPin::TimedOut(void)
{
    return SMEnterPinBase::TimedOut();
}

SMStateBase * SMEnterPin::OnWtIncreaseNotAllowed(void)
{
    return SMEnterPinBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMEnterPin::OnWtDecrease(void)
{
    return SMEnterPinBase::OnWtDecrease();
}

SMStateBase * SMEnterPin::PSButtonHelp(void)
{
    return SMEnterPinBase::PSButtonHelp();
}

SMStateBase * SMEnterPin::PSButtonGoBack(void)
{
    return SMEnterPinBase::PSButtonGoBack();
}

SMStateBase * SMEnterPin::PSNumericKey(const long lKey)
{
    return SMEnterPinBase::PSNumericKey(lKey);
}

SMStateBase * SMEnterPin::DMEncryptorSecured(void)
{
    return SMEnterPinBase::DMEncryptorSecured();
}

SMStateBase * SMEnterPin::DMEncryptorCanceled(void)
{
    return SMEnterPinBase::DMEncryptorCanceled();
}

SMStateBase * SMEnterPin::DMEncryptorCompleted(void)
{
    return SMEnterPinBase::DMEncryptorCompleted();
}

SMStateBase * SMEnterPin::DMEncryptorPinIsTooShort(void)
{
    return SMEnterPinBase::DMEncryptorPinIsTooShort();
}

SMStateBase * SMEnterPin::PSClearKey(void)
{
    return SMEnterPinBase::PSClearKey();
}
