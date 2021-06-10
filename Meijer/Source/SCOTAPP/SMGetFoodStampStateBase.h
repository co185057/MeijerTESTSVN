//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMGetFoodStampStateBase.h
//
// Description: The GetFoodStampStateBase state is used when we want
//              to get the food stamp total from the host app.
//              Since we are trying to make a synchronous call to the
//              host app we need a state to wait in.
//
//
// Author:  Michael Prather
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GETFOODSTAMPSTATEBASESTATE
#define _GETFOODSTAMPSTATEBASESTATE

#include "SMState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMGetFoodStampStateBase : public STATE(State)
{
public:
  // Constructor - must tell GetFoodStampStateBase the mode of the
  // current state that's creating it.
  SMGetFoodStampStateBase();
  virtual SMStateBase* Initialize();
  virtual bool     GetFoodStampState() {return true;}
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);

  virtual SMStateBase  *OnWtDecrease(void);
  virtual void UnInitialize(void);
	
protected:
  virtual SMStateBase  * TimedOut();
  virtual SMStateBase  * DMScanner();
  virtual SMStateBase  * DMCardReader();
	
	
  DECLARE_DYNCREATE(SMGetFoodStampStateBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMGetFoodStampState(void);
// or
//    DLLEXPORT SMStateBase *makeSMGetFoodStampStateBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMGetFoodStampState objects.  This function is called by
// RETURNSTATE(GetFoodStampState) instead of calling new STATE(GetFoodStampState) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMGetFoodStampStateBase.cpp and SMGetFoodStampState.cpp
// by using the macro IMPLEMENT_STATE(GetFoodStampState)

//DECLARE_STATE(GetFoodStampState)

#endif
