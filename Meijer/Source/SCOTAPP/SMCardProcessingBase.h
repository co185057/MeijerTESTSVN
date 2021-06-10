//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCardProcessingBase.h
//
// TITLE: Class header for Confirn Signature
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CARDPROCESSBASESTATE
#define _CARDPROCESSBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMCardProcessingBase : public SMCustomerModeState
{

public:
	SMCardProcessingBase();
  virtual SMStateBase  *Initialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *DMScanner(void); //TAR252694
  virtual SMStateBase  *DMAcceptor(void); //SSCOADK-1110
  virtual void UnInitialize(void);
	
protected:
  DECLARE_DYNCREATE(SMCardProcessingBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMCardProcessing(void);
// or
//    DLLEXPORT SMStateBase *makeSMCardProcessingBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMCardProcessing objects.  This function is called by
// RETURNSTATE(CardProcessing) instead of calling new STATE(CardProcessing) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMCardProcessingBase.cpp and SMCardProcessing.cpp
// by using the macro IMPLEMENT_STATE(CardProcessing)

//DECLARE_STATE(CardProcessing)

#endif
