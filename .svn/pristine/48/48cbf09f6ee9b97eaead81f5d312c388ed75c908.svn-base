// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearCurrentIntervention.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ClearCurrentIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearCurrentIntervention.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 4     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     4/15/05 9:18a Dm185016
 * Merge from Patch B
 * 
 * 1     4/14/05 10:11p Dm185016
 * TAR 298021
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ClearCurrentIntervention.h"
#include "RemoteAPMgr.h"
#include "Message.h"


CClearCurrentIntervention::CClearCurrentIntervention()
{
    CRCMActionBase::SetName(_T("CClearCurrentIntervention"));
}


CClearCurrentIntervention::~CClearCurrentIntervention()
{
}


HRESULT CClearCurrentIntervention::Execute(bool *pResult, IMessage &)
{
    CIntervention * pIntervention = rapmgr().GetCurrentIntervention();

    rapmgr().SetCurrentIntervention(NULL);

    delete pIntervention;

    *pResult = true;

    return S_OK;
}


template <>
IAction* CCreateable<CClearCurrentIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CClearCurrentIntervention;

    return pAction;
}


template <>
void CCreateable<CClearCurrentIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ClearCurrentIntervention)
DEFINE_ACTION_RELEASE_FXN(ClearCurrentIntervention)
