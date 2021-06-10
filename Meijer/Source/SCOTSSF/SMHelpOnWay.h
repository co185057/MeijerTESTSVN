//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMHelpOnWay.h
//
// TITLE: Class header for help is coming state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HELPONWAYSTATE
#define _HELPONWAYSTATE

#include "SMHelpOnWayBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMHelpOnWay : public SMHelpOnWayBase
{
public:
    SMHelpOnWay();
    virtual SMStateBase *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase *PSButtonHelp(void);
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *PSEnterKey(void);
    virtual SMStateBase *TimedOut(void);
    DECLARE_DYNCREATE(SMHelpOnWay)// MFC Runtime class/object information
};

#endif