//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAbortBase.h
//
// TITLE: Class header for store mode abort state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmAbortBaseSTATE
#define _SmAbortBaseSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmAbortBase : public STATE(StoreModeState)
{
public:
	SMSmAbortBase();
  virtual SMStateBase  *Initialize(void);
protected:
  virtual void UnInitialize(void);
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *PSEnterKey(void);
  virtual SMStateBase  *PSCancelKey(void);
  // NewUI
  virtual SMStateBase *PSReceiptUp(void);     // to disable and enable scroll buttons when 
  virtual SMStateBase *PSReceiptDown(void);   // to disable and enable scroll buttons when 
  // NewUI	
  DECLARE_DYNCREATE(SMSmAbortBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmAbort(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmAbortBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmAbort objects.  This function is called by
// RETURNSTATE(SmAbort) instead of calling new STATE(SmAbort) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmAbortBase.cpp and SMSmAbort.cpp
// by using the macro IMPLEMENT_STATE(SmAbort)

//DECLARE_STATE(SmAbort)

#endif
