//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterQuantityBase.h
//
// TITLE: Class header for Enter Quantity state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _EnterQuantityBaseSTATE
#define _EnterQuantityBaseSTATE

#ifndef _CPPUNIT
#include "SMCustomerModeState.h"
#else
#include "FakeSMCustomerModeState.h"
#endif //_CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMEnterQuantityBase : public SMCUSTOMERMODESTATE
{
public:
	SMEnterQuantityBase();
#ifndef _CPPUNIT
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize();
  virtual bool PSAssistMenuLegalForState() { return true; };
#endif // _CPPUNIT
protected:
#ifndef _CPPUNIT
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSNumericKey(const long);
  virtual SMStateBase  *PSClearKey(void);
#endif // _CPPUNIT
  virtual SMStateBase  *PSEnterKey(void);
#ifndef _CPPUNIT
  virtual SMStateBase  *PSCancelKey(void);
  virtual SMStateBase  *PSBackSpaceKey(void); //SR828
  virtual SMStateBase  *PSButtonGoBack(void);

#endif // _CPPUNIT
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
