//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMItemQuantityExceededBase.h
//
// TITLE: Class header for per item quantity limit exceeded
//
//
// AUTHOR:    Gary Miller
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ItemQuantityExceededBaseSTATE
#define _ItemQuantityExceededBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMItemQuantityExceededBase : public SMCustomerModeState
{
public:
	SMItemQuantityExceededBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize();
  virtual bool PSAssistMenuLegalForState() { return true; };	
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSEnterKey(void);


	
  DECLARE_DYNCREATE(SMItemQuantityExceededBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMItemQuantityExceeded(void);
// or
//    DLLEXPORT SMStateBase *makeSMItemQuantityExceededBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMItemQuantityExceeded objects.  This function is called by
// RETURNSTATE(ItemQuantityExceeded) instead of calling new STATE(ItemQuantityExceeded) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMItemQuantityExceededBase.cpp and SMItemQuantityExceeded.cpp
// by using the macro IMPLEMENT_STATE(ItemQuantityExceeded)

//DECLARE_STATE(ItemQuantityExceeded)

#endif
