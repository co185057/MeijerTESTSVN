//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMHelpModeState.h
//
// TITLE: Derived from SMState, all Help Mode states
//        are derived from this state.
//
// AUTHOR: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMHELPMODESTATE
#define _SMHELPMODESTATE

#include "Common.h"

#include "SMHelpModeStateBase.h"

//////////////////////////////////////
#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMHelpModeState : public SMHelpModeStateBase
{
	
public:
    SMHelpModeState();
    virtual bool helpMode();
    virtual bool PSAssistMenuLegalForState();
    virtual bool PSRemoteLegalForState();
    DECLARE_DYNCREATE(SMHelpModeState)// MFC Runtime class/object information

};

#endif
