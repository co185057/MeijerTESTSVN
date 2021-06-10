// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsNotComfirmSuspend.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsNotConfirmSuspend.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
 *
 * Copyright 2004 by NCR Corporation,
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsNotComfirmSuspend.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 2     2/28/05 4:12p Dm185016
 * lint
 * 
 * 1     2/08/05 11:03a Tp151000
 * 290952
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 2     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
// 
// 1     11/16/04 11:23a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsNotConfirmSuspend.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"


CIsNotConfirmSuspend::CIsNotConfirmSuspend()
{
    CRCMActionBase::SetName(_T("CIsNotConfirmSuspend"));
}


CIsNotConfirmSuspend::~CIsNotConfirmSuspend()
{
}


HRESULT CIsNotConfirmSuspend::Execute(bool *pResult, IMessage &)
{
    *pResult = !rapmgr().IsConfirmationForSuspendingTransaction();
    return S_OK;
}

template <>
IAction* CCreateable<CIsNotConfirmSuspend, IAction>::NewInstance()
{
    IAction *pAction = new CIsNotConfirmSuspend;

    return pAction;
}


template <>
void CCreateable<CIsNotConfirmSuspend, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsNotConfirmSuspend)
DEFINE_ACTION_RELEASE_FXN(IsNotConfirmSuspend)
