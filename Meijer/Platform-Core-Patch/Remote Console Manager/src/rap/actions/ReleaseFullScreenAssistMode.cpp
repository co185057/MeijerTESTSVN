// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ReleaseFullScreenAssistMode.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)ReleaseFullScreenAssistMode.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ReleaseFullScreenAssistMode.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:12p Dm185016
 * lint
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 1     12/02/04 3:33p Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ReleaseFullScreenAssistMode.h"
#include "RemoteAPMgr.h"


CReleaseFullScreenAssistMode::CReleaseFullScreenAssistMode()
{
    CRCMActionBase::SetName(_T("CReleaseFullScreenAssistMode"));
}


CReleaseFullScreenAssistMode::~CReleaseFullScreenAssistMode()
{
}


HRESULT CReleaseFullScreenAssistMode::Execute(bool *pResult, IMessage &)
{
    *pResult = true;

    //TODO:  Need to fill in this!

    return S_OK;
}


template <>
IAction* CCreateable<CReleaseFullScreenAssistMode, IAction>::NewInstance()
{
    IAction *pAction = new CReleaseFullScreenAssistMode;

    return pAction;
}


template <>
void CCreateable<CReleaseFullScreenAssistMode, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ReleaseFullScreenAssistMode)
DEFINE_ACTION_RELEASE_FXN(ReleaseFullScreenAssistMode)
