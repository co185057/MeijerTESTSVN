// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsInterventionTypeOutstanding.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsInterventionTypeOutstanding.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsInterventionTypeOutstanding.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/07/05 6:58p Dm185016
 * TAR 293999
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsInterventionTypeOutstanding.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"


BEGIN_KV_HANDLER_CLASS(CIsInterventionTypeOutstanding::IITOParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CIsInterventionTypeOutstanding::IITOParms, KEY_TYPE, Type)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CIsInterventionTypeOutstanding::IITOParms, Type)


void CIsInterventionTypeOutstanding::IITOParms::Clear()
{
    m_Type.Empty();
}


CIsInterventionTypeOutstanding::CIsInterventionTypeOutstanding()
{
    CRCMActionBase::SetName(_T("CIsInterventionTypeOutstanding"));
}


CIsInterventionTypeOutstanding::~CIsInterventionTypeOutstanding()
{
}


HRESULT CIsInterventionTypeOutstanding::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = m_parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    *pResult = rapmgr().IsDelayedInterventionTypePending(m_parms.GetType());
    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Intervention type outstanding:  ") + IString(*pResult));

    return S_OK;
}

template <>
IAction* CCreateable<CIsInterventionTypeOutstanding, IAction>::NewInstance()
{
    IAction *pAction = new CIsInterventionTypeOutstanding;

    return pAction;
}


template <>
void CCreateable<CIsInterventionTypeOutstanding, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsInterventionTypeOutstanding)
DEFINE_ACTION_RELEASE_FXN(IsInterventionTypeOutstanding)
