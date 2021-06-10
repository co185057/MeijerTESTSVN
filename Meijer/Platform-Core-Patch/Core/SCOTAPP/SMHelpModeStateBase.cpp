//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMHelpModeStateBase.cpp
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
#define T_ID _T("HelpModeStateBase")

IMPLEMENT_DYNCREATE(SMHelpModeStateBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(HelpModeState)


DEFINE_TIMESTAMP

//////////////////////////////////
SMHelpModeStateBase::SMHelpModeStateBase()
{
  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
  IMPLEMENT_TIMESTAMP
}
// Assist menu states may be run legally from the RAP station
bool SMHelpModeStateBase::PSRemoteLegalForState()
{
	return (co.IsInAssistMenus() && PSAssistMenuLegalForState());
}
