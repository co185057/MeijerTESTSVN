//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmDebitAmount.cpp
//
// TITLE: Class impliementation for check signature state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMConfirmDebitAmount.h"     // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ConfirmDebitAmount")

IMPLEMENT_DYNCREATE(SMConfirmDebitAmount, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////
SMConfirmDebitAmount::SMConfirmDebitAmount(const TBTENDERTYPE nTyp,const long lAmt)
: SMConfirmDebitAmountBase(nTyp, lAmt)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMConfirmDebitAmount::SMConfirmDebitAmount()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMConfirmDebitAmount::Deliver(const TBTENDERTYPE nTyp,const long lAmt)
{
  SMConfirmDebitAmountBase::Deliver(nTyp, lAmt);

  return this;
}

