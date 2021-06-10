// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SetReleaseExclusiveAccessRequired.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SetReleaseExclusiveAccessRequired.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SetReleaseExclusiveAccessRequired.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/07/05 6:58p Dm185016
 * TAR 294609.
 * TAR 294610.
*/
#include "StdAfx.h"

#include "SetReleaseExclusiveAccessRequired.h"
#include "RCManager.h"
#include "RCMKeyValue.h"


BEGIN_KV_HANDLER_CLASS(CSetReleaseExclusiveAccessRequired::SREAParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSetReleaseExclusiveAccessRequired::SREAParms, KEY_EV_RELEASE_NEEDED, ReleaseNeeded)
END_KV_HANDLER_CLASS()
DEFINE_KV_BOOL(CSetReleaseExclusiveAccessRequired::SREAParms, ReleaseNeeded)


void CSetReleaseExclusiveAccessRequired::SREAParms::Clear()
{
    SetReleaseNeeded(true);
}


CSetReleaseExclusiveAccessRequired::CSetReleaseExclusiveAccessRequired()
{
    CRCMActionBase::SetName(_T("CSetReleaseExclusiveAccessRequired"));
}


CSetReleaseExclusiveAccessRequired::~CSetReleaseExclusiveAccessRequired()
{
}


// Get all control names from the xml
HRESULT CSetReleaseExclusiveAccessRequired::ParseOperands()
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


HRESULT CSetReleaseExclusiveAccessRequired::Execute(bool *pResult, IMessage &)
{
    *pResult = true;

    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    rcm().SetExclusiveAccessReleaseRequired(m_parms.IsReleaseNeeded());
    
    return hr;
}


template <>
IAction* CCreateable<CSetReleaseExclusiveAccessRequired, IAction>::NewInstance()
{
    IAction *pAction = new CSetReleaseExclusiveAccessRequired;

    return pAction;
}


template <>
void CCreateable<CSetReleaseExclusiveAccessRequired, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SetReleaseExclusiveAccessRequired)
DEFINE_ACTION_RELEASE_FXN(SetReleaseExclusiveAccessRequired)
