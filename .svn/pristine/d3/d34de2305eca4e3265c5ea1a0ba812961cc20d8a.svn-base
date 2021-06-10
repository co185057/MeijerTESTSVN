//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTimeRestrictedItemBase.h
//
// TITLE: Class header for Time Restricted Item State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TIMERESTRICTEDITEMBASESTATE
#define _TIMERESTRICTEDITEMBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMTimeRestrictedItemBase : public SMCustomerModeState
{
public:
	SMTimeRestrictedItemBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize();
  virtual bool PSAssistMenuLegalForState() { return true; };
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSEnterKey(void);
 

	
  DECLARE_DYNCREATE(SMTimeRestrictedItemBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMTimeRestrictedItem(void);
// or
//    DLLEXPORT SMStateBase *makeSMTimeRestrictedItemBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMTimeRestrictedItem objects.  This function is called by
// RETURNSTATE(TimeRestrictedItem) instead of calling new STATE(TimeRestrictedItem) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMTimeRestrictedItemBase.cpp and SMTimeRestrictedItem.cpp
// by using the macro IMPLEMENT_STATE(TimeRestrictedItem)

//DECLARE_STATE(TimeRestrictedItem)

#endif
