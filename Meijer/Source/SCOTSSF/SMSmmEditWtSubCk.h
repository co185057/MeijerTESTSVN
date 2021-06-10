//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmEditWtSubCk.h
//
// TITLE: Class header for store mode Security Maintenance Mode
//
// AUTHOR: Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMMEDITWTSUBCKSTATE
#define _SMMEDITWTSUBCKSTATE

#include "SMSmmEditWtSubCkBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

SMSmmEditWtSubCk : public SMSmmEditWtSubCkBase
{
public:
    SMSmmEditWtSubCk();					// constructor
    virtual SMStateBase * Initialize(void);
    virtual void     UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButton8(void);


    DECLARE_DYNCREATE(SMSmmEditWtSubCk)	// MFC Runtime class/object information
};

#endif
