// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/NotifyAssistMode.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)NotifyAssistMode.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/NotifyAssistMode.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     3/30/05 3:54p Dm185016
 * TAR 297008
 * 
 * 3     3/17/05 2:41p Dm185016
 * TAR 296081
 * 
 * 2     2/28/05 4:12p Dm185016
 * lint
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 5     1/18/05 3:44p Dm185016
// UNICODE support
// 
// 4     1/13/05 1:27p Dm185016
// Corrected machine name in event parameter list.
// 
// 3     12/14/04 11:19a Dm185016
// Added enter/exit store mode functions
// 
// 2     11/30/04 4:20p Dm185016
// Added reporting functions.
// 
// 1     11/16/04 11:23a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "NotifyAssistMode.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMEvent.h"
#include "Message.h"
#include "RCMKeyValue.h"


CNotifyAssistMode::CNotifyAssistMode()
    : m_bAssistModeRequested(false)
    , m_eConnMode(RA_CONNECTION_MODE_CUSTOMER)
{
    CRCMActionBase::SetName(_T("CNotifyAssistMode"));
}


CNotifyAssistMode::~CNotifyAssistMode()
{
}


// Get all control names from the xml
HRESULT CNotifyAssistMode::ParseOperands()
{ 
    HRESULT hr = S_OK;

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    // Get the control name for the receipt control
    if (iter != parms.end())
    {
        m_szEnterAssistMode = *iter;
        m_bAssistModeRequested = false;
        if ( m_szEnterAssistMode == KEY_AMEV_OPERATION_ENTER_AM )
        {
            m_eConnMode = RA_CONNECTION_MODE_ASSIST;
            m_bAssistModeRequested = true;
        }
        else if ( m_szEnterAssistMode == KEY_AMEV_OPERATION_EXIT_AM )
        {
            m_eConnMode = RA_CONNECTION_MODE_CUSTOMER;
            m_bAssistModeRequested = false;
        }
        else if ( m_szEnterAssistMode == KEY_AMEV_OPERATION_ENTER_SM )
        {
            m_eConnMode = RA_CONNECTION_MODE_STORE;
            m_bAssistModeRequested = true;
        }
        else if ( m_szEnterAssistMode == KEY_AMEV_OPERATION_EXIT_SM )
        {
            m_eConnMode = RA_CONNECTION_MODE_CUSTOMER;
            m_bAssistModeRequested = false;
        }
        else if ( m_szEnterAssistMode == KEY_AMEV_OPERATION_ENTER_REPORTING )
        {
            m_eConnMode = RA_CONNECTION_MODE_REPORTING;
            m_bAssistModeRequested = true;
        }
        else if ( m_szEnterAssistMode == KEY_AMEV_OPERATION_EXIT_REPORTING )
        {
            m_eConnMode = RA_CONNECTION_MODE_CUSTOMER;
            m_bAssistModeRequested = false;
        }
        else if ( m_szEnterAssistMode == KEY_AMEV_OPERATION_ENTER_DATA_NEEDED )
        {
            m_eConnMode = RA_CONNECTION_MODE_DATA_NEEDED;
            m_bAssistModeRequested = true;
        }
        else if ( m_szEnterAssistMode == KEY_AMEV_OPERATION_EXIT_DATA_NEEDED )
        {
            m_eConnMode = RA_CONNECTION_MODE_CUSTOMER;
            m_bAssistModeRequested = false;
        }
        else
            hr = STATEM_INVALID_PARAMETER;
    }
    else
        hr = STATEM_MISSING_PARAMETER;
    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  m_szEnterAssistMode:  ") + IString(m_szEnterAssistMode.c_str()));

    return hr;
}


HRESULT CNotifyAssistMode::Execute(bool *pResult, IMessage &)
{
    m_bAssistModeRequested  = false;
    m_eConnMode             = RA_CONNECTION_MODE_CUSTOMER;

    m_szEnterAssistMode.clear();

    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    rapmgr().SetAssistModeRequested(m_bAssistModeRequested);
    rapmgr().SetConnectionMode(m_eConnMode);

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR *>::Put(parmList
                        , KEY_AMEV_OPERATION
                        , m_szEnterAssistMode.c_str());
    CKeyValue<const _TCHAR *>::Put(parmList
                        , KEY_AMEV_MACHINE_NAME
                        , rcm().GetRemoteMachineName());
    CKeyValue<const _TCHAR *>::Put(parmList
                        , KEY_AMEV_REMOTE_CONNECTION_NAME
                        , rcm().GetRemoteConnectionName());

    rapmgr().FireEvent( new CAssistModeEvent((const _TCHAR *)parmList) );

    *pResult = true;

    return S_OK;
}


template <>
IAction* CCreateable<CNotifyAssistMode, IAction>::NewInstance()
{
    IAction *pAction = new CNotifyAssistMode;

    return pAction;
}


template <>
void CCreateable<CNotifyAssistMode, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(NotifyAssistMode)
DEFINE_ACTION_RELEASE_FXN(NotifyAssistMode)
