//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmInvalidWtTol.h
//
// TITLE: Class header for store mode Security Maintenance Mode
//
// AUTHOR: Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMMINVALIDWTTOLSTATE
#define _SMMINVALIDWTTOLSTATE

#include "SMSmmInvalidWtTolBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

SMSmmInvalidWtTol : public SMSmmInvalidWtTolBase
{
public:
    SMSmmInvalidWtTol();				// constructor
    virtual SMStateBase * Initialize(void);
    virtual void     UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);

    DECLARE_DYNCREATE(SMSmmInvalidWtTol)// MFC Runtime class/object information
};

#endif
