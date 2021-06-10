//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmEBTAmount.cpp
//
// TITLE: Class impliementation for Confirm EBT
//
//
// AUTHOR:    Michael Prather
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMConfirmEBTAmount.h"     // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ConfirmEBTAmount")

IMPLEMENT_DYNCREATE(SMConfirmEBTAmount, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////
SMConfirmEBTAmount::SMConfirmEBTAmount(const TBTENDERTYPE nTyp,const long lEBT, const long lCash)
: SMConfirmEBTAmountBase(nTyp, lEBT, lCash)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMConfirmEBTAmount::SMConfirmEBTAmount()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMConfirmEBTAmount::Deliver(const TBTENDERTYPE nTyp,const long lEBT, const long lCash)
{
  SMConfirmEBTAmountBase::Deliver(nTyp, lEBT, lCash);

  return this;
}

