//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMPutBagOnScale.h
//
// TITLE: Class header for Put Bag On Scale State - added for SR085
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PUTBAGONSCALESTATE
#define _PUTBAGONSCALESTATE

#include "SMPutBagOnScaleBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMPutBagOnScale : public SMPutBagOnScaleBase
{
public:
    SMPutBagOnScale();

    virtual SMStateBase *Initialize(void);
    virtual void UnInitialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);

protected:
    virtual SMStateBase *PSButtonHelp(void);
    virtual SMStateBase *PSButton2(void);
    virtual SMStateBase *PSEnterKey(void);
    virtual SMStateBase *TareNone(void);
    virtual SMStateBase *TareOnlyAtAttract(void);
    virtual SMStateBase *TimedOut(void);


    DECLARE_DYNCREATE(SMPutBagOnScale)// MFC Runtime class/object information

};

#endif