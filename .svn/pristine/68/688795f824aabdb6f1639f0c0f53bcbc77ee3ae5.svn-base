// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CanSuspendNowEvent.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)CanSuspendNowEvent.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CanSuspendNowEvent.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/11/05 1:45p Dm185016
 * Moved to rap from rcm
 * 
 * 2     2/28/05 4:31p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 2     1/12/05 5:58p Dm185016
// Cleaned up trace log.
// 
// 1     10/29/04 9:04a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "CanSuspendNowEvent.h"
#include "RCMKeyValue.h"
#include "Message.h"
#include "KVParmList.h"

BEGIN_KV_HANDLER_MAP(CCanSuspendNowEvent)
    DEFINE_KV_HANDLER_MAP_ENTRY(CCanSuspendNowEvent, KEY_EV_VALUE, OnValue)
END_KV_HANDLER_MAP()


CCanSuspendNowEvent::CCanSuspendNowEvent()
    : m_bValue(false)
{
    CRCMActionBase::SetName(_T("CCanSuspendNowEvent"));
}


CCanSuspendNowEvent::~CCanSuspendNowEvent()
{
}


DEFINE_KV_HANDLER(CCanSuspendNowEvent, OnValue, keys)
{
    m_bValue = CKeyValue<bool>::Get(keys, false);
    ITRACE_DEVELOP(_T("m_bValue:  ") + IString(m_bValue));
}


HRESULT CCanSuspendNowEvent::Execute(bool *pResult, IMessage &message)
{
    IMODTRACE_DEVELOP();

    m_bValue    = false;

    HRESULT hr = ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    *pResult = m_bValue;

    return S_OK;
}


template <>
IAction* CCreateable<CCanSuspendNowEvent, IAction>::NewInstance()
{
    IAction *pAction = new CCanSuspendNowEvent;

    return pAction;
}


template <>
void CCreateable<CCanSuspendNowEvent, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(CanSuspendNowEvent)
DEFINE_ACTION_RELEASE_FXN(CanSuspendNowEvent)
