// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchLaneContextCommand.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SwitchLaneContextCommand.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchLaneContextCommand.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 1     1/20/05 10:35a Dm185016
// Added handler for lane context switches.
*/
#include "StdAfx.h"
#include "SwitchLaneContextCommand.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMEvent.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "SwitchContext.h"

using namespace fastlane;


BEGIN_KV_HANDLER_CLASS(CSwitchLaneContextCommand::SLCCParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSwitchLaneContextCommand::SLCCParms, KEY_CT_CONTEXT, Context)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CSwitchLaneContextCommand::SLCCParms, Context)


void CSwitchLaneContextCommand::SLCCParms::Clear()
{
    m_Context.Empty();
}


CSwitchLaneContextCommand::CSwitchLaneContextCommand()
{
    CRCMActionBase::SetName(_T("CSwitchLaneContextCommand"));
}


CSwitchLaneContextCommand::~CSwitchLaneContextCommand()
{
}


HRESULT CSwitchLaneContextCommand::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = m_parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    StringType sContext = (const _TCHAR *)m_parms.GetContext();
    if (sContext.empty())
        return STATEM_MISSING_PARAMETER;

    rapmgr().SetCurrentLaneContext(sContext.c_str());

    *pResult = true;

    return hr;
}


template <>
IAction* CCreateable<CSwitchLaneContextCommand, IAction>::NewInstance()
{
    IAction *pAction = new CSwitchLaneContextCommand;

    return pAction;
}


template <>
void CCreateable<CSwitchLaneContextCommand, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SwitchLaneContextCommand)
DEFINE_ACTION_RELEASE_FXN(SwitchLaneContextCommand)
