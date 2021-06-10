//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRestrictedNotAllowed.cpp
//
// TITLE: Class implimentation for unknown Item State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMRestrictedNotAllowed.h"          // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("RestrictedNotAllowed")

IMPLEMENT_DYNCREATE(SMRestrictedNotAllowed, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMRestrictedNotAllowed::SMRestrictedNotAllowed(const PSTEXT nMsg)
:SMRestrictedNotAllowedBase(nMsg)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMRestrictedNotAllowed::SMRestrictedNotAllowed()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMRestrictedNotAllowed::Deliver(const PSTEXT nMsg)
{
  SMRestrictedNotAllowedBase::Deliver(nMsg);

  return this;
}

bool SMRestrictedNotAllowed::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMRestrictedNotAllowedBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMRestrictedNotAllowed::Initialize(void)
{
    return SMRestrictedNotAllowedBase::Initialize();
}

void SMRestrictedNotAllowed::UnInitialize(void)
{
    SMRestrictedNotAllowedBase::UnInitialize();
}

bool SMRestrictedNotAllowed::PSAssistMenuLegalForState()
{
    return SMRestrictedNotAllowedBase::PSAssistMenuLegalForState();
}

SMStateBase * SMRestrictedNotAllowed::PSButton1(void)
{
    return SMRestrictedNotAllowedBase::PSButton1();
}

SMStateBase * SMRestrictedNotAllowed::PSEnterKey(void)
{
    return SMRestrictedNotAllowedBase::PSEnterKey();
}

