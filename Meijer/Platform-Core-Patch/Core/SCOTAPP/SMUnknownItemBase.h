//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMUnknownItemBase.h
//
// TITLE: Class header for Unknown Item State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _UnknownItemBaseSTATE
#define _UnknownItemBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMUnknownItemBase : public SMCustomerModeState
{
public:
	SMUnknownItemBase();
  virtual SMStateBase *Initialize(void);
  virtual void UnInitialize(void);
  virtual bool PSAssistMenuLegalForState() { return true; };
protected:
  virtual SMStateBase *TimedOut(void);
  virtual SMStateBase *PSButtonHelp(void);
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSEnterKey(void);

  static bool m_bFromEnterCoupons; //dp185016 tar 286763
  
	
  DECLARE_DYNCREATE(SMUnknownItemBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMUnknownItem(void);
// or
//    DLLEXPORT SMStateBase *makeSMUnknownItemBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMUnknownItem objects.  This function is called by
// RETURNSTATE(UnknownItem) instead of calling new STATE(UnknownItem) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMUnknownItemBase.cpp and SMUnknownItem.cpp
// by using the macro IMPLEMENT_STATE(UnknownItem)

//DECLARE_STATE(UnknownItem)

#endif
