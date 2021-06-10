// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ExitStoreMode.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ExitStoreMode.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ExitStoreMode.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     3/16/05 2:28p Dm185016
 *    
 * 
 * 
 * 1     3/11/05 2:41p Dm185016
 * 
 * 2     2/28/05 4:35p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 4     1/12/05 6:00p Dm185016
// Cleaned up trace log.
// 
// 3     12/31/04 9:19a Dm185016
// TAR 288930 - Fixed problem logging enter/exit store mode.  Name of
// transaction variable now read from XML.
// 
// 2     10/29/04 9:10a Dm185016
// Filled out fxn.
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ExitStoreMode.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"
#include "RAPReceipt.h"


BEGIN_KV_HANDLER_CLASS(CExitStoreMode::ESMParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CExitStoreMode::ESMParms, KEY_EV_STRING_ID, StringId)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CExitStoreMode::ESMParms, StringId)


void CExitStoreMode::ESMParms::Clear()
{
    m_StringId.Empty();
}


CExitStoreMode::CExitStoreMode()
{
    CRCMActionBase::SetName(_T("CExitStoreMode"));
}


CExitStoreMode::~CExitStoreMode()
{
}


// Get all control names from the xml
HRESULT CExitStoreMode::ParseOperands()
{ 
    m_parms.Clear();

    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();
    CParameterList::iterator end    = parms.end();

    HRESULT hr = m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    return CPSXAction::NextOperands(iter, end);
}


HRESULT CExitStoreMode::Execute(bool *pResult, IMessage &)
{
    *pResult = true;

	HRESULT hr = ParseOperands();

    if (hr == S_OK)
    {
        StringType sStringName = (const _TCHAR *)GetString(m_parms.GetStringId());

        CMessageReceiptItem item(GetTraceHandle(), sStringName.c_str());

        hr = AddReceiptLine(item, false);
    }

    return hr;
}


template <>
IAction* CCreateable<CExitStoreMode, IAction>::NewInstance()
{
    IAction *pAction = new CExitStoreMode;

    return pAction;
}


template <>
void CCreateable<CExitStoreMode, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ExitStoreMode)
DEFINE_ACTION_RELEASE_FXN(ExitStoreMode)
