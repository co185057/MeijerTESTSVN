// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsInterventionOutstanding.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsInterventionOutstanding.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsInterventionOutstanding.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     5/09/05 11:18a Dm185016
 * 
 * 2     2/28/05 4:12p Dm185016
 * lint
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 3     1/18/05 3:44p Dm185016
// UNICODE support
// 
// 2     1/13/05 1:27p Dm185016
// Removed trace.
// 
// 1     12/14/04 11:13a Dm185016
// TAR 288478  Added actions to clear button text
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsInterventionOutstanding.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"


CIsInterventionOutstanding::CIsInterventionOutstanding()
{
    CRCMActionBase::SetName(_T("CIsInterventionOutstanding"));
}


CIsInterventionOutstanding::~CIsInterventionOutstanding()
{
}


HRESULT CIsInterventionOutstanding::Execute(bool *pResult, IMessage &)
{
    *pResult = rapmgr().IsImmediateInterventionPending();
    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Immediate intevention outstanding:  ") + IString(*pResult));
    *pResult = *pResult || rapmgr().IsDelayedInterventionPending();
    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Delayed intevention outstanding:  ") + IString(rapmgr().IsDelayedInterventionPending()));
    *pResult = *pResult || (rapmgr().GetCurrentIntervention() != NULL);
    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Currently Handling intervention:  ") + IString((rapmgr().GetCurrentIntervention() != NULL)));

    return S_OK;
}

template <>
IAction* CCreateable<CIsInterventionOutstanding, IAction>::NewInstance()
{
    IAction *pAction = new CIsInterventionOutstanding;

    return pAction;
}


template <>
void CCreateable<CIsInterventionOutstanding, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsInterventionOutstanding)
DEFINE_ACTION_RELEASE_FXN(IsInterventionOutstanding)
