//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMUnknownPrice.cpp
//
// TITLE: Class implimentation for unknown price State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMUnknownPrice.h"         // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("UnknownPrice")

IMPLEMENT_DYNCREATE(SMUnknownPrice, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////
SMUnknownPrice::SMUnknownPrice()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMUnknownPrice::Initialize(void)
{
    return SMUnknownPriceBase::Initialize();
}

void SMUnknownPrice::UnInitialize(void)
{
    SMUnknownPriceBase::UnInitialize();
}

bool SMUnknownPrice::PSAssistMenuLegalForState()
{
    return SMUnknownPriceBase::PSAssistMenuLegalForState();
}

SMStateBase * SMUnknownPrice::PSButton1(void)
{
    return SMUnknownPriceBase::PSButton1();
}

SMStateBase * SMUnknownPrice::PSEnterKey(void)
{
    return SMUnknownPriceBase::PSEnterKey();
}
