// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/PushState.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)PushState.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/PushState.cpp $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 4     3/22/05 2:50p Dm185016
 * UNICODE
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
// 3     3/31/04 10:16a Dm185016
// Added mechanism
// 
// 2     3/25/04 5:13p Dm185016
// Added ability to push the current state
// 
// 1     3/25/04 3:19p Dm185016
// New action to push the state
*/
#include "StdAfx.h"
#include "PushState.h"
#include "State.h"
#include "SMError.h"
#include "InternalOps.h"

using namespace STATE_MACHINE_NAMESPACE;


//-----------------------------------------------------------------------------
// No-arg constructor
//-----------------------------------------------------------------------------
CPushState::CPushState()
{
    SetName(_T("CPushState"));
}


CPushState::~CPushState()
{
}


bool CPushState::IsInternal() 
{
    return true; 
}


//-----------------------------------------------------------------------------
// Examines the operands from the XML, which is expected to have the form:
//
//  <action>
//      <action-name>push-state</action-name>
//      <parameter>
//          <value>state-name-on-return</value>
//      </parameter>
//
//      ...
//
//  </action>
// The first <parameter> tag specifies an optional state to return to
// when a corresponding pop-state operation is executed
//-----------------------------------------------------------------------------
HRESULT CPushState::ParseOperands()
{ 
    HRESULT hr = S_OK;
    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter != parms.end())
    {
        StringType sTimerName = *iter;
        ITRACE_DEVELOP(_T("state name=") + IString(sStateName.c_str()));
        //hr = stateMachine().GetTimer(&m_pTimer, sTimerName.c_str());
        //if (hr != S_OK)
        //    return hr;

        m_bUseCurrentState = false;
    }

    return hr;
}

    
//-----------------------------------------------------------------------------
// Called by the State Machine to perform timer related operations such as 
// Start and Stop.
//-----------------------------------------------------------------------------
HRESULT CPushState::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = S_OK;

    m_bUseCurrentState = true;

    hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    ITRACE_DEVELOP(m_bUseCurrentState ? _T("Pushing current state!") 
                                      : _T("Pushing state:  " + IString(sStateName.c_str())));

    IState *pState;    
    if (m_bUseCurrentState)
        hr = stateMachine().GetCurrentState(&pState);
    else
        hr = stateMachine().GetState(&pState, sStateName.c_str());

    if (hr != S_OK)
        return hr;

    stateMachine().PushState(pState->GetId());

    *pResult = (hr == S_OK);

    return hr;
}


//-----------------------------------------------------------------------------
// Template specialization of a creation function for CPushState.  This
// will be used by the State Machine for creating instances of that class.
//-----------------------------------------------------------------------------
template <>
IAction* CCreateable<CPushState, IAction>::NewInstance()
{
      IAction *pAction = new CPushState;

    return pAction;
}


template <>
void CCreateable<CPushState, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(PushState)
DEFINE_ACTION_RELEASE_FXN(PushState)
