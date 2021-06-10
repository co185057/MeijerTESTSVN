// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearRemoteIntervention.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ClearRemoteIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
 *
 * Copyright 2006 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearRemoteIntervention.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 3     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/24/06 10:38a Dm185016
 * TAR 315980
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ClearRemoteIntervention.h"
#include "RemoteAPMgr.h"
#include "RCMKeyValue.h"
#include "Message.h"
#include "algorithms/ClearInterventions.h"


BEGIN_KV_HANDLER_CLASS(CClearRemoteIntervention::CCCIParms)
	DEFINE_KV_HANDLER_MAP_ENTRY(CClearRemoteIntervention::CCCIParms, KEY_COOKIE, Cookie) // TAR 311902
END_KV_HANDLER_CLASS()
DEFINE_KV_INT(CClearRemoteIntervention::CCCIParms, Cookie) // TAR 311902
BEGIN_KV_HANDLER_CLASS_CLEAR(CClearRemoteIntervention::CCCIParms)
    CLEAR_KV_INT(Cookie, -1)
END_KV_HANDLER_CLASS_CLEAR()


CClearRemoteIntervention::CClearRemoteIntervention()
{
    CRCMActionBase::SetName(_T("CClearRemoteIntervention"));
}


CClearRemoteIntervention::~CClearRemoteIntervention()
{
}

HRESULT CClearRemoteIntervention::ParseInterventionData(const CIntervention *pIntervention)
{
    return m_intervention.ParseKeys((const _TCHAR *)pIntervention->GetData());
}


bool CClearRemoteIntervention::accept(IAction *pCci, const CIntervention *pIntervention)
{
    _ASSERTE(pCci!=NULL);

    CClearRemoteIntervention &cci = *dynamic_cast<CClearRemoteIntervention *>(pCci);
    
    cci.ParseInterventionData(pIntervention);

    return (cci.m_parms.GetCookie()  == cci.m_intervention.GetCookie() );
}


HRESULT CClearRemoteIntervention::Execute(bool *pResult, IMessage &message)
{
    m_parms.Clear();
    HRESULT hr = m_parms.ParseKeys((_TCHAR *)message.GetData());
    if (hr != S_OK)
        return hr;

    CIntervention * pIntervention = rapmgr().GetCurrentIntervention();
    hr = ParseInterventionData(pIntervention);
    if (hr != S_OK)
        return hr;

    if (m_parms.GetCookie() == m_intervention.GetCookie())
        rapmgr().SetCurrentIntervention(NULL);

    hr = ClearInterventions(this, &CClearRemoteIntervention::accept);

    *pResult = true;

    return S_OK;
}


template <>
IAction* CCreateable<CClearRemoteIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CClearRemoteIntervention;

    return pAction;
}


template <>
void CCreateable<CClearRemoteIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ClearRemoteIntervention)
DEFINE_ACTION_RELEASE_FXN(ClearRemoteIntervention)
