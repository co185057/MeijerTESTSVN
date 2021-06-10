//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecurityLevel2Base.h
//
// TITLE: Class header for Level 2 Security agent State.
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SecurityLevel2Base
#define _SecurityLevel2Base

#include "SMHelpModeState.h"
 
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSecurityLevel2Base : public STATE(HelpModeState)
{
public:
  SMSecurityLevel2Base(CString csBar, CString csPop);
	SMSecurityLevel2Base();
	SMStateBase *Deliver(CString csBar, CString csPop);
  virtual SMStateBase * Initialize(void);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase * PSButton1(void);
	
//private:
  CString csBarMsg, csPopUpMsg;
	
  DECLARE_DYNCREATE(SMSecurityLevel2Base)// MFC Runtime class/object information
};

//DECLARE_STATE(SecurityLevel2)

#endif
