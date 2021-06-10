//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRemoveItemFromProduceScale.h
//
// TITLE: Class header for scale broken State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REMOVEITEMFROMPRODUCESCALESTATE
#define _REMOVEITEMFROMPRODUCESCALESTATE

#include "SMRemoveItemFromProduceScaleBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMRemoveItemFromProduceScale : public SMRemoveItemFromProduceScaleBase
{
public:
    SMRemoveItemFromProduceScale();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *DMScale(void);
    DECLARE_DYNCREATE(SMRemoveItemFromProduceScale)// MFC Runtime class/object information
};

#endif
