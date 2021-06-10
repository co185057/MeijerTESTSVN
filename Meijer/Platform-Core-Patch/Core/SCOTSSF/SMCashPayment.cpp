//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCashPayment.cpp
//
// TITLE: Class implementation for Cash Payment State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMCashPayment.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("CashPayment")

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMCashPayment, CObject)// MFC Runtime class/object information

//////////////////////////////////////////
SMCashPayment::SMCashPayment()
{
  IMPLEMENT_TIMESTAMP
}

