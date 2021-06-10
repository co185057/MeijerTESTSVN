//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMContinueTrans.h
//
// TITLE: Class header for unknown continue transaction state
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CONTINUETRANSSTATE
#define _CONTINUETRANSSTATE

#include "SMContinueTransBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMContinueTrans : public SMContinueTransBase
{
public:
    SMContinueTrans(void);
    virtual SMStateBase *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase *TimedOut(void);
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *PSButton2(void);
    virtual SMStateBase *DMCardReader(void);

	virtual SMStateBase *DMAcceptor(void);

    DECLARE_DYNCREATE(SMContinueTrans)// MFC Runtime class/object information
};

#endif