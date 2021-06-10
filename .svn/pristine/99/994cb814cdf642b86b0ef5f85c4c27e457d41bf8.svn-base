//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCouponItems.CPP
//
// TITLE: Class implementation for store mode Coupon Items state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmCouponItems.h"        // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmCouponItems")

IMPLEMENT_DYNCREATE(SMSmCouponItems, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////
SMSmCouponItems::SMSmCouponItems()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmCouponItems::Initialize(void)
{
    return SMSmCouponItemsBase::Initialize();
}

void SMSmCouponItems::UnInitialize(void)
{
    SMSmCouponItemsBase::UnInitialize();
}

SMStateBase * SMSmCouponItems::PSButton1(void)
{
    return SMSmCouponItemsBase::PSButton1();
}

SMStateBase * SMSmCouponItems::PSButton8(void)
{
    return SMSmCouponItemsBase::PSButton8();
}

SMStateBase * SMSmCouponItems::PSReceiptUp(void)
{
    return SMSmCouponItemsBase::PSReceiptUp();
}

SMStateBase * SMSmCouponItems::PSReceiptDown(void)
{
    return SMSmCouponItemsBase::PSReceiptDown();
}

/* sscoadk-5431
SMStateBase * SMSmCouponItems::RAParse(MessageElement* me)
{
    return SMSmCouponItemsBase::RAParse(me);
}
*/
