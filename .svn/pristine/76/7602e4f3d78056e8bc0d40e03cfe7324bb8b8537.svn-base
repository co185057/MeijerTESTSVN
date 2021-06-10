// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/EnableButton.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)EnableButton.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/EnableButton.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     2/28/05 4:35p Dm185016
 * lint
 * 
 * 3     2/21/05 8:08p Dm185016
 * Cleaned hard coded values.
 * 
 * 2     2/17/05 5:56p Dm185016
 * Corrected trace output.
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 4     1/20/05 10:39a Dm185016
// TAR 291218 - Need to be able to specify ALL_CONTEXTS for buttons
// 
// 3     1/18/05 3:48p Dm185016
// UNICODE support
// 
// 2     1/12/05 6:00p Dm185016
// Cleaned up trace log.
// 
// 1     10/29/04 9:04a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "EnableButton.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"
#include "RCMError.h"

BEGIN_KV_HANDLER_CLASS(CEnableButton::EParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CEnableButton::EParms, KEY_CONTEXT_NAME, ContextName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CEnableButton::EParms, KEY_BUTTON_NAME, ButtonName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CEnableButton::EParms, ContextName)
DEFINE_KV_CSTRING(CEnableButton::EParms, ButtonName)


void CEnableButton::EParms::Clear()
{
    m_ContextName.Empty();
    m_ButtonName.Empty();
}


CEnableButton::CEnableButton()
{
    CRCMActionBase::SetName(_T("CEnableButton"));
}


CEnableButton::~CEnableButton()
{
}


// Get all control names from the xml
HRESULT CEnableButton::NextOperands(CParameterList::iterator &iter, CParameterList::const_iterator &end)
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
        {
            CRCMErrorInfo error(hr);
            ITRACE_ERROR(error.text());
        }

        ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Enabling:  ") + IString((const _TCHAR *)m_parms.GetButtonName()));

        bool bUseAllContexts = (m_parms.GetContextName() == KEY_CONTEXT_NAME_ALL);

        hr = EnableControl((const _TCHAR *)m_parms.GetButtonName(), true, bUseAllContexts);
        if (hr != S_OK)
        {
            CRCMErrorInfo error(hr);
            ITRACE_ERROR(error.text());
        }
    
        iter++;
    }

    return hr;
}


// Get all control names from the xml
HRESULT CEnableButton::ParseOperands()
{ 
    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();
    CParameterList::iterator end    = parms.end();

    return NextOperands(iter, end);
}


HRESULT CEnableButton::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = ParseOperands();

    *pResult = true;

    return hr;
}

template <>
IAction* CCreateable<CEnableButton, IAction>::NewInstance()
{
    IAction *pAction = new CEnableButton;

    return pAction;
}


template <>
void CCreateable<CEnableButton, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(EnableButton)
DEFINE_ACTION_RELEASE_FXN(EnableButton)
