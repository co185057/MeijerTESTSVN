// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayInterventionBase.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisplayInterventionBase.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayInterventionBase.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 11    10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 9     1/13/06 3:46p Tk124825
 * TAR 301329
 * 
 * 8     1/13/06 2:29p Dm185016
 * TAR 301329
 * 
 * 7     6/27/05 2:12p Dm185016
 * TAR 304115
 * 
 * 5     5/16/05 5:36p Dm185016
 * TAR 300719
 * 
 * 3     2/28/05 4:12p Dm185016
 * lint
 * 
 * 2     1/26/05 2:43p Dm185016
 * Moved to new Repository.
// 
// 3     1/20/05 5:25p Tp151000
// TAR 290961
// 
// 2     1/12/05 5:50p Dm185016
// TAR 289897
// 
// 1     12/22/04 10:34a Dm185016
// TAR 288934 - New base class for updating buttons for intervention
// display.
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "DisplayInterventionBase.h"
#include "Message.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "RCMError.h"
#include "KVParmList.h"

BEGIN_KV_HANDLER_CLASS(CDisplayInterventionBase::DIBParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisplayInterventionBase::DIBParms, KEY_EV_DESCRIPTION, Description)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CDisplayInterventionBase::DIBParms, Description)
BEGIN_KV_HANDLER_CLASS_CLEAR(CDisplayInterventionBase::DIBParms)
    CLEAR_KV_CSTRING(Description)
END_KV_HANDLER_CLASS_CLEAR()


// Get all control names from the xml
HRESULT CDisplayInterventionBase::ParseInterventionData(const _TCHAR *pszInterventionData)
{ 
    m_parms.Clear();

    return m_parms.ParseKeys(pszInterventionData, GetTraceHandle());
}


HRESULT CDisplayInterventionBase::Execute(bool *pResult, IMessage &)
{
    *pResult = true;

    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    // Get the next intervention to display.  If it is null, then we have a violation
    // of the contract for this action.  Prior to its execution, this action expects 
    // the user to have determined that there truly is an intervention to display.
    // If there is no intervention, then log the error and return.
    CIntervention *pIntervention = GetIntervention();
    if (pIntervention == NULL)
    {
        ITRACE_ERROR(_T(__FUNCTION__) _T(".  No intervention to display"));
        return S_OK;
    }

    CIntervention &intervention = *pIntervention;

    m_parms.ParseKeys((_TCHAR *)intervention.GetData());

    if (GetDescription() == NULL)
    {
        ITRACE_ERROR(_T(__FUNCTION__) _T(".  Missing parameter:  ") + IString(KEY_EV_DESCRIPTION));
        return RCM_API_FAILURE_INVALID_KEY_VALUE_LIST;
    }

    ParseDescription();

    UpdateDescription(OverrideDisplay(), IsAddDescriptionToReceipt());

    return hr;
}


const _TCHAR *CDisplayInterventionBase::GetDescription() const { return m_parms.GetDescription(); }
