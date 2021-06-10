// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CanApproveCommand.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)CanApproveCommand.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CanApproveCommand.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/11/05 1:45p Dm185016
 * Moved to rap from rcm
 * 
 * 3     3/11/05 1:43p Dm185016
 * 
 * 2     2/28/05 4:31p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 4     1/12/05 5:58p Dm185016
// Cleaned up trace log.
// 
// 3     11/16/04 11:36a Dm185016
// Fixed return code
// 
// 2     10/29/04 9:08a Dm185016
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "CanApproveCommand.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"

BEGIN_KV_HANDLER_MAP(CCanApproveCommand)
    DEFINE_KV_HANDLER_MAP_ENTRY(CCanApproveCommand, KEY_CA_TYPE, OnType)
END_KV_HANDLER_MAP()


CCanApproveCommand::CCanApproveCommand()
    : m_pMessage(NULL)
    , m_hr(S_OK)
{
    CRCMActionBase::SetName(_T("CCanApproveCommand"));
}


CCanApproveCommand::~CCanApproveCommand()
{
    m_pMessage = NULL;
}


DEFINE_KV_HANDLER(CCanApproveCommand, OnType, keys)
{
    m_hr = S_OK;

    m_szType = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
    ITRACE_DEVELOP(_T("type:  ") + IString(m_szType.c_str()));
}


HRESULT CCanApproveCommand::Execute(bool *pResult, IMessage &message)
{
    IMODTRACE_DEVELOP();

    m_pMessage = &message;

    HRESULT hr = ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    bool bCanApprove = rapmgr().CanApprove(m_szType.c_str());

    SetSynchronousReturnCode(message, 0);

    CKeyValueParmList parmList;
    CKeyValue<bool>::Put(parmList, KEY_CA_APPROVAL_GRANTED, bCanApprove);
    _bstr_t bstrParmList((const _TCHAR *)parmList); 

    stateMachine().SetText(message, bstrParmList.Detach());

    *pResult = true;

    return m_hr;
}


IMessage &CCanApproveCommand::GetMessage() 
{ 
    _ASSERTE(m_pMessage!=NULL);
    return *m_pMessage; 
}

template <>
IAction* CCreateable<CCanApproveCommand, IAction>::NewInstance()
{
    IAction *pAction = new CCanApproveCommand;

    return pAction;
}


template <>
void CCreateable<CCanApproveCommand, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(CanApproveCommand)
DEFINE_ACTION_RELEASE_FXN(CanApproveCommand)
