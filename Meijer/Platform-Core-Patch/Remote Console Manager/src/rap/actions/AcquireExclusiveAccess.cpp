// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/AcquireExclusiveAccess.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)AcquireExclusiveAccess.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/AcquireExclusiveAccess.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 12    10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 8     3/30/05 5:05p Dm185016
 * TAR 297001
 * 
 * 7     3/22/05 2:45p Dm185016
 * UNICODE
 * 
 * 6     3/14/05 8:28p Dm185016
 * Added trace.
 * 
 * 5     3/07/05 6:53p Dm185016
 * TAR 294609.
 * TAR 294610.
 * 
 * 4     3/02/05 1:45p Dm185016
 * Added error checking for current remote connection pointer.
 * 
 * 3     2/28/05 4:06p Dm185016
 * lint
 * 
 * 2     2/07/05 7:16p Dm185016
 * TAR 292647
 * 
 * 1     1/26/05 2:18p Dm185016
 * Moved to new Repository.
// 
// 2     1/13/05 1:37p Dm185016
// Fixed compile errors
// 
// 1     1/13/05 1:28p Dm185016
// TAR 290304
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "AcquireExclusiveAccess.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "PSXMfc.h"
#include "RAPUIDefines.h"
#include "RCMKeyValue.h"
#include "verifypsx.h"
#include "RCMError.h"


CAcquireExclusiveAccess::CAcquireExclusiveAccess()
{
    CRCMActionBase::SetName(_T("CAcquireExclusiveAccess"));
}


CAcquireExclusiveAccess::~CAcquireExclusiveAccess()
{
}


HRESULT CAcquireExclusiveAccess::Execute(bool *pResult, IMessage &)
{
    *pResult = true;

    HRESULT hr = S_OK;

    PREMOTECONN pRemoteConn = rcm().GetCurrentRemoteConnection();
    if (pRemoteConn==NULL)
        return RCM_NO_REMOTE_CONNECTION_AVAILABLE;

    CRemoteConnection &currentRemoteConn = *pRemoteConn;

    CKeyValueParmList pList;
   	CKeyValue<const _TCHAR*>::Put(pList, KEY_OPERATION,  KEY_OPERATION_DISABLE_ALL_CONTEXTS);
    _variant_t varParam = (const _TCHAR*)pList;

    const CRemoteConnectionsMap &connections = rcm().GetRemoteConnections();
    CRemoteConnectionsMap::const_iterator iter = connections.begin();
    for (;iter != connections.end(); iter++)
    {
        CRemoteConnection &remoteConn = *iter->second;
        if (currentRemoteConn != remoteConn)
        {
            remoteConn.SetEnabled(false);

            ITRACE_DEVELOP(_T("Sending:  ") + IString((const _TCHAR*)pList) + 
                           _T(" to ") + IString(remoteConn.GetRemoteConnectionName()) );
            VERIFY_PSX(hr, psx().GenerateEvent(_T(""),
                                            _T(""),
                                            UI::EVENTREMOTEDATA,
                                            varParam,
                                            remoteConn.GetRemoteConnectionName()));
        }
    }

    pList.Clear();
   	CKeyValue<const _TCHAR*>::Put(pList, KEY_OPERATION, KEY_OPERATION_ACQUIRE);

    ITRACE_DEVELOP(_T("Sending:  ") + IString((const _TCHAR*)pList) + 
                    _T(" to ") + IString(currentRemoteConn.GetRemoteConnectionName()) );

    varParam = (const _TCHAR*)pList;
    VERIFY_PSX(hr, psx().GenerateEvent(_T(""),
                                       _T(""),
                                       UI::EVENTREMOTEDATA,
                                       varParam,
                                       currentRemoteConn.GetRemoteConnectionName()));

    rcm().SetExclusiveAccessGranted(true);
    rcm().SetExclusiveAccessReleaseRequired(true);

    return hr;
}


template <>
IAction* CCreateable<CAcquireExclusiveAccess, IAction>::NewInstance()
{
    IAction *pAction = new CAcquireExclusiveAccess;

    return pAction;
}


template <>
void CCreateable<CAcquireExclusiveAccess, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(AcquireExclusiveAccess)
DEFINE_ACTION_RELEASE_FXN(AcquireExclusiveAccess)
