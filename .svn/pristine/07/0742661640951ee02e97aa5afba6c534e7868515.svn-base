// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/SendMsg.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)SendMsg.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/SendMsg.cpp $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     3/29/05 10:14a Dm185016
 * Fixed problem with old style send-msg parameters.
 * 
 * 4     3/28/05 5:09p Dm185016
 * Lint
 * 
 * 3     3/22/05 2:50p Dm185016
 * UNICODE
 * 
 * 2     2/10/05 5:47p Dm185016
 * Removed trace
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
// 
// 8     1/12/05 6:30p Dm185016
// Select support.
// 
// 7     11/15/04 11:42a Dm185016
// Moved return code from action to message.
// 
// 6     11/02/04 3:20p Dm185016
// More updates
// 
// 5     10/29/04 9:27a Dm185016
// Removed reference to MessageObject.
// Added copy-plist function parameter.
// 
// 4     10/04/04 6:44p Dm185016
// Trace object is no longer shared.
// 
// 2     6/30/04 3:30p Dm185016
// Added parameter list support
// 
// 1     4/30/04 1:01p Dm185016
// Action to send a message given its name
*/
#include "StdAfx.h"
#include "SendMsg.h"
#include "SMError.h"
#include "Message.h"
#include "StateMachineKeyValue.h"

using namespace STATE_MACHINE_NAMESPACE;


BEGIN_KV_HANDLER_CLASS(CSendMsg::SMParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSendMsg::SMParms, KEY_MESSAGE_NAME, MsgName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSendMsg::SMParms, KEY_OPERATION, Operation)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSendMsg::SMParms, KEY_IMMEDIATE   , Immediate)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSendMsg::SMParms, KEY_COPY_PLIST   , CopyPlist)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CSendMsg::SMParms, MsgName)
DEFINE_KV_CSTRING(CSendMsg::SMParms, Operation)
DEFINE_KV_BOOL(CSendMsg::SMParms, Immediate)
DEFINE_KV_BOOL(CSendMsg::SMParms, CopyPlist)


void CSendMsg::SMParms::Clear()
{
    m_MsgName.Empty();
    m_Operation.Empty();
    m_Immediate = true;
    m_CopyPlist = false;
}


CSendMsg::CSendMsg()
{
    SetName(_T("CSendMsg"));
    Comment.Value(_T("Resends the received message to the SM."));
}


CSendMsg::~CSendMsg()
{
}


bool CSendMsg::IsInternal() 
{
    return true; 
}


HRESULT CSendMsg::ParseOperands()
{ 
//  IMODTRACE_DEVELOP();
    HRESULT hr = S_OK;

    m_parms.Clear();

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter == parms.end())
    {
        ITRACE_ERROR(_T("Missing message name"));
        return SECURITY_MISSING_PARAMETER;
    }

    if (parms.size() == 1)
    {    
        m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());   // Do NOT want to check
                                                                // return code here.  We
                                                                // are allowing both formats,
                                                                // with or without KV list
        if (!m_parms.GetMsgName().IsEmpty())
            return hr;
    }

    m_parms.SetMsgName((*iter).c_str());

    m_sParameterList.clear();
    iter++;
    if (iter != parms.end())
    {
        m_parms.SetCopyPlist(false);
        if (*iter == _T("copy-plist"))
            m_parms.SetCopyPlist(true);
        else
            m_sParameterList = *iter;
    }

    return hr;
}


HRESULT CSendMsg::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = S_OK;
    *pResult = true;

    hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    IMessage *pNewMsg = NULL;

    ITRACE_DEVELOP(_T("Sending message:  ") + IString(m_parms.GetMsgName()));

    if (m_parms.IsCopyPlist())
        hr = stateMachine().GetMessage(&pNewMsg, (const _TCHAR *)m_parms.GetMsgName()
                                     , message.GetData(), message.GetDataLength());
    else
        hr = stateMachine().GetMessage(&pNewMsg, (const _TCHAR *)m_parms.GetMsgName()
                                     , m_sParameterList.c_str(), m_sParameterList.size());

    // Enqueue the message to the state machine
    if (hr == S_OK)
        hr = stateMachine().PostMsg(*pNewMsg, m_parms.IsImmediate());

    SetSynchronousReturnCode(message, hr);

    return hr;
}


template <>
IAction* CCreateable<CSendMsg, IAction>::NewInstance()
{
    return new CSendMsg;
}


template <>
void CCreateable<CSendMsg, IAction>::ReleaseInstance(IAction *pAction)
{
    IMODTRACE_DEVELOP_STATIC(pAction->GetTraceHandle());
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SendMsg)
DEFINE_ACTION_RELEASE_FXN(SendMsg)
