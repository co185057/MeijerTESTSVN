//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectPayment.cpp
//
// TITLE: Class implementation for select a debut payment amount State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "SMSelectPayment.h"        // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SelectPayment")

IMPLEMENT_DYNCREATE(SMSelectPayment, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////
SMSelectPayment::SMSelectPayment(const TBTENDERTYPE nTyp)
: SMSelectPaymentBase(nTyp)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSelectPayment::SMSelectPayment()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSelectPayment::Deliver(const TBTENDERTYPE nTyp)
{
  SMSelectPaymentBase::Deliver(nTyp);

  return this;
}


