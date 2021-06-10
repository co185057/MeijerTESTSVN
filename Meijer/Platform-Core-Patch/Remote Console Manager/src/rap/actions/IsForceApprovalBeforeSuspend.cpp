// $Header:
/*
 * @(#)IsForceApprovalBeforeSuspend.cpp
 *
 * Copyright 2007 by NCR Corporation,
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */

#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsForceApprovalBeforeSuspend.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"

CIsForceApprovalBeforeSuspend::CIsForceApprovalBeforeSuspend()
{
    CRCMActionBase::SetName(_T("CIsForceApprovalBeforeSuspend"));
}


CIsForceApprovalBeforeSuspend::~CIsForceApprovalBeforeSuspend()
{
}


long CIsForceApprovalBeforeSuspend::GetValue() const { return 0; }


void CIsForceApprovalBeforeSuspend::SetValue(long) {}


HRESULT CIsForceApprovalBeforeSuspend::Execute(bool *pResult, IMessage &)
{
    *pResult = rapmgr().IsForceApprovalBeforeSuspend();

    return S_OK;
}


template <>
IAction* CCreateable<CIsForceApprovalBeforeSuspend, IAction>::NewInstance()
{
    IAction *pAction = new CIsForceApprovalBeforeSuspend;

    return pAction;
}


template <>
void CCreateable<CIsForceApprovalBeforeSuspend, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsForceApprovalBeforeSuspend)
DEFINE_ACTION_RELEASE_FXN(IsForceApprovalBeforeSuspend)
