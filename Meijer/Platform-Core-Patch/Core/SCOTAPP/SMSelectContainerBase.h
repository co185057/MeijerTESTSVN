//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectContainerBase.h
//
// TITLE: Class header for void item state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SELECTCONTAINERBASESTATE
#define _SELECTCONTAINERBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSelectContainerBase : public SMCustomerModeState
{
public:
	SMSelectContainerBase();
  virtual SMStateBase *Initialize(void);
  virtual bool PSAssistMenuLegalForState() { return true; };
  virtual void UnInitialize(void);		// RFC 330050
protected:
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSButton2(void);
  virtual SMStateBase *PSButton3(void);
  virtual SMStateBase *PSButton4(void);
  virtual SMStateBase *PSButtonGoBack(void);
virtual SMStateBase  *PSCancelKey(void);/// override where needed

  DECLARE_DYNCREATE(SMSelectContainerBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSelectContainer(void);
// or
//    DLLEXPORT SMStateBase *makeSMSelectContainerBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSelectContainer objects.  This function is called by
// RETURNSTATE(SelectContainer) instead of calling new STATE(SelectContainer) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSelectContainerBase.cpp and SMSelectContainer.cpp
// by using the macro IMPLEMENT_STATE(SelectContainer)

//DECLARE_STATE(SelectContainer)

#endif
