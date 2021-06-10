//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMHelpModeStateBase.h
//
// TITLE: Derived from SMState, all Help Mode states
//        are derived from this state.
//
// AUTHOR: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMHelpModeStateBase
#define _SMHelpModeStateBase

#include "Common.h"

#include "SMState.h"

//////////////////////////////////////
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMHelpModeStateBase : public STATE(State)
{
	
public:
  SMHelpModeStateBase();
  virtual bool helpMode() { return true; }
  virtual bool PSAssistMenuLegalForState() { return true; };
  virtual bool PSRemoteLegalForState();
  DECLARE_DYNCREATE(SMHelpModeStateBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMHelpModeState(void);
// or
//    DLLEXPORT SMStateBase *makeSMHelpModeStateBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMHelpModeState objects.  This function is called by
// RETURNSTATE(HelpModeState) instead of calling new STATE(HelpModeState) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMHelpModeStateBase.cpp and SMHelpModeState.cpp
// by using the macro IMPLEMENT_STATE(HelpModeState)

//DECLARE_STATE(HelpModeState)

#endif
