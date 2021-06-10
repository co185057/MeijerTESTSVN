//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMHostOffLine.h
//
// TITLE: Class header for host offline state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HOSTOFFLINE
#define _HOSTOFFLINE

#include "SMHostOffLineBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMHostOffLine : public SMHostOffLineBase
{
public:
    SMHostOffLine();
    virtual SMStateBase *Initialize(void);
    virtual void         UnInitialize(void);
    virtual void         showScreen(bool passwordVerified = false);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnMatchedWt(void);

    DECLARE_DYNCREATE(SMHostOffLine) // MFC Runtime class/object information
};

#endif
