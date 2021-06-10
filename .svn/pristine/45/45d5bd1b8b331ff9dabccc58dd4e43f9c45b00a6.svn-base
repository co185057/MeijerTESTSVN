//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRestrictedAgeYN.h
//
// TITLE: Class header for store mode abort state
//
//
// AUTHOR:  
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMRESTRICTEDAGEYN
#define __SMRESTRICTEDAGEYN

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMRestrictedAgeYN: public SMCustomerModeState
{
public:
    SMRestrictedAgeYN()
		;
    virtual SMStateBase  *Initialize(void);
//    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
//    virtual SMStateBase  *OnWtDecrease(void);

protected:
    virtual void UnInitialize(void);
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
//    virtual SMStateBase  *PSEnterKey(void);
//    virtual SMStateBase  *PSCancelKey(void);
//    virtual SMStateBase *PSReceiptUp(void);
//    virtual SMStateBase *PSReceiptDown(void);

    DECLARE_DYNCREATE(SMRestrictedAgeYN)// MFC Runtime class/object information
};

#endif