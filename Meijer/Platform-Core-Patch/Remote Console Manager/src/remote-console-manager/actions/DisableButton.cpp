// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/DisableButton.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)DisableButton.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/DisableButton.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:35p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 3     1/20/05 10:39a Dm185016
// TAR 291218 - Need to be able to specify ALL_CONTEXTS for buttons
// 
// 2     1/18/05 3:48p Dm185016
// UNICODE support
// 
// 1     10/29/04 9:04a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "DisableButton.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"

BEGIN_KV_HANDLER_CLASS(CDisableButton::DParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisableButton::DParms, KEY_CONTEXT_NAME, ContextName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisableButton::DParms, KEY_BUTTON_NAME, ButtonName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CDisableButton::DParms, ContextName)
DEFINE_KV_CSTRING(CDisableButton::DParms, ButtonName)


void CDisableButton::DParms::Clear()
{
    m_ContextName.Empty();
    m_ButtonName.Empty();
}


CDisableButton::CDisableButton()
{
    CRCMActionBase::SetName(_T("CDisableButton"));
}


CDisableButton::~CDisableButton()
{
}


// Get all control names from the xml
HRESULT CDisableButton::NextOperands(CParameterList::iterator &iter, CParameterList::const_iterator &end)
{ 
    HRESULT hr = S_OK;

    if (iter == end)
        return STATEM_MISSING_PARAMETER;

    // Enable all buttons specified for the action
    while (iter != end)
    {
        m_parms.Clear();
        hr = m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
        if (hr != S_OK)
            return hr;

        ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Enabling:  ") + IString((const _TCHAR *)m_parms.GetButtonName()));

        bool bUseAllContexts = (m_parms.GetContextName() == _T("all"));

        hr = EnableControl((const _TCHAR *)m_parms.GetButtonName(), false, bUseAllContexts);
        if (hr != S_OK)
            return hr;
    
        iter++;
    }

    return hr;
}


// Get all control names from the xml
HRESULT CDisableButton::ParseOperands()
{ 
    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();
    CParameterList::iterator end    = parms.end();

    return NextOperands(iter, end);
}


HRESULT CDisableButton::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = ParseOperands();

    *pResult = true;

    return hr;
}

template <>
IAction* CCreateable<CDisableButton, IAction>::NewInstance()
{
    IAction *pAction = new CDisableButton;

    return pAction;
}


template <>
void CCreateable<CDisableButton, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(DisableButton)
DEFINE_ACTION_RELEASE_FXN(DisableButton)
