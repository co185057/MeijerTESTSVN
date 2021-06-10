// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/actions/ApproveIntervention.cpp 4     4/29/09 4:06p Sm133076 $
/*
 * @(#)ApproveIntervention.cpp    $Revision: 4 $ $Date: 4/29/09 4:06p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/actions/ApproveIntervention.cpp $
 * 
 * 4     4/29/09 4:06p Sm133076
 * 381428 Meijer lab 4.32.6 FL Reports: Daily Summary Report by Cashier
 * are reporting incorrect operator ID
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     3/17/05 10:31a Dm185016
 * Fixed yellow light after Approve&Collect pressed.
 * 
 * 2     2/28/05 4:08p Dm185016
 * lint
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
// 
// 9     1/19/05 2:09p Dm185016
// TAR 290775 - Operator not available on any but the one that handled the
// signon.
// 
// 8     1/13/05 1:24p Dm185016
// Added sanity check to prevent DrWatson.
// 
// 7     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
// 
// 6     12/02/04 3:32p Dm185016
// Added count of pending approvals to event passed to SCOTAPP.
// 
// 5     11/22/04 3:43p Dm185016
// Added assert for error checking.
// 
// 4     11/19/04 1:33p Dm185016
// Fixes for delayed interventions
// 
// 3     11/18/04 1:37p Dm185016
// Appends ';' to command name if not present
// 
// 2     11/16/04 11:28a Dm185016
// Rewritten
// 
// 1     11/02/04 3:05p Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ApproveIntervention.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMEvent.h"
#include "Message.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "RAViews.h"
#include "Operator.h"
#include "RCMError.h"


BEGIN_KV_HANDLER_MAP(CApproveIntervention)
    DEFINE_KV_HANDLER_MAP_ENTRY(CApproveIntervention, KEY_EV_VIEW, OnView)
    DEFINE_KV_HANDLER_MAP_ENTRY(CApproveIntervention, KEY_EV_TYPE, OnType)
    DEFINE_KV_HANDLER_MAP_ENTRY(CApproveIntervention, KEY_EV_BUTTON_COMMAND, OnCommand)
    DEFINE_KV_HANDLER_MAP_ENTRY(CApproveIntervention, KEY_EV_APPROVAL_CODE, OnApprovalCode)
    DEFINE_KV_HANDLER_MAP_ENTRY(CApproveIntervention, KEY_EV_CATEGORY, OnCategory)
END_KV_HANDLER_MAP()


CApproveIntervention::CApproveIntervention()
    : m_nView(0)
    , m_hr(S_OK)
{
    CRCMActionBase::SetName(_T("CApproveIntervention"));
}


CApproveIntervention::~CApproveIntervention()
{
}


DEFINE_KV_HANDLER(CApproveIntervention, OnType, keys)
{
    m_sApprovalCategory = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
}


DEFINE_KV_HANDLER(CApproveIntervention, OnCategory, keys)
{
    m_sCategory = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
}


DEFINE_KV_HANDLER(CApproveIntervention, OnView, keys)
{
    m_nView = CKeyValue<int>::Get(keys, 0);

    m_hr = rcmStateMachine().GetViewName(m_sViewName, m_nView);
}


DEFINE_KV_HANDLER(CApproveIntervention, OnCommand, keys)
{
    m_sCommand = CKeyValue<const _TCHAR *>::Get(keys, 0);
    if (StringType::npos==m_sCommand.find(';'))
        m_sCommand += ';';
}


DEFINE_KV_HANDLER(CApproveIntervention, OnApprovalCode, keys)
{
    m_sApprovalCode = CKeyValue<const _TCHAR *>::Get(keys, 0);
    if (   (m_sApprovalCode != KEY_AEV_APPROVAL_CODE_APPROVED)
        && (m_sApprovalCode != KEY_AEV_APPROVAL_CODE_VERIFY_APPROVED)
        && (m_sApprovalCode != KEY_AEV_APPROVAL_CODE_REDO) )
        m_hr = STATEM_INVALID_PARAMETER;
}


// Get all control names from the xml
HRESULT CApproveIntervention::ParseOperands()
{ 
    HRESULT hr = S_OK;

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter != parms.end())
    {
        StringType sKeyString = *iter;
        ParseKeys(sKeyString.c_str(), GetTraceHandle());
    }

    return hr;
}


HRESULT CApproveIntervention::Execute(bool *pResult, IMessage &)
{
    m_hr = S_OK;

    m_nView         = DATANEEDED_NOVIEW;
    m_sApprovalCode = KEY_AEV_APPROVAL_CODE_APPROVED;
    m_sCommand.clear();
    m_sApprovalCategory.clear();
    m_sCategory.clear();

    CIntervention *pIntervention = rapmgr().GetCurrentIntervention();
    _ASSERT(pIntervention!= NULL);

    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    if (pIntervention != NULL)
        ParseKeys((const _TCHAR *)pIntervention->GetData(), GetTraceHandle());

    PApproval pApproval;
    const _TCHAR *pszApprovalCode   = m_sApprovalCode.c_str();
    const _TCHAR *pszCommand        = m_sCommand.c_str();
    if (!m_sCommand.empty())
    {
        pApproval = new CApproval(GetTraceHandle(), pszApprovalCode, pszCommand, pIntervention, m_nView);
    }
    else
    {
        pApproval = new CApproval(GetTraceHandle(), pszApprovalCode, pIntervention, m_nView);
    }

    if (!m_sCategory.empty())
        pApproval->GenerateCategory(m_sCategory);

    if (rapmgr().HasEntryText())
    {
        pApproval->AppendText(_T(";"));
        pApproval->AppendText(rapmgr().GetEntryText());
    }

    rapmgr().SetCurrentApproval(pApproval);


    CKeyValueParmList parmList;

    parmList.Append(pApproval->GetApprovalInfo());
    CKeyValue<int>::Put(parmList
                        , KEY_AEV_PENDING_APPROVALS
                        , rapmgr().GetDelayedInterventionPendingCount());

    if (rapmgr().HasOperator())
    {
        const COperator &op = rapmgr().GetOperator();
        CKeyValue<const _TCHAR *>::Put(parmList
                            , KEY_AEV_OP_ID
                            , op.GetOperatorId());
        CKeyValue<const _TCHAR *>::Put(parmList
                            , KEY_AEV_OP_PASSWORD
                            , op.GetOperatorPwd());
    }
    else
    {
		ITRACE_DEVELOP(_T("No operator is available"));
    }
	PREMOTECONN pRConn = rcm().GetCurrentRemoteConnection();
	_ASSERTE(pRConn!=NULL);
	if (pRConn==NULL)
		return S_OK;

	CRemoteConnection &rconn = *pRConn;
	CKeyValue<const _TCHAR *>::Put(parmList
                , KEY_AEV_REMOTE_CONNECTION_NAME                  
				, rconn.GetRemoteConnectionName());

	rapmgr().FireEvent( new CRequestApprovalEvent((const _TCHAR *)parmList) );

    *pResult = true;

    return S_OK;
}


template <>
IAction* CCreateable<CApproveIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CApproveIntervention;

    return pAction;
}


template <>
void CCreateable<CApproveIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ApproveIntervention)
DEFINE_ACTION_RELEASE_FXN(ApproveIntervention)
