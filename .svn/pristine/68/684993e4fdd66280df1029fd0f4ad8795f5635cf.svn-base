//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmInvalidWtTol.CPP
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
#include "SMSmmInvalidWtTol.h"		// this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmmInvalidWtTol")

IMPLEMENT_DYNCREATE(SMSmmInvalidWtTol, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmmInvalidWtTol::SMSmmInvalidWtTol()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmmInvalidWtTol::Initialize(void)
{
    return SMSmmInvalidWtTolBase::Initialize();
}

void SMSmmInvalidWtTol::UnInitialize(void)
{
    SMSmmInvalidWtTolBase::UnInitialize();
}

SMStateBase * SMSmmInvalidWtTol::PSButton1(void)
{
    return SMSmmInvalidWtTolBase::PSButton1();
}
