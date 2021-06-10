//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMScanCard.h
//
// TITLE: Class header for Confirn Signature
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ScanCardSTATE
#define _ScanCardSTATE

#include "SMScanCardBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMScanCard : public SMScanCardBase
{

public:

    SMScanCard(const TBTENDERTYPE, const long);
    SMScanCard();
    SMStateBase *Deliver(const TBTENDERTYPE, const long);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase *Initialize(void);
    virtual void     UnInitialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);

protected:
    virtual SMStateBase *PSCancelKey(void);
    virtual SMStateBase *SAEventOccurred(const long);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *DMScanner(void);

    DECLARE_DYNCREATE(SMScanCard)// MFC Runtime class/object information
};


#endif
