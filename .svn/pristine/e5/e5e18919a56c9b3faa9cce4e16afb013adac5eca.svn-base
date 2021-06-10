// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchContextDelayedIntervention.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SwitchContextDelayedIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchContextDelayedIntervention.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     2/28/05 4:15p Dm185016
 * lint
 * 
 * 2     2/10/05 5:27p Dm185016
 * Removed trace.
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 1     12/22/04 10:35a Dm185016
// TAR 288934 - Factored functionality out of CheckDelayedIntervention
// 
// 4     11/22/04 3:44p Dm185016
// Saved current intervention in RemoteAPMgr.
// 
// 3     11/19/04 1:34p Dm185016
// Cannot post the original - causes it to be freed.
// 
// 2     11/16/04 11:27a Dm185016
// Now allows multiple state switches
// 
// 1     10/29/04 8:53a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SwitchContextDelayedIntervention.h"
#include "RemoteAPMgr.h"
#include "Message.h"

using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

CSwitchContextDelayedIntervention::CSwitchContextDelayedIntervention()
{
    CRCMActionBase::SetName(_T("CSwitchContextDelayedIntervention"));
}


CSwitchContextDelayedIntervention::~CSwitchContextDelayedIntervention()
{
}


// Get all control names from the xml
HRESULT CSwitchContextDelayedIntervention::ParseOperands()
{ 
    HRESULT hr = S_OK;

    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();
    CParameterList::iterator end    = parms.end();

    hr = NextOperands(iter, end);

    return hr;
}


HRESULT CSwitchContextDelayedIntervention::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    const _TCHAR *pszTableName = GetStateTableName();
    _ASSERTE(pszTableName[0]!=0);
    if (pszTableName[0]==0)
        return STATEM_MISSING_PARAMETER;

    CIntervention *pIntervention = rapmgr().GetCurrentIntervention();
    _ASSERT(pIntervention!= NULL);

    SetNextState(pszTableName, pIntervention->GetName());

    *pResult = true;

    return S_OK;
}


template <>
IAction* CCreateable<CSwitchContextDelayedIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CSwitchContextDelayedIntervention;

    return pAction;
}


template <>
void CCreateable<CSwitchContextDelayedIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SwitchContextDelayedIntervention)
DEFINE_ACTION_RELEASE_FXN(SwitchContextDelayedIntervention)
