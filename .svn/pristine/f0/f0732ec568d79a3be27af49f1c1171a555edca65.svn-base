// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendClientMachineNameEvent.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SendClientMachineNameEvent.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendClientMachineNameEvent.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     3/02/05 1:45p Dm185016
 * Added error checking for current remote connection pointer.
 * 
 * 2     2/28/05 4:15p Dm185016
 * lint
 * 
 * 1     2/18/05 9:43a Dm185016
 * TAR 293708
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SendClientMachineNameEvent.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMEvent.h"
#include "Message.h"
#include "RCMKeyValue.h"
#include "RCMError.h"


CSendClientMachineNameEvent::CSendClientMachineNameEvent()
{
    CRCMActionBase::SetName(_T("CSendClientMachineNameEvent"));
}


CSendClientMachineNameEvent::~CSendClientMachineNameEvent()
{
}


HRESULT CSendClientMachineNameEvent::Execute(bool *pResult, IMessage &)
{
    PREMOTECONN pRemoteConn = rcm().GetCurrentRemoteConnection();
    if (pRemoteConn==NULL)
        return RCM_NO_REMOTE_CONNECTION_AVAILABLE;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR *>::Put(parmList
                        , KEY_RCEV_OPERATION
                        , KEY_RCEV_OPERATION_SEND_CLIENT_MACHINE_NAME);
    CKeyValue<const _TCHAR *>::Put(parmList
                        , KEY_RCEV_CLIENT_MACHINE_NAME
                        , pRemoteConn->GetMachineName());

    rapmgr().FireEvent( new CRemoteConsoleEvent((const _TCHAR *)parmList) );

    *pResult = true;

    return S_OK;
}


template <>
IAction* CCreateable<CSendClientMachineNameEvent, IAction>::NewInstance()
{
    IAction *pAction = new CSendClientMachineNameEvent;

    return pAction;
}


template <>
void CCreateable<CSendClientMachineNameEvent, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SendClientMachineNameEvent)
DEFINE_ACTION_RELEASE_FXN(SendClientMachineNameEvent)
