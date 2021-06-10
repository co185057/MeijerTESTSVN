
#ifdef _TESTSSF
#ifndef _SSFCardRechargeDispenserTakeCardSTATE
#define _SSFCardRechargeDispenserTakeCardSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

  SMSSFCardRechargeDispenserTakeCard : public SMCustomerModeState
{
public:
  SMSSFCardRechargeDispenserTakeCard();
  virtual SMStateBase *Initialize(void);
  virtual SMStateBase *TimedOut(void);
  virtual void UnInitialize(void);  
protected:
  virtual SMStateBase* DMDispenserPositionEvent( long lStat); 
  DECLARE_DYNCREATE(SMSSFCardRechargeDispenserTakeCard)// MFC Runtime class/object information		
};
#endif
#endif