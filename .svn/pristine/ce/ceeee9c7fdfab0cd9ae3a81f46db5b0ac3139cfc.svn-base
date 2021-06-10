//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMApplyPromotionalItems.h
//
// TITLE: Class header for apply promotional items state (POS19072)
//
// CHANGE HISTORY
//
// POS19072 WORK REQUEST:16656 Name: Matt Condra Date: November 23, 2011
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ApplyPromotionalItemsSTATE
#define _ApplyPromotionalItemsSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

SMApplyPromotionalItems : public SMCustomerModeState
{
public:
  SMApplyPromotionalItems();
  virtual SMStateBase *Initialize(void);
  virtual void UnInitialize(void);
protected:
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSButton2(void);
  virtual SMStateBase *TimedOut(void);     
  DECLARE_DYNCREATE(SMApplyPromotionalItems)// MFC Runtime class/object information		
};
#endif
