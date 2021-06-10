//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmOutOfServiceBase.h
//
// TITLE: Class header for store mode Out Of Service state
//
// Description: This state inherits from StoreModeState.  It represents
//   a Lane Closed condition for the terminal.
//
//
// AUTHOR:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmOutOfServiceBaseSTATE
#define _SmOutOfServiceBaseSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmOutOfServiceBase : public STATE(StoreModeState)
{
public:
	SMSmOutOfServiceBase();
  virtual SMStateBase  *Initialize(void);
	
protected:
	
  DECLARE_DYNCREATE(SMSmOutOfServiceBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmOutOfService(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmOutOfServiceBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmOutOfService objects.  This function is called by
// RETURNSTATE(SmOutOfService) instead of calling new STATE(SmOutOfService) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmOutOfServiceBase.cpp and SMSmOutOfService.cpp
// by using the macro IMPLEMENT_STATE(SmOutOfService)

//DECLARE_STATE(SmOutOfService)

#endif
