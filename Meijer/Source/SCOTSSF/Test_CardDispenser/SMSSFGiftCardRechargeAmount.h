
#ifdef _TESTSSF
#ifndef _SSFGiftCardRechargeAmountSTATE
#define _SSFGiftCardRechargeAmountSTATE

#include "SMCustomerModeState.h"


#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

  SMSSFGiftCardRechargeAmount : public SMCustomerModeState
{
public:
  SMSSFGiftCardRechargeAmount();
  virtual SMStateBase *Initialize(void);
  virtual void UnInitialize(void);  

protected:
  virtual SMStateBase *PSButtonGoBack(void);
  virtual SMStateBase *TimedOut(void);     
  virtual SMStateBase  *PSListLookup(CString csButtonIndex);// override for list/lookups
  DECLARE_DYNCREATE(SMSSFGiftCardRechargeAmount)// MFC Runtime class/object information		
};
#endif
#endif