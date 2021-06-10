//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMSelectDebitType.CPP
//
// TITLE:    Class implementation for Select Debit Type State
//
//
// AUTHOR:   Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSelectDebitType.h"      // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SelectDebitType")

IMPLEMENT_DYNCREATE(SMSelectDebitType, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSelectDebitType::SMSelectDebitType(const bool fCshBck)
: SMSelectDebitTypeBase(fCshBck)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSelectDebitType::SMSelectDebitType()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSelectDebitType::Deliver(const bool fCshBck)
{
  SMSelectDebitTypeBase::Deliver(fCshBck);

  return this;
}


