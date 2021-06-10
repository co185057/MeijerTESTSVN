//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmEditAdd.h
//
// TITLE: Class header for store mode Security Maintenance Mode
//
// AUTHOR: Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMMEDITADDSTATE
#define _SMMEDITADDSTATE

#include "SMSmmEditAddBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

SMSmmEditAdd : public SMSmmEditAddBase
{
public:
    SMSmmEditAdd();					// constructor
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *DMScanner(void);
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButton4(void);
    virtual SMStateBase  *PSButton5(void);
    virtual SMStateBase  *PSButton6(void);
    virtual SMStateBase  *PSButton7(void);
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase *PSReceiptUp(void);
    virtual SMStateBase *PSReceiptDown(void);
    virtual SMStateBase *DMScannerEncrypted(void);

    DECLARE_DYNCREATE(SMSmmEditAdd)		// MFC Runtime class/object information
};

#endif
