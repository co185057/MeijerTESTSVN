// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckDefaultButton.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)CheckDefaultButton.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckDefaultButton.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     3/22/05 3:45p Dm185016
 * UNICODE
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
// 
// 5     1/18/05 3:44p Dm185016
// UNICODE support
// 
// 4     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
// 
// 3     11/16/04 11:26a Dm185016
// Passing on data to generated message
// 
// 2     11/02/04 3:06p Dm185016
// More updates
// 
// 1     10/29/04 8:53a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "CheckDefaultButton.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "RCMLaneXML.h"
#include "RCMError.h"
#include "PSXMfc.h"
#include "KVParmList.h"
#include "verify.h"

BEGIN_KV_HANDLER_MAP(CCheckDefaultButton)
    DEFINE_KV_HANDLER_MAP_ENTRY(CCheckDefaultButton, KEY_EV_VIEW, OnView)
END_KV_HANDLER_MAP()


CCheckDefaultButton::CCheckDefaultButton()
{
    CRCMActionBase::SetName(_T("CCheckDefaultButton"));
}


CCheckDefaultButton::~CCheckDefaultButton()
{
}


DEFINE_KV_HANDLER(CCheckDefaultButton, OnView, keys)
{
    m_nView = CKeyValue<int>::Get(keys, 0);

    m_hr = rcmStateMachine().GetViewName(m_sViewName, m_nView);
}


//-----------------------------------------------------------------------------
// Examines the operands from the XML, which is expected to have the form:
//
//  <action>
//      <action-name>push-state</action-name>
//      <parameter>
//          <value>state-name-on-return</value>
//      </parameter>
//
//      ...
//
//  </action>
// The first <parameter> tag specifies an optional state to return to
// when a corresponding pop-state operation is executed
//-----------------------------------------------------------------------------
HRESULT CCheckDefaultButton::ParseOperands()
{ 
    HRESULT hr = S_OK;
    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter == parms.end())
    {
        ITRACE_ERROR(_T("Missing operation"));
        return STATEM_MISSING_PARAMETER;
    }
    m_sVariableName = *iter;
    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Variable name:  ") + IString(m_sVariableName.c_str()));

    return hr;
}


HRESULT CCheckDefaultButton::Execute(bool *pResult, IMessage &message)
{
    m_sViewName.clear();
    m_sVariableName.clear();

    m_hr = ParseOperands();
    if (m_hr != S_OK)
        return m_hr;

    ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());

    if (m_sViewName.empty())
    {
        ITRACE_ERROR(_T("Invalid parameter value:  ") + IString(KEY_EV_VIEW));
        return RCM_API_FAILURE_INVALID_KEY_VALUE_LIST;
    }

    fastlane::PCONTEXT pContext = NULL;
    m_hr = rcmStateMachine().GetContext(&pContext, m_sViewName.c_str());
    if (m_hr != S_OK)
        return m_hr;

    fastlane::IContext &context = *pContext;

    if (context.HasDefaultButton())
    {
        const _TCHAR *pszDefaultButtonName = context.GetDefaultButton();

        PMESSAGE pMessage = NULL;
        m_hr = rcmStateMachine().GetMessage(&pMessage
                                           , pszDefaultButtonName
                                           , message.GetData()
                                           , message.GetDataLength());
        if (m_hr != S_OK)
			return m_hr;

		IState *pState;
		m_hr = rcmStateMachine().GetState(&pState, context.GetName());
        if (m_hr != S_OK)
			return m_hr;

		IVariable *pVariable;
		m_hr = rcmStateMachine().GetVariable(&pVariable, m_sVariableName.c_str());
        if (m_hr != S_OK)
			return m_hr;

		pVariable->SetValue(pState->GetId());

		m_hr = rcmStateMachine().PostMsg(*pMessage);
        if (m_hr != S_OK)
			return m_hr;

        *pResult = false;
    }
    else
    {
        *pResult = true;
    }

    return m_hr;
}


template <>
IAction* CCreateable<CCheckDefaultButton, IAction>::NewInstance()
{
    IAction *pAction = new CCheckDefaultButton;

    return pAction;
}


template <>
void CCreateable<CCheckDefaultButton, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(CheckDefaultButton)
DEFINE_ACTION_RELEASE_FXN(CheckDefaultButton)
