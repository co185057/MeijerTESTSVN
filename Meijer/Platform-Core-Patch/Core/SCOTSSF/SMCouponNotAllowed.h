//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCouponNotAllowed.h
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _COUPONNOTALLOWED
#define _COUPONNOTALLOWED
#include "SMCouponNotAllowedBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMCouponNotAllowed : public SMCouponNotAllowedBase
{
public:
  SMCouponNotAllowed();                // constructor
	
  DECLARE_DYNCREATE(SMCouponNotAllowed)// MFC Runtime class/object information
};

#endif
