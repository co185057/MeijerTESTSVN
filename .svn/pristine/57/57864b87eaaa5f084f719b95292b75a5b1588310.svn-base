//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterWeightBase.h
//
// TITLE: Class header for enter weight state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _EnterWeightBaseSTATE
#define _EnterWeightBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMEnterWeightBase : public SMCustomerModeState
{
public:
	SMEnterWeightBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize();
  virtual bool PSAssistMenuLegalForState() { return true; };
protected:
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSCancelKey(void);
  virtual SMStateBase  *DMScale(void);
  virtual SMStateBase  *PSButtonGoBack(void);
  CString csCurContext;    // Tar 238987
  
  DECLARE_DYNCREATE(SMEnterWeightBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMEnterWeight(void);
// or
//    DLLEXPORT SMStateBase *makeSMEnterWeightBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMEnterWeight objects.  This function is called by
// RETURNSTATE(EnterWeight) instead of calling new STATE(EnterWeight) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMEnterWeightBase.cpp and SMEnterWeight.cpp
// by using the macro IMPLEMENT_STATE(EnterWeight)

//DECLARE_STATE(EnterWeight)

#endif
