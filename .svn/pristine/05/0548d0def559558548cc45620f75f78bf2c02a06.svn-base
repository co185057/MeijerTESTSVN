//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmEditWtTolerance.h
//
// TITLE: Class header for store mode Security Maintenance Mode
//
// AUTHOR: Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMMEDITWTTOLERANCESTATE
#define _SMMEDITWTTOLERANCESTATE

#include "SMSmmEditWtToleranceBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

SMSmmEditWtTolerance : public SMSmmEditWtToleranceBase
{
public:
    SMSmmEditWtTolerance();					  // constructor
    virtual SMStateBase * Initialize(void);
    virtual void     UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton8(void);

    DECLARE_DYNCREATE(SMSmmEditWtTolerance)// MFC Runtime class/object information
};

#endif
