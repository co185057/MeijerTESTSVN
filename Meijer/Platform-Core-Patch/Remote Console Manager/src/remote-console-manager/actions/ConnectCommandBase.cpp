// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ConnectCommandBase.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ConnectCommandBase.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ConnectCommandBase.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     3/15/05 3:37p Dm185016
 * TAR 295676 - AcceptConnection is now handled by a separate thread to
 * prevent hanging the state machine thread.
 * 
 * 1     2/25/05 5:41p Tk124825
 * Initial Version
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ConnectCommandBase.h"
#include "RCManager.h"
#include "KVParmList.h"
#include "PSXMfc.h"
#include "verifypsx.h"
#include "PSXConnectionThread.h"


CConnectCommandBase::CConnectCommandBase()
{
}


CConnectCommandBase::~CConnectCommandBase()
{
}


HRESULT CConnectCommandBase::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr          = S_OK;

    CPSXConnectionThread *pThread = new CPSXConnectionThread(GetTraceHandle());

    pThread->SetParmlist(GetOperands());

    pThread->Start(message);

    *pResult = true;

    SetSynchronousReturnCode(message, hr);

    return hr;
}
