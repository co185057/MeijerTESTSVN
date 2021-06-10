// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/GenerateEvent.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)GenerateEvent.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/GenerateEvent.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     2/28/05 4:42p Dm185016
 * Fixed compile error
 * 
 * 2     2/28/05 4:35p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 3     1/18/05 3:51p Dm185016
// UNICODE support
// 
// 2     1/13/05 1:42p Dm185016
// Cleaned up trace.
// 
// 1     1/12/05 6:01p Dm185016
// Multiple RAP support.
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "GenerateEvent.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "PSXMfc.h"
#include "RAPUIDefines.h"
#include "verifypsx.h"
#include "RCMError.h"


CGenerateEvent::CGenerateEvent()
{
    CRCMActionBase::SetName(_T("CGenerateEvent"));
}


CGenerateEvent::~CGenerateEvent()
{
}


// Get all control names from the xml
HRESULT CGenerateEvent::ParseOperands()
{ 
    HRESULT hr = S_OK;

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    m_sParameters = *iter;

    return hr;
}


const _TCHAR *CGenerateEvent::GetEventText()
{
    return m_sParameters.c_str();
}


HRESULT CGenerateEvent::GenerateEvent()
{
    HRESULT hr = S_OK;

    PREMOTECONN pRemoteConn = rcm().GetCurrentRemoteConnection();
    if (pRemoteConn==NULL)
        return RCM_NO_REMOTE_CONNECTION_AVAILABLE;

    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Event:  ") + IString(GetEventText()));

    _variant_t varParam = GetEventText();

    VERIFY_PSX(hr, psx().GenerateEvent(_T(""),
                                       _T(""),
                                       UI::EVENTREMOTEDATA,
                                       varParam,
                                       (*pRemoteConn).GetRemoteConnectionName()));

    return hr;
}


HRESULT CGenerateEvent::Execute(bool *pResult, IMessage &)
{
    *pResult = true;

	HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    hr = GenerateEvent();

    return hr;
}


template <>
IAction* CCreateable<CGenerateEvent, IAction>::NewInstance()
{
    IAction *pAction = new CGenerateEvent;

    return pAction;
}


template <>
void CCreateable<CGenerateEvent, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(GenerateEvent)
DEFINE_ACTION_RELEASE_FXN(GenerateEvent)
