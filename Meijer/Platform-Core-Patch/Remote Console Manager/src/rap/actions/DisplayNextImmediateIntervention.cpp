// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayNextImmediateIntervention.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisplayNextImmediateIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayNextImmediateIntervention.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     2/04/05 9:56a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "DisplayNextImmediateIntervention.h"
#include "RemoteAPMgr.h"


CDisplayNextImmediateIntervention::CDisplayNextImmediateIntervention()
{
    CRCMActionBase::SetName(_T("CDisplayNextImmediateIntervention"));
}


CDisplayNextImmediateIntervention::~CDisplayNextImmediateIntervention()
{
}


CIntervention *CDisplayNextImmediateIntervention::GetIntervention()
{
    return CActionBaseRAP::rapmgr().PeekNextImmediateIntervention();
}


bool CDisplayNextImmediateIntervention::OverrideDisplay() { return true; }


bool CDisplayNextImmediateIntervention::IsAddDescriptionToReceipt() { return true; }


template <>
IAction* CCreateable<CDisplayNextImmediateIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CDisplayNextImmediateIntervention;

    return pAction;
}


template <>
void CCreateable<CDisplayNextImmediateIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(DisplayNextImmediateIntervention)
DEFINE_ACTION_RELEASE_FXN(DisplayNextImmediateIntervention)
