// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ScrollReceipt.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)ScrollReceipt.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ScrollReceipt.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 5     1/13/06 3:46p Tk124825
 * TAR 301329
 * 
 * 2     2/28/05 4:40p Dm185016
 * lint
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
// 
// 2     1/12/05 6:08p Dm185016
// Cleaned up trace log.
// 
// 1     12/09/04 1:49p Dm185016
// TAR 287907
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ScrollReceipt.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"
#include "UIDefines.h"
#include "PSXMfc.h"
#include "verifypsx.h"


BEGIN_KV_HANDLER_CLASS(CScrollReceipt::SIParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CScrollReceipt::SIParms, KEY_EV_RECEIPT_CONTROL_NAME   , ReceiptControlName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CScrollReceipt::SIParms, KEY_EV_SCROLL_TYPE            , ScrollType)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CScrollReceipt::SIParms, ReceiptControlName)
DEFINE_KV_CSTRING(CScrollReceipt::SIParms, ScrollType)


void CScrollReceipt::SIParms::Clear()
{
    m_ReceiptControlName.Empty();
    m_ScrollType.Empty();
}


CScrollReceipt::CScrollReceipt()
{
    CRCMActionBase::SetName(_T("CScrollReceipt"));
}


CScrollReceipt::~CScrollReceipt()
{
}


// Get all control names from the xml
HRESULT CScrollReceipt::ParseOperands()
{ 
    HRESULT hr = S_OK;

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;
    
    m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());

    return hr;
}


HRESULT CScrollReceipt::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = ParseOperands();

    *pResult = true;

    if (hr == S_OK)
    {
        StringType sScrollType = m_parms.GetScrollType();
        if (sScrollType == KEY_EV_SCROLL_TYPE_PAGE_UP)
        {
            COleVariant vReturn, vScrollMode;
            vScrollMode = (long) UI::PageUp;
            VERIFY_PSX(hr, psx().SendCommand((const _TCHAR *)m_parms.GetReceiptControlName(), UI::COMMANDSCROLL, vReturn, 1, vScrollMode));
        }
        else if (sScrollType == KEY_EV_SCROLL_TYPE_PAGE_DOWN)
        {
            COleVariant vReturn, vScrollMode;
            vScrollMode = (long) UI::PageDown;
            VERIFY_PSX(hr, psx().SendCommand((const _TCHAR *)m_parms.GetReceiptControlName(), UI::COMMANDSCROLL, vReturn, 1, vScrollMode));
        }
        else
            hr = STATEM_INVALID_PARAMETER;
    }

    return hr;
}

template <>
IAction* CCreateable<CScrollReceipt, IAction>::NewInstance()
{
    IAction *pAction = new CScrollReceipt;

    return pAction;
}


template <>
void CCreateable<CScrollReceipt, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ScrollReceipt)
DEFINE_ACTION_RELEASE_FXN(ScrollReceipt)
