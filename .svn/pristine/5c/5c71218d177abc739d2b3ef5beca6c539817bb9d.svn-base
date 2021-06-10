// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckAssistModeControl.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)CheckAssistModeControl.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckAssistModeControl.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     4/05/05 11:56a Dm185016
 * TAR 297356 - Made IEvaluateable for <if>
 * 
 * 2     2/28/05 4:08p Dm185016
 * lint
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
// 
// 4     1/18/05 3:44p Dm185016
// UNICODE support
// 
// 3     1/13/05 1:25p Dm185016
// Added trace.
// 
// 2     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
// 
// 1     11/16/04 11:23a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "CheckAssistModeControl.h"
#include "RemoteAPMgr.h"
#include "RCMEvent.h"
#include "Message.h"
#include "RCMKeyValue.h"


BEGIN_KV_HANDLER_MAP(CCheckAssistModeControl)
    DEFINE_KV_HANDLER_MAP_ENTRY(CCheckAssistModeControl, KEY_VALID_VALUES, OnValidValues)
END_KV_HANDLER_MAP()


CCheckAssistModeControl::CCheckAssistModeControl()
    : m_bRetValue(false)
{
    CRCMActionBase::SetName(_T("CCheckAssistModeControl"));
}


CCheckAssistModeControl::~CCheckAssistModeControl()
{
}


DEFINE_KV_HANDLER(CCheckAssistModeControl, OnValidValues, keys)
{
    m_bRetValue = false;

    long lAMControl = rapmgr().GetAssistModeControl();
    long lValue;

    ITRACE_DEVELOP(_T(__FUNCTION__) _T(" AM Control mode:  ") + IString(lAMControl));

    // Check each of the possible values to see if the am control mode matches
    int nValueCnt = keys.GetValueCount();
    for (int i=0;!m_bRetValue && (i<nValueCnt);++keys, i++)
    {
        lValue = CKeyValue<long>::Get(keys, 0);

        if (lValue == lAMControl)
            m_bRetValue = true;
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(" Checking:  ") + IString(lValue));
    }
    ITRACE_DEVELOP(_T(__FUNCTION__) _T(" Returning:  ") + IString(m_bRetValue));
}


// Get data string from XML
HRESULT CCheckAssistModeControl::ParseOperands()
{ 
    HRESULT hr = S_OK;

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter != parms.end())
    {
        StringType sKeyString = *iter;
        ParseKeys(sKeyString.c_str(), GetTraceHandle());
    }

    return hr;
}


HRESULT CCheckAssistModeControl::Execute(bool *pResult, IMessage &)
{
    IMODTRACE_DEVELOP();

    HRESULT hr = ParseOperands();

    if (hr != S_OK)
        return hr;

    *pResult = m_bRetValue;

    return hr;
}


long CCheckAssistModeControl::GetValue() const { return 0; }


void CCheckAssistModeControl::SetValue(long) {}


template <>
IAction* CCreateable<CCheckAssistModeControl, IAction>::NewInstance()
{
    IAction *pAction = new CCheckAssistModeControl;

    return pAction;
}


template <>
void CCreateable<CCheckAssistModeControl, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(CheckAssistModeControl)
DEFINE_ACTION_RELEASE_FXN(CheckAssistModeControl)
