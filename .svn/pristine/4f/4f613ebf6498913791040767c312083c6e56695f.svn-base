// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ControlCommand.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ControlCommand.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ControlCommand.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 9     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 5     3/14/05 8:27p Dm185016
 * Added comments
 * 
 * 4     3/08/05 10:26a Dm185016
 * 
 * 3     2/28/05 4:35p Dm185016
 * lint
 * 
 * 2     2/14/05 7:08p Dm185016
 * Removed commented out code.
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 8     1/13/05 1:40p Dm185016
// TAR 290304
// 
// 7     1/12/05 5:59p Dm185016
// Cleaned up trace log.
// 
// 6     12/22/04 11:11a Dm185016
// TAR 288934 - Made switch context asynchronous.
// 
// 5     12/07/04 4:20p Dm185016
// Added reload options
// 
// 4     11/16/04 11:37a Dm185016
// New operations
// 
// 3     11/02/04 3:09p Dm185016
// 
// 2     10/29/04 9:08a Dm185016
// New operations
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ControlCommand.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"


BEGIN_KV_HANDLER_CLASS(CControlCommand::CCParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CControlCommand::CCParms, KEY_CT_OPERATION, Operation)
    DEFINE_KV_HANDLER_MAP_ENTRY(CControlCommand::CCParms, KEY_CT_LOOKUP_TABLE_NAME, LookupTableName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CControlCommand::CCParms, KEY_CT_VARIABLE_NAME, VariableName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CControlCommand::CCParms, Operation)
DEFINE_KV_CSTRING(CControlCommand::CCParms, LookupTableName)
DEFINE_KV_CSTRING(CControlCommand::CCParms, VariableName)


void CControlCommand::CCParms::Clear()
{
    m_Operation.Empty();
    m_LookupTableName.Empty();
    m_VariableName.Empty();
}


CControlCommand::CControlCommand()
{
    CRCMActionBase::SetName(_T("CControlCommand"));
}


CControlCommand::~CControlCommand()
{
}


HRESULT CControlCommand::ParseOperands()
{ 
    HRESULT hr = S_OK;

    m_parms.Clear();

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());

    if (m_parms.GetLookupTableName() == _T(""))
        return STATEM_MISSING_PARAMETER;

    if (m_parms.GetVariableName() == _T(""))
        return STATEM_MISSING_PARAMETER;

    return hr;
}


HRESULT CControlCommand::SetError(IMessage &message, HRESULT hr)
{
    SetSynchronousReturnCode(message, hr);

    return hr;
}


///<summary>This API command determines which control operation is being requested and
///sets the value of the given variable with an index corresponding to that operation.</summary>
///<xml-param name="lookup-table-name">Name of the lookup table used to determine which operation.
///Note that this table accepts a string key and returns an integer value.<xml-parm>
///<xml-param name="variable-name">Name of the variable to receive the look up value<xml-parm>
///<msg-param name="operation">Name of the operation specified by the calling application<msg-parm>
HRESULT CControlCommand::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return SetError(message, hr);

    hr = m_parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return SetError(message, hr);

    IVariable *pVariable = NULL;
    hr = stateMachine().GetVariable(&pVariable, m_parms.GetVariableName());
    if (hr != S_OK)
        return SetError(message, hr);
    _ASSERTE(pVariable!=NULL);

    ILookupTable *pTable = NULL;
    hr = stateMachine().GetLookupTable(&pTable, m_parms.GetLookupTableName());
    if (hr != S_OK)
        return hr;
    _ASSERTE(pTable!=NULL);

    hr = pTable->Lookup(m_parms.GetOperation());
    if (hr == S_OK)
    {
        (*pVariable).SetValue(pTable->GetInteger());
    }

    *pResult = true;

    SetSynchronousReturnCode(message, hr);

    return hr;
}


template <>
IAction* CCreateable<CControlCommand, IAction>::NewInstance()
{
    IAction *pAction = new CControlCommand;

    return pAction;
}


template <>
void CCreateable<CControlCommand, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ControlCommand)
DEFINE_ACTION_RELEASE_FXN(ControlCommand)
