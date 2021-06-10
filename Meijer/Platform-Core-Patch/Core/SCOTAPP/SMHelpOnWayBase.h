//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMHelpOnWayBase.h
//
// TITLE: Class header for Help Coming state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HelpOnWayBaseSTATE
#define _HelpOnWayBaseSTATE

#include "SMHelpModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMHelpOnWayBase : public STATE(HelpModeState)
{
public:
	SMHelpOnWayBase();
  virtual SMStateBase *Initialize(void);
  virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase *PSButtonHelp(void);
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSEnterKey(void);
  virtual SMStateBase *TimedOut(void);
  
  DECLARE_DYNCREATE(SMHelpOnWayBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMHelpOnWay(void);
// or
//    DLLEXPORT SMStateBase *makeSMHelpOnWayBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMHelpOnWay objects.  This function is called by
// RETURNSTATE(HelpOnWay) instead of calling new STATE(HelpOnWay) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMHelpOnWayBase.cpp and SMHelpOnWay.cpp
// by using the macro IMPLEMENT_STATE(HelpOnWay)

//DECLARE_STATE(HelpOnWay)

#endif
