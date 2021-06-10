//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMScanCard.cpp
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
#include "SMScanCard.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ScanCard")


IMPLEMENT_DYNCREATE(SMScanCard, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP


////////////////////////////////////////
SMScanCard::SMScanCard(const TBTENDERTYPE nTyp,const long lAmt)
: SMScanCardBase(nTyp, lAmt)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMScanCard::SMScanCard()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMScanCard::Deliver(const TBTENDERTYPE nTyp,const long lAmt)
{
  SMScanCardBase::Deliver(nTyp, lAmt);

  return this;
}
