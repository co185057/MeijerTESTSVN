// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearApprovedIntervention.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ClearApprovedIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearApprovedIntervention.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 5     6/23/06 8:32p Lt185007
 * tar migration 300190
 * 
 * 2     2/28/05 4:08p Dm185016
 * lint
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
// 
// 2     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
// 
// 1     12/09/04 1:45p Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ClearApprovedIntervention.h"
#include "RemoteAPMgr.h"
#include "RCMKeyValue.h"
#include "Message.h"
#include "SMConstants.h"
#include "algorithms/ClearInterventions.h"

BEGIN_KV_HANDLER_CLASS(CClearApprovedIntervention::CAIParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CClearApprovedIntervention::CAIParms, KEY_TYPE, Type)
    DEFINE_KV_HANDLER_MAP_ENTRY(CClearApprovedIntervention::CAIParms, KEY_COOKIE, Cookie)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CClearApprovedIntervention::CAIParms, Type)
DEFINE_KV_INT(CClearApprovedIntervention::CAIParms, Cookie)
BEGIN_KV_HANDLER_CLASS_CLEAR(CClearApprovedIntervention::CAIParms)
    CLEAR_KV_CSTRING(Type)
    CLEAR_KV_INT(Cookie, 0)
END_KV_HANDLER_CLASS_CLEAR()


CClearApprovedIntervention::CClearApprovedIntervention()
{
    CRCMActionBase::SetName(_T("CClearApprovedIntervention"));
}


CClearApprovedIntervention::~CClearApprovedIntervention()
{
}



// Get all control names from the xml
HRESULT CClearApprovedIntervention::ParseOperands()
{ 
    HRESULT hr = S_OK;

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;
    
    m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());

    m_ApprovalType = m_parms.GetType();
    
    return hr;
}


bool CClearApprovedIntervention::accept(IAction *pCai, const CIntervention *pIntervention)
{
    _ASSERTE(pCai!=NULL);

    CClearApprovedIntervention &cai = *dynamic_cast<CClearApprovedIntervention *>(pCai);
    
    //ITRACE_DUMP_STATIC(cai.GetTraceHandle(), _T("accept data"), (const _TCHAR *)pIntervention->GetData(), pIntervention->GetDataLength());
    cai.m_parms.ParseKeys((const _TCHAR *)pIntervention->GetData(), pIntervention->GetTraceHandle());

    bool bAccepted = (cai.m_ApprovalType == (const _TCHAR *)cai.m_parms.GetType());

    if (bAccepted)
        cai.SignalApproval();

    return bAccepted;
}

void CClearApprovedIntervention::SignalApproval()
{
    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR *>::Put(parmList
                        , KEY_RCEV_OPERATION
                        , KEY_RCEV_OPERATION_INTERVENTION_APPROVED);
    CKeyValue<const _TCHAR *>::Put(parmList
                        , KEY_RCEV_TYPE
                        , m_parms.GetType());
    CKeyValue<INT>::Put(parmList
                        , KEY_RCEV_COOKIE
                        , m_parms.GetCookie());

    rapmgr().FireEvent( new CRemoteConsoleEvent((const _TCHAR *)parmList) );
}


HRESULT CClearApprovedIntervention::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    *pResult = true;

    hr = ClearInterventions(this, &CClearApprovedIntervention::accept);

    return hr;
}


template <>
IAction* CCreateable<CClearApprovedIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CClearApprovedIntervention;

    return pAction;
}


template <>
void CCreateable<CClearApprovedIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ClearApprovedIntervention)
DEFINE_ACTION_RELEASE_FXN(ClearApprovedIntervention)
