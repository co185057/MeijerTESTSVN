//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmEditWtExp.h
//
// TITLE: Class header for store mode Security Maintenance Mode
//
// AUTHOR: Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMMEDITWTEXPSTATE
#define _SMMEDITWTEXPSTATE

#include "SMSmmEditWtExpBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

SMSmmEditWtExp : public SMSmmEditWtExpBase
{
public:
    SMSmmEditWtExp();					// constructor
    virtual SMStateBase * Initialize(void);
    virtual void     UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButton4(void);
    virtual SMStateBase  *PSButton8(void);

    DECLARE_DYNCREATE(SMSmmEditWtExp)	// MFC Runtime class/object information
};

#endif
