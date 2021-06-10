// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayCurrentIntervention.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisplayCurrentIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
 *
 * Copyright 2006 by NCR Corporation,
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "DisplayCurrentIntervention.h"
#include "RemoteAPMgr.h"


CDisplayCurrentIntervention::CDisplayCurrentIntervention()
{
    CRCMActionBase::SetName(_T("CDisplayCurrentIntervention"));
}


CDisplayCurrentIntervention::~CDisplayCurrentIntervention()
{
}


CIntervention *CDisplayCurrentIntervention::GetIntervention()
{
    return CActionBaseRAP::rapmgr().GetCurrentIntervention();
}


bool CDisplayCurrentIntervention::OverrideDisplay() { return true; }


bool CDisplayCurrentIntervention::IsAddDescriptionToReceipt() { return false; }


template <>
IAction* CCreateable<CDisplayCurrentIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CDisplayCurrentIntervention;

    return pAction;
}


template <>
void CCreateable<CDisplayCurrentIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(DisplayCurrentIntervention)
DEFINE_ACTION_RELEASE_FXN(DisplayCurrentIntervention)
