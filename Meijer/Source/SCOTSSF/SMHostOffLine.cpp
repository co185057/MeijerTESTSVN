//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMHostOffLine.cpp
//
// TITLE: Class implementation for host offline state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMHostOffLine.h"
#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SMHostOffLine")

IMPLEMENT_DYNCREATE(SMHostOffLine, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMHostOffLine::SMHostOffLine()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase *SMHostOffLine::Initialize(void)
{
    return SMHostOffLineBase::Initialize();
}

void SMHostOffLine::UnInitialize(void)
{
    SMHostOffLineBase::UnInitialize();
}

void SMHostOffLine::showScreen(bool passwordVerified)
{
    SMHostOffLineBase::showScreen(passwordVerified );
}

SMStateBase *SMHostOffLine::OnWtIncreaseNotAllowed(void)
{
    return SMHostOffLineBase::OnWtIncreaseNotAllowed();
}

SMStateBase *SMHostOffLine::OnWtDecrease(void)
{
    return SMHostOffLineBase::OnWtDecrease();
}

SMStateBase *SMHostOffLine::OnMatchedWt(void)
{
    return SMHostOffLineBase::OnMatchedWt();
}

