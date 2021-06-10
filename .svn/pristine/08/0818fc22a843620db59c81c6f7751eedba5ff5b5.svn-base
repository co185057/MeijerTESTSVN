//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmVoidedItemsBase.h
//
// TITLE: Class header for store mode Voided Items state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmVoidedItemsBaseSTATE
#define _SmVoidedItemsBaseSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmVoidedItemsBase : public STATE(StoreModeState)
{
public:
	SMSmVoidedItemsBase();							  // constructor
  virtual SMStateBase  *Initialize(void);        // required override
	
protected:
  virtual SMStateBase  *PSButton1(void);         // override for button 1
  virtual SMStateBase  *PSButton8(void);         // override for button 8
//  virtual SMStateBase  *PSCancelKey(void);       // override for cancel key
	
  DECLARE_DYNCREATE(SMSmVoidedItemsBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmVoidedItems(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmVoidedItemsBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmVoidedItems objects.  This function is called by
// RETURNSTATE(SmVoidedItems) instead of calling new STATE(SmVoidedItems) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmVoidedItemsBase.cpp and SMSmVoidedItems.cpp
// by using the macro IMPLEMENT_STATE(SmVoidedItems)

//DECLARE_STATE(SmVoidedItems)

#endif
