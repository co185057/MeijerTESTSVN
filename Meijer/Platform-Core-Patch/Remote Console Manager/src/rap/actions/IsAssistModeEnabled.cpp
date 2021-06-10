// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsAssistModeEnabled.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsAssistModeEnabled.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsAssistModeEnabled.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     4/15/05 9:19a Dm185016
 * Merge from Patch B
 * 
 * 2     4/14/05 10:09p Dm185016
 * Fixed problem with action not being suitable for <if>
 * 
 * 1     4/07/05 1:44p Dm185016
 * TAR 297347
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsAssistModeEnabled.h"
#include "RemoteAPMgr.h"


CIsAssistModeEnabled::CIsAssistModeEnabled()
{
    CRCMActionBase::SetName(_T("CIsAssistModeEnabled"));
}


CIsAssistModeEnabled::~CIsAssistModeEnabled()
{
}


long CIsAssistModeEnabled::GetValue() const { return 0; }


void CIsAssistModeEnabled::SetValue(long) {}


HRESULT CIsAssistModeEnabled::Execute(bool *pResult, IMessage &)
{
    *pResult = rapmgr().IsAssistModeEnabled();

    return S_OK;
}


template <>
IAction* CCreateable<CIsAssistModeEnabled, IAction>::NewInstance()
{
    IAction *pAction = new CIsAssistModeEnabled;

    return pAction;
}


template <>
void CCreateable<CIsAssistModeEnabled, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsAssistModeEnabled)
DEFINE_ACTION_RELEASE_FXN(IsAssistModeEnabled)
