// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchContextImmediateIntervention.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SwitchContextImmediateIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchContextImmediateIntervention.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/03/05 3:14p Dm185016
 * TAR 291700
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SwitchContextImmediateIntervention.h"
#include "RemoteAPMgr.h"
#include "Message.h"
#include "RCMKeyValue.h"

using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;


BEGIN_KV_HANDLER_CLASS(CSwitchContextImmediateIntervention::SCIIParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSwitchContextImmediateIntervention::SCIIParms, KEY_EV_TYPE, Type)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CSwitchContextImmediateIntervention::SCIIParms, Type)


void CSwitchContextImmediateIntervention::SCIIParms::Clear()
{
    m_Type.Empty();
}

CSwitchContextImmediateIntervention::CSwitchContextImmediateIntervention()
{
    CRCMActionBase::SetName(_T("CSwitchContextImmediateIntervention"));
}


CSwitchContextImmediateIntervention::~CSwitchContextImmediateIntervention()
{
}


// Get all control names from the xml
HRESULT CSwitchContextImmediateIntervention::ParseOperands()
{ 
    HRESULT hr = S_OK;

    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();
    CParameterList::iterator end    = parms.end();

    hr = NextOperands(iter, end);

    return hr;
}


HRESULT CSwitchContextImmediateIntervention::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    SCIIParms parms;

    hr = parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    const _TCHAR *pszTableName = GetStateTableName();
    _ASSERTE(pszTableName[0]!=0);
    if (pszTableName[0]==0)
        return STATEM_MISSING_PARAMETER;

    SetNextState(pszTableName, parms.GetType());

    *pResult = true;

    return S_OK;
}


template <>
IAction* CCreateable<CSwitchContextImmediateIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CSwitchContextImmediateIntervention;

    return pAction;
}


template <>
void CCreateable<CSwitchContextImmediateIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SwitchContextImmediateIntervention)
DEFINE_ACTION_RELEASE_FXN(SwitchContextImmediateIntervention)
