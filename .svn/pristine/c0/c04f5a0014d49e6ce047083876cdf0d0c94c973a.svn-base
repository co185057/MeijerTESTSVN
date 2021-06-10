// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendInterventionEventBase.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SendInterventionEventBase.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendInterventionEventBase.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     3/22/05 2:47p Dm185016
 * UNICODE
 * 
 * 3     3/07/05 6:55p Dm185016
 * Green intervention problem per Dan.
 * 
 * 2     2/21/05 7:57p Dm185016
 * Added error check and tracing.
 * 
 * 1     2/17/05 5:55p Dm185016
 * Alert buttons functionality
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SendInterventionEventBase.h"
#include "RCManager.h"
#include "RCMKeyValue.h"
#include "RCMLaneXML.h"
#include "RCMError.h"
#include "RAPUIDefines.h"
#include "PSXMfc.h"
#include "KVParmList.h"
#include "verifypsx.h"


HRESULT CSendInterventionEventBase::SendEvent()
{
    HRESULT hr = S_OK;

    CKeyValueParmList parmlist;
    _variant_t varParam;

    if (IsOperationRequired())
   	    CKeyValue<const _TCHAR*>::Put(parmlist, KEY_CT_OPERATION,  GetOperation());
   	CKeyValue<const _TCHAR*>::Put(parmlist, KEY_CT_INTERVENTION_TYPE,  GetInterventionType());

    SYSTEMTIME time;
    ::GetSystemTime(&time);

    CKeyValue<int>::Put(parmlist, KEY_CT_INTERVENTION_TIME, (int)time.wYear);
    CKeyValue<int>::Put(parmlist, (int)time.wMonth);
    CKeyValue<int>::Put(parmlist, (int)time.wDay);
    CKeyValue<int>::Put(parmlist, (int)time.wHour);
    CKeyValue<int>::Put(parmlist, (int)time.wMinute);
    CKeyValue<int>::Put(parmlist, (int)time.wSecond);

    ITRACE_DEVELOP(_T("Event:  ") + IString((const _TCHAR*)parmlist));

    // Enable the contexts of other remote connections.
    const CRemoteConnectionsMap &connections = rcm().GetRemoteConnections();
    CRemoteConnectionsMap::const_iterator iter = connections.begin();
    for (;iter != connections.end(); iter++)
    {
        PREMOTECONN pRConn = iter->second;

        varParam = (const _TCHAR*)parmlist;
        VERIFY_PSX(hr, psx().GenerateEvent(_T(""),
                                        _T(""),
                                        UI::EVENTREMOTEDATA,
                                        varParam,
                                        pRConn->GetRemoteConnectionName()));
    }

    return hr;
}
