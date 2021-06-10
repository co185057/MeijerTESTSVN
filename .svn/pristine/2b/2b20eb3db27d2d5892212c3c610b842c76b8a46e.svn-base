// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/RemoteDataCommand.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RemoteDataCommand.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/RemoteDataCommand.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 5     4/12/05 2:54p Dm185016
 * Changes for AllLaneStatusSummary
 * 
 * 3     3/03/05 3:16p Dm185016
 * Added reset of parameter parser before usage.
 * 
 * 2     2/28/05 4:40p Dm185016
 * lint
 * 
 * 1     2/14/05 7:03p Dm185016
 * Release Exclusive Access Event support
*/
#include "StdAfx.h"
#include "RemoteDataCommand.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMEvent.h"
#include "RCMKeyValue.h"

using namespace fastlane;


BEGIN_KV_HANDLER_CLASS(CRemoteDataCommand::RDCParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CRemoteDataCommand::RDCParms, KEY_EV_AUX_DATA, AuxData)
    DEFINE_KV_HANDLER_MAP_ENTRY(CRemoteDataCommand::RDCParms, KEY_EV_OPERATION, Operation)
    DEFINE_KV_HANDLER_MAP_ENTRY(CRemoteDataCommand::RDCParms, KEY_EV_LOOKUP_TABLE_NAME, LookupTableName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CRemoteDataCommand::RDCParms, AuxData)
DEFINE_KV_CSTRING(CRemoteDataCommand::RDCParms, Operation)
DEFINE_KV_CSTRING(CRemoteDataCommand::RDCParms, LookupTableName)


void CRemoteDataCommand::RDCParms::Clear()
{
    m_AuxData.Empty();
    m_Operation.Empty();
    m_LookupTableName.Empty();
}


CRemoteDataCommand::CRemoteDataCommand()
{
    CRCMActionBase::SetName(_T("CRemoteDataCommand"));
}


CRemoteDataCommand::~CRemoteDataCommand()
{
}


// Get all control names from the xml
HRESULT CRemoteDataCommand::ParseOperands()
{ 
    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());

    if (m_parms.GetLookupTableName() == _T(""))
        return STATEM_MISSING_PARAMETER;

    return S_OK;
}


HRESULT CRemoteDataCommand::ParseAuxData(const _TCHAR *pszRemoteData)
{
    return m_parms.ParseKeys(pszRemoteData, GetTraceHandle());
}


const _TCHAR * CRemoteDataCommand::GetOperation()
{
    return m_parms.GetOperation();
}


HRESULT CRemoteDataCommand::Execute(bool *pResult, IMessage &message)
{
    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;
    
    hr = ParseAuxData(m_parms.GetAuxData());
    if (hr != S_OK)
        return hr;

    hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    ILookupTable *pTable = NULL;
    hr = stateMachine().GetLookupTable(&pTable, m_parms.GetLookupTableName());
    if (hr != S_OK)
        return hr;

    hr = pTable->Lookup(GetOperation());
    if (hr != S_OK)
    {
        hr = S_OK;
        *pResult = false;
    }
    else
    {
        const _TCHAR *pszNewMsgName = pTable->GetString();
        IMessage &newMsg 
            = stateMachine().MakeAsynchCopyEx(message, pszNewMsgName);
        hr = stateMachine().PostMsg(newMsg);

        *pResult = true;
    }

    return hr;
}


template <>
IAction* CCreateable<CRemoteDataCommand, IAction>::NewInstance()
{
    IAction *pAction = new CRemoteDataCommand;

    return pAction;
}


template <>
void CCreateable<CRemoteDataCommand, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(RemoteDataCommand)
DEFINE_ACTION_RELEASE_FXN(RemoteDataCommand)
