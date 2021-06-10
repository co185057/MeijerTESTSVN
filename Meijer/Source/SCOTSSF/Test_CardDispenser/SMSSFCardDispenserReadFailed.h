
#ifdef _TESTSSF
#ifndef _SSFCardDispenserReadFailedSTATE
#define _SSFCardDispenserReadFailedSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

  SMSSFCardDispenserReadFailed : public SMCustomerModeState
{
public:
  SMSSFCardDispenserReadFailed();
  virtual SMStateBase *Initialize(void);
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase *TimedOut(void);     
  DECLARE_DYNCREATE(SMSSFCardDispenserReadFailed)// MFC Runtime class/object information		
};
#endif
#endif