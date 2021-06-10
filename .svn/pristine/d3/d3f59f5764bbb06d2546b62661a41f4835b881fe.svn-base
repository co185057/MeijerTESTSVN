//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecurityLevel3Base.h
//
// TITLE: Class header for Level 3 Security agent State.
//
//
// AUTHOR:    DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SecurityLevel3Base
#define _SecurityLevel3Base

#include "SMOperatorPasswordState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSecurityLevel3Base : public STATE(OperatorPasswordState)
{
public:
  SMSecurityLevel3Base(CString csBar, CString csPop);
  SMSecurityLevel3Base();
  SMStateBase *Deliver(CString csBar, CString csPop);
  virtual SMStateBase  *Initialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
  virtual SMStateBase  *OnWtDecrease(void);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSCancelKey(void);
  virtual void     showScreen(bool passwordVerified);
  // Stay here after operator/password entered
  virtual SMStateBase * stateAfterPasswordVerify() { return STATE_NULL; }

  //@@@INFOPOINT
  virtual SMStateBase  *PSButtonGoBack(void);         // override for GoBack Key
  //@@@INFOPOINT

  
//private:
  CString csBarMsg, csPopUpMsg;
	
  DECLARE_DYNCREATE(SMSecurityLevel3Base)// MFC Runtime class/object information
};

#endif
