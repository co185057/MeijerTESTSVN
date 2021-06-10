//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmQuantity.cpp
//
// TITLE: Class implimentation for Confirm Quantity State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMConfirmQuantity.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ConfirmQuantity")

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMConfirmQuantity, CObject)// MFC Runtime class/object information

//////////////////////////////////////////
SMConfirmQuantity::SMConfirmQuantity()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMConfirmQuantity::Initialize(void)
{
    return SMConfirmQuantityBase::Initialize();
}

void SMConfirmQuantity::UnInitialize(void)
{
    SMConfirmQuantityBase::UnInitialize();
}

bool SMConfirmQuantity::PSAssistMenuLegalForState()
{
    return SMConfirmQuantityBase::PSAssistMenuLegalForState();
}

SMStateBase * SMConfirmQuantity::PSButton1(void)
{
    return SMConfirmQuantityBase::PSButton1();
}

SMStateBase * SMConfirmQuantity::PSButton2(void)
{
    return SMConfirmQuantityBase::PSButton2();
}
