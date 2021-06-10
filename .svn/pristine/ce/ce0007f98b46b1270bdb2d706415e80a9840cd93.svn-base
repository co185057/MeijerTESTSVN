//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmPrintReceipt.h
//
// TITLE: Class header for store mode Print Receipt state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMPrintReceiptSTATE
#define _SMPrintReceiptSTATE

#include "SMSmPrintReceiptBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmPrintReceipt : public SMSmPrintReceiptBase
{
public:
    SMSmPrintReceipt();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);  
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton8(void); 

    DECLARE_DYNCREATE(SMSmPrintReceipt)// MFC Runtime class/object information
};

#endif
