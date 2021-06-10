//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCouponItemsBase.h
//
// TITLE: Class header for store mode Coupon Items state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmCouponItemsBaseSTATE
#define _SmCouponItemsBaseSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmCouponItemsBase : public STATE(StoreModeState)
{
public:
	SMSmCouponItemsBase();							  // constructor
  virtual SMStateBase  *Initialize(void);        // required override
  virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase  *PSButton1(void);         // override for button 1
  virtual SMStateBase  *PSButton8(void);         // override for button 8
  //virtual SMStateBase  *PSCancelKey(void);       // override for cancel key ----> removed per tar 121487
  // NewUI
  virtual SMStateBase *PSReceiptUp(void);     // to disable and enable scroll buttons when 
  virtual SMStateBase *PSReceiptDown(void);   // to disable and enable scroll buttons when 
  // NewUI
	
  DECLARE_DYNCREATE(SMSmCouponItemsBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmCouponItems(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmCouponItemsBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmCouponItems objects.  This function is called by
// RETURNSTATE(SmCouponItems) instead of calling new STATE(SmCouponItems) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmCouponItemsBase.cpp and SMSmCouponItems.cpp
// by using the macro IMPLEMENT_STATE(SmCouponItems)

//DECLARE_STATE(SmCouponItems)

#endif
