//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCouponItems.h
//
// TITLE: Class header for store mode Coupon Items state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMCOUPONITEMSSTATE
#define _SMCOUPONITEMSSTATE

#include "SMSmCouponItemsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmCouponItems : public SMSmCouponItemsBase
{
public:
	SMSmCouponItems();							  // constructor

	
  DECLARE_DYNCREATE(SMSmCouponItems)// MFC Runtime class/object information
};

#endif