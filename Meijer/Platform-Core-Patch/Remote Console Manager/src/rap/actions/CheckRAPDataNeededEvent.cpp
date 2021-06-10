// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/actions/CheckRAPDataNeededEvent.cpp 5     11/05/08 11:49a Sm133076 $
/*
 * @(#)CheckRAPDataNeededEvent.cpp    $Revision: 5 $ $Date: 11/05/08 11:49a $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/actions/CheckRAPDataNeededEvent.cpp $
 * 
 * 5     11/05/08 11:49a Sm133076
 * Patch G Migration
 * 
 * 15    10/17/08 7:13p Sc185102
 * SR678
 * 
 * 14    3/17/08 3:16p Sk185127
 * Made changes for SR085 - Improve Bag/Box Behavior
 * 
 * 13    1/29/08 1:11p Cs185088
 * RFC - RAP Intervention in every Training Mode Transaction
 * 
 * 13    1/23/08 10:46p cs185088
 * RFC - RAP Intervention in every Training Mode Transaction
 *
 * 12    1/22/08 4:46a pl250020
 * TAR 352409 : Added for Suspension Decline Event
 *
 * 11    10/20/06 9:50a Tn185011
 * Roll back to 244
 *
 * 9     2/03/06 5:50p Tp151000
 * tar 308562
 *
 * 8     1/31/06 5:22p Tp151000
 * 314116
 *
 * 7     6/24/05 1:38p Dm185016
 * Added capability for customer created rap-data-needed contexts.
 *
 * 5     5/02/05 2:58p Tp151000
 * TAR 299442
 *
 * 4     4/15/05 9:19a Dm185016
 * Merge from Patch B
 *
 * 3     4/14/05 10:06p Dm185016
 * TAR 298021
 *
 * 2     2/28/05 4:08p Dm185016
 * lint
 *
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
//
// 3     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
//
// 2     11/19/04 1:35p Dm185016
// Corrected value for view.
//
// 1     11/18/04 1:37p Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "CheckRAPDataNeededEvent.h"
#include "RemoteAPMgr.h"
#include "RCMKeyValue.h"
#include "LookupTable.h"
#include "Message.h"
#include "KVParmList.h"
#include "RAEvents.h"


BEGIN_KV_HANDLER_CLASS(CCheckRAPDataNeededEvent::CRDNEParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CCheckRAPDataNeededEvent::CRDNEParms, KEY_LOOKUP_TABLE_NAME, LookupTableName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CCheckRAPDataNeededEvent::CRDNEParms, KEY_VIEW, View)
    DEFINE_KV_HANDLER_MAP_ENTRY(CCheckRAPDataNeededEvent::CRDNEParms, KEY_PRIORITY, Priority)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CCheckRAPDataNeededEvent::CRDNEParms, LookupTableName)
DEFINE_KV_CSTRING(CCheckRAPDataNeededEvent::CRDNEParms, Priority)
DEFINE_KV_INT(CCheckRAPDataNeededEvent::CRDNEParms, View)
BEGIN_KV_HANDLER_CLASS_CLEAR(CCheckRAPDataNeededEvent::CRDNEParms)
    CLEAR_KV_CSTRING(LookupTableName)
    CLEAR_KV_CSTRING(Priority)
    CLEAR_KV_INT(View, DATANEEDED_NOVIEW)
END_KV_HANDLER_CLASS_CLEAR()


DWORD CCheckRAPDataNeededEvent::GetPriority()
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


CCheckRAPDataNeededEvent::CCheckRAPDataNeededEvent()
{
    CRCMActionBase::SetName(_T("CCheckRAPDataNeededEvent"));
}


CCheckRAPDataNeededEvent::~CCheckRAPDataNeededEvent()
{
}


HRESULT CCheckRAPDataNeededEvent::CheckUserRAPDataNeeded()
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
    if (hr == S_OK)
    {
        hr = m_parms.ParseKeys((const _TCHAR *)pTable->GetString(), GetTraceHandle());

        switch (GetPriority())
        {
            case RA_PRIORITY_URGENT :
                hr = S_OK;
                break;
            case RA_PRIORITY_DEFER :
            default:
                hr = S_FALSE;
                break;
        }
    }

    return hr;
}



// Get all control names from the xml
HRESULT CCheckRAPDataNeededEvent::ParseOperands()
{
    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();

    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    return m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
}


HRESULT CCheckRAPDataNeededEvent::Execute(bool *pResult, IMessage &message)
{
    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys((const _TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    *pResult = true;

    switch (m_parms.GetView())
    {
        case DATANEEDED_NOVIEW :
            _ASSERTE(FALSE);
            break;
        case DATANEEDED_CONFIRMSUSPEND:
        case DATANEEDED_PRICENOTMATCH:
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
        case DATANEEDED_CANCELLTRANSACTION:
        case DATANEEDED_ENTERACCOUNTNUMBER:
        case DATANEEDED_ENTEREXPDATE:
        case DATANEEDED_PRICEPRMT:
        case DATANEEDED_ASSISTEDTENDER:
        case DATANEEDED_ENDORSEDOCUMENT:
        case DATANEEDED_DOCUMENTNUM:
        case DATANEEDED_BAGAREABACKUP:
        case DATANEEDED_VERIFYSIGNATURE:
        case DATANEEDED_ASSISTEDITEM:
        case DATANEEDED_GIFTCARDLIMIT:
        case DATANEEDED_STARTTRANSACTION:
        case DATANEEDED_HOSTOFFLINE:
        case DATANEEDED_WLDBFAILURE:
        case DATANEEDED_STARTTRANSACTIONAM:
        case DATANEEDED_APPROVEVOIDEDITEM:
        case DATANEEDED_SUSPENSION_DECLINE:     // TAR 352409
            *pResult = false;
            break;

        // Delayed intervention that
        case DATANEEDED_STARTTRAININGTRANS:
        case DATANEEDED_PRICEPROMPT:
        case DATANEEDED_CLOSELANE:
        case DATANEEDED_MOREDATA:
        case DATANEEDED_DEVICEERROR:
        case DATANEEDED_CHECKONLANE:
		case DATANEEDED_CHECKONLANERAP:
		case DATANEEDED_MAXCOUPONLIMIT:					//SR678
        case DATANEEDED_MAXCOUPONLIMITFINALIZATION:		//SR678
            *pResult = (GetPriority() == RA_PRIORITY_URGENT);
            break;
        case DATANEEDED_DONTBAGTHRESHOLDEXCEEDED:
        case DATANEEDED_BAGITEMTIMEREXPIRED:
        case DATANEEDED_DONEREMOVINGTHRESHOLDEXCEEDED:
        case DATANEEDED_WALKAWAYTIMEREXPIRED:
        case DATANEEDED_NEWITEMENTRY:
        case DATANEEDED_MISMATCHWEIGHT:
        case DATANEEDED_UNEXPECTEDINCREASE:
		case DATANEEDED_BAGBOXNOTVALID: //SR085
        case DATANEEDED_BAGBOXTHRESHOLDEXCEEDED:
        case DATANEEDED_UNEXPECTEDDECREASE:
        case DATANEEDED_UNDEACTIVATEDITEM:
        case DATANEEDED_TAKECHANGETIMEREXPIRED:
        case DATANEEDED_THANKYOUEXPIRED:
        case DATANEEDED_INSERTCOUPONTIMEREXPIRED:
        case DATANEEDED_COLLECTGIFTCARD:
            break;
        default:
            hr = CheckUserRAPDataNeeded();
            *pResult = (hr == S_OK);
            break;
    }

    return hr;
}


template <>
IAction* CCreateable<CCheckRAPDataNeededEvent, IAction>::NewInstance()
{
    IAction *pAction = new CCheckRAPDataNeededEvent;

    return pAction;
}


template <>
void CCreateable<CCheckRAPDataNeededEvent, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(CheckRAPDataNeededEvent)
DEFINE_ACTION_RELEASE_FXN(CheckRAPDataNeededEvent)
