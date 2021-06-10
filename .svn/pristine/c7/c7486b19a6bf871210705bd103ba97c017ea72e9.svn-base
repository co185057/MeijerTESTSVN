//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmOutOfService.CPP
//
// TITLE: Class implementation for store mode Out Of Service state
//
// AUTHOR:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"                 // MGV common includes

#include "SMSmOutOfService.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmOutOfService")

IMPLEMENT_DYNCREATE(SMSmOutOfService, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSmOutOfService::SMSmOutOfService()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmOutOfService::Initialize(void)
{
    return SMSmOutOfServiceBase::Initialize();
}

