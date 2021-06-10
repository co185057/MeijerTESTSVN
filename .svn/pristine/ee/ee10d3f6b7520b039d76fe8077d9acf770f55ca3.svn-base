//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTenderBalance.cpp
//
// TITLE: Class implimentation for TenderBalance State
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
 
#include "SMState.h"                // MGV base state
#include "SMTenderBalance.h"          // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("TenderBalance")

IMPLEMENT_DYNCREATE(SMTenderBalance, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMTenderBalance::SMTenderBalance()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMTenderBalance::Initialize(void)
{
    return SMTenderBalanceBase::Initialize();
}

void SMTenderBalance::UnInitialize(void)
{
    SMTenderBalanceBase::UnInitialize();
}

SMStateBase * SMTenderBalance::OnWtIncreaseNotAllowed(void)
{
    return SMTenderBalanceBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMTenderBalance::OnWtDecrease(void)
{
    return SMTenderBalanceBase::OnWtDecrease();
}

SMStateBase * SMTenderBalance::TimedOut(void)
{
    return SMTenderBalanceBase::TimedOut();
}

SMStateBase * SMTenderBalance::PSButton1(void)
{
    return SMTenderBalanceBase::PSButton1();
}

SMStateBase * SMTenderBalance::PSEnterKey(void)
{
    return SMTenderBalanceBase::PSEnterKey();
}
