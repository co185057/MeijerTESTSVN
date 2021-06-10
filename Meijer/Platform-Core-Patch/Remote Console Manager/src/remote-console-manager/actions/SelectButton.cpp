/*
 * @(#)SelectButton.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SelectButton.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     4/05/05 12:00p Dm185016
 * TAR 297356 - Now allows translation of button name to another.
 * 
 * 2     2/28/05 4:40p Dm185016
 * lint
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
// 
// 4     1/12/05 6:08p Dm185016
// Cleaned up trace log.
// 
// 3     11/16/04 11:42a Dm185016
// Added ability to ignore remote keystrokes for assist mode, which is
// controlled by SCOTAPP.
// 
// 2     10/29/04 9:14a Dm185016
// Filled in fxn.
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SelectButton.h"
#include "RemoteAPMgr.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMError.h"
#include "RCMKeyValue.h"


BEGIN_KV_HANDLER_CLASS(CSelectButton::SBParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSelectButton::SBParms, KEY_EV_LOOKUP_TABLE_NAME, LookupTableName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSelectButton::SBParms, KEY_IE_LPARAM, LParam)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSelectButton::SBParms, KEY_IE_WPARAM, WParam)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSelectButton::SBParms, KEY_IE_EVENT_ID, EventID)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSelectButton::SBParms, KEY_IE_CONTROL_NAME, ControlName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSelectButton::SBParms, KEY_IE_CONTEXT_NAME, ContextName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSelectButton::SBParms, KEY_IE_EVENT_NAME, EventName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSelectButton::SBParms, KEY_IE_REMOTE_CONNECTION_NAME, RemoteConnectionName)
END_KV_HANDLER_CLASS()
DEFINE_KV_LONG(CSelectButton::SBParms, LParam)
DEFINE_KV_LONG(CSelectButton::SBParms, WParam)
DEFINE_KV_LONG(CSelectButton::SBParms, EventID)
DEFINE_KV_CSTRING(CSelectButton::SBParms, LookupTableName)
DEFINE_KV_CSTRING(CSelectButton::SBParms, ControlName)
DEFINE_KV_CSTRING(CSelectButton::SBParms, ContextName)
DEFINE_KV_CSTRING(CSelectButton::SBParms, EventName)
DEFINE_KV_CSTRING(CSelectButton::SBParms, RemoteConnectionName)


BEGIN_KV_HANDLER_CLASS_CLEAR(CSelectButton::SBParms)
    CLEAR_KV_LONG(LParam, 0)
    CLEAR_KV_LONG(WParam, 0)
    CLEAR_KV_LONG(EventID, 0)
    CLEAR_KV_CSTRING(LookupTableName)
    CLEAR_KV_CSTRING(ControlName)
    CLEAR_KV_CSTRING(ContextName)
    CLEAR_KV_CSTRING(EventName)
    CLEAR_KV_CSTRING(RemoteConnectionName)
END_KV_HANDLER_CLASS_CLEAR()


CSelectButton::CSelectButton()
{
    CRCMActionBase::SetName(_T("CSelectButton"));
}


CSelectButton::~CSelectButton()
{
}


HRESULT CSelectButton::ParseOperands()
{ 
    m_parms.Clear();

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    return m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
}


HRESULT CSelectButton::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    // If RCM is not accepting input events then ignore this one and
    // let the next handler process it
    if (!rapmgr().IsInputEventsEnabled())
    {
        *pResult = false;

        SetSynchronousReturnCode(message, S_FALSE);

        return S_OK;
    }

    hr = m_parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    if (m_parms.GetControlName().IsEmpty())
    {
        ITRACE_ERROR(_T("Missing parameter:  ") + IString(KEY_IE_CONTROL_NAME));
        return RCM_API_FAILURE_INVALID_KEY_VALUE_LIST;
    }

    StringType sButtonName = TranslateButtonName(m_parms.GetControlName());

    if (rcmStateMachine().IsControl(sButtonName.c_str()))
    {
        IMessage *pNewMsg = NULL;

        hr = stateMachine().GetMessage(&pNewMsg, sButtonName.c_str()
                                    , message.GetData(), message.GetDataLength());

        // Enqueue the message to the state machine
        if (hr == S_OK)
        {
            hr = stateMachine().PostMsg(*pNewMsg, true);

            *pResult = true;

            SetSynchronousReturnCode(message, S_OK);
        }
    }
    else
    {
        *pResult = false;

        SetSynchronousReturnCode(message, S_FALSE);
    }


    return hr;
}


StringType CSelectButton::TranslateButtonName(const _TCHAR *pcszButtonName)
{
    StringType sButtonName = pcszButtonName;

    ILookupTable *pTable = NULL;
    HRESULT hr = stateMachine().GetLookupTable(&pTable, m_parms.GetLookupTableName());
    if (hr != S_OK)
        return sButtonName;
    _ASSERTE(pTable!=NULL);

    hr = pTable->Lookup(pcszButtonName);
    if (hr == S_OK)
    {
        sButtonName = pTable->GetString();

        ITRACE_DEVELOP(_T(__FUNCTION__) _T(" translating button name.  original:  ") 
            + IString(pcszButtonName) + _T(".  new:  ") 
            + IString(sButtonName.c_str()));
    }
    else
        sButtonName = pcszButtonName;

    return sButtonName;
}


template <>
IAction* CCreateable<CSelectButton, IAction>::NewInstance()
{
    IAction *pAction = new CSelectButton;

    return pAction;
}


template <>
void CCreateable<CSelectButton, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SelectButton)
DEFINE_ACTION_RELEASE_FXN(SelectButton)
