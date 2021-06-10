//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCouponNoMatch.cpp
//
// TITLE: Class impliementation for coupon no match State
//
// This state accepts and handles inputs from:
// PSButton2 :  OK
// PSEnterKey:  like button 2
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMCouponNoMatch.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("CouponNoMatch")

IMPLEMENT_DYNCREATE(SMCouponNoMatch, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMCouponNoMatch::SMCouponNoMatch()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMCouponNoMatch::Initialize(void)
{
    return SMCouponNoMatchBase::Initialize();
}

void SMCouponNoMatch::UnInitialize(void)
{
    SMCouponNoMatchBase::UnInitialize();
}

bool SMCouponNoMatch::PSAssistMenuLegalForState()
{
    return SMCouponNoMatchBase::PSAssistMenuLegalForState();
}

SMStateBase * SMCouponNoMatch::PSButton1(void)
{
    return SMCouponNoMatchBase::PSButton1();
}

SMStateBase * SMCouponNoMatch::PSEnterKey(void)
{
    return SMCouponNoMatchBase::PSEnterKey();
}

SMStateBase * SMCouponNoMatch::PSCancelKey(void)
{
    return SMCouponNoMatchBase::PSCancelKey();
}

SMStateBase * SMCouponNoMatch::PSButtonGoBack(void)
{
    return SMCouponNoMatchBase::PSButtonGoBack();
}
