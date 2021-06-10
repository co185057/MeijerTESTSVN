// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ConnectCommand.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ConnectCommand.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ConnectCommand.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:31p Dm185016
 * Alert button support.
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 8     1/18/05 3:48p Dm185016
// UNICODE support
// 
// 7     1/12/05 5:59p Dm185016
// Cleaned up trace log.
// 
// 6     12/22/04 11:10a Dm185016
// HH support.  Factored out remote connection data into RemoteConnection
// object.
// 
// 5     12/16/04 10:02a Dm185016
// Problem with copy ctor of iterator.
// 
// 4     12/14/04 11:25a Dm185016
// HH support.
// 
// 3     11/16/04 11:37a Dm185016
// fixed return code
// 
// 2     11/02/04 3:09p Dm185016
// 
// 1     10/29/04 9:04a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ConnectCommand.h"


CConnectCommand::CConnectCommand()
{
    CConnectCommandBase::SetName(_T("CConnectCommand"));
}


CConnectCommand::~CConnectCommand()
{
}


StringType CConnectCommand::GetOperands()
{
    return _T("");
}


template <>
IAction* CCreateable<CConnectCommand, IAction>::NewInstance()
{
    IAction *pAction = new CConnectCommand;

    return pAction;
}


template <>
void CCreateable<CConnectCommand, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ConnectCommand)
DEFINE_ACTION_RELEASE_FXN(ConnectCommand)
