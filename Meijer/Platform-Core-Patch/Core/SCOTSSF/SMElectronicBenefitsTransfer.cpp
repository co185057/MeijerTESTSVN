//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMElectronicBenefitsTransfer.CPP
//
// TITLE:    Class implementation for Electronic Benefits Transfer State
//
//
// AUTHOR:   Michael Prather
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMElectronicBenefitsTransfer.h"      // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ElectronicBenefitsTransfer")

IMPLEMENT_DYNCREATE(SMElectronicBenefitsTransfer, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMElectronicBenefitsTransfer::SMElectronicBenefitsTransfer(const bool fCshBck)
: SMElectronicBenefitsTransferBase(fCshBck)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMElectronicBenefitsTransfer::SMElectronicBenefitsTransfer()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMElectronicBenefitsTransfer::Deliver(const bool fCshBck)
{
  SMElectronicBenefitsTransferBase::Deliver(fCshBck);

  return this;
}


