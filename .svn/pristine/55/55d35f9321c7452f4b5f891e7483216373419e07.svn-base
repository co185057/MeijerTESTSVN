//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmKeyInItemCode.CPP
//
// TITLE: Class implementation for store mode Security Maintenance Mode 
//
// AUTHOR: Rhonah Entize
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // common includes

#include "SMState.h"                // base state
#include "SMSmmKeyInItemCode.h"		// this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmmKeyInItemCode")

IMPLEMENT_DYNCREATE(SMSmmKeyInItemCode, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmmKeyInItemCode::SMSmmKeyInItemCode()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmmKeyInItemCode::Initialize(void)
{
    return SMSmmKeyInItemCodeBase::Initialize();
}

void SMSmmKeyInItemCode::UnInitialize(void)
{
    SMSmmKeyInItemCodeBase::UnInitialize();
}

SMStateBase * SMSmmKeyInItemCode::PSEnterKey()
{
    return SMSmmKeyInItemCodeBase::PSEnterKey();
}

SMStateBase * SMSmmKeyInItemCode::PSNumericKey(const long lKey)
{
    return SMSmmKeyInItemCodeBase::PSNumericKey(lKey);
}

SMStateBase * SMSmmKeyInItemCode::PSClearKey()
{
    return SMSmmKeyInItemCodeBase::PSClearKey();
}

SMStateBase * SMSmmKeyInItemCode::PSBackSpaceKey(void)
{
    return SMSmmKeyInItemCodeBase::PSBackSpaceKey();
}

SMStateBase * SMSmmKeyInItemCode::PSButton8(void)
{
    return SMSmmKeyInItemCodeBase::PSButton8();
}

