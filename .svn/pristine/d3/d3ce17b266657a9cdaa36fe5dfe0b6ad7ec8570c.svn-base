// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsCurrentInterventionInProgress.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsHandheldAssistModeAllowed.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsCurrentInterventionInProgress.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 1     11/21/06 4:29p Jj100011
 * Tar 338040
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     4/05/05 11:53a Dm185016
 * TAR 297356
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsCurrentInterventionInProgress.h"
#include "RemoteAPMgr.h"


CIsCurrentInterventionInProgress::CIsCurrentInterventionInProgress()
{
    CRCMActionBase::SetName(_T("CIsCurrentInterventionInProgress"));
}


CIsCurrentInterventionInProgress::~CIsCurrentInterventionInProgress()
{
}


HRESULT CIsCurrentInterventionInProgress::Execute(bool *pResult, IMessage &)
{
    *pResult = (rapmgr().GetCurrentIntervention() != NULL);

    return S_OK;
}


long CIsCurrentInterventionInProgress::GetValue() const { return 0; }


void CIsCurrentInterventionInProgress::SetValue(long) {}


template <>
IAction* CCreateable<CIsCurrentInterventionInProgress, IAction>::NewInstance()
{
    IAction *pAction = new CIsCurrentInterventionInProgress;

    return pAction;
}


template <>
void CCreateable<CIsCurrentInterventionInProgress, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsCurrentInterventionInProgress)
DEFINE_ACTION_RELEASE_FXN(IsCurrentInterventionInProgress)
