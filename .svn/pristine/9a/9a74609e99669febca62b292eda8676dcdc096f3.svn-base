// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/AddOtherEventCommand.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)AddOtherEventCommand.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/AddOtherEventCommand.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/11/05 2:39p Dm185016
 * 
 * 2     2/28/05 4:31p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 6     1/13/05 1:40p Dm185016
// Cleaned up trace.
// 
// 5     1/12/05 5:58p Dm185016
// Cleaned up trace log.
// 
// 4     12/16/04 10:00a Dm185016
// Support to clear receipt in Attract and suspend transaction.
// 
// 3     11/18/04 1:41p Dm185016
// More RAP related COMMANDs
// 
// 2     11/16/04 11:35a Dm185016
// Missing remote commands
// 
// 1     10/29/04 9:04a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "AddOtherEventCommand.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "RCMError.h"
#include "KVParmList.h"

BEGIN_KV_HANDLER_MAP(CAddOtherEventCommand)
    DEFINE_KV_HANDLER_MAP_ENTRY(CAddOtherEventCommand, KEY_EV_DESCRIPTION, OnDescription)
END_KV_HANDLER_MAP()

BEGIN_KEY_MSG_MAP(CAddOtherEventCommand)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_ADD_OTHER_EVENT_MEDIA,                MSG_MEDIA_EVENT)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_ADD_OTHER_EVENT_ENABLE_NUM_ITEMS,     MSG_ENABLE_NUM_ITEMS_EVENT)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_ADD_OTHER_EVENT_DISABLE_NUM_ITEMS,    MSG_DISABLE_NUM_ITEMS_EVENT)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_ADD_OTHER_EVENT_START_XACT_ENTER_AM,  MSG_START_XACT_ENTER_AM)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_ADD_OTHER_EVENT_START_XACT,           MSG_START_XACT)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_ADD_OTHER_EVENT_UIINITIALIZE,         MSG_INITIALIZE_UI)
    KEY_MSG_MAP_ENTRY(KEY_EV_TYPE_ADD_OTHER_EVENT_ASSISTMODESIZETOFULLSCREEEN,  MSG_AM_SIZE_TO_FULLSCREEEN)
END_KEY_MSG_MAP()


CAddOtherEventCommand::CAddOtherEventCommand()
    : m_pMessage(NULL)
    , m_hr(S_OK)
{
    CRCMActionBase::SetName(_T("CAddOtherEventCommand"));
}


CAddOtherEventCommand::~CAddOtherEventCommand()
{
    m_pMessage = NULL;
}


