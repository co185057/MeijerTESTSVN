// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/EnableInputEvents.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)EnableInputEvents.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/EnableInputEvents.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:12p Dm185016
 * lint
 * 
 * 1     2/03/05 5:42p Tp151000
 * TAR 292361
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 2     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
// 
// 1     11/16/04 11:23a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "EnableInputEvents.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCManager.h"
#include "RCMMessages.h"
#include "RAPMessages.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"
#include "RAPReceipt.h"


BEGIN_KV_HANDLER_CLASS(CEnableInputEvents::EIEParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CEnableInputEvents::EIEParms, KEY_EV_ENABLE, Enable)
END_KV_HANDLER_CLASS()
DEFINE_KV_BOOL(CEnableInputEvents::EIEParms, Enable)


void CEnableInputEvents::EIEParms::Clear()
{
}


CEnableInputEvents::CEnableInputEvents()
{
    CRCMActionBase::SetName(_T("CEnableInputEvents"));
}


CEnableInputEvents::~CEnableInputEvents()
{
}


// Get all control names from the xml
HRESULT CEnableInputEvents::ParseOperands()
{ 
    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();
    CParameterList::iterator end    = parms.end();

    if (iter == end)
        return STATEM_MISSING_PARAMETER;

    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    return CPSXAction::NextOperands(iter, end);
}


HRESULT CEnableInputEvents::Execute(bool *pResult, IMessage &)
{
    *pResult = true;

	HRESULT hr = ParseOperands();

    if (hr == S_OK)
    {
        rapmgr().SetInputEventsEnabled(m_parms.IsEnable());
    }

    return hr;
}

template <>
IAction* CCreateable<CEnableInputEvents, IAction>::NewInstance()
{
    IAction *pAction = new CEnableInputEvents;

    return pAction;
}


template <>
void CCreateable<CEnableInputEvents, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(EnableInputEvents)
DEFINE_ACTION_RELEASE_FXN(EnableInputEvents)


