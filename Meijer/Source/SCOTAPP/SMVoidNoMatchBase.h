//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoidNoMatchBase.h
//
// TITLE: Class header for void item no match State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _VoidNoMatchBaseSTATE
#define _VoidNoMatchBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMVoidNoMatchBase : public SMCustomerModeState
{
public:
  SMVoidNoMatchBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize();
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual bool PSAssistMenuLegalForState() { return true; };
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSEnterKey(void);
	
  DECLARE_DYNCREATE(SMVoidNoMatchBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMVoidNoMatch(void);
// or
//    DLLEXPORT SMStateBase *makeSMVoidNoMatchBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMVoidNoMatch objects.  This function is called by
// RETURNSTATE(VoidNoMatch) instead of calling new STATE(VoidNoMatch) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMVoidNoMatchBase.cpp and SMVoidNoMatch.cpp
// by using the macro IMPLEMENT_STATE(VoidNoMatch)

//DECLARE_STATE(VoidNoMatch)

#endif
