
#ifdef _TESTSSF
#ifndef _SSFGiftCardBalanceReadCardSTATE
#define _SSFGiftCardBalanceReadCardSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

  SMSSFGiftCardBalanceReadCard : public SMCustomerModeState
{
public:
  SMSSFGiftCardBalanceReadCard();
  virtual SMStateBase *Initialize(void);
  virtual SMStateBase *TimedOut(void);
  virtual void UnInitialize(void);
protected:
  virtual SMStateBase* DMDispenserPositionEvent( long lStat); 
  virtual SMStateBase *DMCardReader(void);
  virtual SMStateBase  *PSButtonGoBack(void);
  DECLARE_DYNCREATE(SMSSFGiftCardBalanceReadCard )// MFC Runtime class/object information		
};
#endif
#endif