// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ShowControl.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)ShowControl.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ShowControl.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 9     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 5     3/22/05 3:37p Tp151000
 * TAR 296027
 * 
 * 4     2/28/05 4:26p Dm185016
 * lint
 * 
 * 3     2/22/05 8:41a Dm185016
 * Fixed UNICODE compile problem.
 * 
 * 2     2/21/05 8:08p Dm185016
 * TAR 293968
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
// 
// 3     1/18/05 3:51p Dm185016
// UNICODE support
// 
// 2     1/12/05 6:10p Dm185016
// Cleaned up trace log.
// 
// 1     10/29/04 9:04a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ShowControl.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"


BEGIN_KV_HANDLER_CLASS(CShowControl::SCParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CShowControl::SCParms, KEY_CONTEXT_NAME, ContextName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CShowControl::SCParms, KEY_CONTROL_NAME, ControlName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CShowControl::SCParms, ContextName)
DEFINE_KV_CSTRING(CShowControl::SCParms, ControlName)


void CShowControl::SCParms::Clear()
{
    m_ContextName.Empty();
    m_ControlName.Empty();
}


CShowControl::CShowControl()
{
    CRCMActionBase::SetName(_T("CShowControl"));
}


CShowControl::~CShowControl()
{
}


// Get all control names from the xml
HRESULT CShowControl::ParseOperands()
{ 
    m_parms.Clear();

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter == parms.end())
        return S_OK;

    return m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
}


HRESULT CShowControl::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = ParseOperands();

    if (m_parms.GetControlName().IsEmpty())
    {
        ITRACE_ERROR(KEY_CONTROL_NAME _T(" parameter is missing."));
        return STATEM_MISSING_PARAMETER;
    }

    bool bUseCurrentContext = (m_parms.GetContextName() != KEY_CONTEXT_NAME_ALL);

    SetControlVisible((const _TCHAR *)m_parms.GetControlName(), true, bUseCurrentContext);

    *pResult = true;

    return hr;
}


template <>
IAction* CCreateable<CShowControl, IAction>::NewInstance()
{
    IAction *pAction = new CShowControl;

    return pAction;
}


template <>
void CCreateable<CShowControl, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ShowControl)
DEFINE_ACTION_RELEASE_FXN(ShowControl)
