//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMHostOffLineBase.h
//
// TITLE: Class header for host offline state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HostOffLineBaseSTATE
#define _HostOffLineBaseSTATE

#include "SMOperatorPasswordState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMHostOffLineBase : public STATE(OperatorPasswordState)
{
public:
	SMHostOffLineBase();
	virtual SMStateBase *Initialize(void);
	virtual void         UnInitialize(void);
    virtual void         showScreen(bool passwordVerified = false);
	virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
    virtual SMStateBase  *OnWtDecrease(void);
	virtual SMStateBase  *OnMatchedWt(void);
  
  DECLARE_DYNCREATE(SMHostOffLineBase) // MFC Runtime class/object information
};

#endif
