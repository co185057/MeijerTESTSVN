//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoucherPayment.cpp
//
// TITLE: Class implementation for Voucher Payment State
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

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

