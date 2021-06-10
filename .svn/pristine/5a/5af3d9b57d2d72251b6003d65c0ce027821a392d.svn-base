// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/RemoteDataCommandRAP.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)RemoteDataCommandRAP.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/RemoteDataCommandRAP.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     5/03/05 10:23a Dm185016
 * TAR 299832
 * 
 * 2     4/18/05 7:16p Dm185016
 * Toggle support
 * 
 * 1     4/12/05 2:53p Dm185016
 * Changes for AllLaneStatusSummary
*/
#include "StdAfx.h"
#include "RemoteDataCommandRAP.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMEvent.h"
#include "RCMKeyValue.h"

using namespace fastlane;


BEGIN_KV_HANDLER_CLASS(CRemoteDataCommandRAP::RDCRapParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CRemoteDataCommandRAP::RDCRapParms, KEY_EV_COMMAND, Command)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CRemoteDataCommandRAP::RDCRapParms, Command)
BEGIN_KV_HANDLER_CLASS_CLEAR(CRemoteDataCommandRAP::RDCRapParms)
    CLEAR_KV_CSTRING(Command)
END_KV_HANDLER_CLASS_CLEAR()


CRemoteDataCommandRAP::CRemoteDataCommandRAP()
{
    CRCMActionBase::SetName(_T("CRemoteDataCommandRAP"));
}


CRemoteDataCommandRAP::~CRemoteDataCommandRAP()
{
}


HRESULT CRemoteDataCommandRAP::ParseAuxData(const _TCHAR *pszRemoteData)
{
    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys(pszRemoteData, GetTraceHandle());
    if (hr != S_OK)
        return hr;

    return CRemoteDataCommand::ParseAuxData(pszRemoteData);
}


const _TCHAR * CRemoteDataCommandRAP::GetOperation()
{
    if (m_parms.GetCommand().IsEmpty())
        return CRemoteDataCommand::GetOperation();

    return m_parms.GetCommand();
}


template <>
IAction* CCreateable<CRemoteDataCommandRAP, IAction>::NewInstance()
{
    IAction *pAction = new CRemoteDataCommandRAP;

    return pAction;
}


template <>
void CCreateable<CRemoteDataCommandRAP, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(RemoteDataCommandRAP)
DEFINE_ACTION_RELEASE_FXN(RemoteDataCommandRAP)
