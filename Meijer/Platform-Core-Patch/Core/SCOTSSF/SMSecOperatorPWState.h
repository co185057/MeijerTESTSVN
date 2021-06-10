//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecOperatorPWState.h
//
// Description: Derived from SMHelpModeState, all states that need to prompt
//        for an operator and password derive from this state.
//
// AUTHOR: Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSECOPERATORPWSTATE
#define _SMSECOPERATORPWSTATE


#include "SMSecOperatorPWStateBase.h"

//////////////////////////////////////
#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecOperatorPWState : public SMSecOperatorPWStateBase
{
	
public:
  SMSecOperatorPWState();
  SMStateBase * Initialize();

protected:
  SMStateBase * DMScanner();

  DECLARE_DYNCREATE(SMSecOperatorPWState)// MFC Runtime class/object information
	
};

#endif
