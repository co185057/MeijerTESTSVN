//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMUnknownItem.h
//
// TITLE: Class header for unknown Item State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _UNKNOWNITEMSTATE
#define _UNKNOWNITEMSTATE

#include "SMUnknownItemBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMUnknownItem : public SMUnknownItemBase
{
public:
    SMUnknownItem();
    virtual SMStateBase *Initialize(void);
    virtual void UnInitialize(void);
    virtual bool PSAssistMenuLegalForState();
protected:
    virtual SMStateBase *TimedOut(void);
    virtual SMStateBase *PSButtonHelp(void);
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *PSEnterKey(void);

    DECLARE_DYNCREATE(SMUnknownItem)// MFC Runtime class/object information

};

#endif