// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsRemoteConnectionAvailable.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)IsRemoteConnectionAvailable.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsRemoteConnectionAvailable.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:38p Dm185016
 * lint
 * 
 * 1     2/15/05 11:28a Dm185016
 * TAR 293322.
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsRemoteConnectionAvailable.h"
#include "RCManager.h"
#include "RCMStateMachine.h"


CIsRemoteConnectionAvailable::CIsRemoteConnectionAvailable()
{
    CRCMActionBase::SetName(_T("CIsRemoteConnectionAvailable"));
}


CIsRemoteConnectionAvailable::~CIsRemoteConnectionAvailable()
{
}


HRESULT CIsRemoteConnectionAvailable::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = S_OK;

    *pResult = rcm().IsRemoteConnectionAvailable();

    return hr;
}


template <>
IAction* CCreateable<CIsRemoteConnectionAvailable, IAction>::NewInstance()
{
    IAction *pAction = new CIsRemoteConnectionAvailable;

    return pAction;
}


template <>
void CCreateable<CIsRemoteConnectionAvailable, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsRemoteConnectionAvailable)
DEFINE_ACTION_RELEASE_FXN(IsRemoteConnectionAvailable)
