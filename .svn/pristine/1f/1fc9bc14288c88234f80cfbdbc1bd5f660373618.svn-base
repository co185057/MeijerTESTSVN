// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/LoadConfiguration.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)LoadConfiguration.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/LoadConfiguration.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 4     12/07/04 4:21p Dm185016
// Factored out common code into LoadableActionBase
// 
// 3     11/16/04 11:40a Dm185016
// fixed return code
// 
// 2     10/29/04 9:12a Dm185016
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
#include "StdAfx.h"
#include "LoadConfiguration.h"

using namespace fastlane;


CLoadConfiguration::CLoadConfiguration()
{
    CRCMActionBase::SetName(_T("CLoadConfiguration"));
}


CLoadConfiguration::~CLoadConfiguration()
{
}


HRESULT CLoadConfiguration::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = CoInitialize(NULL);
    switch (hr)
    {
        case S_OK :
        case S_FALSE :
            break;
        default :
            ITHROWSYSTEMERROR((ULONG)hr, CoInitialize, IException::unrecoverable)
    }

    hr = LoadConfiguration(this, pResult, message);

    return hr;
}


template <>
IAction* CCreateable<CLoadConfiguration, IAction>::NewInstance()
{
    IAction *pAction = new CLoadConfiguration;

    return pAction;
}


template <>
void CCreateable<CLoadConfiguration, IAction>::ReleaseInstance(IAction *pAction)
{
    IMODTRACE_DEVELOP_STATIC(pAction->GetTraceHandle());

    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(LoadConfiguration)
DEFINE_ACTION_RELEASE_FXN(LoadConfiguration)
