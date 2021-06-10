// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/MediaLowEvent.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)MediaLowEvent.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/MediaLowEvent.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/17/05 2:30p Tp151000
 * Add Media Low event
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
// 5     1/12/05 6:00p Dm185016
// Cleaned up trace log.
// 
// 4     12/31/04 9:19a Dm185016
// TAR 288930 - Fixed problem logging enter/exit store mode.  Name of
// transaction variable now read from XML.
// 
// 3     11/16/04 11:38a Dm185016
// Rewritten
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "MediaLowEvent.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "RCMError.h"
#include "RAPReceipt.h"
#include "KVParmList.h"


BEGIN_KV_HANDLER_CLASS(CMediaLowEvent::MLEParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CMediaLowEvent::MLEParms, KEY_EV_DESCRIPTION, Description)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CMediaLowEvent::MLEParms, Description)


void CMediaLowEvent::MLEParms::Clear()
{
    m_Description.Empty();
}


CMediaLowEvent::CMediaLowEvent()
{
    CRCMActionBase::SetName(_T("CMediaLowEvent"));
}


CMediaLowEvent::~CMediaLowEvent()
{
}


// Get all control names from the xml
HRESULT CMediaLowEvent::ParseOperands()
{ 
    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();
    CParameterList::iterator end    = parms.end();

    return CPSXAction::NextOperands(iter, end);
}


HRESULT CMediaLowEvent::Execute(bool *pResult, IMessage &message)
{
    *pResult = true;

    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    if (m_parms.GetDescription().IsEmpty())
    {
        ITRACE_ERROR(_T("Missing parameter:  ") + IString(KEY_CT_DESCRIPTION));
        return RCM_API_FAILURE_INVALID_KEY_VALUE_LIST;
    }

    CMessageReceiptItem item(GetTraceHandle(), m_parms.GetDescription());

    hr = AddReceiptLine(item, false);

    return hr;
}


template <>
IAction* CCreateable<CMediaLowEvent, IAction>::NewInstance()
{
    IAction *pAction = new CMediaLowEvent;

    return pAction;
}


template <>
void CCreateable<CMediaLowEvent, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(MediaLowEvent)
DEFINE_ACTION_RELEASE_FXN(MediaLowEvent)
