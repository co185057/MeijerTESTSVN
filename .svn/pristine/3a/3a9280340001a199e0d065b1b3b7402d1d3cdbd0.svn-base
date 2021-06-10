// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ConnectCommandComplete.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ConnectCommandComplete.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ConnectCommandComplete.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/15/05 3:44p Dm185016
 * TAR 295676 - AcceptConnection is now handled by a separate thread to
 * prevent hanging the state machine thread.
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ConnectCommandComplete.h"
#include "RCManager.h"


CConnectCommandComplete::CConnectCommandComplete()
{
}


CConnectCommandComplete::~CConnectCommandComplete()
{
}


HRESULT CConnectCommandComplete::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr          = S_OK;

    CRemoteConnection *pRConn = CRemoteConnection::CreateRemoteConnection(&message);

    rcm().AddRemoteConnection(pRConn);

    *pResult = true;

    return hr;
}


template <>
IAction* CCreateable<CConnectCommandComplete, IAction>::NewInstance()
{
    IAction *pAction = new CConnectCommandComplete;

    return pAction;
}


template <>
void CCreateable<CConnectCommandComplete, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ConnectCommandComplete)
DEFINE_ACTION_RELEASE_FXN(ConnectCommandComplete)
