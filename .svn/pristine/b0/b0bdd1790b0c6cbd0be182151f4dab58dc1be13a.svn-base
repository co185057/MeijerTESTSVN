// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ReloadConfiguration.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)ReloadConfiguration.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ReloadConfiguration.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
// 
// 1     12/07/04 4:21p Dm185016
// Added for reloading options
*/
#include "StdAfx.h"
#include "ReloadConfiguration.h"

using namespace fastlane;


CReloadConfiguration::CReloadConfiguration()
{
    CRCMActionBase::SetName(_T("CReloadConfiguration"));
}


CReloadConfiguration::~CReloadConfiguration()
{
}


HRESULT CReloadConfiguration::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = LoadConfiguration(this, pResult, message);

    return hr;
}


template <>
IAction* CCreateable<CReloadConfiguration, IAction>::NewInstance()
{
    IAction *pAction = new CReloadConfiguration;

    return pAction;
}


template <>
void CCreateable<CReloadConfiguration, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ReloadConfiguration)
DEFINE_ACTION_RELEASE_FXN(ReloadConfiguration)
