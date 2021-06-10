// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveMinimumAge.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SaveMinimumAge.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveMinimumAge.cpp $
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
// 2     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
// 
// 1     12/09/04 1:45p Dm185016
*/
#include "StdAfx.h"


#include "SaveMinimumAge.h"
#include "RemoteAPMgr.h"
#include "RCMKeyValue.h"
#include "Message.h"

using namespace rap;
using namespace fastlane;
using namespace utils;

BEGIN_KV_HANDLER_MAP(CSaveMinimumAge)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSaveMinimumAge, KEY_EV_MINIMUM_AGE, OnMinimumAge)
END_KV_HANDLER_MAP()


CSaveMinimumAge::CSaveMinimumAge()
{
    CRCMActionBase::SetName(_T("CSaveMinimumAge"));
}


CSaveMinimumAge::~CSaveMinimumAge()
{
}


DEFINE_KV_HANDLER(CSaveMinimumAge, OnMinimumAge, keys)
{
    int nMinimumAge = CKeyValue<int>::Get(keys, 0);
    ITRACE_DEVELOP(_T("nMinimumAge:  ") + IString(nMinimumAge));

    rapmgr().SetMinimumAge(nMinimumAge);
}


HRESULT CSaveMinimumAge::Execute(bool *pResult, IMessage &message)
{
    IMODTRACE_DEVELOP();

    HRESULT hr = ParseMessageKeys(message);
    if (hr != S_OK)
        return hr;

    *pResult = true;

    return hr;
}


template <>
IAction* CCreateable<CSaveMinimumAge, IAction>::NewInstance()
{
    IAction *pAction = new CSaveMinimumAge;

    return pAction;
}


template <>
void CCreateable<CSaveMinimumAge, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SaveMinimumAge)
DEFINE_ACTION_RELEASE_FXN(SaveMinimumAge)
