//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMTransportItemBase.h
//
// TITLE:	Class header for Transport Item Base state
//
// AUTHOR:  Thanhhieu Pham, Kenny Pitt
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TRANSPORTITEMBASESTATE
#define _TRANSPORTITEMBASESTATE
#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMTransportItemBase : public SMCustomerModeState
{
public:
  SMTransportItemBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);          // optional
  virtual SMStateBase  *TimedOut(void);     //Tar202552
protected:
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnBackToLGW(void);			// Smart Scale reported that the unmatched weight was removed

  virtual SMStateBase  *DMTakeawayReadyForItem();	// takeaway belt event
  virtual SMStateBase  *DMScanner(void);			// scanning allowed when leaving this state 211050 LPM081602

  DECLARE_DYNCREATE(SMTransportItemBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMDeactivateTheftTag(void);
// or
//    DLLEXPORT SMStateBase *makeSMDeactivateTheftTagBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMTransportItem objects.  This function is called by
// RETURNSTATE(TransportItem) instead of calling new STATE(TransportItem) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMTransportItemBase.cpp and SMTransportItem.cpp
// by using the macro IMPLEMENT_STATE(TransportItem)

//DECLARE_STATE(TransportItem)

#endif
