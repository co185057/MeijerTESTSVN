// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/GetApprovalCommand.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)GetApprovalCommand.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/GetApprovalCommand.cpp $
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
 * 2     2/28/05 4:38p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 3     1/12/05 6:01p Dm185016
// Cleaned up trace log.
// 
// 2     10/29/04 9:11a Dm185016
// Filled in fxn.
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "GetApprovalCommand.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"

BEGIN_KV_HANDLER_MAP(CGetApprovalCommand)
    DEFINE_KV_HANDLER_MAP_ENTRY(CGetApprovalCommand, KEY_AI_TYPE, OnType)
END_KV_HANDLER_MAP()


CGetApprovalCommand::CGetApprovalCommand()
    : m_pMessage(NULL)
    , m_hr(S_OK)
{
    CRCMActionBase::SetName(_T("CGetApprovalCommand"));
}


CGetApprovalCommand::~CGetApprovalCommand()
{
    m_pMessage = NULL;
}


DEFINE_KV_HANDLER(CGetApprovalCommand, OnType, keys)
{
    m_hr = S_OK;

    StringType sType = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
    ITRACE_DEVELOP(_T("sType:  ") + IString(sType.c_str()));

    // Return all approvals to SCOTAPP
    if (sType == KEY_AI_TYPE_ALL_APPROVALS)
    {
        CKeyValueParmList parmList;
        //CKeyValue<const _TCHAR *>::Put(parmList, KEY_SC_PARAM, m_szParam.c_str()); 
        //CKeyValue<const _TCHAR *>::Put(parmList, pszValue); 
        _bstr_t bstrParmList((const _TCHAR *)parmList); 

        // TODO:  Need to add code to actually return something here.  Each pending
        // approval needs to be its own record.
        stateMachine().SetText(GetMessage(), bstrParmList.Detach());
    }

    // TODO:  Need to implement the other approval types.
    else
    {
        ITRACE_DEVELOP(_T("Approval type not implemented"));

        _ASSERTE(0);
    }
}


HRESULT CGetApprovalCommand::Execute(bool *pResult, IMessage &message)
{
    m_pMessage = &message;

    HRESULT hr = ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    *pResult = true;

    return m_hr;
}


IMessage &CGetApprovalCommand::GetMessage() 
{ 
    _ASSERTE(m_pMessage!=NULL);
    return *m_pMessage; 
}

template <>
IAction* CCreateable<CGetApprovalCommand, IAction>::NewInstance()
{
    IAction *pAction = new CGetApprovalCommand;

    return pAction;
}


template <>
void CCreateable<CGetApprovalCommand, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(GetApprovalCommand)
DEFINE_ACTION_RELEASE_FXN(GetApprovalCommand)
