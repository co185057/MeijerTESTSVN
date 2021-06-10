//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCustomMessageBase.h
//
// TITLE: Class header for Custom Message State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CustomMessageBaseSTATE
#define _CustomMessageBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMCustomMessageBase : public SMCustomerModeState
{
public:
  SMCustomMessageBase();

  virtual SMStateBase *Initialize(void);
  virtual void UnInitialize(void);
  virtual bool PSAssistMenuLegalForState() { return true; };

protected:
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSEnterKey(void);
  DECLARE_DYNCREATE(SMCustomMessageBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMCustomMessage(void);
// or
//    DLLEXPORT SMStateBase *makeSMCustomMessageBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMCustomMessage objects.  This function is called by
// RETURNSTATE(CustomMessage) instead of calling new STATE(CustomMessage) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMCustomMessageBase.cpp and SMCustomMessage.cpp
// by using the macro IMPLEMENT_STATE(CustomMessage)

//DECLARE_STATE(CustomMessage)

#endif
