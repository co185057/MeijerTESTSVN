//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCouponNoMatchBase.h
//
// TITLE: Class header for coupon no match state
//
// This state accepts and handles inputs from:
// PSButton2 :  Continue
// PSEnterKey:  like button 2
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _COUPONNOMATCHBASESTATE
#define _COUPONNOMATCHBASESTATE
#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMCouponNoMatchBase : public SMCustomerModeState
{
public:
  SMCouponNoMatchBase();                // constructor
  virtual SMStateBase  *Initialize(void);         // required override
  virtual void UnInitialize();
  virtual bool PSAssistMenuLegalForState() { return true; };
protected:
  virtual SMStateBase  *PSButton1(void);          // override for button 3
  virtual SMStateBase  *PSEnterKey(void);
  virtual SMStateBase  *PSCancelKey(void);        // override for cancel key

  //@@@INFOPOINT
  virtual SMStateBase  *PSButtonGoBack(void);
  //@@@INFOPOINT
  
  DECLARE_DYNCREATE(SMCouponNoMatchBase)// MFC Runtime class/object information

};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMCouponNoMatch(void);
// or
//    DLLEXPORT SMStateBase *makeSMCouponNoMatchBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMCouponNoMatch objects.  This function is called by
// RETURNSTATE(CouponNoMatch) instead of calling new STATE(CouponNoMatch) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMCouponNoMatchBase.cpp and SMCouponNoMatch.cpp
// by using the macro IMPLEMENT_STATE(CouponNoMatch)

//DECLARE_STATE(CouponNoMatch)

#endif
