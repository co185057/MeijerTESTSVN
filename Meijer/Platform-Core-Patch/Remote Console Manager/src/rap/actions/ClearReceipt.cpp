// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearReceipt.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ClearReceipt.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearReceipt.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:09p Dm185016
 * lint
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
// 
// 2     1/05/05 1:55p Tk124825
// TAR 287909 - Added code to parse total text, total amount, item text &
// Item  value. 
// 
// 1     12/16/04 9:53a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;
#include "RCMKeyValue.h"
#include "KVParmList.h"
#include "ClearReceipt.h"


CClearReceipt::CClearReceipt()
{
    CRCMActionBase::SetName(_T("CClearReceipt"));
}


CClearReceipt::~CClearReceipt()
{
}


// Get all control names from the xml
HRESULT CClearReceipt::ParseOperands()
{ 
    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();
    CParameterList::iterator end    = parms.end();

    if (iter == end)
        return STATEM_MISSING_PARAMETER;

    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    return CPSXAction::NextOperands(iter, end);
}
BEGIN_KV_HANDLER_CLASS(CClearReceipt::CRParms)
	DEFINE_KV_HANDLER_MAP_ENTRY(CClearReceipt::CRParms, KEY_EV_ITEM_COUNT_STATIC_TEXT_CONTROL_NAME , ItemCountStaticTextControlName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CClearReceipt::CRParms, KEY_EV_TOTAL_AMOUNT_STATIC_TEXT_CONTROL_NAME , TotalAmountStaticTextControlName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CClearReceipt::CRParms, KEY_EV_ITEM_COUNT_TEXT_CONTROL_NAME , ItemCountTextControlName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CClearReceipt::CRParms, KEY_EV_TOTAL_AMOUNT_TEXT_CONTROL_NAME , TotalAmountTextControlName)
	DEFINE_KV_HANDLER_MAP_ENTRY(CClearReceipt::CRParms, KEY_EV_CONTROL_VISIBLE , Visible)

END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CClearReceipt::CRParms, ItemCountStaticTextControlName)
DEFINE_KV_CSTRING(CClearReceipt::CRParms, TotalAmountStaticTextControlName)
DEFINE_KV_CSTRING(CClearReceipt::CRParms, ItemCountTextControlName)
DEFINE_KV_CSTRING(CClearReceipt::CRParms, TotalAmountTextControlName)
DEFINE_KV_BOOL(CClearReceipt::CRParms, Visible)

void CClearReceipt::CRParms::Clear()
{
    m_ItemCountStaticTextControlName.Empty();
    m_TotalAmountStaticTextControlName.Empty();
    m_ItemCountTextControlName.Empty();
    m_TotalAmountTextControlName.Empty();
	
}
HRESULT CClearReceipt::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;
	ClearReceipt(m_parms.GetItemCountTextControlName() ,m_parms.GetTotalAmountTextControlName(),
				m_parms.GetItemCountStaticTextControlName() ,m_parms.GetTotalAmountStaticTextControlName(), m_parms.IsVisible());

    *pResult = true;

    return hr;
}


template <>
IAction* CCreateable<CClearReceipt, IAction>::NewInstance()
{
    IAction *pAction = new CClearReceipt;

    return pAction;
}


template <>
void CCreateable<CClearReceipt, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ClearReceipt)
DEFINE_ACTION_RELEASE_FXN(ClearReceipt)
