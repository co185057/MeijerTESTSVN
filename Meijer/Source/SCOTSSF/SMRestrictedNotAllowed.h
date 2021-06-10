//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRestrictedNotAllowed.h
//
// TITLE: Class header for unknown Item State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RESTRICTEDNOTALLOWEDSTATE
#define _RESTRICTEDNOTALLOWEDSTATE

#include "SMRestrictedNotAllowedBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMRestrictedNotAllowed : public SMRestrictedNotAllowedBase
{
public:
    SMRestrictedNotAllowed(const PSTEXT);
    SMRestrictedNotAllowed();
    SMStateBase *Deliver(const PSTEXT);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase *Initialize(void);
    virtual bool PSAssistMenuLegalForState();
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *PSEnterKey(void);

    DECLARE_DYNCREATE(SMRestrictedNotAllowed)// MFC Runtime class/object information
};

#endif
