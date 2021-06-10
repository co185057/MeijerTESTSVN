// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SetCommand.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)SetCommand.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SetCommand.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 10    11/29/07 4:39p Cb160006
 * TAR 364905
 * 
 * 9     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 6     4/12/05 6:35p Dm185016
 * 
 * 5     4/12/05 6:34p Dm185016
 * TAR 297347
 * 
 * 4     4/05/05 11:59a Dm185016
 * TAR 297356
 * 
 * 3     3/15/05 3:37p Dm185016
 * Add comment.
 * 
 * 2     2/17/05 5:57p Dm185016
 * TAR 293450
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
// 
// 5     1/12/05 6:08p Dm185016
// Cleaned up trace log.
// 
// 4     12/07/04 4:24p Dm185016
// Removed sending the assist mode messages.
// 
// 3     11/16/04 11:42a Dm185016
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

#include "SetCommand.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "RCMError.h"
#include "KVParmList.h"

BEGIN_KV_HANDLER_MAP(CSetCommand)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSetCommand, KEY_SET_ID, OnId)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSetCommand, KEY_SET_CMD_VALUE, OnValue)
END_KV_HANDLER_MAP()


CSetCommand::CSetCommand()
    : m_pMessage(NULL)
{
    CRCMActionBase::SetName(_T("CSetCommand"));
}


CSetCommand::~CSetCommand()
{
    m_pMessage = NULL;
}


DEFINE_KV_HANDLER(CSetCommand, OnId, keys)
{
    m_szId = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
}


DEFINE_KV_HANDLER(CSetCommand, OnValue, keys)
{
    m_szValue = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
}


HRESULT CSetCommand::Set()
{
	HRESULT hr = S_OK;

	if (m_szId == KEY_SET_ID_XACT_TOTAL)
	{
        //Do nothing! Future use?
	}

    else if (m_szId == KEY_SET_ID_REQUEST_ENABLE_ASSIST_MODE)
	{
        // Save the value passed from SCOTAPP
		bool bValue = IString(m_szValue.c_str()).asBool();
		ITRACE_DEVELOP(_T("Assist mode enabled set:  ") + IString(bValue));
		rapmgr().SetAssistModeEnabled(bValue);

        // Send a message to the state machine to give it a chance to perform
        // additional actions.
        IMessage *pNewMsg;
        const _TCHAR *pszMsgName;
        if (bValue)
            pszMsgName = MSG_ENABLE_ASSIST_MODE_BUTTON;
        else
            pszMsgName = MSG_DISABLE_ASSIST_MODE_BUTTON;
		ITRACE_DEVELOP(_T("Sending msg:  ") + IString(pszMsgName));

        hr = stateMachine().GetMessage(&pNewMsg, pszMsgName);
        if (hr == S_OK)
            hr = stateMachine().PostMsg(*pNewMsg);
	}

    else if (m_szId == KEY_SET_ID_REQUEST_ENABLE_HH_ASSIST_MODE)
	{
        // Save the value passed from SCOTAPP
		bool bValue = IString(m_szValue.c_str()).asBool();
		ITRACE_DEVELOP(_T("HH Assist mode enabled set:  ") + IString(bValue));
		rapmgr().SetHandheldAssistModeEnabled(bValue);
	}
	else if (m_szId == KEY_SET_ID_FORCE_APPROVAL_BEFORE_SUSPEND)
	{
		// Save the value passed from SCOTAPP
		bool bValue = IString(m_szValue.c_str()).asBool();
		ITRACE_DEVELOP(_T("Force approval before suspend set:  ") + IString(bValue));
		rapmgr().SetForceApprovalBeforeSuspend(bValue);
	}

	return hr;
}


HRESULT CSetCommand::Execute(bool *pResult, IMessage &message)
{
    m_pMessage = &message;

	m_szId.clear();
    m_szValue.clear();

    ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());

	if (m_szId.empty() || m_szValue.empty())
		m_hr = RCM_API_FAILURE_INVALID_KEY_VALUE_LIST;

	else
		m_hr = Set();

    *pResult = true;

    return m_hr;
}


IMessage &CSetCommand::GetMessage() 
{ 
    _ASSERTE(m_pMessage!=NULL);
    return *m_pMessage; 
}

template <>
IAction* CCreateable<CSetCommand, IAction>::NewInstance()
{
    IAction *pAction = new CSetCommand;

    return pAction;
}


template <>
void CCreateable<CSetCommand, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SetCommand)
DEFINE_ACTION_RELEASE_FXN(SetCommand)
