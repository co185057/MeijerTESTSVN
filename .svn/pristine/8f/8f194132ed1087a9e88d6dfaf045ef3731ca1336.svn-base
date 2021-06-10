//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSwipeCard.cpp
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
#include "SMSwipeCard.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SwipeCard")

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMSwipeCard, CObject)// MFC Runtime class/object information

////////////////////////////////////////
SMSwipeCard::SMSwipeCard(const TBTENDERTYPE nTyp,const long lAmt)
: SMSwipeCardBase(nTyp, lAmt)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSwipeCard::SMSwipeCard()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSwipeCard::Deliver(const TBTENDERTYPE nTyp,const long lAmt)
{
  SMSwipeCardBase::Deliver(nTyp, lAmt);

  return this;
}

