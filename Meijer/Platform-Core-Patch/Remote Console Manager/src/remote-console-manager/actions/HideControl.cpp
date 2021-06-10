// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/HideControl.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)HideControl.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/HideControl.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:38p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 2     1/18/05 3:51p Dm185016
// UNICODE support
// 
// 1     10/29/04 9:04a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "HideControl.h"
#include "RCManager.h"
#include "RCMError.h"
#include "RCMStateMachine.h"
#include "KVParmList.h"


CHideControl::CHideControl()
{
    CRCMActionBase::SetName(_T("CHideControl"));
}


CHideControl::~CHideControl()
{
}


// Get all control names from the xml
HRESULT CHideControl::NextOperands(CParameterList::iterator &iter, CParameterList::const_iterator &end)
{ 
    IMODTRACE_DEVELOP();

    HRESULT hr = S_OK;

    // Get the data source name for the receipt control
    if (iter != end)
    {
        m_sControl = *iter;
        iter++;
    }
    else
        hr = STATEM_MISSING_PARAMETER;

    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  m_sControl:  ") + IString(m_sControl.c_str()));

    return hr;
}


// Get all control names from the xml
HRESULT CHideControl::ParseOperands()
{ 
    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();
    CParameterList::iterator end    = parms.end();

    return NextOperands(iter, end);
}


HRESULT CHideControl::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    // We will allow the PSX call to fail, but will report it.
    hr = SetControlVisible(m_sControl, false);
    if (hr != S_OK)
    {
        CRCMErrorInfo error(hr);
        ITRACE_ERROR(error.text());
    }

    *pResult = true;

    return S_OK;
}

template <>
IAction* CCreateable<CHideControl, IAction>::NewInstance()
{
    IAction *pAction = new CHideControl;

    return pAction;
}


template <>
void CCreateable<CHideControl, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(HideControl)
DEFINE_ACTION_RELEASE_FXN(HideControl)
