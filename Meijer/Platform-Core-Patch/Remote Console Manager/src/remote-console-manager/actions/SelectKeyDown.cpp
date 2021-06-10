// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SelectKeyDown.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)SelectKeyDown.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SelectKeyDown.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 4     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     4/18/05 7:19p Dm185016
 * Keyboard support
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SelectKeyDown.h"
#include "RemoteAPMgr.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMError.h"
#include "RCMKeyValue.h"


BEGIN_KV_HANDLER_CLASS(CSelectKeyDown::SBParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSelectKeyDown::SBParms, KEY_EV_LOOKUP_TABLE_NAME, LookupTableName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSelectKeyDown::SBParms, KEY_IE_LPARAM, LParam)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSelectKeyDown::SBParms, KEY_IE_WPARAM, WParam)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSelectKeyDown::SBParms, KEY_IE_EVENT_ID, EventID)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSelectKeyDown::SBParms, KEY_IE_CONTROL_NAME, ControlName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSelectKeyDown::SBParms, KEY_IE_CONTEXT_NAME, ContextName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSelectKeyDown::SBParms, KEY_IE_EVENT_NAME, EventName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSelectKeyDown::SBParms, KEY_IE_REMOTE_CONNECTION_NAME, RemoteConnectionName)
END_KV_HANDLER_CLASS()
DEFINE_KV_LONG(CSelectKeyDown::SBParms, LParam)
DEFINE_KV_LONG(CSelectKeyDown::SBParms, WParam)
DEFINE_KV_LONG(CSelectKeyDown::SBParms, EventID)
DEFINE_KV_CSTRING(CSelectKeyDown::SBParms, LookupTableName)
DEFINE_KV_CSTRING(CSelectKeyDown::SBParms, ControlName)
DEFINE_KV_CSTRING(CSelectKeyDown::SBParms, ContextName)
DEFINE_KV_CSTRING(CSelectKeyDown::SBParms, EventName)
DEFINE_KV_CSTRING(CSelectKeyDown::SBParms, RemoteConnectionName)


BEGIN_KV_HANDLER_CLASS_CLEAR(CSelectKeyDown::SBParms)
    CLEAR_KV_LONG(LParam, 0)
    CLEAR_KV_LONG(WParam, 0)
    CLEAR_KV_LONG(EventID, 0)
    CLEAR_KV_CSTRING(LookupTableName)
    CLEAR_KV_CSTRING(ControlName)
    CLEAR_KV_CSTRING(ContextName)
    CLEAR_KV_CSTRING(EventName)
    CLEAR_KV_CSTRING(RemoteConnectionName)
END_KV_HANDLER_CLASS_CLEAR()


CSelectKeyDown::CSelectKeyDown()
{
    CRCMActionBase::SetName(_T("CSelectKeyDown"));
}


CSelectKeyDown::~CSelectKeyDown()
{
}


HRESULT CSelectKeyDown::ParseOperands()
{ 
    m_parms.Clear();

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    return m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
}


HRESULT CSelectKeyDown::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    *pResult = true;

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

    if (m_parms.GetControlName() == _T("Display"))
    {
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(" LookupTable name:  ") 
            + IString(m_parms.GetLookupTableName()));

        ILookupTable *pTable = NULL;
        HRESULT hr = stateMachine().GetLookupTable(&pTable, m_parms.GetLookupTableName());
        if (hr != S_OK)
            return hr;
        _ASSERTE(pTable!=NULL);

        hr = pTable->Lookup(m_parms.GetWParam());
        if (hr != S_OK)
            return hr;
        StringType sMsgName = (const _TCHAR *)pTable->GetString();

        ITRACE_DEVELOP(_T(__FUNCTION__) _T(" Message name:  ") 
            + IString(sMsgName.c_str()));

        IMessage *pNewMsg = NULL;
        hr = stateMachine().GetMessage(&pNewMsg, sMsgName.c_str()
                                    , message.GetData(), message.GetDataLength());
        if (hr != S_OK)
            return hr;

        // Enqueue the message to the state machine
        hr = stateMachine().PostMsg(*pNewMsg, true);

        SetSynchronousReturnCode(message, S_OK);

        *pResult = false;
    }
    else
    {
        SetSynchronousReturnCode(message, S_FALSE);
    }

    return hr;
}


StringType CSelectKeyDown::TranslateButtonName(const _TCHAR *pcszButtonName)
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
IAction* CCreateable<CSelectKeyDown, IAction>::NewInstance()
{
    IAction *pAction = new CSelectKeyDown;

    return pAction;
}


template <>
void CCreateable<CSelectKeyDown, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SelectKeyDown)
DEFINE_ACTION_RELEASE_FXN(SelectKeyDown)
