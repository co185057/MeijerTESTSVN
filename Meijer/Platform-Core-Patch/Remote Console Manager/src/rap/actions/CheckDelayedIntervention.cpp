// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckDelayedIntervention.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)CheckDelayedIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckDelayedIntervention.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     2/28/05 4:08p Dm185016
 * lint
 * 
 * 2     2/10/05 5:26p Dm185016
 * Removed trace.
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
// 
// 5     12/22/04 10:31a Dm185016
// TAR 288934 - Split into this and SwitchContextDelayedIntervention.
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

#include "CheckDelayedIntervention.h"
#include "RemoteAPMgr.h"
#include "Message.h"

using namespace STATE_MACHINE_NAMESPACE;

CCheckDelayedIntervention::CCheckDelayedIntervention()
{
    CRCMActionBase::SetName(_T("CCheckDelayedIntervention"));
}


CCheckDelayedIntervention::~CCheckDelayedIntervention()
{
}


HRESULT CCheckDelayedIntervention::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = S_OK;

    if (rapmgr().IsDelayedInterventionPending())
    {
        CIntervention *pIntervention = rapmgr().GetNextDelayedIntervention();
        _ASSERT(pIntervention!= NULL);

        rapmgr().SetCurrentIntervention(pIntervention);

        IMessage &newMsg 
            = stateMachine().MakeAsynchCopy(*pIntervention);

        hr = rapmgr().PostMsg(&newMsg);

        *pResult = false;
    }
    else
        *pResult = true;

    return hr;
}


template <>
IAction* CCreateable<CCheckDelayedIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CCheckDelayedIntervention;

    return pAction;
}


template <>
void CCreateable<CCheckDelayedIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(CheckDelayedIntervention)
DEFINE_ACTION_RELEASE_FXN(CheckDelayedIntervention)
