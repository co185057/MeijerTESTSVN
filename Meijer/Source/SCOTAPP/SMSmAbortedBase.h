//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAbortedBase.h
//
// TITLE: Class header for store mode aborted state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmAbortedBaseSTATE
#define _SmAbortedBaseSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmAbortedBase : public STATE(StoreModeState)
{
public:
  SMSmAbortedBase();									  // constructor
  virtual SMStateBase  *Initialize(void);        // required override
	
protected:
  virtual void UnInitialize(void);
  virtual SMStateBase  *PSButton1(void);         // override for button 1
  virtual SMStateBase  *PSEnterKey(void);        // override for enter key
	
  DECLARE_DYNCREATE(SMSmAbortedBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmAborted(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmAbortedBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmAborted objects.  This function is called by
// RETURNSTATE(SmAborted) instead of calling new STATE(SmAborted) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmAbortedBase.cpp and SMSmAborted.cpp
// by using the macro IMPLEMENT_STATE(SmAborted)

//DECLARE_STATE(SmAborted)

#endif
