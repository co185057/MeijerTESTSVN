// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsInputEventEnabled.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsInputEventEnabled.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsInputEventEnabled.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
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

#include "IsInputEventEnabled.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"


CIsInputEventEnabled::CIsInputEventEnabled()
{
    CRCMActionBase::SetName(_T("CIsInputEventEnabled"));
}


CIsInputEventEnabled::~CIsInputEventEnabled()
{
}


HRESULT CIsInputEventEnabled::Execute(bool *pResult, IMessage &message)
{
    *pResult = rapmgr().IsInputEventsEnabled();

    SetSynchronousReturnCode(message, *pResult ? S_OK : S_FALSE);

    return S_OK;
}

template <>
IAction* CCreateable<CIsInputEventEnabled, IAction>::NewInstance()
{
    IAction *pAction = new CIsInputEventEnabled;

    return pAction;
}


template <>
void CCreateable<CIsInputEventEnabled, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsInputEventEnabled)
DEFINE_ACTION_RELEASE_FXN(IsInputEventEnabled)
