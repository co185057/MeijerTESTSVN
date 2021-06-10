
#ifdef _TESTSSF
#ifndef _SSFSelectGiftCardActivitySTATE
#define _SSFSelectGiftCardActivitySTATE

#include "SMCustomerModeState.h"


#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

  SMSSFSelectGiftCardActivity : public SMCustomerModeState
{
public:
  SMSSFSelectGiftCardActivity();
  virtual SMStateBase *Initialize(void); 
	
protected:
  virtual SMStateBase *PSButtonGoBack(void);
  virtual SMStateBase *TimedOut(void);     
  virtual SMStateBase  *PSListLookup(CString csButtonIndex);// override for list/lookups
  DECLARE_DYNCREATE(SMSSFSelectGiftCardActivity)// MFC Runtime class/object information		
};
#endif
#endif