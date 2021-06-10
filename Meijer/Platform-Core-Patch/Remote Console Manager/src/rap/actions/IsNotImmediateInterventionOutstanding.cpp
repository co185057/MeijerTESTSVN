// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsNotImmediateInterventionOutstanding.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsNotImmediateInterventionOutstanding.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsNotImmediateInterventionOutstanding.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:12p Dm185016
 * lint
 * 
 * 1     2/04/05 9:56a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsNotImmediateInterventionOutstanding.h"
#include "RemoteAPMgr.h"


CIsNotImmediateInterventionOutstanding::CIsNotImmediateInterventionOutstanding()
{
    CRCMActionBase::SetName(_T("CIsNotImmediateInterventionOutstanding"));
}


CIsNotImmediateInterventionOutstanding::~CIsNotImmediateInterventionOutstanding()
{
}


HRESULT CIsNotImmediateInterventionOutstanding::Execute(bool *pResult, IMessage &)
{
    *pResult = !rapmgr().IsImmediateInterventionPending();

    return S_OK;
}


template <>
IAction* CCreateable<CIsNotImmediateInterventionOutstanding, IAction>::NewInstance()
{
    IAction *pAction = new CIsNotImmediateInterventionOutstanding;

    return pAction;
}


template <>
void CCreateable<CIsNotImmediateInterventionOutstanding, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsNotImmediateInterventionOutstanding)
DEFINE_ACTION_RELEASE_FXN(IsNotImmediateInterventionOutstanding)
