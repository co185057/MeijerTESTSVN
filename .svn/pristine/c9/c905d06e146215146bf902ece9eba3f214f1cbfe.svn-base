// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/MatchLaneContext.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)MatchLaneContext.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/MatchLaneContext.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 3     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     5/31/05 11:33a Dm185016
 * TAR 301203
 * 
 * 1     5/26/05 1:31p Dm185016
 * TAR 301203
*/
#include "StdAfx.h"
#include "MatchLaneContext.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "StringComparator.h"

using namespace fastlane;


BEGIN_KV_HANDLER_CLASS(CMatchLaneContext::SLCCParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CMatchLaneContext::SLCCParms, KEY_CT_CONTEXT, Context)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CMatchLaneContext::SLCCParms, Context)


void CMatchLaneContext::SLCCParms::Clear()
{
    m_Context.Empty();
}


CMatchLaneContext::CMatchLaneContext()
{
    CRCMActionBase::SetName(_T("CMatchLaneContext"));
}


CMatchLaneContext::~CMatchLaneContext()
{
}


// Get all control names from the xml
HRESULT CMatchLaneContext::ParseOperands()
{ 
    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    return m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
}


HRESULT CMatchLaneContext::Execute(bool *pResult, IMessage &message)
{
    m_parms.Clear();

    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    StringType sContextName = (const _TCHAR *)m_parms.GetContext();
    if (sContextName.empty())
        return STATEM_MISSING_PARAMETER;

    hr = m_parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    StringType sCurrentContextName = (const _TCHAR *)m_parms.GetContext();
    if (sCurrentContextName.empty())
        return STATEM_MISSING_PARAMETER;

    *pResult = (sContextName == sCurrentContextName);

    return hr;
}


template <>
IAction* CCreateable<CMatchLaneContext, IAction>::NewInstance()
{
    IAction *pAction = new CMatchLaneContext;

    return pAction;
}


template <>
void CCreateable<CMatchLaneContext, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(MatchLaneContext)
DEFINE_ACTION_RELEASE_FXN(MatchLaneContext)
