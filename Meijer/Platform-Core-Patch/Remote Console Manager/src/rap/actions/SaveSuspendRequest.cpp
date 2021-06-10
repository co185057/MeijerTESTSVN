// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveSuspendRequest.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SaveSuspendRequest.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveSuspendRequest.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 1     12/02/04 3:34p Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SaveSuspendRequest.h"
#include "RemoteAPMgr.h"


CSaveSuspendRequest::CSaveSuspendRequest()
{
    CRCMActionBase::SetName(_T("CSaveSuspendRequest"));
}


CSaveSuspendRequest::~CSaveSuspendRequest()
{
}


HRESULT CSaveSuspendRequest::Execute(bool *pResult, IMessage &message)
{
    *pResult = true;

    CIntervention *pIntervention = (CIntervention *)&stateMachine().MakeAsynchCopy(message);

    rapmgr().SetCurrentIntervention(pIntervention);

    return S_OK;
}


template <>
IAction* CCreateable<CSaveSuspendRequest, IAction>::NewInstance()
{
    IAction *pAction = new CSaveSuspendRequest;

    return pAction;
}


template <>
void CCreateable<CSaveSuspendRequest, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SaveSuspendRequest)
DEFINE_ACTION_RELEASE_FXN(SaveSuspendRequest)
