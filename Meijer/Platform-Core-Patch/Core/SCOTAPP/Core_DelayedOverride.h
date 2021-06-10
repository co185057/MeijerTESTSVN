#ifndef _CORE_DELAYEDOVERRIDE_INCLUDED
#define _CORE_DELAYEDOVERRIDE_INCLUDED

#include <afxtempl.h>

#include "SCOTDeferredOverrideClass.h"

class Core_DelayedOverride: public CDelayedOverride
{
public:
  Core_DelayedOverride();
  virtual ~Core_DelayedOverride();

  // pure virtual in CDelayedOverride
  virtual BOOL VoidRatioOk( long gross, long nTotalVoidAmount );
  virtual void ClearCouponTallies ( long nCouponAmount, COUPON_TYPES nType);
  virtual BOOL CouponRatioOk (long gross, long nCouponAmount, COUPON_TYPES nType);

protected:
};


#endif
