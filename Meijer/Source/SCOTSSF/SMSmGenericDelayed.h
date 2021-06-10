//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmGenericDelayed.h
//
// TITLE: Class header for void item state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMGENERICDELAYEDSTATE
#define _SMGENERICDELAYEDSTATE

#include "SMSmGenericDelayedBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmGenericDelayed : public SMSmGenericDelayedBase
{
public:
    SMSmGenericDelayed();
    virtual SMStateBase *Initialize(void);
    virtual void     UnInitialize(void);
    virtual bool PSAssistMenuLegalForState();
protected:


    DECLARE_DYNCREATE(SMSmGenericDelayed)// MFC Runtime class/object information

};

#endif //_SMGenericDelayedSTATE