//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecOperatorPWState.cpp
//
// Description: Derived from SMHelpModeState, all states that need to prompt
//        for an operator and password derive from this state.
//
// AUTHOR: Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"
#include "SMSecOperatorPWState.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SecOperatorPWState")

IMPLEMENT_DYNCREATE(SMSecOperatorPWState, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////////////
SMSecOperatorPWState::SMSecOperatorPWState()
: SMSecOperatorPWStateBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////////
SMStateBase * SMSecOperatorPWState::Initialize()
{
   return SMSecOperatorPWStateBase::Initialize();
}

//////////////////////////////////////////////////
SMStateBase * SMSecOperatorPWState::DMScanner()
{
   return SMSecOperatorPWStateBase::DMScanner();
}