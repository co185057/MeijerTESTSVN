//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCouponTooHighBase.h
//
// TITLE: Class header for coupon value too high state
//
//
// AUTHOR:  Gary Miller
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CouponTooHighBaseSTATE
#define _CouponTooHighBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMCouponTooHighBase : public SMCustomerModeState
{
public:
	SMCouponTooHighBase();
  virtual SMStateBase  *Initialize(void);
  virtual bool PSAssistMenuLegalForState() { return true; };
  virtual void UnInitialize();
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSEnterKey(void);
	static BEST bestSaved; //dp185016 tar 288377
	
  DECLARE_DYNCREATE(SMCouponTooHighBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMCouponTooHigh(void);
// or
//    DLLEXPORT SMStateBase *makeSMCouponTooHighBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMCouponTooHigh objects.  This function is called by
// RETURNSTATE(CouponTooHigh) instead of calling new STATE(CouponTooHigh) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMCouponTooHighBase.cpp and SMCouponTooHigh.cpp
// by using the macro IMPLEMENT_STATE(CouponTooHigh)

//DECLARE_STATE(CouponTooHigh)

#endif
