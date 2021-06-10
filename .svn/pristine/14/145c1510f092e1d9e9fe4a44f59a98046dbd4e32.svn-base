// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayNextDelayedIntervention.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisplayNextDelayedIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayNextDelayedIntervention.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
// 
// 2     1/12/05 5:50p Dm185016
// TAR 289897
// 
// 1     12/22/04 10:35a Dm185016
// TAR 288934 - Action to display the next intervention at the head of the
// delayed intervention Q
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "DisplayNextDelayedIntervention.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
//#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "RCMLaneXML.h"
#include "RCMError.h"
#include "PSXMfc.h"
#include "KVParmList.h"
#include "verify.h"


CDisplayNextDelayedIntervention::CDisplayNextDelayedIntervention()
{
    CRCMActionBase::SetName(_T("CDisplayNextDelayedIntervention"));
}


CDisplayNextDelayedIntervention::~CDisplayNextDelayedIntervention()
{
}


CIntervention *CDisplayNextDelayedIntervention::GetIntervention()
{
    return CActionBaseRAP::rapmgr().PeekNextDelayedIntervention();
}


bool CDisplayNextDelayedIntervention::OverrideDisplay() { return true; }


bool CDisplayNextDelayedIntervention::IsAddDescriptionToReceipt() { return false; }


template <>
IAction* CCreateable<CDisplayNextDelayedIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CDisplayNextDelayedIntervention;

    return pAction;
}


template <>
void CCreateable<CDisplayNextDelayedIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(DisplayNextDelayedIntervention)
DEFINE_ACTION_RELEASE_FXN(DisplayNextDelayedIntervention)
