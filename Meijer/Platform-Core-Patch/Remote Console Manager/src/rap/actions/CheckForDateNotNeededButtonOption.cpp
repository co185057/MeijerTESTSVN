// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckForDateNotNeededButtonOption.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)CheckForDateNotNeededButtonOption.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckForDateNotNeededButtonOption.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     3/30/05 3:54p Dm185016
 * Fixed warning about not all paths returning a value.
 * 
 * 1     3/28/05 2:20p Tp151000
 * TAR 296896
 * 
 * 1     3/17/05 2:30p Tp151000
 * Add Media Low event
 * 
 * 2     3/16/05 2:28p Dm185016
 *    
 * 
 * 
 * 1     3/11/05 2:41p Dm185016
 * 
 * 2     2/28/05 4:35p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 5     1/12/05 6:00p Dm185016
// Cleaned up trace log.
// 
// 4     12/31/04 9:19a Dm185016
// TAR 288930 - Fixed problem logging enter/exit store mode.  Name of
// transaction variable now read from XML.
// 
// 3     11/16/04 11:38a Dm185016
// Rewritten
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "CheckForDateNotNeededButtonOption.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "RCMError.h"
#include "KVParmList.h"


BEGIN_KV_HANDLER_CLASS(CCheckForDateNotNeededButtonOption::CFDNNBOParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CCheckForDateNotNeededButtonOption::CFDNNBOParms, KEY_EV_DOB_NOT_NEEDED, DOBNotNeeded)
    DEFINE_KV_HANDLER_MAP_ENTRY(CCheckForDateNotNeededButtonOption::CFDNNBOParms, KEY_BUTTON_NAME, ButtonName)
END_KV_HANDLER_CLASS()
DEFINE_KV_BOOL(CCheckForDateNotNeededButtonOption::CFDNNBOParms, DOBNotNeeded)
DEFINE_KV_CSTRING(CCheckForDateNotNeededButtonOption::CFDNNBOParms, ButtonName)


void CCheckForDateNotNeededButtonOption::CFDNNBOParms::Clear()
{
    m_ButtonName.Empty();
}


CCheckForDateNotNeededButtonOption::CCheckForDateNotNeededButtonOption()
{
    CRCMActionBase::SetName(_T("CCheckForDateNotNeededButtonOption"));
}


CCheckForDateNotNeededButtonOption::~CCheckForDateNotNeededButtonOption()
{
}


// Get all control names from the xml
HRESULT CCheckForDateNotNeededButtonOption::ParseOperands()
{ 
    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();

    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;
    
    return m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
}


HRESULT CCheckForDateNotNeededButtonOption::Execute(bool *pResult, IMessage &message)
{
    *pResult = true;

    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    hr = ParseOperands();
    if (hr != S_OK)
        return hr;


    if(m_parms.GetButtonName().IsEmpty())
    {
        ITRACE_ERROR(_T("Missing parameter:  ") + IString(KEY_BUTTON_NAME));
        return RCM_API_FAILURE_INVALID_KEY_VALUE_LIST;
    }

    if(m_parms.IsDOBNotNeeded())
    {
        SetControlVisible((const _TCHAR *)m_parms.GetButtonName(), true, true);
    }
    else
    {
        SetControlVisible((const _TCHAR *)m_parms.GetButtonName(), false, true);
    }
    return hr;
}


template <>
IAction* CCreateable<CCheckForDateNotNeededButtonOption, IAction>::NewInstance()
{
    IAction *pAction = new CCheckForDateNotNeededButtonOption;

    return pAction;
}


template <>
void CCreateable<CCheckForDateNotNeededButtonOption, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(CheckForDateNotNeededButtonOption)
DEFINE_ACTION_RELEASE_FXN(CheckForDateNotNeededButtonOption)
