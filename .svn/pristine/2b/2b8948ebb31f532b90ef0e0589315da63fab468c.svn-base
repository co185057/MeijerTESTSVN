// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsExclusiveAccessGranted.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsExclusiveAccessGranted.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsExclusiveAccessGranted.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     4/21/05 7:21p Dm185016
 * Performance for HH
 * 
 * 1     3/07/05 6:58p Dm185016
 * TAR 294609.
 * TAR 294610.
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsExclusiveAccessGranted.h"
#include "RemoteAPMgr.h"


CIsExclusiveAccessGranted::CIsExclusiveAccessGranted()
{
    CRCMActionBase::SetName(_T("CIsExclusiveAccessGranted"));
}


CIsExclusiveAccessGranted::~CIsExclusiveAccessGranted()
{
}


//-------------------------------------------------------------------------
// Methods from IEvaluateable
//-------------------------------------------------------------------------
long CIsExclusiveAccessGranted::GetValue() const { return 0; }
void CIsExclusiveAccessGranted::SetValue(long) {}


HRESULT CIsExclusiveAccessGranted::Execute(bool *pResult, IMessage &)
{
    *pResult = rcm().IsExclusiveAccessGranted();

    return S_OK;
}


template <>
IAction* CCreateable<CIsExclusiveAccessGranted, IAction>::NewInstance()
{
    IAction *pAction = new CIsExclusiveAccessGranted;

    return pAction;
}


template <>
void CCreateable<CIsExclusiveAccessGranted, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsExclusiveAccessGranted)
DEFINE_ACTION_RELEASE_FXN(IsExclusiveAccessGranted)
