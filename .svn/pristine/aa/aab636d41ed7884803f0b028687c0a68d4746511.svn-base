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
//               // MS MFC template header

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

bool SMScanCard::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMScanCardBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMScanCard::Initialize(void)
{
    return SMScanCardBase::Initialize();
}

void SMScanCard::UnInitialize(void)
{
    SMScanCardBase::UnInitialize();
}

SMStateBase * SMScanCard::OnWtIncreaseNotAllowed(void)
{
    return SMScanCardBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMScanCard::OnWtDecrease(void)
{
    return SMScanCardBase::OnWtDecrease();
}

SMStateBase * SMScanCard::PSCancelKey(void)
{
    return SMScanCardBase::PSCancelKey();
}

SMStateBase * SMScanCard::SAEventOccurred(const long lEvent)
{
    return SMScanCardBase::SAEventOccurred(lEvent);
}

SMStateBase * SMScanCard::PSButtonGoBack(void)
{
    return SMScanCardBase::PSButtonGoBack();
}

SMStateBase * SMScanCard::DMScanner(void)
{
    return SMScanCardBase::DMScanner();
}

