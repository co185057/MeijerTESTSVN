//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCouponNotAllowedBase.h
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _COUPONNOTALLOWEDBASESTATE
#define _COUPONNOTALLOWEDBASESTATE
#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMCouponNotAllowedBase : public SMCustomerModeState
{
protected:
  //begin TAR102019
  virtual SMStateBase  *PSButton1(void);		
  virtual SMStateBase  *PSButtonGoBack(void);	
  virtual SMStateBase  *PSEnterKey(void);		
  //end 102019	LPM120799

public:
  SMCouponNotAllowedBase();						// constructor
  virtual SMStateBase  *Initialize(void);		// required override
  virtual void UnInitialize();
	
  DECLARE_DYNCREATE(SMCouponNotAllowedBase)		// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMCouponNotAllowed(void);
// or
//    DLLEXPORT SMStateBase *makeSMCouponNotAllowedBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMCouponNotAllowed objects.  This function is called by
// RETURNSTATE(CouponNotAllowed) instead of calling new STATE(CouponNotAllowed) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMCouponNotAllowedBase.cpp and SMCouponNotAllowed.cpp
// by using the macro IMPLEMENT_STATE(CouponNotAllowed)

//DECLARE_STATE(CouponNotAllowed)

#endif
