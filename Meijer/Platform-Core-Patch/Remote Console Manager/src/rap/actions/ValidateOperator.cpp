// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ValidateOperator.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)ValidateOperator.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ValidateOperator.cpp $
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
// 2     1/12/05 6:10p Dm185016
// Cleaned up trace log.
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ValidateOperator.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

BEGIN_KV_HANDLER_MAP(CValidateOperator)
    //DEFINE_KV_HANDLER_MAP_ENTRY(CValidateOperator, KEY_OPERATION, OnOperation)
    //DEFINE_KV_HANDLER_MAP_ENTRY(CValidateOperator, KEY_AC_SET_APP_STATE, OnSetAppState)
END_KV_HANDLER_MAP()


CValidateOperator::CValidateOperator()
    : m_pMessage(NULL)
    , m_hr(S_OK)
{
    CRCMActionBase::SetName(_T("CValidateOperator"));
}


CValidateOperator::~CValidateOperator()
{
    m_pMessage = NULL;
}


DEFINE_KV_HANDLER(CValidateOperator, OnOperation, keys)
{
    m_hr = S_OK;

    m_szOperation = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
    ITRACE_DEVELOP(_T("operation:  ") + IString(m_szOperation.c_str()));
    //if (m_szOperation == KEY_AC_SKIP_BAGGING)
    //{
    //    IMessage &newMsg 
    //        = stateMachine().MakeAsynchCopyEx(GetMessage(), MSG_SKIP_BAGGING);
    //    m_hr = stateMachine().PostMsg(newMsg);
    //}

    //else if (m_szOperation == KEY_AC_ITEM_REMOVED)
    //{
    //    IMessage &newMsg 
    //        = stateMachine().MakeAsynchCopyEx(GetMessage(), MSG_ITEM_REMOVED);
    //    m_hr = stateMachine().PostMsg(newMsg);
    //}


    //else 
    //{
    //    ITRACE_ERROR(_T("Invalid Application Control operation:  ") 
    //               + IString(m_szOperation.c_str()));
    //    m_hr = E_FAIL;
    //}
}


DEFINE_KV_HANDLER(CValidateOperator, OnSetAppState, keys)
{
    m_hr = S_OK;

    IMessage &newMsg 
        = stateMachine().MakeAsynchCopyEx(GetMessage(), MSG_EVENT_COMMAND);
    m_hr = stateMachine().PostMsg(newMsg);
}


HRESULT CValidateOperator::Execute(bool *pResult, IMessage &message)
{
    m_pMessage = &message;

    ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());

    *pResult = true;

    return m_hr;
}


IMessage &CValidateOperator::GetMessage() 
{ 
    _ASSERTE(m_pMessage!=NULL);
    return *m_pMessage; 
}

template <>
IAction* CCreateable<CValidateOperator, IAction>::NewInstance()
{
    IAction *pAction = new CValidateOperator;

    return pAction;
}


template <>
void CCreateable<CValidateOperator, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ValidateOperator)
DEFINE_ACTION_RELEASE_FXN(ValidateOperator)
