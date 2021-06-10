//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMFatalErrorBase.h
//
// TITLE: Class impliementation for contextual help state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _FatalErrorBaseSTATE
#define _FatalErrorBaseSTATE

#include "SMOperatorPasswordState.h"
//#include "SMSmAuthorization.h"		//Tar 194454 RF020702

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMFatalErrorBase : public STATE(OperatorPasswordState)
{
public:
	SMFatalErrorBase();
	virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
	virtual SMStateBase *Deliver();
    virtual SMStateBase  *OnWtDecrease(void);
	virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase *Initialize(void);
  virtual SMStateBase *PSButtonHelp(void);
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

  DECLARE_DYNCREATE(SMFatalErrorBase)// MFC Runtime class/object information
};

#endif
