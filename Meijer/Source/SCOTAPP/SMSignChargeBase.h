//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSignChargeBase.h
//
// TITLE: Class header for sign charge state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SignChargeBaseSTATE
#define _SignChargeBaseSTATE

#include "SMOperatorPasswordState.h"
//#include "SMConfirmSignature.h"		// Tar 194454 RF020702

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSignChargeBase : public STATE(OperatorPasswordState)
{
public:
  SMSignChargeBase();                            // constructor
  virtual SMStateBase * Initialize(void);        // required override
  virtual SMStateBase * TimedOut(void);          // optional override
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
  virtual SMStateBase  *OnWtDecrease(void);
  virtual void UnInitialize(void);		// RFC 330050

protected:
  //virtual SMStateBase * stateAfterPasswordVerify(void)  RETURNSTATE(ConfirmSignature) //Tar 194454 RF020702
  virtual SMStateBase * stateAfterPasswordVerify(void); 
  virtual void          showScreen(bool passwordVerified);
  virtual SMStateBase * PSButton1(void);

	
  DECLARE_DYNCREATE(SMSignChargeBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSignCharge(void);
// or
//    DLLEXPORT SMStateBase *makeSMSignChargeBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSignCharge objects.  This function is called by
// RETURNSTATE(SignCharge) instead of calling new STATE(SignCharge) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSignChargeBase.cpp and SMSignCharge.cpp
// by using the macro IMPLEMENT_STATE(SignCharge)

//DECLARE_STATE(SignCharge)

#endif
