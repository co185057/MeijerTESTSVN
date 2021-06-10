//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSetAllItemsAsideBase.h
//
// TITLE: Class header for unknown price State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SetAllItemsAsideBaseSTATE
#define _SetAllItemsAsideBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSetAllItemsAsideBase : public SMCustomerModeState
{
public:
  SMSetAllItemsAsideBase();                      // constructor
  virtual SMStateBase  *Initialize(void);        // required override
  virtual SMStateBase  *TimedOut(void);          // optional
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual void UnInitialize(void);		// RFC 330050

protected:
  virtual SMStateBase  *PSButton1(void);         // override for button 1
  virtual SMStateBase  *PSEnterKey(void);        // override for enter key
	
  DECLARE_DYNCREATE(SMSetAllItemsAsideBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSetAllItemsAside(void);
// or
//    DLLEXPORT SMStateBase *makeSMSetAllItemsAsideBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSetAllItemsAside objects.  This function is called by
// RETURNSTATE(SetAllItemsAside) instead of calling new STATE(SetAllItemsAside) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSetAllItemsAsideBase.cpp and SMSetAllItemsAside.cpp
// by using the macro IMPLEMENT_STATE(SetAllItemsAside)

//DECLARE_STATE(SetAllItemsAside)

#endif
