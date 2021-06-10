// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsHandheld.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)IsHandheld.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsHandheld.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     3/02/05 5:26p Dm185016
 * TAR 294890.
 * 
 * 2     2/28/05 4:38p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 1     12/22/04 11:13a Dm185016
// HH support.  action returns true if current remote connection is
// handheld.
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsHandheld.h"
#include "RCManager.h"
#include "Message.h"
#include "RCMKeyValue.h"


BEGIN_KV_HANDLER_CLASS(CIsHandheld::IHParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CIsHandheld::IHParms, KEY_CT_USE_MESSAGE, UseMessage)
    DEFINE_KV_HANDLER_MAP_ENTRY(CIsHandheld::IHParms, KEY_CT_REMOTE_CONNECTION_NAME, RemoteConnectionName)
END_KV_HANDLER_CLASS()
DEFINE_KV_BOOL(CIsHandheld::IHParms, UseMessage)
DEFINE_KV_CSTRING(CIsHandheld::IHParms, RemoteConnectionName)


void CIsHandheld::IHParms::Clear()
{
    SetUseMessage(false);
    m_RemoteConnectionName.Empty();
}


CIsHandheld::CIsHandheld()
{
    CRCMActionBase::SetName(_T("CIsHandheld"));
}


CIsHandheld::~CIsHandheld()
{
}


// Get all control names from the xml
HRESULT CIsHandheld::ParseOperands()
{ 
    m_parms.Clear();

    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();
    CParameterList::iterator end    = parms.end();
    if (iter != end)
        return m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());

    return S_OK;
}


HRESULT CIsHandheld::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    if (m_parms.IsUseMessage())
    {
        HRESULT hr = m_parms.ParseKeys((const _TCHAR *)message.GetData(), GetTraceHandle());
        if (hr != S_OK)
            return hr;

        *pResult = rcm().IsHandHeld(m_parms.GetRemoteConnectionName());
    }
    else
        *pResult = rcm().IsHandHeld();

    return S_OK;
}


template <>
IAction* CCreateable<CIsHandheld, IAction>::NewInstance()
{
    IAction *pAction = new CIsHandheld;

    return pAction;
}


template <>
void CCreateable<CIsHandheld, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsHandheld)
DEFINE_ACTION_RELEASE_FXN(IsHandheld)
