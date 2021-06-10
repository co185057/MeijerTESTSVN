
#ifdef _TESTSSF
#ifndef _SSFNewGiftCardAmountSTATE
#define _SSFNewGiftCardAmountSTATE

#include "SMCustomerModeState.h"


#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

  SMSSFNewGiftCardAmount : public SMCustomerModeState
{
public:
  SMSSFNewGiftCardAmount();
  virtual SMStateBase *Initialize(void);
  virtual void UnInitialize(void);  

protected:
  virtual SMStateBase *PSButtonGoBack(void);
  virtual SMStateBase *TimedOut(void);     
  virtual SMStateBase  *PSListLookup(CString csButtonIndex);// override for list/lookups
  DECLARE_DYNCREATE(SMSSFNewGiftCardAmount)// MFC Runtime class/object information		
};
#endif
#endif