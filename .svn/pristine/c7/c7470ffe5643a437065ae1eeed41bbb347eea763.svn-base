// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckImmediateIntervention.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)CheckImmediateIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckImmediateIntervention.cpp $
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
 * 2     2/03/05 5:42p Dm185016
 * TAR 292378 - Converted single immediate intervention to use of stack
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
// 
// 4     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
// 
// 3     11/22/04 3:46p Dm185016
// Saved current intervention in RemoteAPMgr.
// Posted copy rather than original message.
// 
// 2     11/02/04 3:06p Dm185016
// More updates
// 
// 1     10/29/04 8:53a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "CheckImmediateIntervention.h"
#include "RemoteAPMgr.h"


CCheckImmediateIntervention::CCheckImmediateIntervention()
{
    CRCMActionBase::SetName(_T("CCheckImmediateIntervention"));
}


CCheckImmediateIntervention::~CCheckImmediateIntervention()
{
}


HRESULT CCheckImmediateIntervention::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = S_OK;

    if (rapmgr().IsImmediateInterventionPending())
    {
        CIntervention *pIntervention = rapmgr().GetNextImmediateIntervention();
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
IAction* CCreateable<CCheckImmediateIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CCheckImmediateIntervention;

    return pAction;
}


template <>
void CCreateable<CCheckImmediateIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(CheckImmediateIntervention)
DEFINE_ACTION_RELEASE_FXN(CheckImmediateIntervention)
