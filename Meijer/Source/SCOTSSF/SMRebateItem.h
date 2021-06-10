//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRebateItem.h
//
// TITLE: Class header for tagging rebate item state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RebateItemSTATE
#define _RebateItemSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

  SMRebateItem : public SMCustomerModeState
{
public:
  SMRebateItem();
  virtual SMStateBase *Initialize(void);
  virtual void UnInitialize(void);
  virtual bool PSAssistMenuLegalForState() { return true; };
protected:
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSButton5(void);
  virtual SMStateBase *PSReceiptUp(void);   // to disable and enable scroll buttons when 
  virtual SMStateBase *PSReceiptDown(void); // at the ends of the list
  virtual SMStateBase *PSButtonGoBack(void);
  virtual SMStateBase *TBItemDetails(void);
  CString csCurContext;    // Tar 238987
  DECLARE_DYNCREATE(SMRebateItem)// MFC Runtime class/object information		
};
#endif
