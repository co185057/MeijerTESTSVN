//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeChange.cpp
//
// TITLE: Class impliementation file for Take Change State
//
// This state represents a successful transaction paid for in cash with
// change due back.
//
//
// AUTHOR:    Peter Denhoed, Updated DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMTakeChange.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("TakeChange")

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMTakeChange, CObject)// MFC Runtime class/object information

////////////////////////////
SMTakeChange::SMTakeChange(const bool fEOT)
: SMTakeChangeBase(fEOT)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMTakeChange::SMTakeChange()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMTakeChange::Deliver(const bool fEOT)
{
  SMTakeChangeBase::Deliver(fEOT);

  return this;
}


