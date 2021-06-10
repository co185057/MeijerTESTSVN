
#ifdef _TESTSSF
#ifndef _SSFCardDispenserHealthCheckFailedSTATE
#define _SSFCardDispenserHealthCheckFailedSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

  SMSSFCardDispenserHealthCheckFailed : public SMCustomerModeState
{
public:
  SMSSFCardDispenserHealthCheckFailed();
  virtual SMStateBase *Initialize(void);
  virtual void UnInitialize(void); 
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase *TimedOut(void);     
  DECLARE_DYNCREATE(SMSSFCardDispenserHealthCheckFailed)// MFC Runtime class/object information		
};
#endif
#endif