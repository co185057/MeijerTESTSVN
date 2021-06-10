//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmEBTAmount.h
//
// TITLE: Class header for Confirm EBT
//
//
// AUTHOR:    Michael Prather
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ConfirmEBTAmountSTATE
#define _ConfirmEBTAmountSTATE

#include "SMConfirmEBTAmountBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMConfirmEBTAmount : public SMConfirmEBTAmountBase
{

public:
	SMConfirmEBTAmount(const TBTENDERTYPE, const long, const long);
	SMConfirmEBTAmount();
	SMStateBase *Deliver(const TBTENDERTYPE, const long, const long);

    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *Initialize(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButtonGoBack(void);

    DECLARE_DYNCREATE(SMConfirmEBTAmount)// MFC Runtime class/object information
};

#endif
