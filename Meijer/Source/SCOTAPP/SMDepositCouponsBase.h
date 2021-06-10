//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMDepositCouponsBase.h
//
// TITLE: Class header for Deposit Coupons State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DEPOSITCOUPONSBASESTATE
#define _DEPOSITCOUPONSBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMDepositCouponsBase : public SMCustomerModeState
{
public:
  SMDepositCouponsBase();
  virtual SMStateBase  *Initialize(void);
  virtual void         UnInitialize(void);
	
protected:
  virtual SMStateBase  *PSButton1(void);
	
  DECLARE_DYNCREATE(SMDepositCouponsBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMDepositCoupons(void);
// or
//    DLLEXPORT SMStateBase *makeSMDepositCouponsBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMDepositCoupons objects.  This function is called by
// RETURNSTATE(DepositCoupons) instead of calling new STATE(DepositCoupons) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMDepositCouponsBase.cpp and SMDepositCoupons.cpp
// by using the macro IMPLEMENT_STATE(DepositCoupons)

//DECLARE_STATE(DepositCoupons)

#endif
