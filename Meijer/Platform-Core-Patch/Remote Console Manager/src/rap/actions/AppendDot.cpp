// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/AppendDot.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)AppendDot.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/AppendDot.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 8     6/12/08 9:26a mv185026
 * TAR 373128 - can input dot more than once
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
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
// 1     12/15/04 5:55p Tp151000
// TAR 287344 and 288069
// 
// 1     11/16/04 11:23a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "AppendDot.h"
#include "RemoteAPMgr.h"
#include "RCMLaneXML.h"
#include "RCMKeyValue.h"


BEGIN_KV_HANDLER_CLASS(CAppendDot::ADParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CAppendDot::ADParms, KEY_EV_TRANSACTION_VARIABLE_NAME, TransactionVariableName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CAppendDot::ADParms, TransactionVariableName)


void CAppendDot::ADParms::Clear()
{
    m_TransactionVariableName.Empty();
}


CAppendDot::CAppendDot()
{
    CRCMActionBase::SetName(_T("CAppendDot"));
}


CAppendDot::~CAppendDot()
{
}


// Get all control names from the xml
HRESULT CAppendDot::ParseOperands()
{ 
    m_parms.Clear();

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());

    return S_OK;
}


HRESULT CAppendDot::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    *pResult = true;

	StringType m_szCharacter;
    m_szCharacter = _T(".");

	StringType theDot = rapmgr().GetEntryText(); 
	if(theDot.find('.') == -1) // if there's no dot append it
	{
		rapmgr().AppendText(m_szCharacter.c_str());
	}
    COleVariant varText = rapmgr().GetEntryText(); 
    SetTransactionVariable(m_parms.GetTransactionVariableName(), varText);

    return S_OK;
}


template <>
IAction* CCreateable<CAppendDot, IAction>::NewInstance()
{
    IAction *pAction = new CAppendDot;

    return pAction;
}


template <>
void CCreateable<CAppendDot, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(AppendDot)
DEFINE_ACTION_RELEASE_FXN(AppendDot)
