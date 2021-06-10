//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOutOfService.h
//
// TITLE: Class header for wait for helpf for bad card state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _OUTOFSERVICESTATE
#define _OUTOFSERVICESTATE

#include "SMOutOfServiceBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMOutOfService : public SMOutOfServiceBase
{
public:
    SMOutOfService(const int);
    SMOutOfService();
    SMStateBase *Deliver(const int);

    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase *Initialize(void);
    virtual void     UnInitialize(void);
    virtual SMStateBase *InitializeAll(void);
    virtual SMStateBase *UnInitializeAll(void);
    virtual SMStateBase *ProcessMaintenance(void);
    virtual long DMConfigureScaleDisplay(void);

protected:
    virtual void TBShow2x20(void);
    virtual void UpdateTransactionCountRegistry(void);
    virtual SMStateBase * DMHandleStartupError(void);
    virtual long DMPrintVersion(void);
    virtual long SecMgrInitialize(void);

    DECLARE_DYNCREATE(SMOutOfService)// MFC Runtime class/object information

};

#endif
