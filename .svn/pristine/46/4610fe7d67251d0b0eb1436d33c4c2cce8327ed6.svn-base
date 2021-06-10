
#ifdef _TESTSSF
#ifndef _SSFGiftCardDisplayBalanceSTATE
#define _SSFGiftCardDisplayBalanceSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

  SMSSFGiftCardDisplayBalance : public SMCustomerModeState
{
public:
  SMSSFGiftCardDisplayBalance();
  virtual SMStateBase *Initialize(void);
  virtual void UnInitialize(void);  
protected:
  virtual SMStateBase  *PSButton1(void);
  DECLARE_DYNCREATE(SMSSFGiftCardDisplayBalance)// MFC Runtime class/object information		
};
#endif
#endif