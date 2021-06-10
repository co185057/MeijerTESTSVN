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
//               // MS MFC template header

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

SMStateBase * SMSwipeCard::Initialize(void)
{
    return SMSwipeCardBase::Initialize();
}

void SMSwipeCard::UnInitialize(void)
{
    SMSwipeCardBase::UnInitialize();
}

SMStateBase * SMSwipeCard::OnWtIncreaseNotAllowed(void)
{
    return SMSwipeCardBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMSwipeCard::OnWtDecrease(void)
{
    return SMSwipeCardBase::OnWtDecrease();
}

bool SMSwipeCard::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMSwipeCardBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMSwipeCard::PSCancelKey(void)
{
    return SMSwipeCardBase::PSCancelKey();
}

SMStateBase * SMSwipeCard::PSButton1(void)
{
    return SMSwipeCardBase::PSButton1();
}

SMStateBase * SMSwipeCard::SAEventOccurred(const long lEvent)
{
    return SMSwipeCardBase::SAEventOccurred(lEvent);
}

SMStateBase * SMSwipeCard::PSButtonGoBack(void)
{
    return SMSwipeCardBase::PSButtonGoBack();
}

SMStateBase * SMSwipeCard::DMCardReader(void)
{
    return SMSwipeCardBase::DMCardReader();
}

SMStateBase * SMSwipeCard::DMAcceptor(void)
{
    return SMSwipeCardBase::DMAcceptor();
}

SMStateBase * SMSwipeCard::OnRAPUnsolicitedDataReceived(LPCTSTR sData)
{
    return SMSwipeCardBase::OnRAPUnsolicitedDataReceived(sData);
}
