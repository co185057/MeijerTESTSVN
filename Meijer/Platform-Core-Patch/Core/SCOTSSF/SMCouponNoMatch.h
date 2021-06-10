//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCouponNoMatch.h
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

#ifndef _COUPONNOMATCHSTATE
#define _COUPONNOMATCHSTATE
#include "SMCouponNoMatchBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMCouponNoMatch : public SMCouponNoMatchBase
{
  
public:
  SMCouponNoMatch();                // constructor
	
  DECLARE_DYNCREATE(SMCouponNoMatch)// MFC Runtime class/object information
};

#endif