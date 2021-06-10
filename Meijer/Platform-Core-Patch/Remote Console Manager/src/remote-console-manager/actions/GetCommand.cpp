// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/GetCommand.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)GetCommand.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/GetCommand.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 10    10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 7     4/12/05 6:35p Dm185016
 * 
 * 6     4/12/05 6:34p Dm185016
 * TAR 297347
 * 
 * 5     4/05/05 11:59a Dm185016
 * TAR 297356
 * 
 * 4     3/30/05 2:37p Dm185016
 * TAR 297008
 * 
 * 3     3/03/05 3:16p Dm185016
 * TAR 291700 - Cleaned this up.  Added new Gets for
 * remote-connection-handheld and remote-connection-name.
 * 
 * 2     2/28/05 4:38p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 4     1/12/05 6:01p Dm185016
// Cleaned up trace log.
// 
// 3     11/16/04 11:39a Dm185016
// Added more implementations
// 
// 2     10/29/04 9:11a Dm185016
// Filled in fxn.
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "GetCommand.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "RCMError.h"
#include "LookupTable.h"
#include "Approval.h"
#include "KVParmList.h"


BEGIN_KV_HANDLER_CLASS(CGetCommand::GCParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CGetCommand::GCParms, KEY_GET_ID, Id)
    DEFINE_KV_HANDLER_MAP_ENTRY(CGetCommand::GCParms, KEY_EV_LOOKUP_TABLE_NAME, LookupTableName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CGetCommand::GCParms, KEY_CT_REMOTE_CONNECTION_NAME, RemoteConnectionName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CGetCommand::GCParms, Id)
DEFINE_KV_CSTRING(CGetCommand::GCParms, LookupTableName)
DEFINE_KV_CSTRING(CGetCommand::GCParms, RemoteConnectionName)


void CGetCommand::GCParms::Clear()
{
    SetId(false);
    m_LookupTableName.Empty();
    m_RemoteConnectionName.Empty();
}


CGetCommand::CGetCommand()
{
    CRCMActionBase::SetName(_T("CGetCommand"));
}


CGetCommand::~CGetCommand()
{
}


// Get all control names from the xml
HRESULT CGetCommand::ParseOperands()
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


HRESULT CGetCommand::Execute(bool *pResult, IMessage &message)
{
    CKeyValueParmList parmlist;

    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    ILookupTable *pTable = NULL;
    hr = stateMachine().GetLookupTable(&pTable, m_parms.GetLookupTableName());
    if (hr != S_OK)
        return hr;

    hr = pTable->Lookup(m_parms.GetId());
    if (hr == S_OK)
    {
        switch (pTable->GetInteger())
        {
            case GET_ID_REQUEST_ENABLE_AM :
                hr = get_id_request_enable_am(parmlist);
                break;

            case GET_ID_REQUEST_ENABLE_HH_AM :
                hr = get_id_request_enable_hh_am(parmlist);
                break;

            case GET_ID_CURRENT_APPROVAL_CODE :
                hr = get_id_current_approval_code(parmlist);
                break;

            case GET_ID_RAP_DATA_AVAILABLE :
                hr = get_id_rap_data_available(parmlist);
                break;

            case GET_ID_RAP_DATA:
                hr = get_id_rap_data(parmlist);
                break;

            case GET_ID_PREVIOUS_VIEW:
                hr = get_id_previous_view(parmlist);
                break;

            case GET_ID_RAP_DATA_PENDING_COUNT:
                hr = get_id_rap_data_pending_count(parmlist);
                break;

            case GET_ID_CONNECTION_MODE :
                hr = get_id_connection_mode(parmlist);
                break;

            case GET_ID_REMOTE_CONNECTION_HANDHELD :
                hr = get_id_remote_connection_handheld(parmlist);
                break;

            case GET_ID_REMOTE_CONNECTION_NAME :
                hr = get_id_remote_connection_name(parmlist);
                break;
        }
    }

    ITRACE_DEVELOP(_T("parmlist=") + IString((const _TCHAR *)parmlist));
    _bstr_t bstrParmList((const _TCHAR *)parmlist); 

    // Save the returned value in the message
    stateMachine().SetText(message, bstrParmList.Detach());

    *pResult = true;

    return hr;
}


HRESULT CGetCommand::get_id_request_enable_am(CKeyValueParmList &parmlist)
{
    CKeyValue<const _TCHAR *>::Put(parmlist
                    , KEY_GET_ID
                    , KEY_GET_ID_REQUEST_ENABLE_ASSIST_MODE);
    CKeyValue<bool>::Put(parmlist
                    , KEY_GET_VALUE 
                    , rapmgr().IsAssistModeEnabled());

    return S_OK;
}


