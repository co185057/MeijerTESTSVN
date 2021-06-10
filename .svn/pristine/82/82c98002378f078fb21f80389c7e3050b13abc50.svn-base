//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmConfirmSignature.cpp
//
// TITLE: Class implementation for check signature state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmConfirmSignature.h"     // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmConfirmSignature")

IMPLEMENT_DYNCREATE(SMSmConfirmSignature, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////
SMSmConfirmSignature::SMSmConfirmSignature()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmConfirmSignature::Initialize(void)
{
    return SMSmConfirmSignatureBase::Initialize();
}

void SMSmConfirmSignature::UnInitialize(void)
{
    SMSmConfirmSignatureBase::UnInitialize();
}

SMStateBase * SMSmConfirmSignature::OnWtIncreaseNotAllowed(void)
{
    return SMSmConfirmSignatureBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMSmConfirmSignature::OnWtDecrease(void)
{
    return SMSmConfirmSignatureBase::OnWtDecrease();
}

SMStateBase * SMSmConfirmSignature::PSButton1(void)
{
    return SMSmConfirmSignatureBase::PSButton1();
}

SMStateBase * SMSmConfirmSignature::PSButton2(void)
{
    return SMSmConfirmSignatureBase::PSButton2();
}

SMStateBase * SMSmConfirmSignature::TimedOut(void)
{
    return SMSmConfirmSignatureBase::TimedOut();
}

SMStateBase * SMSmConfirmSignature::PSButtonHelp(void)
{
    return SMSmConfirmSignatureBase::PSButtonHelp();
}
