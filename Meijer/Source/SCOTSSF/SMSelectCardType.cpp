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
#include "SMSelectCardType.h"       // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SelectCardType")

IMPLEMENT_DYNCREATE(SMSelectCardType, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSelectCardType::SMSelectCardType()
{
  IMPLEMENT_TIMESTAMP 
}

SMStateBase * SMSelectCardType::Initialize(void)
{
    return SMSelectCardTypeBase::Initialize();
}

void SMSelectCardType::UnInitialize(void)
{
    SMSelectCardTypeBase::UnInitialize();
}

SMStateBase * SMSelectCardType::OnWtDecrease(void)
{
    return SMSelectCardTypeBase::OnWtDecrease();
}

SMStateBase * SMSelectCardType::PSButton1(void)
{
    return SMSelectCardTypeBase::PSButton1();
}

SMStateBase * SMSelectCardType::PSButton2(void)
{
    return SMSelectCardTypeBase::PSButton2();
}

SMStateBase * SMSelectCardType::PSButton3(void)
{
    return SMSelectCardTypeBase::PSButton3();
}

SMStateBase * SMSelectCardType::PSButton4(void)
{
    return SMSelectCardTypeBase::PSButton4();
}

SMStateBase * SMSelectCardType::PSCancelKey(void)
{
    return SMSelectCardTypeBase::PSCancelKey();
}

SMStateBase * SMSelectCardType::PSButtonGoBack(void)
{
    return SMSelectCardTypeBase::PSButtonGoBack();
}

SMStateBase * SMSelectCardType::SAEventOccurred(const long lEvent)
{
    return SMSelectCardTypeBase::SAEventOccurred(lEvent);
}
