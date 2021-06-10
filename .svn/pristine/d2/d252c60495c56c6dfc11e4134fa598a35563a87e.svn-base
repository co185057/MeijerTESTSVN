// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SignoffCommand.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SignoffCommand.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SignoffCommand.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/11/05 2:48p Dm185016
 * 
 * 2     2/28/05 4:26p Dm185016
 * lint
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
// 
// 4     1/12/05 6:10p Dm185016
// Cleaned up trace log.
// 
// 3     11/16/04 11:43a Dm185016
// fixed return code
// 
// 2     11/02/04 3:17p Dm185016
// More updates
// 
// 1     10/29/04 9:04a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SignoffCommand.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "RCMError.h"
#include "KVParmList.h"
#include "PSXMfc.h"

BEGIN_KV_HANDLER_MAP(CSignoffCommand)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSignoffCommand, KEY_CT_OP_ID, OnOperatorID)
END_KV_HANDLER_MAP()


CSignoffCommand::CSignoffCommand()
{
    CRCMActionBase::SetName(_T("CSignoffCommand"));
}


CSignoffCommand::~CSignoffCommand()
{
}


DEFINE_KV_HANDLER(CSignoffCommand, OnOperatorID, keys)
{
    m_szOpId = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
    ITRACE_DEVELOP(_T("m_szOpId:  ") + IString(m_szOpId.c_str()));
}


HRESULT CSignoffCommand::Execute(bool *pResult, IMessage &message)
{
    IMODTRACE_DEVELOP();

    HRESULT hr = S_OK;

    ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());

    if (m_szOpId.empty())
    {
        ITRACE_ERROR(_T("Missing parameter:  ") + IString(KEY_CT_OP_ID));
        return RCM_API_FAILURE_INVALID_KEY_VALUE_LIST;
    }

    ITRACE_DEVELOP(_T("Operator Id:  ") + IString(m_szOpId.c_str()) + _T(" signed off"));
    // TODO:  Add code to sign off operator

    *pResult = true;

    SetSynchronousReturnCode(message, hr);

    return hr;
}

template <>
IAction* CCreateable<CSignoffCommand, IAction>::NewInstance()
{
    IAction *pAction = new CSignoffCommand;

    return pAction;
}


template <>
void CCreateable<CSignoffCommand, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SignoffCommand)
DEFINE_ACTION_RELEASE_FXN(SignoffCommand)
