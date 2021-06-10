// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsNotStartTransactionView.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsNotStartTransactionView.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsNotStartTransactionView.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 3     1/13/05 1:27p Dm185016
// Removed trace.
// 
// 2     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
// 
// 1     11/16/04 11:23a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsNotStartTransactionView.h"
#include "RCMKeyValue.h"
#include "Message.h"
#include "RAViews.h"
#include "KVParmList.h"

BEGIN_KV_HANDLER_MAP(CIsNotStartTransactionView)
    DEFINE_KV_HANDLER_MAP_ENTRY(CIsNotStartTransactionView, KEY_EV_VIEW, OnView)
END_KV_HANDLER_MAP()


CIsNotStartTransactionView::CIsNotStartTransactionView()
{
    CRCMActionBase::SetName(_T("CIsNotStartTransactionView"));
}


CIsNotStartTransactionView::~CIsNotStartTransactionView()
{
}


DEFINE_KV_HANDLER(CIsNotStartTransactionView, OnView, keys)
{
    m_nView = CKeyValue<int>::Get(keys, 0);
}


HRESULT CIsNotStartTransactionView::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = ParseMessageKeys(message);
    if (hr != S_OK)
        return hr;

    *pResult = (m_nView != DATANEEDED_STARTTRANSACTION);

    return hr;
}


template <>
IAction* CCreateable<CIsNotStartTransactionView, IAction>::NewInstance()
{
    IAction *pAction = new CIsNotStartTransactionView;

    return pAction;
}


template <>
void CCreateable<CIsNotStartTransactionView, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsNotStartTransactionView)
DEFINE_ACTION_RELEASE_FXN(IsNotStartTransactionView)
