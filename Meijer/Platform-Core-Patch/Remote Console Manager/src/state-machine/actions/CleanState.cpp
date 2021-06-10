// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/CleanState.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)CleanState.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/CleanState.cpp $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     5/31/05 11:39a Dm185016
 * TAR 301203
*/
#include "StdAfx.h"
#include "CleanState.h"
#include "SMError.h"
#include "InternalOps.h"

using namespace STATE_MACHINE_NAMESPACE;


//-----------------------------------------------------------------------------
// No-arg constructor
//-----------------------------------------------------------------------------
CCleanState::CCleanState()
{
    SetName(_T("CCleanState"));
}


CCleanState::~CCleanState()
{
}


bool CCleanState::IsInternal() 
{
    return true; 
}

    
//-----------------------------------------------------------------------------
// Called by the State Machine to clean up the state stack and return to normal.
//-----------------------------------------------------------------------------
HRESULT CCleanState::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = S_OK;

    stateMachine().CleanState();

    *pResult = true;

    return hr;
}


//-----------------------------------------------------------------------------
// Template specialization of a creation function for CCleanState.  This
// will be used by the State Machine for creating instances of that class.
//-----------------------------------------------------------------------------
template <>
IAction* CCreateable<CCleanState, IAction>::NewInstance()
{
      IAction *pAction = new CCleanState;

    return pAction;
}


template <>
void CCreateable<CCleanState, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(CleanState)
DEFINE_ACTION_RELEASE_FXN(CleanState)
