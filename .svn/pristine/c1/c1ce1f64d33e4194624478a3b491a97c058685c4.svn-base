//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMGetFoodStampState.h
//
// Description: The GetFoodStampState state is used when the transition from a
//        state to the next state might take some time.  GetFoodStampState is a
//        "chameleon" state with respect to Customer Mode, Store Mode,
//        and Help Mode - it becomes the mode of the creating state and
//        therefore does not inherit from the above classes.
//
//
// Author:  Michael Prather
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GETFOODSTAMPSTATESTATE
#define _GETFOODSTAMPSTATESTATE

#include "SMGetFoodStampStateBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMGetFoodStampState : public SMGetFoodStampStateBase
{
public:
  // Constructor - must tell GetFoodStampState the mode of the
  // current state that's creating it.
    SMGetFoodStampState();
    virtual SMStateBase* Initialize();
    virtual bool     GetFoodStampState();
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);

    virtual SMStateBase  *OnWtDecrease(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  * TimedOut();
    virtual SMStateBase  * DMScanner();
    virtual SMStateBase  * DMCardReader();

    DECLARE_DYNCREATE(SMGetFoodStampState)// MFC Runtime class/object information
};

#endif