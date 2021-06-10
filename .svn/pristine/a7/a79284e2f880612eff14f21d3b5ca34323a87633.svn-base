//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoidNoMatch.cpp
//
// TITLE: Class impliementation for void item no match State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMVoidNoMatch.h"          // MGV this

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("VoidNoMatch")

IMPLEMENT_DYNCREATE(SMVoidNoMatch, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////
SMVoidNoMatch::SMVoidNoMatch()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMVoidNoMatch::Initialize(void)
{
    return SMVoidNoMatchBase::Initialize();
}

void SMVoidNoMatch::UnInitialize(void)
{
    SMVoidNoMatchBase::UnInitialize();
}

SMStateBase * SMVoidNoMatch::OnWtIncreaseNotAllowed(void)
{
    return SMVoidNoMatchBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMVoidNoMatch::OnWtDecrease(void)
{
    return SMVoidNoMatchBase::OnWtDecrease();
}

bool SMVoidNoMatch::PSAssistMenuLegalForState()
{
    return SMVoidNoMatchBase::PSAssistMenuLegalForState();
}

SMStateBase * SMVoidNoMatch::PSButton1(void)
{
    return SMVoidNoMatchBase::PSButton1();
}

SMStateBase * SMVoidNoMatch::PSEnterKey(void)
{
    return SMVoidNoMatchBase::PSEnterKey();
}

