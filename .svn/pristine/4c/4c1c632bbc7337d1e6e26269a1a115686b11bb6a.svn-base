#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RemoteConnection.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RemoteConnection.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RemoteConnection.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 8     3/30/05 5:05p Dm185016
 * TAR 297001
 * 
 * 7     3/24/05 7:21p Dm185016
 * UNICODE
 * 
 * 6     3/15/05 3:35p Dm185016
 * TAR 295676 - AcceptConnection is now handled by a separate thread to
 * prevent hanging the state machine thread.
 * 
 * 5     3/07/05 6:56p Dm185016
 * Added ToString() method.
 * 
 * 4     2/28/05 6:02p Dm185016
 * Unicode fix
 * 
 * 3     2/28/05 4:23p Dm185016
 * lint
 * 
 * 2     2/07/05 7:17p Dm185016
 * TAR 292647
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
// 
// 3     1/18/05 3:47p Dm185016
// UNICODE support
// 
// 2     1/12/05 5:56p Dm185016
// Multiple RAP support.  Removed starting context as no longer needed.
// 
// 1     12/22/04 11:08a Dm185016
// HH support.  Contains data describing the remote connection.
*/

#include "StdAfx.h"
#include "RemoteConnection.h"
#include "RCMKeyValue.h"
#include "Message.h"
#include "xerror.h"
#include "RCMError.h"

using namespace fastlane;
using namespace STATE_MACHINE_NAMESPACE;

CRemoteConnectionKeyType CRemoteConnection::m_lCount = 1;


CRemoteConnection *CRemoteConnection::CreateRemoteConnection(const IMessage *pMsg)
{
    RCParms parms;

    ParseMsg(parms, pMsg);

    CRemoteConnection *pRConn       = new CRemoteConnection();
    pRConn->m_sRemoteConnectionName = parms.GetRemoteConnectionName();
    pRConn->m_bHandheld             = parms.IsHandheld();

    // Get the machine name from the remote connection name.  Skip the prefix token.
    IString data((const _TCHAR *)parms.GetRemoteConnectionName());
    IString::token_iterator &iter   = data.tokens(_T("."));
    iter++; 
    pRConn->m_sMachineName          = (*iter).second;
    ITRACE_DEVELOP_STATIC(pMsg->GetTraceHandle(), _T("Connection made from machine name:  ") 
                 + IString((*iter).second));

    return pRConn;
}


StringType CRemoteConnection::GetRemoteConnectionName(const IMessage *pMsg)
{
    RCParms parms;

    ParseMsg(parms, pMsg);

    StringType sRemoteConnectionName = parms.GetRemoteConnectionName();

    return sRemoteConnectionName;
}


void CRemoteConnection::ParseMsg(RCParms &parms, const IMessage *pMsg)
{
    parms.Clear();

    const IMessage &msg                   = *pMsg;

    HRESULT hr = parms.ParseKeys((const _TCHAR *)msg.GetData(), pMsg->GetTraceHandle());
    if (hr != S_OK)
        ITHROW(IException(_T("Paramter list error"), RCM_API_FAILURE_INVALID_KEY_VALUE_LIST));

    if (parms.GetRemoteConnectionName().IsEmpty())
        ITHROW(IException((const _TCHAR *)(_T("Missing parameter:  ") + IString(KEY_IE_REMOTE_CONNECTION_NAME)),
                          RCM_API_FAILURE_INVALID_KEY_VALUE_LIST));
}


CRemoteConnection::CRemoteConnection()
    : m_pszMachineName(NULL) 
    , m_bHandheld(false)
    , m_bEnabled(true)
    , m_Id(m_lCount++) 
{}


bool CRemoteConnection::operator!=(const CRemoteConnection& rhs) const
{
    return (this != &rhs);
}


CRemoteConnectionKeyType CRemoteConnection::GetId() { return m_Id; }



const _TCHAR *CRemoteConnection::GetCurrentContext() const { return m_sCurrentContext.c_str(); }
void CRemoteConnection::SetCurrentContext(const _TCHAR *pszName) { m_sCurrentContext = pszName ; }


const _TCHAR *CRemoteConnection::GetRemoteConnectionName() const { return m_sRemoteConnectionName.c_str(); }
const _TCHAR *CRemoteConnection::GetMachineName() const { return m_sMachineName.c_str(); }
bool CRemoteConnection::IsHandheld() const { return m_bHandheld; }


void CRemoteConnection::SetEnabled(bool bEnabled) { m_bEnabled = bEnabled; } 
bool CRemoteConnection::IsEnabled() const { return m_bEnabled; }


BEGIN_KV_HANDLER_CLASS(CRemoteConnection::RCParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CRemoteConnection::RCParms, KEY_IE_REMOTE_CONNECTION_NAME, RemoteConnectionName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CRemoteConnection::RCParms, KEY_IE_REMOTE_HANDHELD, Handheld)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CRemoteConnection::RCParms, RemoteConnectionName)
DEFINE_KV_BOOL(CRemoteConnection::RCParms, Handheld)


void CRemoteConnection::RCParms::Clear()
{
    m_RemoteConnectionName.Empty();
    m_Handheld = false;
}


StringType CRemoteConnection::ToString() throw()
{
    IStringBuffer sBuffer(128);
    sBuffer += CBaseObject::ToString().c_str();
    sBuffer += _T("(");
    sBuffer += GetRemoteConnectionName();
    sBuffer += _T(")");

    StringType sRetValue = (const _TCHAR *)sBuffer;
    return sRetValue;
}


