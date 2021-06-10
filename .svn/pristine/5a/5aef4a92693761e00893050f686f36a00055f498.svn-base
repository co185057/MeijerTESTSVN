// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsHandheldAssistModeAllowed.cpp 3     10/21/08 4:07p Sm133076 $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsHandheldAssistModeAllowed.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
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

#include "IsHandheldAssistModeAllowed.h"
#include "RemoteAPMgr.h"


CIsHandheldAssistModeAllowed::CIsHandheldAssistModeAllowed()
{
    CRCMActionBase::SetName(_T("CIsHandheldAssistModeAllowed"));
}


CIsHandheldAssistModeAllowed::~CIsHandheldAssistModeAllowed()
{
}


HRESULT CIsHandheldAssistModeAllowed::Execute(bool *pResult, IMessage &)
{
    *pResult = rapmgr().IsHandheldAssistModeEnabled();

    return S_OK;
}


long CIsHandheldAssistModeAllowed::GetValue() const { return 0; }


void CIsHandheldAssistModeAllowed::SetValue(long) {}


template <>
IAction* CCreateable<CIsHandheldAssistModeAllowed, IAction>::NewInstance()
{
    IAction *pAction = new CIsHandheldAssistModeAllowed;

    return pAction;
}


template <>
void CCreateable<CIsHandheldAssistModeAllowed, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsHandheldAssistModeAllowed)
DEFINE_ACTION_RELEASE_FXN(IsHandheldAssistModeAllowed)
