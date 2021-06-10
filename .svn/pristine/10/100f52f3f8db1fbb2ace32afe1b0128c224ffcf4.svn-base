//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMContextHelpBase.h
//
// TITLE: Class impliementation for contextual help state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ContextHelpBaseSTATE
#define _ContextHelpBaseSTATE

#include "SMOperatorPasswordState.h"
//#include "SMSmAuthorization.h"		//Tar 194454 RF020702

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMContextHelpBase : public STATE(OperatorPasswordState)
{
public:
	SMContextHelpBase();
	virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
    virtual SMStateBase  *OnWtDecrease(void);
	virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase *Initialize(void);
  virtual SMStateBase *PSButtonHelp(void);
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSButton2(void);

  virtual SMStateBase *PSButton3(void);
  virtual SMStateBase *PSCancelKey(void);
  virtual SMStateBase *PSListLookup(CString);
  virtual SMStateBase *TimedOut(void);
  virtual SMStateBase *stateAfterPasswordVerify(void) ;
  
  virtual void     showScreen(bool passwordVerified);

  //@@@INFOPOINT
  virtual SMStateBase *PSButtonGoBack(void);
  //@@@INFOPOINT

  DECLARE_DYNCREATE(SMContextHelpBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMContextHelp(void);
// or
//    DLLEXPORT SMStateBase *makeSMContextHelpBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMContextHelp objects.  This function is called by
// RETURNSTATE(ContextHelp) instead of calling new STATE(ContextHelp) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMContextHelpBase.cpp and SMContextHelp.cpp
// by using the macro IMPLEMENT_STATE(ContextHelp)

//DECLARE_STATE(ContextHelp)

#endif
