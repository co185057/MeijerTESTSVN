// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/PopState.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)PopState.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/PopState.cpp $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     2/28/05 3:50p Dm185016
 * lint
 * 
 * 2     2/10/05 5:48p Dm185016
 * TAR 292915
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
// 
// 1     3/31/04 10:14a Dm185016
// New for push/pop state mechanism
*/
#include "StdAfx.h"
#include "PopState.h"
#include "SMError.h"
#include "InternalOps.h"

using namespace STATE_MACHINE_NAMESPACE;


//-----------------------------------------------------------------------------
// No-arg constructor
//-----------------------------------------------------------------------------
CPopState::CPopState()
{
    SetName(_T("CPopState"));
}


CPopState::~CPopState()
{
}


bool CPopState::IsInternal() 
{
    return true; 
}

    
//-----------------------------------------------------------------------------
// Called by the State Machine to perform timer related operations such as 
// Start and Stop.
//-----------------------------------------------------------------------------
HRESULT CPopState::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = S_OK;

    stateMachine().PopState();

    *pResult = true;

    return hr;
}


//-----------------------------------------------------------------------------
// Template specialization of a creation function for CPopState.  This
// will be used by the State Machine for creating instances of that class.
//-----------------------------------------------------------------------------
template <>
IAction* CCreateable<CPopState, IAction>::NewInstance()
{
      IAction *pAction = new CPopState;

    return pAction;
}


template <>
void CCreateable<CPopState, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(PopState)
DEFINE_ACTION_RELEASE_FXN(PopState)
