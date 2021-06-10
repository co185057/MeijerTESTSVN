// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendSuspendRequest.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SendSuspendRequest.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendSuspendRequest.cpp $
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
// 2     1/12/05 5:52p Dm185016
// Static messages now go to correct log file.
// 
// 1     11/30/04 4:18p Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SendSuspendRequest.h"
#include "RCManager.h"
#include "RCMEvent.h"
#include "Message.h"
#include "RCMKeyValue.h"


BEGIN_KV_HANDLER_MAP(CSendSuspendRequest)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSendSuspendRequest, KEY_SREV_REASON, OnReason)
END_KV_HANDLER_MAP()


CSendSuspendRequest::CSendSuspendRequest()
{
    CRCMActionBase::SetName(_T("CSendSuspendRequest"));
}


CSendSuspendRequest::~CSendSuspendRequest()
{
}


DEFINE_KV_HANDLER(CSendSuspendRequest, OnReason, keys)
{
    m_sReason = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
    ITRACE_DEVELOP(_T("m_sReason:  ") + IString(m_sReason.c_str()));
}


// Get data string from XML
HRESULT CSendSuspendRequest::ParseOperands()
{ 
    HRESULT hr = S_OK;

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter != parms.end())
    {
        StringType sKeyString = *iter;
        ParseKeys(sKeyString.c_str(), GetTraceHandle());
    }
    else
        hr = STATEM_MISSING_PARAMETER;

    return hr;
}


HRESULT CSendSuspendRequest::Execute(bool *pResult, IMessage &)
{
    IMODTRACE_DEVELOP();

    m_sReason.clear();

    HRESULT hr = ParseOperands();

    if (hr != S_OK)
        return hr;

    ITRACE_DEVELOP("Sending suspend request ...  ");

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR *>::Put(parmList
                        , KEY_SREV_REASON
                        , m_sReason.c_str());

    rcm().FireEvent( new CRequestSuspendEvent((const _TCHAR *)parmList) );

    *pResult = true;

    return hr;
}


template <>
IAction* CCreateable<CSendSuspendRequest, IAction>::NewInstance()
{
    IAction *pAction = new CSendSuspendRequest;

    return pAction;
}


template <>
void CCreateable<CSendSuspendRequest, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SendSuspendRequest)
DEFINE_ACTION_RELEASE_FXN(SendSuspendRequest)
