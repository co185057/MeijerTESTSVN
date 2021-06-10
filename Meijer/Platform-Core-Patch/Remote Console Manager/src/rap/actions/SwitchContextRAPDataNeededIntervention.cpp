// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchContextRAPDataNeededIntervention.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SwitchContextRAPDataNeededIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchContextRAPDataNeededIntervention.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     6/24/05 1:39p Dm185016
 * Added capability for customer created rap-data-needed contexts.
 * 
 * 1     4/15/05 9:18a Dm185016
 * Merge from Patch B
 * 
 * 1     4/14/05 10:11p Dm185016
 * TAR 298021
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SwitchContextRAPDataNeededIntervention.h"
#include "RemoteAPMgr.h"
#include "LookupTable.h"
#include "State.h"
#include "RCMKeyValue.h"
#include "Message.h"

using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;


BEGIN_KV_HANDLER_CLASS(CSwitchContextRAPDataNeededIntervention::SCRDNIParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSwitchContextRAPDataNeededIntervention::SCRDNIParms, KEY_LOOKUP_TABLE_NAME, LookupTableName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSwitchContextRAPDataNeededIntervention::SCRDNIParms, KEY_VIEW, View)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSwitchContextRAPDataNeededIntervention::SCRDNIParms, KEY_PRIORITY, Priority)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSwitchContextRAPDataNeededIntervention::SCRDNIParms, KEY_MSG_NAME, MessageName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSwitchContextRAPDataNeededIntervention::SCRDNIParms, KEY_STATE_NAME, StateName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CSwitchContextRAPDataNeededIntervention::SCRDNIParms, LookupTableName)
DEFINE_KV_CSTRING(CSwitchContextRAPDataNeededIntervention::SCRDNIParms, Priority)
DEFINE_KV_CSTRING(CSwitchContextRAPDataNeededIntervention::SCRDNIParms, MessageName)
DEFINE_KV_CSTRING(CSwitchContextRAPDataNeededIntervention::SCRDNIParms, StateName)
DEFINE_KV_INT(CSwitchContextRAPDataNeededIntervention::SCRDNIParms, View)
BEGIN_KV_HANDLER_CLASS_CLEAR(CSwitchContextRAPDataNeededIntervention::SCRDNIParms)
    CLEAR_KV_CSTRING(LookupTableName)
    CLEAR_KV_CSTRING(Priority)
    CLEAR_KV_CSTRING(MessageName)
    CLEAR_KV_CSTRING(StateName)
    CLEAR_KV_INT(View, -1)
END_KV_HANDLER_CLASS_CLEAR()


CSwitchContextRAPDataNeededIntervention::CSwitchContextRAPDataNeededIntervention()
{
    CRCMActionBase::SetName(_T("CSwitchContextRAPDataNeededIntervention"));
}


CSwitchContextRAPDataNeededIntervention::~CSwitchContextRAPDataNeededIntervention()
{
}


// Get all control names from the xml
HRESULT CSwitchContextRAPDataNeededIntervention::ParseOperands()
{ 
    m_parms.Clear();

    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();

    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    return m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
}


HRESULT CSwitchContextRAPDataNeededIntervention::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    hr = m_parms.ParseKeys((const _TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    ILookupTable *pTable = NULL;
    hr = stateMachine().GetLookupTable(&pTable, m_parms.GetLookupTableName());
    if (hr != S_OK)
        return STATEM_UNDEFINED_LOOKUPTABLE_ID;
    _ASSERTE(pTable!=NULL);

    hr = pTable->Lookup(m_parms.GetView());
    if (hr == S_OK)
    {
        hr = m_parms.ParseKeys((const _TCHAR *)pTable->GetString(), GetTraceHandle());
        if (hr != S_OK)
            return hr;

        ITRACE_DEVELOP(_T(__FUNCTION__) _T(" Message:  ") 
            + IString(m_parms.GetMessageName()) + _T(".  State:  ") 
            + IString(m_parms.GetStateName()));
        
        IState *pState;
        hr = stateMachine().GetState(&pState, m_parms.GetStateName());
        if (hr != S_OK)
            return hr;

        SetNextState(pState->GetId());

        IMessage *pNewMsg = NULL;
        hr = stateMachine().GetMessage(&pNewMsg, m_parms.GetMessageName()
                                    , message.GetData(), message.GetDataLength());
        if (hr == S_OK)
            hr = stateMachine().PostMsg(*pNewMsg, false);
    }
    else
    {
        ITRACE_ERROR(_T("Unknown view:  ") + IString(m_parms.GetView()));
    }

    *pResult = true;

    return hr;
}


template <>
IAction* CCreateable<CSwitchContextRAPDataNeededIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CSwitchContextRAPDataNeededIntervention;

    return pAction;
}


template <>
void CCreateable<CSwitchContextRAPDataNeededIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SwitchContextRAPDataNeededIntervention)
DEFINE_ACTION_RELEASE_FXN(SwitchContextRAPDataNeededIntervention)
