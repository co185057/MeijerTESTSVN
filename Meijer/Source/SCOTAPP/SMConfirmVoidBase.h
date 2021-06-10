//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmVoidBase.h
//
// TITLE: Class header for Confirm Void
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CONFIRMVOIDBASESTATE
#define _CONFIRMVOIDBASESTATE
#ifndef _CPPUNIT
#include "SMCustomerModeState.h"
#else
#include "FakeSMCustomerModeState.h"
#endif
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMConfirmVoidBase : public SMCUSTOMERMODESTATE
{
public:
	SMConfirmVoidBase();
  virtual SMStateBase  *Initialize(void);
#ifndef _CPPUNIT
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual bool PSAssistMenuLegalForState() { return true; };
  virtual void UnInitialize(void);		// RFC 330050
#endif
protected:
#ifndef _CPPUNIT
  CString csWork;
  CString csCode;
	
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
#endif
	
  DECLARE_DYNCREATE(SMConfirmVoidBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMConfirmVoid(void);
// or
//    DLLEXPORT SMStateBase *makeSMConfirmVoidBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMConfirmVoid objects.  This function is called by
// RETURNSTATE(ConfirmVoid) instead of calling new STATE(ConfirmVoid) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMConfirmVoidBase.cpp and SMConfirmVoid.cpp
// by using the macro IMPLEMENT_STATE(ConfirmVoid)

//DECLARE_STATE(ConfirmVoid)

#endif
