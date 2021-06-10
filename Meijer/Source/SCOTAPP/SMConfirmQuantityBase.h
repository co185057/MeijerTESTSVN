//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmQuantityBase.h
//
// TITLE: Class header for Confirm Quantity
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CONFIRMQUANTITYBASESTATE
#define _CONFIRMQUANTITYBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMConfirmQuantityBase : public SMCustomerModeState
{
public:
	SMConfirmQuantityBase();
  virtual SMStateBase  *Initialize(void);
  virtual bool PSAssistMenuLegalForState() { return true; };
  virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
	
  DECLARE_DYNCREATE(SMConfirmQuantityBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMConfirmQuantity(void);
// or
//    DLLEXPORT SMStateBase *makeSMConfirmQuantityBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMConfirmQuantity objects.  This function is called by
// RETURNSTATE(ConfirmQuantity) instead of calling new STATE(ConfirmQuantity) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMConfirmQuantityBase.cpp and SMConfirmQuantity.cpp
// by using the macro IMPLEMENT_STATE(ConfirmQuantity)

//DECLARE_STATE(ConfirmQuantity)

#endif
