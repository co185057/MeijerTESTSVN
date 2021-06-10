//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterQuantityBase.h
//
// TITLE: Class header for Enter Quantity state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _EnterQuantityBaseSTATE
#define _EnterQuantityBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMEnterQuantityBase : public SMCustomerModeState
{
public:
	SMEnterQuantityBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize();
  virtual bool PSAssistMenuLegalForState() { return true; };
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSNumericKey(const long);
  virtual SMStateBase  *PSClearKey(void);
  virtual SMStateBase  *PSEnterKey(void);
  virtual SMStateBase  *PSCancelKey(void);
  virtual SMStateBase  *PSButtonGoBack(void);

  DECLARE_DYNCREATE(SMEnterQuantityBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMEnterQuantity(void);
// or
//    DLLEXPORT SMStateBase *makeSMEnterQuantityBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMEnterQuantity objects.  This function is called by
// RETURNSTATE(EnterQuantity) instead of calling new STATE(EnterQuantity) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMEnterQuantityBase.cpp and SMEnterQuantity.cpp
// by using the macro IMPLEMENT_STATE(EnterQuantity)

//DECLARE_STATE(EnterQuantity)

#endif
