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
#include <afxtempl.h>               // MS MFC template header
#include <mmsystem.h>
#include "resource.h"

#include "Common.h"                 // MGV common includes

#include "SMOutOfService.h"
#define COMP_ID ID_SM               // base state component
#define T_ID    _T("OutOfService")


DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMOutOfService, CObject)// MFC Runtime class/object information

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

