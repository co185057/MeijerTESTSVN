// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/EventCommand.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)EventCommand.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/EventCommand.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 9     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 5     3/14/05 8:27p Dm185016
 * 
 * 4     3/10/05 11:46a Tp151000
 * TAR 295124
 * TP & AQ
 * 
 * 
 * 3     2/28/05 4:35p Dm185016
 * lint
 * 
 * 2     2/14/05 7:08p Dm185016
 * Release Exclusive Access Event support
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 4     1/21/05 4:42p Dm185016
// Now keeping track of lane context switches.
// 
// 3     1/12/05 6:00p Dm185016
// Cleaned up trace log.
// 
// 2     12/22/04 11:12a Dm185016
// TAR 288934 - Made switch context asynchronous.
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "EventCommand.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"


BEGIN_KV_HANDLER_CLASS(CEventCommand::ECParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CEventCommand::ECParms, KEY_EV_TYPE, Type)
    DEFINE_KV_HANDLER_MAP_ENTRY(CEventCommand::ECParms, KEY_EV_LOOKUP_TABLE_NAME, LookupTableName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CEventCommand::ECParms, Type)
DEFINE_KV_CSTRING(CEventCommand::ECParms, LookupTableName)


void CEventCommand::ECParms::Clear()
{
    m_Type.Empty();
    m_LookupTableName.Empty();
}


CEventCommand::CEventCommand()
{
    CRCMActionBase::SetName(_T("CEventCommand"));
}


CEventCommand::~CEventCommand()
{
}


HRESULT CEventCommand::ParseOperands()
{ 
    HRESULT hr = S_OK;

    m_parms.Clear();

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());

    return hr;
}


HRESULT CEventCommand::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    hr = m_parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    ILookupTable *pTable = NULL;
    hr = stateMachine().GetLookupTable(&pTable, m_parms.GetLookupTableName());
    if (hr != S_OK)
        return hr;
    _ASSERTE(pTable!=NULL);

    hr = pTable->Lookup(m_parms.GetType());
    if (hr == S_OK)
    {
        const _TCHAR *pszNewMsgName = pTable->GetString();

        ITRACE_DEVELOP(_T(__FUNCTION__) _T(" sending msg:  ") + IString(pszNewMsgName));

        IMessage &newMsg 
            = stateMachine().MakeAsynchCopyEx(message, pszNewMsgName);
        hr = stateMachine().PostMsg(newMsg);
    }

    *pResult = true;

    return hr;
}


template <>
IAction* CCreateable<CEventCommand, IAction>::NewInstance()
{
    IAction *pAction = new CEventCommand;

    return pAction;
}


template <>
void CCreateable<CEventCommand, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(EventCommand)
DEFINE_ACTION_RELEASE_FXN(EventCommand)
