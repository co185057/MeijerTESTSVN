//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMScaleBroken.h
//
// TITLE: Class header for scale broken State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SCALEBROKENSTATE
#define _SCALEBROKENSTATE

#include "SMScaleBrokenBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMScaleBroken : public SMScaleBrokenBase
{
public:
    SMScaleBroken();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *DMScale(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    DECLARE_DYNCREATE(SMScaleBroken)// MFC Runtime class/object information
};

#endif
