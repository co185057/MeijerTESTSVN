//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCouponTooHigh.cpp
//
// TITLE: Class implimentation for coupon too high
//
//
// AUTHOR:   Gary Miller
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMCouponTooHigh.h"          // MGV this

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("CouponTooHigh")

IMPLEMENT_DYNCREATE(SMCouponTooHigh, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////
SMCouponTooHigh::SMCouponTooHigh()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMCouponTooHigh::Initialize(void)
{
    return SMCouponTooHighBase::Initialize();
}

void SMCouponTooHigh::UnInitialize(void)
{
    SMCouponTooHighBase::UnInitialize();
}

bool SMCouponTooHigh::PSAssistMenuLegalForState()
{
    return SMCouponTooHighBase::PSAssistMenuLegalForState();
}

SMStateBase * SMCouponTooHigh::PSButton1(void)
{
    return SMCouponTooHighBase::PSButton1();
}

SMStateBase * SMCouponTooHigh::PSEnterKey(void)
{
    return SMCouponTooHighBase::PSEnterKey();
}
