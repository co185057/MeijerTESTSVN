//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterCouponValue.CPP
//
// TITLE: Class implementation for enter Coupon Value state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMEnterCouponValue.h"     // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("EnterCouponValue")

IMPLEMENT_DYNCREATE(SMEnterCouponValue, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMEnterCouponValue::SMEnterCouponValue()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMEnterCouponValue::Initialize(void)
{
    return SMEnterCouponValueBase::Initialize();
}

void SMEnterCouponValue::UnInitialize(void)
{
    SMEnterCouponValueBase::UnInitialize();
}

bool SMEnterCouponValue::PSAssistMenuLegalForState()
{
    return SMEnterCouponValueBase::PSAssistMenuLegalForState();
}

SMStateBase * SMEnterCouponValue::PSButton1(void)
{
    return SMEnterCouponValueBase::PSButton1();
}

SMStateBase * SMEnterCouponValue::PSButton3(void)
{
    return SMEnterCouponValueBase::PSButton3();
}

SMStateBase * SMEnterCouponValue::PSNumericKey(const long lKey)
{
    return SMEnterCouponValueBase::PSNumericKey(lKey);
}

SMStateBase * SMEnterCouponValue::PSEnterKey(void)
{
    return SMEnterCouponValueBase::PSEnterKey();
}

SMStateBase * SMEnterCouponValue::PSClearKey(void)
{
    return SMEnterCouponValueBase::PSClearKey();
}

SMStateBase * SMEnterCouponValue::PSBackSpaceKey(void)
{
    return SMEnterCouponValueBase::PSBackSpaceKey();
}

SMStateBase * SMEnterCouponValue::PSCancelKey(void)
{
    return SMEnterCouponValueBase::PSCancelKey();
}

SMStateBase * SMEnterCouponValue::PSButtonGoBack(void)
{
    return SMEnterCouponValueBase::PSButtonGoBack();
}

