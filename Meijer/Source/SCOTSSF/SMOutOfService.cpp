//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOutOfService.CPP
//
// TITLE: Class implementation for out of service state
//
//
// AUTHOR:    Peter Denhoed, Updated DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#include <mmsystem.h>
#include "resource.h"

#include "Common.h"                 // MGV common includes

#include "SMOutOfService.h"
#include "LaneStateData.h"
#define COMP_ID ID_SM               // base state component
#define T_ID    _T("OutOfService")


DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMOutOfService, CObject)// MFC Runtime class/object information

#ifdef _TRADESHOWDEMO
#include "..\TradeShowDemo\SMOutOfServiceTSDemo.cpp"
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
SMOutOfService::SMOutOfService(const int nWhy)
: SMOutOfServiceBase(nWhy)
{
  IMPLEMENT_TIMESTAMP
		
}

////////////////////////////////////////
SMOutOfService::SMOutOfService()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMOutOfService::Deliver(const int nWhy)
{
  SMOutOfServiceBase::Deliver(nWhy);

  return this;
}

bool SMOutOfService::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMOutOfServiceBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMOutOfService::Initialize(void)
{
    return SMOutOfServiceBase::Initialize();
}

void SMOutOfService::UnInitialize(void)
{
    SMOutOfServiceBase::UnInitialize();
}

#ifndef _TRADESHOWDEMO
SMStateBase * SMOutOfService::InitializeAll(void)
{
    SMStateBase* retState = SMOutOfServiceBase::InitializeAll();
    eoSSF.Initialize(&to);
    return retState;
}
#endif//

SMStateBase * SMOutOfService::UnInitializeAll(void)
{
    eoSSF.Uninitialize();
    return SMOutOfServiceBase::UnInitializeAll();
}

SMStateBase * SMOutOfService::ProcessMaintenance(void)
{
    return SMOutOfServiceBase::ProcessMaintenance();
}

long SMOutOfService::DMConfigureScaleDisplay(void)
{
    return SMOutOfServiceBase::DMConfigureScaleDisplay();
}

void SMOutOfService::TBShow2x20(void)
{
    SMOutOfServiceBase::TBShow2x20();
}

void SMOutOfService::UpdateTransactionCountRegistry(void)
{
    SMOutOfServiceBase::UpdateTransactionCountRegistry();
}

SMStateBase * SMOutOfService::DMHandleStartupError(void)
{
    return SMOutOfServiceBase::DMHandleStartupError();
}

long SMOutOfService::DMPrintVersion(void)
{
    return SMOutOfServiceBase::DMPrintVersion();
}

long SMOutOfService::SecMgrInitialize(void)
{
    return SMOutOfServiceBase::SecMgrInitialize();
}
