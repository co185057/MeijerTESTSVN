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
#include <afxtempl.h>               // MS MFC template header

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


