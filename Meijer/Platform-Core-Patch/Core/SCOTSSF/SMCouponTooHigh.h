//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCouponTooHigh.h
//
// TITLE: Class header for coupon too high State
//
//
// AUTHOR:   Gary Miller
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _COUPONTOOHIGHSTATE
#define _COUPONTOOHIGHSTATE

#include "SMCouponTooHighBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMCouponTooHigh : public SMCouponTooHighBase
{
public:
	SMCouponTooHigh();
 	
  DECLARE_DYNCREATE(SMCouponTooHigh)// MFC Runtime class/object information
		
};

#endif