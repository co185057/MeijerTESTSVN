// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SendUnsolicitedData.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)SendUnsolicitedData.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SendUnsolicitedData.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 9     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 6     4/27/05 5:38p Tp151000
 * TAR 299443
 * 
 * 5     4/12/05 2:56p Dm185016
 * Changes for AllLaneStatusSummary
 * 
 * 3     3/15/05 3:37p Dm185016
 * UNICODE
 * 
 * 2     2/28/05 4:40p Dm185016
 * lint
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
// 
// 2     1/12/05 6:08p Dm185016
// Cleaned up trace log.
// 
// 1     11/16/04 11:42a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SendUnsolicitedData.h"
#include "RCManager.h"
#include "RCMEvent.h"
#include "Message.h"
#include "RCMKeyValue.h"


BEGIN_KV_HANDLER_MAP(CSendUnsolicitedData)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSendUnsolicitedData, KEY_UNSOLICITED_DATA, OnUnsolicitedData)
END_KV_HANDLER_MAP()


CSendUnsolicitedData::CSendUnsolicitedData()
{
    CRCMActionBase::SetName(_T("CSendUnsolicitedData"));
}


CSendUnsolicitedData::~CSendUnsolicitedData()
{
}


DEFINE_KV_HANDLER(CSendUnsolicitedData, OnUnsolicitedData, keys)
{
    m_sData = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
    ITRACE_DEVELOP(_T("Data:  ") + IString(m_sData.c_str()));
}


// Get data string from XML
HRESULT CSendUnsolicitedData::ParseOperands()
{ 
    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    return ParseKeys((*iter).c_str(), GetTraceHandle());
}


HRESULT CSendUnsolicitedData::ParseMessage(IMessage &)
{
    return S_OK;
}


const _TCHAR *CSendUnsolicitedData::GetUnsolicitedData()
{
    return m_sData.c_str();
}


HRESULT CSendUnsolicitedData::Execute(bool *pResult, IMessage &message)
{
    IMODTRACE_DEVELOP();

    m_sData.clear();

    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    hr = ParseMessage(message);
    if (hr != S_OK)
        return hr;

    ITRACE_DEVELOP(_T("Sending unsolicited data:  "));

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR *>::Put(parmList
                        , KEY_AEV_AUX_DATA
                        , GetUnsolicitedData());

    rcm().FireEvent( new CUnsolicitedDataEvent((const _TCHAR *)parmList) );

    *pResult = true;

    return hr;
}


template <>
IAction* CCreateable<CSendUnsolicitedData, IAction>::NewInstance()
{
    IAction *pAction = new CSendUnsolicitedData;

    return pAction;
}


template <>
void CCreateable<CSendUnsolicitedData, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SendUnsolicitedData)
DEFINE_ACTION_RELEASE_FXN(SendUnsolicitedData)
