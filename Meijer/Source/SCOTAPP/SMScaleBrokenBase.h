//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMScaleBrokenBase.h
//
// TITLE: Class header for scale broken State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ScaleBrokenBaseSTATE
#define _ScaleBrokenBaseSTATE

#include "SMHelpModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMScaleBrokenBase : public STATE(HelpModeState)
{
public:
	SMScaleBrokenBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase  *TimedOut(void);
  virtual SMStateBase  *DMScale(void);
  virtual SMStateBase  *PSButtonGoBack(void);
  DECLARE_DYNCREATE(SMScaleBrokenBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMScaleBroken(void);
// or
//    DLLEXPORT SMStateBase *makeSMScaleBrokenBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMScaleBroken objects.  This function is called by
// RETURNSTATE(ScaleBroken) instead of calling new STATE(ScaleBroken) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMScaleBrokenBase.cpp and SMScaleBroken.cpp
// by using the macro IMPLEMENT_STATE(ScaleBroken)

//DECLARE_STATE(ScaleBroken)

#endif
