// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ShowMessage.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)ShowMessage.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ShowMessage.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 10    6/05/08 8:46a Bs185067
 * TAR 376639
 * 
 * 9     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 5     3/24/05 7:20p Dm185016
 * UNICODE
 * 
 * 4     3/16/05 6:21p Dm185016
 * Was not resetting m_parms before use.
 * 
 * 3     2/28/05 4:15p Dm185016
 * lint
 * 
 * 2     2/10/05 5:27p Dm185016
 * Changed to use kv <parameters>
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 3     1/18/05 3:45p Dm185016
// UNICODE support
// 
// 2     1/12/05 5:52p Dm185016
// Approve age check
// 
// 1     11/16/04 11:23a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ShowMessage.h"
#include "RemoteAPMgr.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"
#include "Message.h"
#include "RCMLaneXML.h"
#include "PSXMfc.h"
#include "LocaleEx.h"


BEGIN_KV_HANDLER_CLASS(CShowMessage::SMParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CShowMessage::SMParms, KEY_EV_TRANSACTION_VARIABLE_NAME, TransactionVariableName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CShowMessage::SMParms, KEY_EV_STRING_ID, StringId)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CShowMessage::SMParms, TransactionVariableName)
DEFINE_KV_CSTRING(CShowMessage::SMParms, StringId)


void CShowMessage::SMParms::Clear()
{
    m_TransactionVariableName.Empty();
    m_StringId.Empty();
}


CShowMessage::CShowMessage()
{
    CRCMActionBase::SetName(_T("CShowMessage"));
}


CShowMessage::~CShowMessage()
{
}


// Get all control names from the xml
HRESULT CShowMessage::ParseOperands()
{ 
    m_parms.Clear();

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());

    return S_OK;
}


HRESULT CShowMessage::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    *pResult = true;

    CString sPatternText = GetString( m_parms.GetStringId() );
    IString sString((const _TCHAR *)sPatternText);

    IString sDateFormat = (const _TCHAR *)CLocale::GetDateFormat(GetTraceHandle());
    IString sText;
    sText.format(sString, (const _TCHAR *)sDateFormat);

    COleVariant varMsg = (const _TCHAR *)sText;
    SetTransactionVariable(m_parms.GetTransactionVariableName(), varMsg);

    return S_OK;
}


template <>
IAction* CCreateable<CShowMessage, IAction>::NewInstance()
{
    IAction *pAction = new CShowMessage;

    return pAction;
}


template <>
void CCreateable<CShowMessage, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ShowMessage)
DEFINE_ACTION_RELEASE_FXN(ShowMessage)
