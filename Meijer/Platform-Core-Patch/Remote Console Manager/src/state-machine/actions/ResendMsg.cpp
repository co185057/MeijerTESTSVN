// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/ResendMsg.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)ResendMsg.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/ResendMsg.cpp $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
// 
// 7     10/29/04 9:26a Dm185016
// Removed reference to MessageObject.
// 
// 5     3/19/04 3:07p Dm185016
// Removed trace
// 
// 4     3/01/04 10:11a Dm185016
// Added Allocation/Release functions
// 
// 3     2/24/04 3:38p Dm185016
// Added msg to head of queue to give it priority
// 
// 2     2/10/04 11:46a Dm185016
// Moved common support to base
// 
// 1     1/27/04 2:04p Dm185016
// New internal action to resend the message to the state machine.
*/
#include "StdAfx.h"
#include "ResendMsg.h"

using namespace STATE_MACHINE_NAMESPACE;


CResendMsg::CResendMsg()
{
    SetName(_T("CResendMsg"));
    Comment.Value(_T("Resends the received message to the SM."));
}


CResendMsg::~CResendMsg()
{
}


bool CResendMsg::IsInternal() 
{
    return true; 
}


HRESULT CResendMsg::Execute(bool *pResult, IMessage &message)
{
    *pResult = true;

    IMessage &newMsg = stateMachine().MakeAsynchCopy(message);

    // Enqueue the message back to the state machine
    stateMachine().PostMsg(newMsg, true);

    return S_OK;
}


template <>
IAction* CCreateable<CResendMsg, IAction>::NewInstance()
{
    return new CResendMsg;
}


template <>
void CCreateable<CResendMsg, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ResendMsg)
DEFINE_ACTION_RELEASE_FXN(ResendMsg)
