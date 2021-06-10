#include "stdafx.h"
#include <fcntl.h>
#include <stdio.h>

#include "Core_DelayedOverride.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static _TCHAR THIS_FILE[] = __FILE__;
#endif


Core_DelayedOverride::Core_DelayedOverride()
{
}

Core_DelayedOverride::~Core_DelayedOverride()
{
}

BOOL Core_DelayedOverride::VoidRatioOk( long gross, long nTotalVoidAmount )
{
  return TRUE;
}

void Core_DelayedOverride::ClearCouponTallies ( long nCouponAmount, COUPON_TYPES nType)
{
}

BOOL Core_DelayedOverride::CouponRatioOk (long gross, long nCouponAmount, COUPON_TYPES nType)
{
  return TRUE;
}
