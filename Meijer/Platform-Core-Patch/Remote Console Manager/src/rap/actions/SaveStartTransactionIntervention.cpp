// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveStartTransactionIntervention.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SaveStartTransactionIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveStartTransactionIntervention.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:15p Dm185016
 * lint
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 1     1/13/05 1:28p Dm185016
// TAR 290304
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SaveStartTransactionIntervention.h"
#include "RemoteAPMgr.h"
#include "RCMKeyValue.h"
#include "RCMMessages.h"
#include "RAEvents.h"
#include "RCMError.h"
#include "Message.h"

using namespace STATE_MACHINE_NAMESPACE;

BEGIN_KV_HANDLER_CLASS(CSaveStartTransactionIntervention::SSTIParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSaveStartTransactionIntervention::SSTIParms, KEY_EV_VIEW, View)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSaveStartTransactionIntervention::SSTIParms, KEY_EV_PRIORITY, Priority)
END_KV_HANDLER_CLASS()
DEFINE_KV_LONG(CSaveStartTransactionIntervention::SSTIParms, View)
DEFINE_KV_CSTRING(CSaveStartTransactionIntervention::SSTIParms, Priority)


void CSaveStartTransactionIntervention::SSTIParms::Clear()
{
    m_View = -1;
    m_Priority.Empty();
}

CSaveStartTransactionIntervention::CSaveStartTransactionIntervention()
{
    CRCMActionBase::SetName(_T("CSaveStartTransactionIntervention"));
}


CSaveStartTransactionIntervention::~CSaveStartTransactionIntervention()
{
}


HRESULT CSaveStartTransactionIntervention::Execute(bool *pResult, IMessage &message)
{
    IMODTRACE_DEVELOP();

    *pResult = true;

    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys((const _TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    if (m_parms.GetPriority() == KEY_EV_PRIORITY_URGENT)
        m_dwPriority = RA_PRIORITY_URGENT;
    else if (m_parms.GetPriority() == KEY_EV_PRIORITY_DELAYED)
        m_dwPriority = RA_PRIORITY_DEFER;
    else
        return RCM_API_FAILURE_INVALID_KEY_VALUE_LIST;

    CIntervention *pIntervention ;

    switch (m_parms.GetView())
    {
        case DATANEEDED_STARTTRANSACTION:   pIntervention = &stateMachine().MakeAsynchCopyEx(message, MSG_START_XACT);  break;
        case DATANEEDED_STARTTRANSACTIONAM: pIntervention = &stateMachine().MakeAsynchCopyEx(message, MSG_START_XACT_ENTER_AM);  break;
        default :
            ITRACE_ERROR(_T("Incorrect view type:  ") + IString(m_parms.GetView()));
            _ASSERTE(0);
            return STATEM_INVALID_PARAMETER;
    }

    rapmgr().SetCurrentIntervention(pIntervention);

    return hr;
}


template <>
IAction* CCreateable<CSaveStartTransactionIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CSaveStartTransactionIntervention;

    return pAction;
}


template <>
void CCreateable<CSaveStartTransactionIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SaveStartTransactionIntervention)
DEFINE_ACTION_RELEASE_FXN(SaveStartTransactionIntervention)
