//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMHelpModeState.cpp
//
// TITLE:  Base class for Help Mode states
//
// AUTHOR:  Jeff Connelly
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"
#include "SMHelpModeState.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("HelpModeState")

IMPLEMENT_DYNCREATE(SMHelpModeState, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////
SMHelpModeState::SMHelpModeState()
{
  IMPLEMENT_TIMESTAMP
}


