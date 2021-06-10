//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMItemNotForSaleBase.h
//
// TITLE: Class header for Item Recalled State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ItemNotForSaleBaseSTATE
#define _ItemNotForSaleBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMItemNotForSaleBase : public SMCustomerModeState
{
public:
	SMItemNotForSaleBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize();
  virtual bool PSAssistMenuLegalForState() { return true; };
	
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSEnterKey(void);


	
  DECLARE_DYNCREATE(SMItemNotForSaleBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMItemNotForSale(void);
// or
//    DLLEXPORT SMStateBase *makeSMItemNotForSaleBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMItemNotForSale objects.  This function is called by
// RETURNSTATE(ItemNotForSale) instead of calling new STATE(ItemNotForSale) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMItemNotForSaleBase.cpp and SMItemNotForSale.cpp
// by using the macro IMPLEMENT_STATE(ItemNotForSale)

//DECLARE_STATE(ItemNotForSale)

#endif
