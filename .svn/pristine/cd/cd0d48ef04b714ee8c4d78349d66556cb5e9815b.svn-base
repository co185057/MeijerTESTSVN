//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterID.h
//
// TITLE: Class header for Enter ID state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ENTERIDSTATE
#define _ENTERIDSTATE

#include "SMEnterIDBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMEnterID : public SMEnterIDBase
{
public:
    SMEnterID();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSNumericKey(const long);
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase  *PSEnterKey(void);
    virtual SMStateBase  *PSButtonGoBack(void);

    DECLARE_DYNCREATE(SMEnterID)// MFC Runtime class/object information
};

#endif