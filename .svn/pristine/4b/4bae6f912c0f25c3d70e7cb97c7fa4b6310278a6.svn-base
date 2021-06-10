//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMContinueTransBase.h
//
// TITLE: Class header for Continue Transaction state
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ContinueTransBaseSTATE
#define _ContinueTransBaseSTATE

#include "SMHelpModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMContinueTransBase : public STATE(HelpModeState)
{
public:
	SMContinueTransBase(void);
  virtual SMStateBase *Initialize(void);
  virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase *TimedOut(void);
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSButton2(void);
  virtual SMStateBase *DMCardReader(void);  //TAR323145
	
  DECLARE_DYNCREATE(SMContinueTransBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMContinueTrans(void);
// or
//    DLLEXPORT SMStateBase *makeSMContinueTransBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMContinueTrans objects.  This function is called by
// RETURNSTATE(ContinueTrans) instead of calling new STATE(ContinueTrans) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMContinueTransBase.cpp and SMContinueTrans.cpp
// by using the macro IMPLEMENT_STATE(ContinueTrans)

//DECLARE_STATE(ContinueTrans)

#endif
