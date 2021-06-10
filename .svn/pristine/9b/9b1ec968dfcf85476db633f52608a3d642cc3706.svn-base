// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SetTransactionState.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SetTransactionState.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SetTransactionState.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/11/05 2:45p Dm185016
 * 
 * 3     3/01/05 4:42p Dm185016
 * 
 * 2     3/01/05 2:03p Dm185016
 * TAR 294587.  Added application state functionality.
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
// 
// 2     1/12/05 6:10p Dm185016
// Cleaned up trace log.
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SetTransactionState.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachineRAP.h"
#include "AppState.h"
#include "RCMKeyValue.h"
#include "RCMMessages.h"
#include "KVParmList.h"

using namespace rap;

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )


BEGIN_KV_HANDLER_CLASS(CSetTransactionState::STSParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSetTransactionState::STSParms, KEY_CT_STATE, State)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CSetTransactionState::STSParms, State)


void CSetTransactionState::STSParms::Clear()
{
    m_State.Empty();
}


CSetTransactionState::CSetTransactionState()
    : m_pState(NULL)
{
    CRCMActionBase::SetName(_T("CSetTransactionState"));
}


CSetTransactionState::~CSetTransactionState()
{
}


HRESULT CSetTransactionState::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = m_parms.ParseKeys((const _TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    ExitCurrentState();

    CRCMStateMachineRAP *pSm = rapmgr().GetStateMachineRAP();
    _ASSERTE(pSm!=NULL);

    hr = pSm->GetAppState(&m_pState, m_parms.GetState());

    if (hr != S_OK)
    {
        ITRACE_DEVELOP(_T("Unknown state detected.  Ignored:  ") 
                     + IString((const _TCHAR *)m_parms.GetState()));
        // TODO:  Add code to handle unknown application state

        hr = S_OK;
    }
    else
    {
        EnterNewState();
    }

    *pResult = true;

    return S_OK;
}


void CSetTransactionState::ExitCurrentState()
{
    IMODTRACE_DEVELOP();

    CApplicationState &currentState = *rapmgr().GetCurrentApplicationState();

    int nMsgID = currentState.GetExitMsgID();
    IMessage *pMsg;
    CAbstractStateMachine &sm = stateMachine(); 
    HRESULT hr = sm.GetMessage(&pMsg, nMsgID);
    if (hr == S_OK)
        hr = stateMachine().PostMsg(*pMsg, true);

    if (currentState.IsPushAssistModeEnabled())
    {
        rapmgr().SetAssistModeEnabled(currentState.IsSavedAssistModeEnabled());
    }
}


void CSetTransactionState::EnterNewState()
{
//  IMODTRACE_DEVELOP();

    _ASSERTE(m_pState!=NULL);

    ITRACE_DEVELOP(_T("push-assist-mode:  ") + IString(m_pState->IsPushAssistModeEnabled()));
    ITRACE_DEVELOP(_T("enable-assist-mode:  ") + IString(m_pState->IsAssistModeEnabled()));
    ITRACE_DEVELOP(_T("disable-assist-mode:  ") + IString(m_pState->IsAssistModeDisabled()));
    // Save the current setting if required by the XML settings for this state
    if (m_pState->IsPushAssistModeEnabled())
    {
        m_pState->SetSavedAssistModeEnabled(rapmgr().IsAssistModeEnabled());
    }

    if (m_pState->IsAssistModeEnabled())
    {
        EnableAssistModeButton(true);
    }
    else if (m_pState->IsAssistModeDisabled())
    {
        EnableAssistModeButton(false);
    }

    // Save the new current application state
    rapmgr().SetCurrentApplicationState(m_pState);

    int nMsgID = m_pState->GetEntryMsgID();
    IMessage *pMsg;
    HRESULT hr = stateMachine().GetMessage(&pMsg, nMsgID);
    if (hr == S_OK)
        hr = stateMachine().PostMsg(*pMsg, true);

}


void CSetTransactionState::EnableAssistModeButton(bool bEnable)
{
    // Save the value passed from SCOTAPP
	ITRACE_DEVELOP(_T("Assist mode enabled set:  ") + IString(bEnable));
	rapmgr().SetAssistModeEnabled(bEnable);

    // Send a message to the state machine to give it a chance to perform
    // additional actions.
    IMessage *pNewMsg;
    const _TCHAR *pszMsgName;
    if (bEnable)
        pszMsgName = MSG_ENABLE_ASSIST_MODE_BUTTON;
    else
        pszMsgName = MSG_DISABLE_ASSIST_MODE_BUTTON;

    HRESULT hr = stateMachine().GetMessage(&pNewMsg, pszMsgName);
    if (hr == S_OK)
        hr = stateMachine().PostMsg(*pNewMsg);
}


template <>
IAction* CCreateable<CSetTransactionState, IAction>::NewInstance()
{
    IAction *pAction = new CSetTransactionState;

    return pAction;
}


template <>
void CCreateable<CSetTransactionState, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SetTransactionState)
DEFINE_ACTION_RELEASE_FXN(SetTransactionState)
