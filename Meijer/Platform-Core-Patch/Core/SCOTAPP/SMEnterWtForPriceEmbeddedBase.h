//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterWtForPriceEmbeddedBase.h
//
// TITLE: Class header for enter weight state
//
// AUTHOR:  Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _EnterWtForPriceEmbeddedBaseSTATE
#define _EnterWtForPriceEmbeddedBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMEnterWtForPriceEmbeddedBase : public SMCustomerModeState
{
public:
	SMEnterWtForPriceEmbeddedBase();
  virtual SMStateBase  *Initialize(void);
	virtual bool PSAssistMenuLegalForState() { return true; };
	virtual void UnInitialize(void);		// RFC 330050
protected:
  virtual SMStateBase  *DMScale(void);
  virtual SMStateBase  *PSButtonGoBack(void);
  
  DECLARE_DYNCREATE(SMEnterWtForPriceEmbeddedBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMEnterWtForPriceEmbedded(void);
// or
//    DLLEXPORT SMStateBase *makeSMEnterWtForPriceEmbeddedBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMEnterWtForPriceEmbedded objects.  This function is called by
// RETURNSTATE(EnterWtForPriceEmbedded) instead of calling new STATE(EnterWtForPriceEmbedded)
// so that SCOTSSF works correctly.
//
// The function is implemented in SMEnterWtForPriceEmbeddedBase.cpp and SMEnterWtForPriceEmbedded.cpp
// by using the macro IMPLEMENT_STATE(EnterWtForPriceEmbedded)

//DECLARE_STATE(EnterWtForPriceEmbedded)

#endif
