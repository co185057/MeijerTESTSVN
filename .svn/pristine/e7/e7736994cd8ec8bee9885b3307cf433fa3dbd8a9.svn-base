// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/EnterStoreMode.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)EnterStoreMode.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/EnterStoreMode.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 7     11/27/07 5:43p Lp185019
 * SR629 (TAR 334800) - Resolve CISP Issue: RCM is tracing password by
 * default
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
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

#include "EnterStoreMode.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "RCMError.h"
#include "RAPReceipt.h"
#include "KVParmList.h"
#include "Operator.h"


BEGIN_KV_HANDLER_CLASS(CEnterStoreMode::ESMParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CEnterStoreMode::ESMParms, KEY_EV_STRING_ID, StringId)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CEnterStoreMode::ESMParms, StringId)


void CEnterStoreMode::ESMParms::Clear()
{
    m_StringId.Empty();
}


BEGIN_KV_HANDLER_MAP(CEnterStoreMode)
    DEFINE_KV_HANDLER_MAP_ENTRY(CEnterStoreMode, KEY_CT_OP_ID, OnOperatorID)
    DEFINE_KV_HANDLER_MAP_ENTRY(CEnterStoreMode, KEY_CT_OP_PASSWORD, OnOperatorPwd)
END_KV_HANDLER_MAP()


DEFINE_KV_HANDLER(CEnterStoreMode, OnOperatorID, keys)
{
    m_szOpId = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
    ITRACE_DEVELOP(_T("m_szOpId:  ") + IString(m_szOpId.c_str()));
}


DEFINE_KV_HANDLER(CEnterStoreMode, OnOperatorPwd, keys)
{
    m_szOpPwd = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
    ITRACE_DEVELOP(_T("m_szOpPwd: \"****\""));
}


CEnterStoreMode::CEnterStoreMode()
{
    CRCMActionBase::SetName(_T("CEnterStoreMode"));
}


CEnterStoreMode::~CEnterStoreMode()
{
}


// Get all control names from the xml
HRESULT CEnterStoreMode::ParseOperands()
{ 
    m_parms.Clear();

    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();
    CParameterList::iterator end    = parms.end();

    HRESULT hr = m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    return CPSXAction::NextOperands(iter, end);
}


HRESULT CEnterStoreMode::Execute(bool *pResult, IMessage &message)
{
    *pResult = true;

    HRESULT hr = ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    if (m_szOpId.empty())
    {
        ITRACE_ERROR(_T("Missing parameter:  ") + IString(KEY_CT_OP_ID));
        return RCM_API_FAILURE_INVALID_KEY_VALUE_LIST;
    }

    if (m_szOpPwd.empty())
    {
        ITRACE_ERROR(_T("Missing parameter:  ") + IString(KEY_CT_OP_PASSWORD));
        return RCM_API_FAILURE_INVALID_KEY_VALUE_LIST;
    }

	hr = ParseOperands();
    if (hr != S_OK)
		return hr;

	COperator *pOperator = new COperator(m_szOpId.c_str(), m_szOpPwd.c_str());
    rapmgr().SetOperator(pOperator);

	StringType sStringName = (const _TCHAR *)GetString(m_parms.GetStringId());

    CMessageReceiptItem item(GetTraceHandle(), sStringName.c_str());

    hr = AddReceiptLine(item, false);

    return hr;
}


template <>
IAction* CCreateable<CEnterStoreMode, IAction>::NewInstance()
{
    IAction *pAction = new CEnterStoreMode;

    return pAction;
}


template <>
void CCreateable<CEnterStoreMode, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(EnterStoreMode)
DEFINE_ACTION_RELEASE_FXN(EnterStoreMode)
