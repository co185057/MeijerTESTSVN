//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRemoveItemFromProduceScaleBase.h
//
// TITLE: Class header for scale broken State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RemoveItemFromProduceScaleBaseSTATE
#define _RemoveItemFromProduceScaleBaseSTATE

#ifndef _CPPUNIT
#include "SMHelpModeState.h"
#else
#include "FakeSMHelpModeStateBase.h"
#endif //_CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMRemoveItemFromProduceScaleBase : public STATE(HelpModeState)
{
public:
  SMRemoveItemFromProduceScaleBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase  *TimedOut(void);
  virtual SMStateBase  *DMScale(void);
  DECLARE_DYNCREATE(SMRemoveItemFromProduceScaleBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMRemoveItemFromProduceScale(void);
// or
//    DLLEXPORT SMStateBase *makeSMRemoveItemFromProduceScaleBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMRemoveItemFromProduceScale objects.  This function is called by
// RETURNSTATE(RemoveItemFromProduceScale) instead of calling new STATE(RemoveItemFromProduceScale) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMRemoveItemFromProduceScaleBase.cpp and SMRemoveItemFromProduceScale.cpp
// by using the macro IMPLEMENT_STATE(RemoveItemFromProduceScale)

//DECLARE_STATE(RemoveItemFromProduceScale)

#endif
