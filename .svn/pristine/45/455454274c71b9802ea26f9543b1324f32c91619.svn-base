//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoucherPayment.cpp
//
// TITLE: Class implementation for Voucher Payment State
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMVoucherPayment.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("VoucherPayment")

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMVoucherPayment, CObject)// MFC Runtime class/object information

//////////////////////////////////////////
SMVoucherPayment::SMVoucherPayment()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMVoucherPayment::Initialize(void)
{
    return SMVoucherPaymentBase::Initialize();
}

void SMVoucherPayment::UnInitialize(void)
{
    SMVoucherPaymentBase::UnInitialize();
}

SMStateBase * SMVoucherPayment::OnWtIncreaseNotAllowed(void)
{
    return SMVoucherPaymentBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMVoucherPayment::OnWtDecrease(void)
{
    return SMVoucherPaymentBase::OnWtDecrease();
}

SMStateBase * SMVoucherPayment::PSButton1(void)
{
    return SMVoucherPaymentBase::PSButton1();
}

SMStateBase * SMVoucherPayment::DMScanner(void)
{
    return SMVoucherPaymentBase::DMScanner();
}

SMStateBase * SMVoucherPayment::SAEventOccurred(const long lEvent)
{
    return SMVoucherPaymentBase::SAEventOccurred(lEvent);
}

SMStateBase * SMVoucherPayment::PSCancelKey(void)
{
    return SMVoucherPaymentBase::PSCancelKey();
}

SMStateBase * SMVoucherPayment::PSButtonGoBack(void)
{
    return SMVoucherPaymentBase::PSButtonGoBack();
}
