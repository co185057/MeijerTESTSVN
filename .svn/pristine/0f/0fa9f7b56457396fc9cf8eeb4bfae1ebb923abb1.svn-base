//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterCoupons.CPP
//
// TITLE: Class header for enter coupons State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMEnterCoupons.h"         // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("EnterCoupons")

IMPLEMENT_DYNCREATE(SMEnterCoupons, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMEnterCoupons::SMEnterCoupons()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMEnterCoupons::Initialize(void)
{
	//TAR369164
	bPressHelpOnWay = false;
    return SMEnterCouponsBase::Initialize();
}

void SMEnterCoupons::UnInitialize(void)
{
    SMEnterCouponsBase::UnInitialize();
}

SMStateBase * SMEnterCoupons::PSButton1(void)
{
    return SMEnterCouponsBase::PSButton1();
}

SMStateBase * SMEnterCoupons::PSButton2(void)
{
    return SMEnterCouponsBase::PSButton2();
}

SMStateBase * SMEnterCoupons::PSButton3(void)
{
    return SMEnterCouponsBase::PSButton3();
}

SMStateBase * SMEnterCoupons::PSButton4(void)
{
    return SMEnterCouponsBase::PSButton4();
}

SMStateBase * SMEnterCoupons::PSCancelKey(void)
{
    return SMEnterCouponsBase::PSCancelKey();
}

SMStateBase * SMEnterCoupons::DMScanner(void)
{
    return SMEnterCouponsBase::DMScanner();
}

SMStateBase * SMEnterCoupons::DMCardReader(void)
{
    return SMEnterCouponsBase::DMCardReader();
}

SMStateBase * SMEnterCoupons::DMAcceptor(void)
{
    return SMEnterCouponsBase::DMAcceptor();
}
