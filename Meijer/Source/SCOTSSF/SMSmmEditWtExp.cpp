//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmEditWtExp.CPP
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
#include "SMSmmEditWtExp.h"				// this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmmEditWtExp")

IMPLEMENT_DYNCREATE(SMSmmEditWtExp, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmmEditWtExp::SMSmmEditWtExp()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmmEditWtExp::Initialize(void)
{
    return SMSmmEditWtExpBase::Initialize();
}

void SMSmmEditWtExp::UnInitialize(void)
{
    SMSmmEditWtExpBase::UnInitialize();
}

SMStateBase * SMSmmEditWtExp::PSButton1(void)
{
    return SMSmmEditWtExpBase::PSButton1();
}

SMStateBase * SMSmmEditWtExp::PSButton2(void)
{
    return SMSmmEditWtExpBase::PSButton2();
}

SMStateBase * SMSmmEditWtExp::PSButton3(void)
{
    return SMSmmEditWtExpBase::PSButton3();
}

SMStateBase * SMSmmEditWtExp::PSButton4(void)
{
    return SMSmmEditWtExpBase::PSButton4();
}

SMStateBase * SMSmmEditWtExp::PSButton8(void)
{
    return SMSmmEditWtExpBase::PSButton8();
}

