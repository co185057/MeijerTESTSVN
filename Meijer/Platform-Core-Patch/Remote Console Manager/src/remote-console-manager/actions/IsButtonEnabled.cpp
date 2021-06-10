// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsButtonEnabled.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)IsButtonEnabled.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsButtonEnabled.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     1/13/06 3:46p Tk124825
 * TAR 301329
 * 
 * 1     5/01/05 8:59a Tp151000
 * Performmance
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsButtonEnabled.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"
#include "RCMError.h"
#include "verifypsx.h"
#include "PSXMfc.h"

BEGIN_KV_HANDLER_CLASS(CIsButtonEnabled::IBEParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CIsButtonEnabled::IBEParms, KEY_CONTEXT_NAME, ContextName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CIsButtonEnabled::IBEParms, KEY_BUTTON_NAME, ButtonName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CIsButtonEnabled::IBEParms, ContextName)
DEFINE_KV_CSTRING(CIsButtonEnabled::IBEParms, ButtonName)
BEGIN_KV_HANDLER_CLASS_CLEAR(CIsButtonEnabled::IBEParms)
    CLEAR_KV_CSTRING(ContextName)
    CLEAR_KV_CSTRING(ButtonName)
END_KV_HANDLER_CLASS_CLEAR()


CIsButtonEnabled::CIsButtonEnabled()
{
    CRCMActionBase::SetName(_T("CIsButtonEnabled"));
}


CIsButtonEnabled::~CIsButtonEnabled()
{
}



// Get all control names from the xml
HRESULT CIsButtonEnabled::ParseOperands()
{ 
    m_parms.Clear();

    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();

    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    return m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
}


long CIsButtonEnabled::GetValue() const { return 0; }
void CIsButtonEnabled::SetValue(long) {}


HRESULT CIsButtonEnabled::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    if (m_parms.GetButtonName().IsEmpty())
        return STATEM_MISSING_PARAMETER;

    if (m_parms.GetContextName().IsEmpty())
        return STATEM_MISSING_PARAMETER;

    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Checking:  ") + IString((const _TCHAR *)m_parms.GetButtonName()) +
         _T(".  Context:  ") + IString((const _TCHAR *)m_parms.GetContextName()));

    bool bUseAllContexts = (m_parms.GetContextName() == KEY_CONTEXT_NAME_ALL);
    const _TCHAR *pszContextName = (bUseAllContexts ? UI::ALL_CONTEXTS : rcm().GetCurrentContext());

    COleVariant varState;
    VERIFY_PSX(hr, psx().SetConfigProperty((const _TCHAR *)m_parms.GetButtonName(), pszContextName, UI::PROPERTYSTATE, varState));
    if (hr != S_OK)
        *pResult = false;
    else
    {
        _variant_t state((LPVARIANT)varState);
        *pResult = ((long)state == (long)UI::Normal);
    }

    return hr;
}

template <>
IAction* CCreateable<CIsButtonEnabled, IAction>::NewInstance()
{
    IAction *pAction = new CIsButtonEnabled;

    return pAction;
}


template <>
void CCreateable<CIsButtonEnabled, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsButtonEnabled)
DEFINE_ACTION_RELEASE_FXN(IsButtonEnabled)
