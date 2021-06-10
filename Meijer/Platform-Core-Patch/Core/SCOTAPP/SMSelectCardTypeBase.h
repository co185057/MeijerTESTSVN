//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMSelectCardTypeBase.H
//
// TITLE:    Class header for Select Card Type State
//
//
// AUTHOR:   Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SELECTCARDTYPEBASESTATE
#define _SELECTCARDTYPEBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
SMSelectCardTypeBase : public SMCustomerModeState
{
public:
  SMSelectCardTypeBase();
  virtual SMStateBase *Initialize(void);
  virtual void     UnInitialize(void);
  virtual SMStateBase  *OnWtDecrease(void);
protected:
  bool m_bAllowEBTScreen;
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSButton2(void);
  virtual SMStateBase *PSButton3(void);
  virtual SMStateBase *PSCancelKey(void);
  virtual SMStateBase *PSButton4(void);
  virtual SMStateBase *SAEventOccurred(const long);
  //@@@INFOPOINT
  virtual SMStateBase  *PSButtonGoBack(void);         // override for GoBack Key
  //@@@INFOPOINT

	
  DECLARE_DYNCREATE(SMSelectCardTypeBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSelectCardType(void);
// or
//    DLLEXPORT SMStateBase *makeSMSelectCardTypeBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSelectCardType objects.  This function is called by
// RETURNSTATE(SelectCardType) instead of calling new STATE(SelectCardType) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSelectCardTypeBase.cpp and SMSelectCardType.cpp
// by using the macro IMPLEMENT_STATE(SelectCardType)

//DECLARE_STATE(SelectCardType)

#endif
