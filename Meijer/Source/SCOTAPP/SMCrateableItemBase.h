//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCrateableItemBase.h
//
// TITLE: Class header for Crate-able Item state
//
// AUTHOR: Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CrateableItemBaseSTATE
#define _CrateableItemBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMCrateableItemBase : public SMCustomerModeState
{
public:
  SMCrateableItemBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize();
  virtual bool PSAssistMenuLegalForState() { return true; };

protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);

  DECLARE_DYNCREATE(SMCrateableItemBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMCrateableItem(void);
// or
//    DLLEXPORT SMStateBase *makeSMCrateableItemBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMCrateableItem objects.  This function is called by
// RETURNSTATE(CrateableItem) instead of calling new STATE(CrateableItem) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMCrateableItemBase.cpp and SMCrateableItem.cpp
// by using the macro IMPLEMENT_STATE(CrateableItem)

//DECLARE_STATE(CrateableItem)

#endif