HRESULT CGetCommand::get_id_request_enable_hh_am(CKeyValueParmList &parmlist)
{
    CKeyValue<const _TCHAR *>::Put(parmlist
                    , KEY_GET_ID
                    , KEY_GET_ID_REQUEST_ENABLE_HH_ASSIST_MODE);
    CKeyValue<bool>::Put(parmlist
                    , KEY_GET_VALUE 
                    , rapmgr().IsHandheldAssistModeEnabled());

    return S_OK;
}


HRESULT CGetCommand::get_id_current_approval_code(CKeyValueParmList &parmlist)
{
    CApproval &approval = rapmgr().GetCurrentApproval();
    CKeyValue<const _TCHAR *>::Put(parmlist
                    , KEY_GET_ID
                    , KEY_GET_ID_CURRENT_APPROVAL_CODE);
    CKeyValue<const _TCHAR *>::Put(parmlist
                    , KEY_GET_VALUE 
                    , approval.GetApprovalType());

    return S_OK;
}


HRESULT CGetCommand::get_id_rap_data_available(CKeyValueParmList &parmlist)
{
    CApproval &approval = rapmgr().GetCurrentApproval();
    CKeyValue<const _TCHAR *>::Put(parmlist
                    , KEY_GET_ID
                    , KEY_GET_ID_RAP_DATA_AVAILABLE);
    CKeyValue<bool>::Put(parmlist
                    , KEY_GET_VALUE 
                    , approval.HasData());

    return S_OK;
}


HRESULT CGetCommand::get_id_rap_data(CKeyValueParmList &)
{
    return RCM_API_FAILURE_NOT_IMPLEMENTED;
}


HRESULT CGetCommand::get_id_previous_view(CKeyValueParmList &)
{
    return RCM_API_FAILURE_NOT_IMPLEMENTED;
}


HRESULT CGetCommand::get_id_rap_data_pending_count(CKeyValueParmList &parmlist)
{
    //TODO:  Need to get some real value for this
    CKeyValue<const _TCHAR *>::Put(parmlist
                    , KEY_GET_ID
                    , KEY_GET_ID_RAP_DATA_PENDING_COUNT);
    CKeyValue<int>::Put(parmlist
                    , KEY_GET_VALUE 
                    , 0);

    return S_OK;
}


HRESULT CGetCommand::get_id_connection_mode(CKeyValueParmList &parmlist)
{
    CKeyValue<const _TCHAR *>::Put(parmlist
                    , KEY_GET_ID
                    , KEY_GET_ID_CONNECTION_MODE);
    CKeyValue<int>::Put(parmlist
                    , KEY_GET_VALUE 
                    , rapmgr().GetConnectionMode());

    return S_OK;
}


HRESULT CGetCommand::get_id_remote_connection_handheld(CKeyValueParmList &parmlist)
{
    CKeyValue<const _TCHAR *>::Put(parmlist
                    , KEY_GET_ID
                    , KEY_GET_ID_CURRENT_REMOTE_CONNECTION_HANDHELD);
    PREMOTECONN pRConn;
    if (m_parms.GetRemoteConnectionName().IsEmpty())
        pRConn = rcm().GetCurrentRemoteConnection();
    else
        pRConn = rcm().GetRemoteConnection(m_parms.GetRemoteConnectionName());
    if (pRConn != NULL)
    {
        CKeyValue<bool>::Put(parmlist
                        , KEY_GET_VALUE 
                        , pRConn->IsHandheld());
    }

    return S_OK;
}


HRESULT CGetCommand::get_id_remote_connection_name(CKeyValueParmList &parmlist)
{
    CKeyValue<const _TCHAR *>::Put(parmlist
                    , KEY_GET_ID
                    , KEY_GET_ID_CURRENT_REMOTE_CONNECTION_NAME);
    PREMOTECONN pRConn = rcm().GetCurrentRemoteConnection();
    if (pRConn != NULL)
    {
        CKeyValue<const _TCHAR *>::Put(parmlist
                        , KEY_GET_VALUE 
                        , pRConn->GetRemoteConnectionName());
    }

    return S_OK;
}


template <>
IAction* CCreateable<CGetCommand, IAction>::NewInstance()
{
    IAction *pAction = new CGetCommand;

    return pAction;
}


template <>
void CCreateable<CGetCommand, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(GetCommand)
DEFINE_ACTION_RELEASE_FXN(GetCommand)
