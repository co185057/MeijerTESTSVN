// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/actions/SaveAsDelayedIntervention.cpp 6     11/06/08 6:53p Sa250050 $
/*
 * @(#)SaveAsDelayedIntervention.cpp    $Revision: 6 $ $Date: 11/06/08 6:53p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/actions/SaveAsDelayedIntervention.cpp $
 * 
 * 6     11/06/08 6:53p Sa250050
 * patch fix
 * 
 * 5     11/05/08 11:56a Sm133076
 * Patch G Migration
 * 
 * 4     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 12    10/17/08 7:15p Sc185102
 * SR678
 * 
 * 11    1/29/08 1:25p Cs185088
 * RFC - RAP Intervention in every Training Mode Transaction
 * 
 * 11    1/23/08 10:58p cs185088
 * RCF - RAP Intervention in every Training Mode Transaction
 *
 * 10    1/22/08 5:01a pl250020
 * TAR 352409 : Added for new Suspension Decline Event
 *
 * 9     10/20/06 9:50a Tn185011
 * Roll back to 244
 *
 * 7     1/31/06 5:22p Tp151000
 * 314116
 *
 * 6     6/24/05 1:39p Dm185016
 * Added capability for customer created rap-data-needed contexts.
 *
 * 3     3/17/05 2:41p Dm185016
 * TAR 296081
 *
 * 2     1/27/05 2:37p Dm185016
 * Added support for 4.0 dialogs.
 *
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
//
// 7     1/13/05 1:29p Dm185016
// Removed duplicate include.
//
// 6     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
//
// 5     12/22/04 11:01a Dm185016
// TAR 288934 - Added priority key from SCOTAPP
//
// 4     12/02/04 3:38p Dm185016
// Found more valid base views.
//
// 3     11/30/04 4:21p Dm185016
// Added handling for cancel generic transactions.
//
// 2     11/19/04 1:36p Dm185016
// Changed some placeholder values to correct ones.
//
// 1     11/18/04 1:37p Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SaveAsDelayedIntervention.h"
#include "RemoteAPMgr.h"
#include "RCMKeyValue.h"
#include "RAEvents.h"
#include "KVParmList.h"
#include "LookupTable.h"
#include "Message.h"
#include "RCMMessages.h"
#include "RCMError.h"


BEGIN_KV_HANDLER_CLASS(CSaveAsDelayedIntervention::SADIParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSaveAsDelayedIntervention::SADIParms, KEY_LOOKUP_TABLE_NAME, LookupTableName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSaveAsDelayedIntervention::SADIParms, KEY_VIEW, View)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSaveAsDelayedIntervention::SADIParms, KEY_PRIORITY, Priority)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSaveAsDelayedIntervention::SADIParms, KEY_MSG_NAME, MessageName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSaveAsDelayedIntervention::SADIParms, KEY_EV_VIEW_TEXT, ViewText)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSaveAsDelayedIntervention::SADIParms, KEY_RAP_DATA_NEEDED_SECTION, Section)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CSaveAsDelayedIntervention::SADIParms, LookupTableName)
DEFINE_KV_CSTRING(CSaveAsDelayedIntervention::SADIParms, Priority)
DEFINE_KV_CSTRING(CSaveAsDelayedIntervention::SADIParms, MessageName)
DEFINE_KV_CSTRING(CSaveAsDelayedIntervention::SADIParms, ViewText)
DEFINE_KV_CSTRING(CSaveAsDelayedIntervention::SADIParms, Section)
DEFINE_KV_INT(CSaveAsDelayedIntervention::SADIParms, View)
BEGIN_KV_HANDLER_CLASS_CLEAR(CSaveAsDelayedIntervention::SADIParms)
    CLEAR_KV_CSTRING(LookupTableName)
    CLEAR_KV_CSTRING(Priority)
    CLEAR_KV_CSTRING(MessageName)
    CLEAR_KV_CSTRING(ViewText)
    CLEAR_KV_CSTRING(Section)
    CLEAR_KV_INT(View, DATANEEDED_NOVIEW)
END_KV_HANDLER_CLASS_CLEAR()


CSaveAsDelayedIntervention::CSaveAsDelayedIntervention()
{
    CRCMActionBase::SetName(_T("CSaveAsDelayedIntervention"));
}


CSaveAsDelayedIntervention::~CSaveAsDelayedIntervention()
{
}


DWORD CSaveAsDelayedIntervention::GetPriority()
{
    DWORD dwPriority;
    if (m_parms.GetPriority() == KEY_EV_PRIORITY_URGENT)
        dwPriority = RA_PRIORITY_URGENT;
    else if (m_parms.GetPriority() == KEY_EV_PRIORITY_DELAYED)
        dwPriority = RA_PRIORITY_DEFER;
    else
        dwPriority = RA_PRIORITY_DEFER;

    return dwPriority;
}


HRESULT CSaveAsDelayedIntervention::GetUserMessage(IMessage **ppMsg,  IMessage &originalMessage)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    ILookupTable *pTable = NULL;
    hr = stateMachine().GetLookupTable(&pTable, m_parms.GetLookupTableName());
    if (hr != S_OK)
        return STATEM_UNDEFINED_LOOKUPTABLE_ID;
    _ASSERTE(pTable!=NULL);

    hr = pTable->Lookup(m_parms.GetView());
    if (hr != S_OK)
        return hr;

    hr = m_parms.ParseKeys((const _TCHAR *)pTable->GetString(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    *ppMsg = &stateMachine().MakeAsynchCopyEx(originalMessage, m_parms.GetMessageName());

    return hr;
}


// Get all control names from the xml
HRESULT CSaveAsDelayedIntervention::ParseOperands()
{
    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();

    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    return m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
}


HRESULT CSaveAsDelayedIntervention::Execute(bool *pResult, IMessage &message)
{
    IMODTRACE_DEVELOP();

    *pResult = true;

    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys((const _TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    if (!m_parms.GetViewText().IsEmpty())
        m_parms.ParseKeys(m_parms.GetViewText(), GetTraceHandle());

    DWORD dwPriority = GetPriority();

    IMessage *pNewMsg = NULL;

    switch (m_parms.GetView())
    {
        case DATANEEDED_APPROVEVOIDEDITEM:  pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_VOID_EVENT);  break;
        case DATANEEDED_CONFIRMSUSPEND:     pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_CONFIRM_SUSPEND_EVENT);  break;
        case DATANEEDED_PRICENOTMATCH:      pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_PRICE_NOT_MATCH_EVENT);  break;
        case DATANEEDED_PRICEPROMPT:        pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_PRICE_REQUIRED_EVENT);  break;
        case DATANEEDED_CHECKONLANE:		pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_CHECK_ON_LANE_EVENT);  break;
	    case DATANEEDED_CHECKONLANERAP:		pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_CHECK_ON_LANE_RAP_EVENT);  break;
        case DATANEEDED_STARTTRAININGTRANS: pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_TRAINING_MODE_TRANSACTION_EVENT);  break;
		case DATANEEDED_MAXCOUPONLIMIT:		pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_COUPON_LIMIT_EVENT);  break;
		case DATANEEDED_MAXCOUPONLIMITFINALIZATION: pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_COUPON_LIMIT_FINALIZATION_EVENT);  break;
		case DATANEEDED_DEVICEERROR:        pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_DEVICE_ERROR_EVENT);  break;
        case DATANEEDED_PRICEPRMT:          pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_PRICE_REQUIRED_EVENT);  break;
        case DATANEEDED_VERIFYSIGNATURE:    pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_VERIFY_SIGNATURE_EVENT);  break;
        case DATANEEDED_STARTTRANSACTION:   pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_START_XACT);  break;
        case DATANEEDED_STARTTRANSACTIONAM: pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_START_XACT_ENTER_AM);  break;
        case DATANEEDED_ASSISTEDTENDER:     pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_ASSISTED_TENDER_EVENT);  break;
        case DATANEEDED_BAGAREABACKUP:      pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_BAG_AREA_BACKUP_EVENT);  break;
        case DATANEEDED_ENDORSEDOCUMENT:    pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_ASSISTED_ENDORSEMENT_EVENT);  break;
        case DATANEEDED_SAFETYCOVEROPEN:    pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_SAFETY_COVER_OPEN_EVENT);  break;
        case DATANEEDED_CLOSELANE:          pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_CLOSE_LANE_EVENT);  break;
        case DATANEEDED_WLDBFAILURE:        pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_WLDB_FAILURE_EVENT);  break;
        case DATANEEDED_MULTIPACKITEM:      pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_MULTIPACKITEM);  break;
        case DATANEEDED_OUTOFSYNC_STOP:     pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_OUTOFSYNC_STOP);  break;
        case DATANEEDED_OUTOFSYNC_ASSIST:   pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_OUTOFSYNC_ASSIST);  break;
        case DATANEEDED_CANCELLTRANSACTION: pNewMsg = &HandleCancelTransaction(message);  break;
        case DATANEEDED_SUSPENSION_DECLINE:	pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_SUSPENSION_DECLINE_EVENT);  break;	// TAR 352409
        case DATANEEDED_COUPONTHRESHOLDEXCEEDED:
        case DATANEEDED_COUPONDOLLARMAX:
        case DATANEEDED_QUANTITY :
        case DATANEEDED_ENTERITEM:
        case DATANEEDED_ENTERCOUPON:
        case DATANEEDED_MEASUREMENT:
        case DATANEEDED_ASSEMBLYFEE:
        case DATANEEDED_VERIFYIDENTITY:
        case DATANEEDED_AUTHORIZATIONNUM:
        case DATANEEDED_CSASUSPEND:
        case DATANEEDED_DLIDNUMBER:
        case DATANEEDED_ENTERACCOUNTNUMBER:
        case DATANEEDED_ENTEREXPDATE:
        case DATANEEDED_DOCUMENTNUM:
        case DATANEEDED_ASSISTEDITEM:
        case DATANEEDED_GIFTCARDLIMIT:
        case DATANEEDED_HOSTOFFLINE:
            ITRACE_DEVELOP(_T("View type not handled:  ") + IString(m_parms.GetView()));
            pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_RAPDATANEEDED_NOT_HANDLED_EVENT);
            break;
        default :
            hr = GetUserMessage(&pNewMsg, message);
            break;
    }

    if (pNewMsg == NULL)
        return STATEM_INVALID_PARAMETER;

    rapmgr().SetNextDelayedIntervention(pNewMsg, dwPriority);

    return S_OK;
}


IMessage & CSaveAsDelayedIntervention::HandleCancelTransaction(IMessage &message)
{
    if (m_parms.GetSection() == KEY_RAP_DATA_NEEDED_SECTION_CANCELXACTGENERIC)
        return stateMachine().MakeAsynchCopyEx(message, MSG_CANCEL_XACT_GENERIC);
    else
        return stateMachine().MakeAsynchCopyEx(message, MSG_CANCEL_XACT);
}


template <>
IAction* CCreateable<CSaveAsDelayedIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CSaveAsDelayedIntervention;

    return pAction;
}


template <>
void CCreateable<CSaveAsDelayedIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SaveAsDelayedIntervention)
DEFINE_ACTION_RELEASE_FXN(SaveAsDelayedIntervention)
