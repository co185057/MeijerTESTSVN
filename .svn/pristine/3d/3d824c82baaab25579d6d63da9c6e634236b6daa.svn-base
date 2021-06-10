//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeCard.cpp
//
// TITLE: Class impliementation file for Take Card State
//
//
// AUTHOR:    Peter Denhoed, Updated DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "SMTakeCard.h"             // MGV take Card state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("TakeCard")


DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMTakeCard, CObject)// MFC Runtime class/object information

///////////////////////////////////////
SMTakeCard::SMTakeCard(const bool fEOT, const bool fCardError)
: SMTakeCardBase(fEOT, fCardError)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMTakeCard::SMTakeCard()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMTakeCard::Deliver(const bool fEOT, const bool fCardError)
{
  SMTakeCardBase::Deliver(fEOT, fCardError);

  return this;
}

SMStateBase * SMTakeCard::Initialize(void)
{
    return SMTakeCardBase::Initialize();
}

void SMTakeCard::UnInitialize(void)
{
    SMTakeCardBase::UnInitialize();
}

SMStateBase * SMTakeCard::OnWtIncreaseNotAllowed(void)
{
    return SMTakeCardBase::OnWtIncreaseNotAllowed();
}

bool SMTakeCard::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMTakeCardBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMTakeCard::TimedOut(void)
{
    return SMTakeCardBase::TimedOut();
}

SMStateBase * SMTakeCard::DMCardRemoved(void)
{
    return SMTakeCardBase::DMCardRemoved();
}

SMStateBase * SMTakeCard::OnWtDecrease(void)
{
    return SMTakeCardBase::OnWtDecrease();
}
