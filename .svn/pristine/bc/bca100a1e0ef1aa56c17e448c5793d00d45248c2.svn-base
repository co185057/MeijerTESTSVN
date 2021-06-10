// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/TransactionBegin.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)TransactionBegin.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/TransactionBegin.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/11/05 2:46p Dm185016
 * 
 * 2     2/28/05 4:26p Dm185016
 * lint
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
// 
// 4     12/16/04 10:11a Dm185016
// Factored out the clear receipt functionality into new ClearReceipt
// action.
// 
// 3     12/02/04 3:44p Dm185016
// Now saving status of transaction in RemoteAPMgr.
// 
// 2     10/29/04 9:16a Dm185016
// Filled in fxn.
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "TransactionBegin.h"
#include "RemoteAPMgr.h"


CTransactionBegin::CTransactionBegin()
{
    CRCMActionBase::SetName(_T("CTransactionBegin"));
}


CTransactionBegin::~CTransactionBegin()
{
}


HRESULT CTransactionBegin::Execute(bool *pResult, IMessage &)
{
    rapmgr().SetInsideTransaction();

    *pResult = true;

    return S_OK;
}


template <>
IAction* CCreateable<CTransactionBegin, IAction>::NewInstance()
{
    IAction *pAction = new CTransactionBegin;

    return pAction;
}


template <>
void CCreateable<CTransactionBegin, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(TransactionBegin)
DEFINE_ACTION_RELEASE_FXN(TransactionBegin)
