//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterCouponsBase.h
//
// TITLE: Class header for Scan and Bag State
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ENTERCOUPONSBASESTATE
#define _ENTERCOUPONSBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMEnterCouponsBase : public SMCustomerModeState
{
public:
	SMEnterCouponsBase();
  virtual SMStateBase  *Initialize(void);
  virtual void     UnInitialize(void);
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSButton4(void);
  virtual SMStateBase  *PSCancelKey(void);
  virtual SMStateBase  *DMScanner(void);
  virtual SMStateBase  *DMCardReader(void);
  virtual SMStateBase  *DMAcceptor(void);
  CString csCurContext;    // Tar 238987
	
  DECLARE_DYNCREATE(SMEnterCouponsBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMEnterCoupons(void);
// or
//    DLLEXPORT SMStateBase *makeSMEnterCouponsBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMEnterCoupons objects.  This function is called by
// RETURNSTATE(EnterCoupons) instead of calling new STATE(EnterCoupons) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMEnterCouponsBase.cpp and SMEnterCoupons.cpp
// by using the macro IMPLEMENT_STATE(EnterCoupons)

//DECLARE_STATE(EnterCoupons)

#endif
