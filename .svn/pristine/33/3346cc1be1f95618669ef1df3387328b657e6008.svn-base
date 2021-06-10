// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/NullAction.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)NullAction.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/NullAction.cpp $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     2/28/05 3:50p Dm185016
 * lint
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
// 
// 6     3/19/04 3:07p Dm185016
// Removed trace
// 
// 5     3/01/04 10:11a Dm185016
// Added Allocation/Release functions
// 
// 4     2/10/04 11:46a Dm185016
// Moved common support to base
// 
// 3     1/27/04 2:03p Dm185016
// DLL Support
// 
// 2     1/16/04 9:15a Dm185016
// Namespace support
*/
#include "StdAfx.h"
#include "NullAction.h"

using namespace STATE_MACHINE_NAMESPACE;


CNullAction::CNullAction()
{
    SetName(_T("CNullAction"));
    Comment.Value(_T("Does nothing.  Returns true always."));
}


CNullAction::~CNullAction()
{
}


bool CNullAction::IsInternal() 
{
    return true; 
}


HRESULT CNullAction::Execute(bool *pResult, IMessage &)
{
    *pResult = true;

// Below used only for testing the failure path
//  throw -1;
    
    return S_OK;
}


template <>
IAction* CCreateable<CNullAction, IAction>::NewInstance()
{
    return new CNullAction;
}


template <>
void CCreateable<CNullAction, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(NullAction)
DEFINE_ACTION_RELEASE_FXN(NullAction)
