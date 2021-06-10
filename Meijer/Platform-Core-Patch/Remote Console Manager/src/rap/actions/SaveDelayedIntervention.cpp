// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveDelayedIntervention.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SaveDelayedIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveDelayedIntervention.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 5     1/12/05 5:51p Dm185016
// TAR 290169
// 
// 4     12/22/04 11:01a Dm185016
// TAR 288934 - Added priority key from SCOTAPP
// 
// 3     12/03/04 5:45p Dm185016
// Fixed problem after "Call for help outside transaction".  Was always
// renaming the saved message.
// 
// 2     12/02/04 3:39p Dm185016
// 
// 1     10/29/04 8:53a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SaveDelayedIntervention.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "RCMError.h"
#include "RAEvents.h"
#include "KVParmList.h"
#include "RCMError.h"


CSaveDelayedIntervention::CSaveDelayedIntervention()
{
    CRCMActionBase::SetName(_T("CSaveDelayedIntervention"));
}


CSaveDelayedIntervention::~CSaveDelayedIntervention()
{
}


BEGIN_KV_HANDLER_MAP(CSaveDelayedIntervention)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSaveDelayedIntervention, KEY_MESSAGE_NAME, OnMessageName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSaveDelayedIntervention, KEY_TYPE, OnType)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSaveDelayedIntervention, KEY_EV_PRIORITY, OnPriority)
END_KV_HANDLER_MAP()


DEFINE_KV_HANDLER(CSaveDelayedIntervention, OnMessageName, keys)
{
    m_sMsgName = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
}


DEFINE_KV_HANDLER(CSaveDelayedIntervention, OnType, keys)
{
    m_sType = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
}


DEFINE_KV_HANDLER(CSaveDelayedIntervention, OnPriority, keys)
{
    m_sPriority = CKeyValue<const _TCHAR *>::Get(keys, KEY_EV_PRIORITY_DELAYED);
}


// Get data string from XML
HRESULT CSaveDelayedIntervention::ParseOperands()
{ 
    HRESULT hr = S_OK;

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter != parms.end())
    {
        StringType sKeyString = *iter;
        ParseStringKeys(sKeyString.c_str());
    }

    return hr;
}


HRESULT CSaveDelayedIntervention::Execute(bool *pResult, IMessage &message)
{
    m_sMsgName.clear();

    HRESULT hr = ParseMessageKeys(message);
    if (hr != S_OK)
        return hr;

    if (m_sPriority == KEY_EV_PRIORITY_URGENT)
        m_dwPriority = RA_PRIORITY_URGENT;
    else if (m_sPriority == KEY_EV_PRIORITY_DELAYED)
        m_dwPriority = RA_PRIORITY_DEFER;
    else
        return RCM_API_FAILURE_INVALID_KEY_VALUE_LIST;

    ParseOperands();

    if (!rapmgr().IsDelayedInterventionTypePending(m_sType.c_str()))
    {
        IMessage *pMessage;
        if (m_sMsgName.empty())
            pMessage = &stateMachine().MakeAsynchCopy(message);
        else
            pMessage = &stateMachine().MakeAsynchCopyEx(message, m_sMsgName.c_str());

        rapmgr().SetNextDelayedIntervention(pMessage, m_dwPriority);
    }

    *pResult = true;

    return S_OK;
}


template <>
IAction* CCreateable<CSaveDelayedIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CSaveDelayedIntervention;

    return pAction;
}


template <>
void CCreateable<CSaveDelayedIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SaveDelayedIntervention)
DEFINE_ACTION_RELEASE_FXN(SaveDelayedIntervention)
