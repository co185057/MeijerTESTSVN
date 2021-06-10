// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ApprovalCommand.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ApprovalCommand.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ApprovalCommand.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 7     1/22/08 4:40a pl250020
 * TAR 352409 : Added for new Suspension Decline Event
 *
 * 6     12/22/06 10:36a Jj100011
 * RFC 337360
 *
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 *
 * 1     3/11/05 1:46p Dm185016
 * Move from rcm
 *
 * 3     3/10/05 11:46a Tp151000
 * TAR 295124
 * TP & AQ
 *
 *
 * 2     2/28/05 4:31p Dm185016
 * lint
 *
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
//
// 3     1/12/05 5:58p Dm185016
// Cleaned up trace log.
//
// 2     12/22/04 11:08a Dm185016
//
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ApprovalCommand.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"

BEGIN_KV_HANDLER_MAP(CApprovalCommand)
    DEFINE_KV_HANDLER_MAP_ENTRY(CApprovalCommand, KEY_EV_TYPE, OnType)
END_KV_HANDLER_MAP()


BEGIN_KEY_MSG_MAP(CApprovalCommand)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_ADD_OTHER_EVENT,              MSG_ADD_OTHER_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_SECURITY,                     MSG_SECURITY_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_COUPON,                       MSG_COUPON_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_VOID,                         MSG_VOID_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_AGE_RESTRICTION,              MSG_AGE_RESTRICTION_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_VISUAL_VALIDATION,            MSG_VISUAL_VALIDATION_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_CALL_FOR_HELP,                MSG_CALL_FOR_HELP_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_OTHER_PAYMENT,                MSG_OTHER_PAYMENT_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_SUSPEND,                      MSG_SUSPEND_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_PRICE_REQUIRED,               MSG_PRICE_REQUIRED_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_UNKNOWN_ITEM,                 MSG_UNKNOWN_ITEM_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_RECALLED_ITEM,                MSG_RECALLED_ITEM_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_TIME_RESTRICTED_ITEM,         MSG_TIME_RESTRICTED_ITEM_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_SIG_CAP,                      MSG_SIG_CAP_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_APPLICATION_ERROR,            MSG_APPLICATION_ERROR_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_DEVICE_ERROR,                 MSG_DEVICE_ERROR_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_QUANTITY_RESTRICTED_ITEM,     MSG_QUANTITY_RESTRICTED_ITEM_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_UNDEACTIVATED_ITEM,           MSG_UNDEACTIVATED_ITEM_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_RESTRICTED_NOT_ALLOWED,       MSG_RESTRICTED_NOT_ALLOWED_APPROVAL)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_REMOTE,                       MSG_REMOTE_APPROVAL)
	KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_HOPPER_SUBSTITUTION,          MSG_HOPPER_SUBSTITUTION_APPROVAL)
	KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_SUSPENSIONDECLINED,			MSG_SUSPENSION_DECLINE_APPROVAL)	// TAR 352409

END_KEY_MSG_MAP()

CApprovalCommand::CApprovalCommand()
    : m_pMessage(NULL)
    , m_hr(S_OK)
{
    CRCMActionBase::SetName(_T("CApprovalCommand"));
}


CApprovalCommand::~CApprovalCommand()
{
    m_pMessage = NULL;
}


DEFINE_KV_HANDLER(CApprovalCommand, OnType, keys)
{
    m_hr = S_OK;

    m_szType = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
    ITRACE_DEVELOP(_T("type:  ") + IString(m_szType.c_str()));
}


HRESULT CApprovalCommand::Execute(bool *pResult, IMessage &message)
{
    m_pMessage = &message;

    HRESULT hr = ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    hr = PostKeyMsg(this, message, m_szType.c_str());
    if (hr != S_OK)
        return hr;

    *pResult = true;

    return m_hr;
}


IMessage &CApprovalCommand::GetMessage()
{
    _ASSERTE(m_pMessage!=NULL);
    return *m_pMessage;
}

template <>
IAction* CCreateable<CApprovalCommand, IAction>::NewInstance()
{
    IAction *pAction = new CApprovalCommand;

    return pAction;
}


template <>
void CCreateable<CApprovalCommand, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ApprovalCommand)
DEFINE_ACTION_RELEASE_FXN(ApprovalCommand)
