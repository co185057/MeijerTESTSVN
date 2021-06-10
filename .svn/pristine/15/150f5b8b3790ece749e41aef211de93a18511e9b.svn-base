//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmKeyInWtTol.CPP
//
// TITLE: Class implementation for store mode Security Maintenance Mode 
//
// AUTHOR: Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // common includes

#include "SMState.h"                // base state
#include "SMSmmKeyInWtTol.h"		// this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmmKeyInWtTol")

IMPLEMENT_DYNCREATE(SMSmmKeyInWtTol, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmmKeyInWtTol::SMSmmKeyInWtTol()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmmKeyInWtTol::Initialize(void)
{
    return SMSmmKeyInWtTolBase::Initialize();
}

void SMSmmKeyInWtTol::UnInitialize(void)
{
    SMSmmKeyInWtTolBase::UnInitialize();
}

SMStateBase * SMSmmKeyInWtTol::PSEnterKey(void)
{
    return SMSmmKeyInWtTolBase::PSEnterKey();
}

SMStateBase * SMSmmKeyInWtTol::PSNumericKey(const long lKey)
{
    return SMSmmKeyInWtTolBase::PSNumericKey(lKey);
}

SMStateBase * SMSmmKeyInWtTol::PSDecimalKey(const long ch)
{
    return SMSmmKeyInWtTolBase::PSDecimalKey(ch);
}

SMStateBase * SMSmmKeyInWtTol::PSClearKey()
{
    return SMSmmKeyInWtTolBase::PSClearKey();
}

SMStateBase * SMSmmKeyInWtTol::PSButton8(void)
{
    return SMSmmKeyInWtTolBase::PSButton8();
}
