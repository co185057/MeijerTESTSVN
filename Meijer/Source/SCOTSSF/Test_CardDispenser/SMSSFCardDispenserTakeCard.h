
#ifdef _TESTSSF
#ifndef _SSFCardDispenserTakeCardSTATE
#define _SSFCardDispenserTakeCardSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

  SMSSFCardDispenserTakeCard : public SMCustomerModeState
{
public:
  SMSSFCardDispenserTakeCard();
  virtual SMStateBase *Initialize(void);
  virtual SMStateBase *TimedOut(void);
  virtual void UnInitialize(void);  
protected:
  virtual SMStateBase* DMDispenserPositionEvent( long lStat); 
  DECLARE_DYNCREATE(SMSSFCardDispenserTakeCard)// MFC Runtime class/object information		
};
#endif
#endif