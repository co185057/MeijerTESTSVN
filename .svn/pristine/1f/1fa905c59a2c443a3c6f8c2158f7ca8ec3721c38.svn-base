// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchContextCommand.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SwitchContextCommand.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchContextCommand.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 9     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 5     4/04/05 4:07p Dm185016
 * Trace output was going to SecurityManager logs.
 * 
 * 4     3/02/05 1:45p Dm185016
 * Added error checking for current remote connection pointer.
 * 
 * 3     2/21/05 7:57p Dm185016
 * Added debugging information to new and delete for pooled objects.
 * 
 * 2     2/17/05 5:52p Dm185016
 * Now ignoring context switch if the remote connection name is not the
 * same as the current remote connection.
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 5     1/13/05 1:29p Dm185016
// Cleaned up trace.
// 
// 4     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
// 
// 3     12/07/04 4:18p Dm185016
// Assist mode contexts no longer loaded in LaneRAP.xml
// 
// 2     11/18/04 1:38p Dm185016
// Context not recognized no longer failure and is ignored.
// 
// 1     11/16/04 11:23a Dm185016
*/
#include "StdAfx.h"
#include "SwitchContextCommand.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMEvent.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "SwitchContext.h"

using namespace fastlane;

BEGIN_KV_HANDLER_CLASS(CSwitchContextCommand::SCCParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSwitchContextCommand::SCCParms, KEY_CT_CONTEXT, ContextName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSwitchContextCommand::SCCParms, KEY_CT_REMOTE_CONNECTION_NAME, RemoteConnectionName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CSwitchContextCommand::SCCParms, ContextName)
DEFINE_KV_CSTRING(CSwitchContextCommand::SCCParms, RemoteConnectionName)


void CSwitchContextCommand::SCCParms::Clear()
{
    m_ContextName.Empty();
    m_RemoteConnectionName.Empty();
}



CSwitchContextCommand::CSwitchContextCommand()
    : m_pContextSwitch(NULL)
    , m_pUnknownCS(newpooledobject CUnknownContextSwitch)
    , m_pLocalCS(newpooledobject CLocalContextSwitch)
{
    CRCMActionBase::SetName(_T("CSwitchContextCommand"));
}


CSwitchContextCommand::~CSwitchContextCommand()
{
    m_pContextSwitch = NULL;

    delete m_pUnknownCS;
    delete m_pLocalCS;
}


CContextSwitch &CSwitchContextCommand::GetContextSwitch() const throw()
{
    _ASSERTE(m_pContextSwitch!=NULL);
    return *m_pContextSwitch;
}


HRESULT CSwitchContextCommand::Execute(bool *pResult, IMessage &message)
{
    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys((const _TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    if (m_parms.GetContextName().IsEmpty())
        return STATEM_MISSING_PARAMETER;

    if (m_parms.GetRemoteConnectionName().IsEmpty())
        return STATEM_MISSING_PARAMETER;

    // Ignore this context switch if it is not for the same remote connection as
    // currently has control
    PREMOTECONN pRConn = rcm().GetCurrentRemoteConnection();
    _ASSERTE(pRConn!=NULL);
    if (pRConn==NULL)
        return S_OK;

    CRemoteConnection &rconn = *pRConn;
    if (m_parms.GetRemoteConnectionName() != rconn.GetRemoteConnectionName())
    {
        *pResult = false;

        return S_OK;
    }

    CRCMStateMachine &sm = rcmStateMachine();

    const _TCHAR *pszContextSwitchName = m_parms.GetContextName();

    PCONTEXT pContext;
    hr = sm.GetContext(&pContext, pszContextSwitchName);
    if (hr != S_OK)
    {
        ITRACE_DEVELOP(_T("Unknown context switch detected:  ") 
                     + IString(pszContextSwitchName));
        ITRACE_DEVELOP(_T("Using default ..."));

        hr = S_OK;

        m_pContextSwitch = m_pUnknownCS;
    }

    else 
    {
        hr = sm.GetContextSwitch(&m_pContextSwitch, pszContextSwitchName);
        if (hr != S_OK)
        {
            ITRACE_DEVELOP(_T("Local context switch not defined:  ") 
                        + IString(pszContextSwitchName));
            ITRACE_DEVELOP(_T("Using local ..."));

            hr = S_OK;

            m_pContextSwitch = m_pLocalCS;
        }
    }

    ExitCurrentState();
    EnterNewState();

    *pResult = true;

    return hr;
}


void CSwitchContextCommand::ExitCurrentState()
{
    IMODTRACE_DEVELOP();

    CContextSwitch *pccs = rapmgr().GetCurrentContextSwitch();
    if (pccs == NULL)
        return;

    CContextSwitch &ccs = *pccs;
    int nMsgID = ccs.GetExitMsgID();
    if (nMsgID == -1)
        return;

    IMessage *pMsg;
    HRESULT hr = stateMachine().GetMessage(&pMsg, nMsgID);
    if (hr == S_OK)
        hr = stateMachine().PostMsg(*pMsg, true);

    if (ccs.IsPushInputControlSettings())
    {
        rapmgr().SetInputEventsEnabled(ccs.IsInputEventsEnabled());
    }
}


void CSwitchContextCommand::EnterNewState()
{
    IMODTRACE_DEVELOP();

    _ASSERTE(m_pContextSwitch!=NULL);
    CContextSwitch &cs = GetContextSwitch();

    ITRACE_DEVELOP(CONTEXT_SWITCH_PUSH_IC _T(":  ") + IString(cs.IsPushInputControlSettings()));
    ITRACE_DEVELOP(CONTEXT_SWITCH_DISABLE_IC _T(":  ") + IString(cs.IsDisableInputControl()));
    ITRACE_DEVELOP(CONTEXT_SWITCH_ENABLE_IC _T(":  ") + IString(cs.IsEnableInputControl()));
    // Save the current setting if required by the XML settings for this state
    if (cs.IsPushInputControlSettings())
    {
        cs.SetInputEventsEnabled(rapmgr().IsInputEventsEnabled());
    }

    // Now set the hold weight to the valid specified in the XML settings.
    // If not in the settings, then the value will remain the same as before.
    if (cs.IsDisableInputControl())
    {
        rapmgr().SetInputEventsEnabled(false);
    }
    else if (cs.IsEnableInputControl())
    {
        rapmgr().SetInputEventsEnabled(true);
    }

    // Save the new current application state
    rapmgr().SetCurrentContextSwitch(m_pContextSwitch);

    int nMsgID = GetContextSwitch().GetEntryMsgID();
    if (nMsgID == -1)
        return;

    IMessage *pMsg;
    HRESULT hr = stateMachine().GetMessage(&pMsg, nMsgID);
    if (hr == S_OK)
        hr = stateMachine().PostMsg(*pMsg, true);
}


template <>
IAction* CCreateable<CSwitchContextCommand, IAction>::NewInstance()
{
    IAction *pAction = new CSwitchContextCommand;

    return pAction;
}


template <>
void CCreateable<CSwitchContextCommand, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SwitchContextCommand)
DEFINE_ACTION_RELEASE_FXN(SwitchContextCommand)
