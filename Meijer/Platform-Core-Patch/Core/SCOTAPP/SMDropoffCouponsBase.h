//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMDropoffCouponsBase.h
//
// TITLE: Class header for Deposit Coupons State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DROPOFFCOUPONSBASESTATE
#define _DROPOFFCOUPONSBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMDropoffCouponsBase : public SMCustomerModeState
{
public:
  SMDropoffCouponsBase();
  virtual SMStateBase  *Initialize(void);
  virtual void         UnInitialize(void);
	
protected:
  virtual SMStateBase  *PSButton1(void);
	
  DECLARE_DYNCREATE(SMDropoffCouponsBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMDropoffCoupons(void);
// or
//    DLLEXPORT SMStateBase *makeSMDropoffCouponsBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMDropoffCoupons objects.  This function is called by
// RETURNSTATE(DropoffCoupons) instead of calling new STATE(DropoffCoupons) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMDropoffCouponsBase.cpp and SMDropoffCoupons.cpp
// by using the macro IMPLEMENT_STATE(DropoffCoupons)

//DECLARE_STATE(DropoffCoupons)

#endif
