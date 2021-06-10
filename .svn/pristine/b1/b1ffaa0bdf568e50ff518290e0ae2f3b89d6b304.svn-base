//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMUnknownPriceBase.h
//
// TITLE: Class header for Unknown Price State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _UnknownPriceBaseSTATE
#define _UnknownPriceBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMUnknownPriceBase : public SMCustomerModeState
{
public:
  SMUnknownPriceBase();                 // constructor
  virtual SMStateBase *Initialize(void);        // required override
  virtual bool PSAssistMenuLegalForState() { return true; };
  virtual void UnInitialize(void);		// RFC 330050
protected:
  virtual SMStateBase *PSButton1(void);         // override for button 1
  virtual SMStateBase *PSEnterKey(void);        // override for enter key
	
  DECLARE_DYNCREATE(SMUnknownPriceBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMUnknownPrice(void);
// or
//    DLLEXPORT SMStateBase *makeSMUnknownPriceBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMUnknownPrice objects.  This function is called by
// RETURNSTATE(UnknownPrice) instead of calling new STATE(UnknownPrice) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMUnknownPriceBase.cpp and SMUnknownPrice.cpp
// by using the macro IMPLEMENT_STATE(UnknownPrice)

//DECLARE_STATE(UnknownPrice)

#endif
