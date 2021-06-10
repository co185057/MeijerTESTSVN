//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeLoyaltyCard.cpp
//
// TITLE: Class impliementation file for Take LoyaltyCard State
//
//
//
// AUTHOR:    SCOT CORE TEAM, NCR Future Retail, ATLANTA
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMTakeLoyaltyCard.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("TakeLoyaltyCard")

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMTakeLoyaltyCard, CObject)// MFC Runtime class/object information


////////////////////////////
SMTakeLoyaltyCard::SMTakeLoyaltyCard(TBLOYALTYCARDTYPE LoyaltyCardType)
: SMTakeLoyaltyCardBase(LoyaltyCardType)

{
  IMPLEMENT_TIMESTAMP
}


////////////////////////////////////////
SMStateBase  *SMTakeLoyaltyCard::Deliver(TBLOYALTYCARDTYPE LoyaltyCardType)
{
  SMTakeLoyaltyCardBase::Deliver(LoyaltyCardType);


  return this;
}

SMStateBase * SMTakeLoyaltyCard::Initialize(void)
{
    return SMTakeLoyaltyCardBase::Initialize();
}

void SMTakeLoyaltyCard::UnInitialize(void)
{
    SMTakeLoyaltyCardBase::UnInitialize();
}

bool SMTakeLoyaltyCard::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMTakeLoyaltyCardBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMTakeLoyaltyCard::TimedOut(void)
{
    return SMTakeLoyaltyCardBase::TimedOut();
}

SMStateBase * SMTakeLoyaltyCard::DMCardRemoved(void)
{
    return SMTakeLoyaltyCardBase::DMCardRemoved();
}

SMStateBase * SMTakeLoyaltyCard::OnBackToLGW(void)
{
    return SMTakeLoyaltyCardBase::OnBackToLGW();
}

SMStateBase * SMTakeLoyaltyCard::OnWtIncreaseNotAllowed()
{
    return SMTakeLoyaltyCardBase::OnWtIncreaseNotAllowed();
}
