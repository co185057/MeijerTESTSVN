// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsRemoteConnectionInputEnabled.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)IsRemoteConnectionInputEnabled.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsRemoteConnectionInputEnabled.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/30/05 5:06p Dm185016
 * TAR 297001
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsRemoteConnectionInputEnabled.h"
#include "RCManager.h"
#include "Message.h"
#include "RCMKeyValue.h"
#include "RCMError.h"


BEGIN_KV_HANDLER_CLASS(CIsRemoteConnectionInputEnabled::IRCIEParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CIsRemoteConnectionInputEnabled::IRCIEParms, KEY_CT_REMOTE_CONNECTION_NAME, RemoteConnectionName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CIsRemoteConnectionInputEnabled::IRCIEParms, RemoteConnectionName)


void CIsRemoteConnectionInputEnabled::IRCIEParms::Clear()
{
    m_RemoteConnectionName.Empty();
}


CIsRemoteConnectionInputEnabled::CIsRemoteConnectionInputEnabled()
{
    CRCMActionBase::SetName(_T("CIsRemoteConnectionInputEnabled"));
}


CIsRemoteConnectionInputEnabled::~CIsRemoteConnectionInputEnabled()
{
}


HRESULT CIsRemoteConnectionInputEnabled::Execute(bool *pResult, IMessage &message)
{
    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys((const _TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    if (m_parms.GetRemoteConnectionName().IsEmpty())
        return RCM_API_FAILURE_INVALID_KEY_VALUE_LIST;

    PREMOTECONN pRConn = rcm().GetRemoteConnection(m_parms.GetRemoteConnectionName());
    *pResult = pRConn->IsEnabled();

    return S_OK;
}


template <>
IAction* CCreateable<CIsRemoteConnectionInputEnabled, IAction>::NewInstance()
{
    IAction *pAction = new CIsRemoteConnectionInputEnabled;

    return pAction;
}


template <>
void CCreateable<CIsRemoteConnectionInputEnabled, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsRemoteConnectionInputEnabled)
DEFINE_ACTION_RELEASE_FXN(IsRemoteConnectionInputEnabled)
