// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/DisconnectCommand.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)DisconnectCommand.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/DisconnectCommand.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 9     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 5     3/22/05 3:46p Dm185016
 * UNICODE
 * 
 * 4     2/28/05 4:35p Dm185016
 * lint
 * 
 * 3     2/18/05 4:35p Dm185016
 * 
 * 2     2/15/05 11:27a Dm185016
 * TAR 293322.
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 5     1/18/05 3:48p Dm185016
// UNICODE support
// 
// 4     1/12/05 6:00p Dm185016
// Cleaned up trace log.
// 
// 3     12/16/04 10:03a Dm185016
// Problem with iterator copy ctor.
// 
// 2     11/16/04 11:37a Dm185016
// fixed return code
// 
// 1     10/29/04 9:04a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "DisconnectCommand.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "RCMError.h"
#include "KVParmList.h"
#include "PSXMfc.h"


BEGIN_KV_HANDLER_CLASS(CDisconnectCommand::DParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisconnectCommand::DParms, KEY_IE_REMOTE_CONNECTION_NAME, RemoteConnectionName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CDisconnectCommand::DParms, RemoteConnectionName)


void CDisconnectCommand::DParms::Clear()
{
    m_RemoteConnectionName.Empty();
}


CDisconnectCommand::CDisconnectCommand()
{
    CRCMActionBase::SetName(_T("CDisconnectCommand"));
}


CDisconnectCommand::~CDisconnectCommand()
{
}


/// <summary>This action removes the RemoteConnection object for the connection 
/// being terminated.  If no more connections are available, then it will set the
/// boolean parameter pResult to false.  If more connections are available, then it
/// will set pResult to true.</summary>
HRESULT CDisconnectCommand::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = S_OK;

    m_parms.Clear();

    // Get the remote connection name from the message's parameter list.
    hr = m_parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    if (m_parms.GetRemoteConnectionName().IsEmpty())
    {
        ITRACE_ERROR(_T("Missing parameter:  ") + IString(KEY_IE_REMOTE_CONNECTION_NAME));
        return RCM_API_FAILURE_INVALID_KEY_VALUE_LIST;
    }

    ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  RemoteConnectionName is ") 
                 + IString((const _TCHAR *)m_parms.GetRemoteConnectionName()));

    // Ensure that we do not become disconnected without giving up exclusive access
    PREMOTECONN pRConn = rcm().GetCurrentRemoteConnection();
    if (pRConn == NULL)
    {
        ITRACE_DEVELOP(_T("No remote connection available"));
    }
    else if (m_parms.GetRemoteConnectionName() == pRConn->GetRemoteConnectionName())
    {
        hr = ReleaseCurrentConnection();
        if (hr != S_OK)
        {
            ITRACE_DEVELOP(_T("Current Connection not released.  rc=") + IString(hr).d2x());
        }

        hr = ReleaseOtherConnections();
        if (hr != S_OK)
        {
            ITRACE_DEVELOP(_T("Other Connections not released.  rc=") + IString(hr).d2x());
        }
    }

    // Get the machine name from the remote connection name.  Skip the prefix token.
    IString data((const _TCHAR *)m_parms.GetRemoteConnectionName());
    IString::token_iterator &iter = data.tokens(_T("."));
    iter++;     
    IString machineName((*iter).second);
    ITRACE_DEVELOP(_T("Disconnection from machine name:  ") + machineName);

    // Get rid of this connection
    rcm().RemoveRemoteConnection((const _TCHAR *)m_parms.GetRemoteConnectionName());

    // Return true if there are other connections available.  Return false if
    // no other connections are available
    *pResult = rcm().IsRemoteConnectionAvailable();

    SetSynchronousReturnCode(message, hr);

    return hr;
}

template <>
IAction* CCreateable<CDisconnectCommand, IAction>::NewInstance()
{
    IAction *pAction = new CDisconnectCommand;

    return pAction;
}


template <>
void CCreateable<CDisconnectCommand, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(DisconnectCommand)
DEFINE_ACTION_RELEASE_FXN(DisconnectCommand)
