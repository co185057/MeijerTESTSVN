// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisableAMOnHandheld.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisableAMOnHandheld.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisableAMOnHandheld.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     3/22/05 2:45p Dm185016
 * UNICODE
 * 
 * 1     3/14/05 8:26p Dm185016
 * TAR 294587
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "DisableAMOnHandheld.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "PSXMfc.h"
#include "RAPUIDefines.h"
#include "RCMKeyValue.h"
#include "verifypsx.h"
#include "RCMError.h"

BEGIN_KV_HANDLER_CLASS(CDisableAMOnHandheld::DAMOHParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisableAMOnHandheld::DAMOHParms, KEY_CONTEXT_NAME, ContextName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisableAMOnHandheld::DAMOHParms, KEY_BUTTON_NAME, ButtonName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CDisableAMOnHandheld::DAMOHParms, ContextName)
DEFINE_KV_CSTRING(CDisableAMOnHandheld::DAMOHParms, ButtonName)


void CDisableAMOnHandheld::DAMOHParms::Clear()
{
    m_ContextName = _T("all");
    m_ButtonName.Empty();
}


CDisableAMOnHandheld::CDisableAMOnHandheld()
{
    CRCMActionBase::SetName(_T("CDisableAMOnHandheld"));
}


CDisableAMOnHandheld::~CDisableAMOnHandheld()
{
}


// Get all control names from the xml
HRESULT CDisableAMOnHandheld::ParseOperands()
{ 
    m_parms.Clear();

    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();
    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    return m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
}


HRESULT CDisableAMOnHandheld::Execute(bool *pResult, IMessage &)
{
    *pResult = true;

    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    bool bUseAllContexts = (m_parms.GetContextName() == _T("all"));

    const CRemoteConnectionsMap &connections = rcm().GetRemoteConnections();
    CRemoteConnectionsMap::const_iterator iter = connections.begin();
    for (;iter != connections.end(); iter++)
    {
        CRemoteConnection &remoteConn = *iter->second;
        if (remoteConn.IsHandheld())
        {
            ITRACE_DEVELOP(_T("Disabling AM button on:  ") 
                           + IString(remoteConn.GetRemoteConnectionName()) );

            hr = EnableControl((const _TCHAR *)m_parms.GetButtonName(), false, bUseAllContexts);
            if (hr != S_OK)
                return hr;
        }
    }

    return hr;
}


template <>
IAction* CCreateable<CDisableAMOnHandheld, IAction>::NewInstance()
{
    IAction *pAction = new CDisableAMOnHandheld;

    return pAction;
}


template <>
void CCreateable<CDisableAMOnHandheld, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(DisableAMOnHandheld)
DEFINE_ACTION_RELEASE_FXN(DisableAMOnHandheld)
