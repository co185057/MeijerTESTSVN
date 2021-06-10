//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCardMisRead.cpp
//
// TITLE: Class impliementation for mis-read card State
//
//
// AUTHOR:    Peter Denhoed 98/03/21
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // base state
            
#include "SMCardMisRead.h"          // MGV this


#define COMP_ID ID_SM               // base state component
#define T_ID    _T("CardMisRead")

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMCardMisRead, CObject)// MFC Runtime class/object information

//////////////////////////////////////////////////
SMCardMisRead::SMCardMisRead(LPTSTR nHCrc, PSTEXT Title)
: SMCardMisReadBase(nHCrc, Title)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMCardMisRead::SMCardMisRead()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMCardMisRead::Deliver(LPTSTR nHCrc, PSTEXT Title)
{
  SMCardMisReadBase::Deliver(nHCrc, Title);

  return this;
}


//////////////////////////////////////////////////
SMCardMisRead::SMCardMisRead(const PSTEXT nHCrc, PSTEXT Title)
: SMCardMisReadBase(nHCrc, Title)
{
  IMPLEMENT_TIMESTAMP
}


////////////////////////////////////////
SMStateBase  *SMCardMisRead::Deliver(PSTEXT nHCrc, PSTEXT Title)
{
  SMCardMisReadBase::Deliver(nHCrc, Title);

  return this;
}

bool SMCardMisRead::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMCardMisReadBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMCardMisRead::OnWtIncreaseNotAllowed(void)
{
    return SMCardMisReadBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMCardMisRead::OnWtDecrease(void)
{
    return SMCardMisReadBase::OnWtDecrease();
}

SMStateBase * SMCardMisRead::Initialize(void)
{
    return SMCardMisReadBase::Initialize();
}

void SMCardMisRead::UnInitialize(void)
{
    SMCardMisReadBase::UnInitialize();
}

SMStateBase * SMCardMisRead::PSButton1(void)
{
    return SMCardMisReadBase::PSButton1();
}

SMStateBase * SMCardMisRead::DMCardReader(void)
{
    return SMCardMisReadBase::DMCardReader();
}

SMStateBase * SMCardMisRead::DMCardRemoved(void)
{
    return SMCardMisReadBase::DMCardRemoved();
}

SMStateBase * SMCardMisRead::TimedOut(void)
{
    return SMCardMisReadBase::TimedOut();
}

SMStateBase * SMCardMisRead::PSEnterKey(void)
{
    return SMCardMisReadBase::PSEnterKey();
}

SMStateBase * SMCardMisRead::PSButtonHelp(void)
{
    return SMCardMisReadBase::PSButtonHelp();
}

