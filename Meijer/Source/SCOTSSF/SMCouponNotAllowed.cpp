//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCouponNotAllowed.cpp
//
// AUTHOR:    Scott Barnhart
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMCouponNotAllowed.h"        // MGV this

#define COMP_ID ID_SM               // base state component
#define T_ID _T("CouponNotAllowed")

IMPLEMENT_DYNCREATE(SMCouponNotAllowed, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMCouponNotAllowed::SMCouponNotAllowed()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMCouponNotAllowed::Initialize(void)
{
    return SMCouponNotAllowedBase::Initialize();
}

void SMCouponNotAllowed::UnInitialize(void)
{
    SMCouponNotAllowedBase::UnInitialize();
}

SMStateBase * SMCouponNotAllowed::PSButton1(void)
{
    return SMCouponNotAllowedBase::PSButton1();
}

SMStateBase * SMCouponNotAllowed::PSButtonGoBack(void)
{
    return SMCouponNotAllowedBase::PSButtonGoBack();
}

SMStateBase * SMCouponNotAllowed::PSEnterKey(void)
{
    return SMCouponNotAllowedBase::PSEnterKey();
}
