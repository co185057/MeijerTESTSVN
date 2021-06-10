// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ReleaseExclusiveAccessBase.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)ReleaseExclusiveAccessBase.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ReleaseExclusiveAccessBase.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 9     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 5     3/30/05 5:05p Dm185016
 * TAR 297001
 * 
 * 4     3/22/05 2:46p Dm185016
 * 
 * 3     3/07/05 6:53p Dm185016
 * TAR 294609.
 * TAR 294610.
 * 
 * 2     2/18/05 4:34p Dm185016
 * 
 * 
 * 1     2/18/05 4:27p Dm185016
 * TAR 293807
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ReleaseExclusiveAccessBase.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "PSXMfc.h"
#include "RAPUIDefines.h"
#include "RCMKeyValue.h"
#include "verifypsx.h"


CReleaseExclusiveAccessBase::CReleaseExclusiveAccessBase()
{
}


CReleaseExclusiveAccessBase::~CReleaseExclusiveAccessBase()
{
}


HRESULT CReleaseExclusiveAccessBase::ReleaseCurrentConnection()
{
    HRESULT hr = S_OK;

    // Get the current remote connection
    PREMOTECONN pCurrentRemoteConn = rcm().GetCurrentRemoteConnection();
    if (pCurrentRemoteConn == NULL)
    {
        ITRACE_DEVELOP(__FUNCTION__ ".  Remote connection null");

        return S_FALSE;
    }

    CRemoteConnection &currentRemoteConn = *pCurrentRemoteConn;

    CKeyValueParmList pList;
    _variant_t varParam;

    // If the release is required and the current remote connection is not null
    // then send the event to release
   	CKeyValue<const _TCHAR*>::Put(pList, KEY_OPERATION,  KEY_OPERATION_RELEASE);

    varParam = (const _TCHAR*)pList;
    VERIFY_PSX(hr, psx().GenerateEvent(_T(""),
                                    _T(""),
                                    UI::EVENTREMOTEDATA,
                                    varParam,
                                    currentRemoteConn.GetRemoteConnectionName()));

    rcm().SetExclusiveAccessGranted(false);

    pList.Clear();

    return hr;
}


HRESULT CReleaseExclusiveAccessBase::ReleaseOtherConnections()
{
    HRESULT hr = S_OK;

    CKeyValueParmList pList;
    CKeyValue<const _TCHAR*>::Put(pList, KEY_OPERATION, KEY_OPERATION_ENABLE_ALL_CONTEXTS);

    _variant_t varParam = (const _TCHAR*)pList;

    PREMOTECONN pCurrentRemoteConn = rcm().GetCurrentRemoteConnection();

    // Enable the contexts of other remote connections.
    const CRemoteConnectionsMap &connections = rcm().GetRemoteConnections();
    CRemoteConnectionsMap::const_iterator iter = connections.begin();
    for (;iter != connections.end(); iter++)
    {
        PREMOTECONN pRemoteConn = iter->second;
        if (pCurrentRemoteConn != pRemoteConn)
        {
            CRemoteConnection &remoteConn = *pRemoteConn;

            remoteConn.SetEnabled(true);

            ITRACE_DEVELOP(_T("Sending:  ") + IString((const _TCHAR*)pList) + 
                           _T(" to ") + IString(remoteConn.GetRemoteConnectionName()) );
            VERIFY_PSX(hr, psx().GenerateEvent(_T(""),
                                            _T(""),
                                            UI::EVENTREMOTEDATA,
                                            varParam,
                                            remoteConn.GetRemoteConnectionName()));
        }
    }

    return hr;
}
