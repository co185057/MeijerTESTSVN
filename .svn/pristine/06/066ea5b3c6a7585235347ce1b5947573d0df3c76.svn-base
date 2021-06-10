// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearEntryText.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ClearEntryText.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearEntryText.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     3/16/05 11:01a Dm185016
 * TAR 295691 - Transaction variable name now obtained from the XML
 * 
 * 2     2/28/05 4:08p Dm185016
 * lint
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
// 
// 1     11/16/04 11:23a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ClearEntryText.h"
#include "RemoteAPMgr.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"
#include "RCMLaneXML.h"


BEGIN_KV_HANDLER_CLASS(CClearEntryText::CETParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CClearEntryText::CETParms, KEY_EV_TRANSACTION_VARIABLE_NAME, TransactionVariableName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CClearEntryText::CETParms, TransactionVariableName)


void CClearEntryText::CETParms::Clear()
{
    m_TransactionVariableName.Empty();
}


CClearEntryText::CClearEntryText()
{
    CRCMActionBase::SetName(_T("CClearEntryText"));
}


CClearEntryText::~CClearEntryText()
{
}


// Get all control names from the xml
HRESULT CClearEntryText::ParseOperands()
{ 
    m_parms.Clear();

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());

    return S_OK;
}


HRESULT CClearEntryText::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    *pResult = true;

    rapmgr().ResetEntryText();

    COleVariant varText = _T(""); 
    SetTransactionVariable(m_parms.GetTransactionVariableName(), varText);

    return S_OK;
}


template <>
IAction* CCreateable<CClearEntryText, IAction>::NewInstance()
{
    IAction *pAction = new CClearEntryText;

    return pAction;
}


template <>
void CCreateable<CClearEntryText, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ClearEntryText)
DEFINE_ACTION_RELEASE_FXN(ClearEntryText)
