
#ifdef _TESTSSF
#ifndef _SSFIssueNewGiftCardSTATE
#define _SSFIssueNewGiftCardSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

  SMSSFIssueNewGiftCard : public SMCustomerModeState
{
public:
  SMSSFIssueNewGiftCard();
  virtual SMStateBase *Initialize(void);
  virtual SMStateBase *TimedOut(void);
  virtual void UnInitialize(void);  
protected:
  virtual SMStateBase* DMDispenserPositionEvent( long lStat); 
  DECLARE_DYNCREATE(SMSSFIssueNewGiftCard)// MFC Runtime class/object information		
};
#endif
#endif