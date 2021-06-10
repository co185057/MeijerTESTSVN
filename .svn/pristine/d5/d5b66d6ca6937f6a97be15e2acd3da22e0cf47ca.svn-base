// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SaveRemoteConnection.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)SaveRemoteConnection.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SaveRemoteConnection.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     3/30/05 10:39a Dm185016
 * UNICODE compile error.
 * 
 * 2     3/29/05 11:45a Dm185016
 * Problem:  HHRAP attempted to connect.  Connection timed out after 5
 * seconds but PSX kept connection alive anyway.  When HHRAP pressed
 * button after, RCM did not ignore as it should.  Changed to return
 * false, if the remote connection object does not exist for this
 * connection.
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
// 
// 2     1/12/05 6:08p Dm185016
// Cleaned up trace log.
// 
// 1     12/22/04 11:14a Dm185016
// HH support.  Saves the remote connection in RCManager.
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SaveRemoteConnection.h"
#include "RCManager.h"
#include "RCMKeyValue.h"
#include "Message.h"
#include "KVParmList.h"


BEGIN_KV_HANDLER_CLASS(CSaveRemoteConnection::SRCParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSaveRemoteConnection::SRCParms, KEY_IE_REMOTE_CONNECTION_NAME, RemoteConnectionName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CSaveRemoteConnection::SRCParms, RemoteConnectionName)


void CSaveRemoteConnection::SRCParms::Clear()
{
    m_RemoteConnectionName.Empty();
}


CSaveRemoteConnection::CSaveRemoteConnection()
{
    CRCMActionBase::SetName(_T("CSaveRemoteConnection"));
}


CSaveRemoteConnection::~CSaveRemoteConnection()
{
}


HRESULT CSaveRemoteConnection::Execute(bool *pResult, IMessage &message)
{

    *pResult = true;

    HRESULT hr = m_parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    if (m_parms.GetRemoteConnectionName().IsEmpty())
        return STATEM_MISSING_PARAMETER;

    const _TCHAR *pszRemoteConnectionName = (const _TCHAR *)m_parms.GetRemoteConnectionName();
    
    ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Setting remote connection:  ") + IString(pszRemoteConnectionName));

    // Get the RemoteConnection object.  If one cannot be found, then report the error
    // and ignore any remaining actions in this thread.  If one is found, then make it the
    // current remote connection.
    PREMOTECONN pRConn = rcm().GetRemoteConnection(pszRemoteConnectionName);
    if (pRConn != NULL)
        rcm().SetCurrentRemoteConnection(pRConn);
    else
    {
        ITRACE_ERROR(_T("Unknown remote connection name:  ") + IString(pszRemoteConnectionName));
        *pResult = false;

        //SetSynchronousReturnCode(message, S_FALSE);
    }

    return S_OK;
}


template <>
IAction* CCreateable<CSaveRemoteConnection, IAction>::NewInstance()
{
    IAction *pAction = new CSaveRemoteConnection;

    return pAction;
}


template <>
void CCreateable<CSaveRemoteConnection, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SaveRemoteConnection)
DEFINE_ACTION_RELEASE_FXN(SaveRemoteConnection)
