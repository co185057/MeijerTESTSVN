//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMEnterPin.CPP
//
// TITLE:    Class implementation for entering PIN state
//
//
// AUTHOR:   Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMEnterPin.h"             // MGV this state
#define COMP_ID ID_SM               // base state component
#define T_ID    _T("EnterPin")

IMPLEMENT_DYNCREATE(SMEnterPin, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////
SMEnterPin::SMEnterPin(const TBTENDERTYPE nTyp, const bool fCshBck)
: SMEnterPinBase(nTyp, fCshBck)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMEnterPin::SMEnterPin()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMEnterPin::Deliver(const TBTENDERTYPE nTyp, const bool fCshBck)
{
  SMEnterPinBase::Deliver(nTyp, fCshBck);

  return this;
}


