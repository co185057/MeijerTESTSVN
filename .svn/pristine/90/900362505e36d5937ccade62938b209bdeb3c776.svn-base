//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMElectronicBenefitsTransfer.H
//
// TITLE:    Class header for Electronic Benefits Transfer State
//
//
// AUTHOR:   Michael A. Prather
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ELECTRONICBENEFITSTRANSFERSTATE
#define _ELECTRONICBENEFITSTRANSFERSTATE

#include "SMElectronicBenefitsTransferBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMElectronicBenefitsTransfer : public SMElectronicBenefitsTransferBase
{
public:
    SMElectronicBenefitsTransfer(const bool);
    SMElectronicBenefitsTransfer();
    SMStateBase *Deliver(const bool);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *Initialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *PSCancelKey(void);
    virtual void UnInitialize(void);

    DECLARE_DYNCREATE(SMElectronicBenefitsTransfer)// MFC Runtime class/object information

};

#endif