DEFINE_KV_HANDLER(CAddOtherEventCommand, OnDescription, keys)
{
    m_hr = S_OK;

    IString sDescription = CKeyValue<const _TCHAR *>::Get(keys, _T(""));

    stateMachine().SetData(GetMessage(), (const _TCHAR *)sDescription, sDescription.size());

    if (sDescription.startsWith(KEY_EV_TYPE_ADD_OTHER_EVENT_MEDIA))
        m_hr = PostKeyMsg(this, GetMessage(), KEY_EV_TYPE_ADD_OTHER_EVENT_MEDIA);
    else if (sDescription.startsWith(KEY_EV_TYPE_ADD_OTHER_EVENT_ENABLE_NUM_ITEMS))
        m_hr = PostKeyMsg(this, GetMessage(), KEY_EV_TYPE_ADD_OTHER_EVENT_ENABLE_NUM_ITEMS);
    else if (sDescription.startsWith(KEY_EV_TYPE_ADD_OTHER_EVENT_DISABLE_NUM_ITEMS))
        m_hr = PostKeyMsg(this, GetMessage(), KEY_EV_TYPE_ADD_OTHER_EVENT_DISABLE_NUM_ITEMS);
    else if (sDescription.startsWith(KEY_EV_TYPE_ADD_OTHER_EVENT_START_XACT_ENTER_AM))
        m_hr = PostKeyMsg(this, GetMessage(), KEY_EV_TYPE_ADD_OTHER_EVENT_START_XACT_ENTER_AM);
    else if (sDescription.startsWith(KEY_EV_TYPE_ADD_OTHER_EVENT_START_XACT))
        m_hr = PostKeyMsg(this, GetMessage(), KEY_EV_TYPE_ADD_OTHER_EVENT_START_XACT);
    else if (sDescription.startsWith(KEY_EV_TYPE_ADD_OTHER_EVENT_UIINITIALIZE))
        m_hr = PostKeyMsg(this, GetMessage(), KEY_EV_TYPE_ADD_OTHER_EVENT_UIINITIALIZE);
    else if (sDescription.startsWith(KEY_EV_TYPE_ADD_OTHER_EVENT_ASSISTMODETOGGLETOMAIN))
        ITRACE_INFORMATIONAL(_T("API event support deprecated.  Do not use"));
    else if (sDescription.startsWith(KEY_EV_TYPE_ADD_OTHER_EVENT_ASSISTMODETOGGLETOMAINEXIT))
        ITRACE_INFORMATIONAL(_T("API event support deprecated.  Do not use"));
    else if (sDescription.startsWith(KEY_EV_TYPE_ADD_OTHER_EVENT_ASSISTMODESIZETOFULLSCREEEN))
        m_hr = PostKeyMsg(this, GetMessage(), KEY_EV_TYPE_ADD_OTHER_EVENT_ASSISTMODESIZETOFULLSCREEEN);
    else if (sDescription.startsWith(KEY_EV_TYPE_ADD_OTHER_EVENT_ASSISTMODESIZETOPARTIALSCREEN))
        ITRACE_INFORMATIONAL(_T("API event support deprecated.  Do not use"));
    else if (sDescription.startsWith(KEY_EV_TYPE_ADD_OTHER_EVENT_ASSISTMODESIZERESTORE))
        ITRACE_INFORMATIONAL(_T("API event support deprecated.  Do not use"));
    else if (sDescription.startsWith(KEY_EV_TYPE_ADD_OTHER_EVENT_ASSISTMODETOGGLETOPARTIAL))
        ITRACE_INFORMATIONAL(_T("API event support deprecated.  Do not use"));
    else if (sDescription.startsWith(KEY_EV_TYPE_ADD_OTHER_EVENT_ASSISTMODEREFRESHLANESTATUS))
        ITRACE_INFORMATIONAL(_T("API event support deprecated.  Do not use"));
    else if (sDescription.startsWith(KEY_EV_TYPE_ADD_OTHER_EVENT_RAPINITIATETRANSACTIONASSISTMODE))
        ITRACE_INFORMATIONAL(_T("API event support deprecated.  Do not use"));
    else if (sDescription.startsWith(KEY_EV_TYPE_ADD_OTHER_EVENT_RAPINITIATETRANSACTIONDATANEEDED))
        ITRACE_INFORMATIONAL(_T("API event support deprecated.  Do not use"));
    else
        m_hr = RCM_API_FAILURE_NOT_IMPLEMENTED;

}


HRESULT CAddOtherEventCommand::Execute(bool *pResult, IMessage &message)
{
    m_pMessage = &message;

    m_hr = S_OK;

    HRESULT hr = ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    *pResult = true;

    return m_hr;
}


HRESULT ProcessMedia(const StringType & /* sDescription */ )
{
    HRESULT hr = S_OK;

    return hr;
}


IMessage &CAddOtherEventCommand::GetMessage()
{
    _ASSERTE(m_pMessage!=NULL);
    return *m_pMessage;
}

template <>
IAction* CCreateable<CAddOtherEventCommand, IAction>::NewInstance()
{
    IAction *pAction = new CAddOtherEventCommand;

    return pAction;
}


template <>
void CCreateable<CAddOtherEventCommand, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(AddOtherEventCommand)
DEFINE_ACTION_RELEASE_FXN(AddOtherEventCommand)
