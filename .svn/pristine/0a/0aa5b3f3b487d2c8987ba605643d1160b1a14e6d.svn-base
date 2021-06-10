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
	
  DECLARE_DYNCREATE(SMElectronicBenefitsTransfer)// MFC Runtime class/object information
		
};

#endif
