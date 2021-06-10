
#ifdef _TESTSSF
#ifndef _SSFIssueRechargeSTATE
#define _SSFIssueRechargeSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

  SMSSFIssueRecharge : public SMCustomerModeState
{
public:
  SMSSFIssueRecharge();
  virtual SMStateBase *Initialize(void);
  virtual SMStateBase *TimedOut(void);
  virtual void UnInitialize(void);  
  DECLARE_DYNCREATE(SMSSFIssueRecharge)// MFC Runtime class/object information		
};
#endif
#endif