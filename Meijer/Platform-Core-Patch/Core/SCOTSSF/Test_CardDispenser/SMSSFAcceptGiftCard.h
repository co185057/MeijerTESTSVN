
#ifdef _TESTSSF
#ifndef _SSFAcceptGiftCardSTATE
#define _SSFAcceptGiftCardSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

  SMSSFAcceptGiftCard : public SMCustomerModeState
{
public:
  SMSSFAcceptGiftCard();
  virtual SMStateBase *Initialize(void);
  virtual SMStateBase *TimedOut(void);
  virtual void UnInitialize(void);
protected:
  virtual SMStateBase* DMDispenserPositionEvent( long lStat); 
  virtual SMStateBase *DMCardReader(void);
  virtual SMStateBase  *PSButtonGoBack(void);
  DECLARE_DYNCREATE(SMSSFAcceptGiftCard )// MFC Runtime class/object information		
};
#endif
#endif