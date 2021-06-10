// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsCurrentRemoteConnection.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)IsCurrentRemoteConnection.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsCurrentRemoteConnection.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     3/28/05 5:04p Dm185016
 * TAR 296771 - Made IEvaluateable for use in <if
 * expression="IsCurrentRemoteConnection()">
 * 
 * 2     3/10/05 11:24a Dm185016
 * UNICODE build problem.
 * 
 * 1     3/07/05 6:58p Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsCurrentRemoteConnection.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"
#include "RAPReceipt.h"


BEGIN_KV_HANDLER_CLASS(CIsCurrentRemoteConnection::ICRCParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CIsCurrentRemoteConnection::ICRCParms, KEY_IE_REMOTE_CONNECTION_NAME, RemoteConnectionName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CIsCurrentRemoteConnection::ICRCParms, RemoteConnectionName)


void CIsCurrentRemoteConnection::ICRCParms::Clear()
{
    m_RemoteConnectionName.Empty();
}


CIsCurrentRemoteConnection::CIsCurrentRemoteConnection()
{
    CRCMActionBase::SetName(_T("CIsCurrentRemoteConnection"));
}


CIsCurrentRemoteConnection::~CIsCurrentRemoteConnection()
{
}


long CIsCurrentRemoteConnection::GetValue() const { return 0; }


void CIsCurrentRemoteConnection::SetValue(long) {}


HRESULT CIsCurrentRemoteConnection::Execute(bool *pResult, IMessage &message)
{
    IMODTRACE_DEVELOP();

    PREMOTECONN pRConn = rcm().GetCurrentRemoteConnection();

	HRESULT hr = m_parms.ParseKeys((const _TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    ITRACE_DEVELOP(_T("Current remote connection:  ") + pRConn->ToString());

    *pResult = (m_parms.GetRemoteConnectionName() == pRConn->GetRemoteConnectionName());

    return hr;
}


template <>
IAction* CCreateable<CIsCurrentRemoteConnection, IAction>::NewInstance()
{
    IAction *pAction = new CIsCurrentRemoteConnection;

    return pAction;
}


template <>
void CCreateable<CIsCurrentRemoteConnection, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsCurrentRemoteConnection)
DEFINE_ACTION_RELEASE_FXN(IsCurrentRemoteConnection)
