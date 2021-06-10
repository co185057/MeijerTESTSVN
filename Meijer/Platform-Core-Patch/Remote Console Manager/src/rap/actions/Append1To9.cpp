// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/Append1To9.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)Append1To9.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/Append1To9.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 5     4/18/05 7:15p Dm185016
 * Keyboard support
 * 
 * 3     3/16/05 11:01a Dm185016
 * TAR 295691 - Transaction variable name now obtained from the XML
 * 
 * 2     2/28/05 4:06p Dm185016
 * lint
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
// 
// 2     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
// 
// 1     11/16/04 11:23a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "Append1To9.h"
#include "RemoteAPMgr.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"
#include "Message.h"
#include "RCMLaneXML.h"
#include "verify.h"


BEGIN_KV_HANDLER_CLASS(CAppend1To9::A19Parms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CAppend1To9::A19Parms, KEY_EV_TRANSACTION_VARIABLE_NAME, TransactionVariableName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CAppend1To9::A19Parms, TransactionVariableName)


void CAppend1To9::A19Parms::Clear()
{
    m_TransactionVariableName.Empty();
}


BEGIN_KV_HANDLER_MAP(CAppend1To9)
    DEFINE_KV_HANDLER_MAP_ENTRY(CAppend1To9, KEY_IE_WPARAM, OnWParam)
END_KV_HANDLER_MAP()


CAppend1To9::CAppend1To9()
{
    CRCMActionBase::SetName(_T("CAppend1To9"));
}


CAppend1To9::~CAppend1To9()
{
}


DEFINE_KV_HANDLER(CAppend1To9, OnWParam, keys)
{
    int nText = CKeyValue<int>::Get(keys, 0);
	switch (nText)
	{
		case WPARAM_VALUE_1 : case WPARAM_VALUE_1_2 : case 1 : m_szCharacter = _T("1"); break;
		case WPARAM_VALUE_2 : case WPARAM_VALUE_2_2 : case 2 : m_szCharacter = _T("2"); break;
		case WPARAM_VALUE_3 : case WPARAM_VALUE_3_2 : case 3 : m_szCharacter = _T("3"); break;
		case WPARAM_VALUE_4 : case WPARAM_VALUE_4_2 : case 4 : m_szCharacter = _T("4"); break;
		case WPARAM_VALUE_5 : case WPARAM_VALUE_5_2 : case 5 : m_szCharacter = _T("5"); break;
		case WPARAM_VALUE_6 : case WPARAM_VALUE_6_2 : case 6 : m_szCharacter = _T("6"); break;
		case WPARAM_VALUE_7 : case WPARAM_VALUE_7_2 : case 7 : m_szCharacter = _T("7"); break;
		case WPARAM_VALUE_8 : case WPARAM_VALUE_8_2 : case 8 : m_szCharacter = _T("8"); break;
		case WPARAM_VALUE_9 : case WPARAM_VALUE_9_2 : case 9 : m_szCharacter = _T("9"); break;
        default:
            _ASSERTEX(_T("Invalid data"));
            ITRACE_ERROR(_T("Invalid data received"));
	}
    ITRACE_DEVELOP(_T("m_szCharacter:  ") + IString(m_szCharacter.c_str()));
}


// Get all control names from the xml
HRESULT CAppend1To9::ParseOperands()
{ 
    m_parms.Clear();

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());

    return S_OK;
}


HRESULT CAppend1To9::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    *pResult = true;

    rapmgr().AppendText(m_szCharacter.c_str());

    COleVariant varText = rapmgr().GetEntryText(); 
    SetTransactionVariable(m_parms.GetTransactionVariableName(), varText);

    return S_OK;
}


template <>
IAction* CCreateable<CAppend1To9, IAction>::NewInstance()
{
    IAction *pAction = new CAppend1To9;

    return pAction;
}


template <>
void CCreateable<CAppend1To9, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(Append1To9)
DEFINE_ACTION_RELEASE_FXN(Append1To9)
