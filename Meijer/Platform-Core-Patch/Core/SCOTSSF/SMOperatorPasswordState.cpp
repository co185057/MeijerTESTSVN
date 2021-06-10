//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOperatorPasswordState.cpp
//
// Description: Derived from SMHelpModeState, all states that need to prompt
//        for an operator and password derive from this state.
//
// AUTHOR: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"
#include "SMOperatorPasswordState.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("OperatorPasswordState")

IMPLEMENT_DYNCREATE(SMOperatorPasswordState, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////////////
SMOperatorPasswordState::SMOperatorPasswordState()
: SMOperatorPasswordStateBase()
{
  IMPLEMENT_TIMESTAMP
}

