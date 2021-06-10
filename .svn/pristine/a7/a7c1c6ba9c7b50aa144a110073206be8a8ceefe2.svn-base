// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsNotHandheld.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)IsNotHandheld.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsNotHandheld.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     3/02/05 4:51p Dm185016
 * TAR 294890.
 * 
 * 2     2/28/05 4:38p Dm185016
 * lint
 * 
 * 1     2/18/05 11:26a Dm185016
 * TAR 293795.
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsNotHandheld.h"
#include "RCManager.h"
#include "Message.h"
#include "RCMKeyValue.h"


BEGIN_KV_HANDLER_CLASS(CIsNotHandheld::INHParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CIsNotHandheld::INHParms, KEY_CT_USE_MESSAGE, UseMessage)
    DEFINE_KV_HANDLER_MAP_ENTRY(CIsNotHandheld::INHParms, KEY_CT_REMOTE_CONNECTION_NAME, RemoteConnectionName)
END_KV_HANDLER_CLASS()
DEFINE_KV_BOOL(CIsNotHandheld::INHParms, UseMessage)
DEFINE_KV_CSTRING(CIsNotHandheld::INHParms, RemoteConnectionName)


void CIsNotHandheld::INHParms::Clear()
{
    SetUseMessage(false);
    m_RemoteConnectionName.Empty();
}


CIsNotHandheld::CIsNotHandheld()
{
    CRCMActionBase::SetName(_T("CIsNotHandheld"));
}


CIsNotHandheld::~CIsNotHandheld()
{
}


// Get all control names from the xml
HRESULT CIsNotHandheld::ParseOperands()
{ 
    m_parms.Clear();

    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();
    CParameterList::iterator end    = parms.end();
    if (iter != end)
        return m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());

    return S_OK;
}


HRESULT CIsNotHandheld::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    if (m_parms.IsUseMessage())
    {
        HRESULT hr = m_parms.ParseKeys((const _TCHAR *)message.GetData(), GetTraceHandle());
        if (hr != S_OK)
            return hr;

        *pResult = !rcm().IsHandHeld(m_parms.GetRemoteConnectionName());
    }
    else
        *pResult = !rcm().IsHandHeld();

    return S_OK;
}


template <>
IAction* CCreateable<CIsNotHandheld, IAction>::NewInstance()
{
    IAction *pAction = new CIsNotHandheld;

    return pAction;
}


template <>
void CCreateable<CIsNotHandheld, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsNotHandheld)
DEFINE_ACTION_RELEASE_FXN(IsNotHandheld)
