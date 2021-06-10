// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ReleaseExclusiveAccess.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)ReleaseExclusiveAccess.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ReleaseExclusiveAccess.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 11    10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 7     3/22/05 2:46p Dm185016
 * 
 * 6     3/07/05 6:53p Dm185016
 * TAR 294609.
 * TAR 294610.
 * 
 * 5     2/28/05 4:12p Dm185016
 * lint
 * 
 * 4     2/21/05 7:56p Dm185016
 * Parameters are optional not required.
 * 
 * 3     2/18/05 4:27p Dm185016
 * TAR 293807
 * 
 * 2     2/07/05 7:16p Dm185016
 * TAR 292647
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 2     1/13/05 1:37p Dm185016
// Fixed compile errors
// 
// 1     1/13/05 1:28p Dm185016
// TAR 290304
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ReleaseExclusiveAccess.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "PSXMfc.h"
#include "RAPUIDefines.h"
#include "RCMKeyValue.h"
#include "verifypsx.h"


BEGIN_KV_HANDLER_CLASS(CReleaseExclusiveAccess::REAParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CReleaseExclusiveAccess::REAParms, KEY_EV_RELEASE_NEEDED, ReleaseNeeded)
END_KV_HANDLER_CLASS()
DEFINE_KV_BOOL(CReleaseExclusiveAccess::REAParms, ReleaseNeeded)


void CReleaseExclusiveAccess::REAParms::Clear()
{
    SetReleaseNeeded(true);
}


CReleaseExclusiveAccess::CReleaseExclusiveAccess()
{
    CRCMActionBase::SetName(_T("CReleaseExclusiveAccess"));
}


CReleaseExclusiveAccess::~CReleaseExclusiveAccess()
{
}


// Get all control names from the xml
HRESULT CReleaseExclusiveAccess::ParseOperands()
{ 
    m_parms.Clear();

    // Grab the optional parameter from the XML.  If it is not specified then
    // it will default to release needed.
    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter == parms.end())
        return S_OK;

    return m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
}


HRESULT CReleaseExclusiveAccess::Execute(bool *pResult, IMessage &)
{
    *pResult = true;

    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    if (!rcm().IsExclusiveAccessReleaseRequired())
    {
        ITRACE_DEVELOP("Exclusive Access Release not required.");

        return S_OK;
    }

    // If the release is required and the current remote connection is not null
    // then send the event to release
    if (m_parms.IsReleaseNeeded())
        hr = ReleaseCurrentConnection();
    else
        rcm().SetExclusiveAccessReleaseRequired(false);

    if (hr != S_OK)
    {
        ITRACE_DEVELOP(_T("Current Connection not released.  rc=") + IString(hr).d2x());
    }

    hr = ReleaseOtherConnections();
    
    return hr;
}


template <>
IAction* CCreateable<CReleaseExclusiveAccess, IAction>::NewInstance()
{
    IAction *pAction = new CReleaseExclusiveAccess;

    return pAction;
}


template <>
void CCreateable<CReleaseExclusiveAccess, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ReleaseExclusiveAccess)
DEFINE_ACTION_RELEASE_FXN(ReleaseExclusiveAccess)
