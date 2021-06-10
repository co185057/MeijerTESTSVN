//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecurityLevel2.h
//
// TITLE: Class header for Level 3 Security agent State.
//
//
// AUTHOR:  DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECURITYLEVEL2
#define _SECURITYLEVEL2

#include "SMSecurityLevel2Base.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecurityLevel2 : public SMSecurityLevel2Base
{
public:
	SMSecurityLevel2(CString csBar, CString csPop);
	SMSecurityLevel2();
	SMStateBase *Deliver(CString csBar, CString csPop);
	
  DECLARE_DYNCREATE(SMSecurityLevel2)// MFC Runtime class/object information
};

#endif
