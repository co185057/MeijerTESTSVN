// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/TransactionEnd.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)TransactionEnd.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/TransactionEnd.cpp $
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
// 4     1/05/05 2:35p Tk124825
// TAR 287909 Took out the call to ClearReceipt
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

#include "TransactionEnd.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"


CTransactionEnd::CTransactionEnd()
{
    CRCMActionBase::SetName(_T("CTransactionEnd"));
}


CTransactionEnd::~CTransactionEnd()
{
}


// Get all control names from the xml
HRESULT CTransactionEnd::ParseOperands()
{ 
    HRESULT hr = S_OK;

    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();
    CParameterList::iterator end    = parms.end();

    hr = CPSXAction::NextOperands(iter, end);

    return hr;
}


HRESULT CTransactionEnd::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = S_OK;

    // Send a message to the state machine to turn the light off (grey).
    CKeyValueParmList parmlist;
    CKeyValue<int>::Put(parmlist, KEY_CT_OPERATION_SET_LIGHT_STATE_GREEN, 0);
    CKeyValue<int>::Put(parmlist, KEY_CT_OPERATION_SET_LIGHT_STATE_YELLOW, 0);
    CKeyValue<int>::Put(parmlist, KEY_CT_OPERATION_SET_LIGHT_STATE_RED, 0);

    const _TCHAR *pszPList = (const _TCHAR *)parmlist;

    IMessage *pNewMsg;
    hr = stateMachine().GetMessage(&pNewMsg, MSG_SET_LIGHT_STATE, pszPList, _tcslen(pszPList));
    if (hr == S_OK)
        hr = stateMachine().PostMsg(*pNewMsg);

    
    rapmgr().SetInsideTransaction(false);

    *pResult = true;

    return hr;
}

template <>
IAction* CCreateable<CTransactionEnd, IAction>::NewInstance()
{
    IAction *pAction = new CTransactionEnd;

    return pAction;
}


template <>
void CCreateable<CTransactionEnd, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(TransactionEnd)
DEFINE_ACTION_RELEASE_FXN(TransactionEnd)
