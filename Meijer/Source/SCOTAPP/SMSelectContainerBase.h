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

#ifdef _CPPUNIT
   #include "FakeSMCustomerModeState.h"
   #include "FakeSMStateBase.h"
#else
   #include "SMCustomerModeState.h"
#endif // _CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSelectContainerBase : public SMCUSTOMERMODESTATE
{
public:
	SMSelectContainerBase();
  virtual SMStateBase *Initialize(void);
#ifndef _CPPUNIT
  virtual bool PSAssistMenuLegalForState() { return true; };
  virtual void UnInitialize(void);		// RFC 330050
#endif // _CPPUNIT
protected:
  virtual SMStateBase *PSButton1(void);
#ifndef _CPPUNIT
  virtual SMStateBase *PSButton2(void);
#endif // _CPPUNIT
  virtual SMStateBase *PSButton3(void);
  virtual SMStateBase *PSButton4(void);
#ifndef _CPPUNIT
  virtual SMStateBase *PSButtonGoBack(void);
virtual SMStateBase  *PSCancelKey(void);/// override where needed
#endif // _CPPUNIT

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
