// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayDelayedIntervention.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisplayDelayedIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayDelayedIntervention.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     3/25/05 1:06p Dm185016
 * TAR 296505
 * 
 * 3     3/22/05 2:46p Dm185016
 * UNICODE
 * 
 * 2     3/07/05 6:39p Dm185016
 * TAR 293999.
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
// 
// 5     1/12/05 5:50p Dm185016
// TAR 289897
// 
// 4     12/22/04 10:33a Dm185016
// TAR 288934 - Factored out common code into DisplayInterventionBase.
// 
// 3     12/14/04 2:16p Tp151000
// fix build error
// 
// 2     12/14/04 11:19a Dm185016
// TAR 288478.  Cleanup
// 
// 1     11/30/04 4:18p Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "DisplayDelayedIntervention.h"
#include "RemoteAPMgr.h"
#include "RCMKeyValue.h"
#include "Message.h"

using namespace rap;

BEGIN_KV_HANDLER_CLASS(CDisplayDelayedIntervention::DDIParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisplayDelayedIntervention::DDIParms, KEY_TYPE, Type)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CDisplayDelayedIntervention::DDIParms, Type)


void CDisplayDelayedIntervention::DDIParms::Clear()
{
    m_Type.Empty();
}

CDisplayDelayedIntervention::CDisplayDelayedIntervention()
{
    CRCMActionBase::SetName(_T("CDisplayDelayedIntervention"));
}


CDisplayDelayedIntervention::~CDisplayDelayedIntervention()
{
}


CIntervention *CDisplayDelayedIntervention::GetIntervention()
{
    return m_pMsg;
}


bool CDisplayDelayedIntervention::OverrideDisplay() { return true; }


bool CDisplayDelayedIntervention::IsAddDescriptionToReceipt() 
{ 
    const _TCHAR *pszType = m_parms.GetType();
    ITRACE_DEVELOP(_T(__FUNCTION__) _T("Checking for intervention type:  ") + IString(pszType));
    return !rap::CDescriptionParser::rapmgr().IsDelayedInterventionTypePending(pszType); 
}


HRESULT CDisplayDelayedIntervention::Execute(bool *pResult, IMessage &message)
{
    m_pMsg = &message;

    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys((const _TCHAR *)message.GetData(), GetTraceHandle()); 
    if (hr != S_OK)
        return hr;

    return CDisplayInterventionBase::Execute(pResult, message);
}


template <>
IAction* CCreateable<CDisplayDelayedIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CDisplayDelayedIntervention;

    return pAction;
}


template <>
void CCreateable<CDisplayDelayedIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(DisplayDelayedIntervention)
DEFINE_ACTION_RELEASE_FXN(DisplayDelayedIntervention)
