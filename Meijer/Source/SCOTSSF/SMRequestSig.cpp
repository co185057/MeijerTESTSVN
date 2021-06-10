//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmVoid.cpp
//
// TITLE: Class implimentation for Confirm Void State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMRequestSig.h"          // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("RequestSig")

IMPLEMENT_DYNCREATE(SMRequestSig, SMRequestSigBase)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMRequestSig::SMRequestSig()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMRequestSig::Initialize(void)
{
    return SMRequestSigBase::Initialize();
}

void SMRequestSig::UnInitialize(void)
{
    SMRequestSigBase::UnInitialize();
}

SMStateBase * SMRequestSig::TimedOut(void)
{
    return SMRequestSigBase::TimedOut();
}

SMStateBase * SMRequestSig::PSButtonHelp(void)
{
    return SMRequestSigBase::PSButtonHelp();
}

SMStateBase * SMRequestSig::DMSigcapStarted()
{
    return SMRequestSigBase::DMSigcapStarted();
}

SMStateBase * SMRequestSig::DMSigcapEnded()
{
    return SMRequestSigBase::DMSigcapEnded();
}

SMStateBase * SMRequestSig::OnWtIncreaseNotAllowed(void)
{
    return SMRequestSigBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMRequestSig::OnWtDecrease(void)
{
    return SMRequestSigBase::OnWtDecrease();
}

SMStateBase * SMRequestSig::OnBackToLGW(void)
{
    return SMRequestSigBase::OnBackToLGW();
}
