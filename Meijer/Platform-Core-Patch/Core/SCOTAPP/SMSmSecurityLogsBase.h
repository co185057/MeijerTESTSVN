//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSecurityLogsBase.h
//
// TITLE: Class header for store mode Security Logs state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmSecurityLogsBaseSTATE
#define _SmSecurityLogsBaseSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmSecurityLogsBase : public STATE(StoreModeState)
{
public:
	SMSmSecurityLogsBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton8(void);
	
  DECLARE_DYNCREATE(SMSmSecurityLogsBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmSecurityLogs(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmSecurityLogsBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmSecurityLogs objects.  This function is called by
// RETURNSTATE(SmSecurityLogs) instead of calling new STATE(SmSecurityLogs) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmSecurityLogsBase.cpp and SMSmSecurityLogs.cpp
// by using the macro IMPLEMENT_STATE(SmSecurityLogs)

//DECLARE_STATE(SmSecurityLogs)

#endif
