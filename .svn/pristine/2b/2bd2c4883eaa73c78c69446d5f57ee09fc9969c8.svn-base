//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmAddWeight.CPP
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
#include "SMSmmAddWeight.h"			// this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmmAddWeight")

IMPLEMENT_DYNCREATE(SMSmmAddWeight, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmmAddWeight::SMSmmAddWeight()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmmAddWeight::Initialize(void)
{
    return SMSmmAddWeightBase::Initialize();
}

void SMSmmAddWeight::UnInitialize(void)
{
    SMSmmAddWeightBase::UnInitialize();
}

SMStateBase * SMSmmAddWeight::SASMMBagScaleEvent(void)
{
    return SMSmmAddWeightBase::SASMMBagScaleEvent();
}

SMStateBase * SMSmmAddWeight::PSButton8(void)
{
    return SMSmmAddWeightBase::PSButton8();
}
