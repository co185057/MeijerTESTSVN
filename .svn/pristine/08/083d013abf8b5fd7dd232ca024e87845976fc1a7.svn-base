//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterCouponValueBase.h
//
// TITLE: Class header for enter Coupon value state
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ENTERCOUPONVALUEBASESTATE
#define _ENTERCOUPONVALUEBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMEnterCouponValueBase : public SMCustomerModeState
{
public:
	SMEnterCouponValueBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize();
  virtual bool PSAssistMenuLegalForState() { return true; };
protected:
  virtual SMStateBase  *PSButton1(void);		//TAR#116924	LPM081199
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSNumericKey(const long);
  virtual SMStateBase  *PSEnterKey(void);
  virtual SMStateBase  *PSClearKey(void);
  virtual SMStateBase  *PSBackSpaceKey(void); //SR828
  virtual SMStateBase  *PSCancelKey(void);

  //@@@INFOPOINT		TAR#116924	LPM081199
  virtual SMStateBase  *PSButtonGoBack(void);
  //@@@INFOPOINT
	
  DECLARE_DYNCREATE(SMEnterCouponValueBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMEnterCouponValue(void);
// or
//    DLLEXPORT SMStateBase *makeSMEnterCouponValueBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMEnterCouponValue objects.  This function is called by
// RETURNSTATE(EnterCouponValue) instead of calling new STATE(EnterCouponValue) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMEnterCouponValueBase.cpp and SMEnterCouponValue.cpp
// by using the macro IMPLEMENT_STATE(EnterCouponValue)

//DECLARE_STATE(EnterCouponValue)

#endif
