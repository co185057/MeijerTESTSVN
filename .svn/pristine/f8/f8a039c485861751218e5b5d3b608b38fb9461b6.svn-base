// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/GotoState.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)GotoState.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/GotoState.cpp $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 6     4/12/05 2:49p Dm185016
 * 
 * 5     4/12/05 11:44a Dm185016
 * TAR 296771
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
// 2     11/02/04 3:20p Dm185016
// More updates
// 
// 1     10/29/04 9:27a Dm185016
*/
#include "StdAfx.h"
#include "GotoState.h"
#include "State.h"
#include "SMError.h"
#include "InternalOps.h"
#include "Variable.h"
#include "StateMachineKeyValue.h"

using namespace STATE_MACHINE_NAMESPACE;


BEGIN_KV_HANDLER_CLASS(CGotoState::GTSParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CGotoState::GTSParms, KEY_STATE_NAME, StateName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CGotoState::GTSParms, KEY_STATE_ID, StateId)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CGotoState::GTSParms, StateName)
DEFINE_KV_CSTRING(CGotoState::GTSParms, StateId)
BEGIN_KV_HANDLER_CLASS_CLEAR(CGotoState::GTSParms)
    CLEAR_KV_CSTRING(StateName)
    CLEAR_KV_CSTRING(StateId)
END_KV_HANDLER_CLASS_CLEAR()


//-----------------------------------------------------------------------------
// No-arg constructor
//-----------------------------------------------------------------------------
CGotoState::CGotoState()
{
    SetName(_T("CGotoState"));
}


CGotoState::~CGotoState()
{
}


bool CGotoState::IsInternal() 
{
    return true; 
}


//-----------------------------------------------------------------------------
// Examines the operands from the XML, which is expected to have the form:
//
//  <action>
//      <action-name>goto-state</action-name>
//      <parameter>
//          <value>state-name=name</value> or <value>state-id=id-expression</value>
//      </parameter>
//
//      ...
//
//  </action>
// The first <parameter> tag specifies an KV string containing either the name
// of the transition state or an id expression used to retrieve the name of the
// state from the state machine.
//-----------------------------------------------------------------------------
HRESULT CGotoState::ParseOperands()
{ 
    HRESULT hr = S_OK;

    m_parms.Clear();

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    hr = m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    if (m_parms.GetStateName().IsEmpty() && m_parms.GetStateId().IsEmpty())
        return STATEM_MISSING_PARAMETER;

    return hr;
}

    
//-----------------------------------------------------------------------------
// Called by the State Machine to perform timer related operations such as 
// Start and Stop.
//-----------------------------------------------------------------------------
HRESULT CGotoState::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = S_OK;

    hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    StringType sStateName;
	if (!m_parms.GetStateName().IsEmpty())
		sStateName = m_parms.GetStateName();

	else 
	{
		IVariable *pVariable;
		hr = stateMachine().GetVariable(&pVariable, m_parms.GetStateId());
		if (hr == S_OK)
		{
			int nVariableId = pVariable->GetValue();
			sStateName = stateMachine().GetStateName(nVariableId);
		}
		else
            sStateName.clear();
	}

    ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Transitioning to state:  ") + IString(sStateName.c_str()));

    IState *pState;    

    hr = stateMachine().GetState(&pState, sStateName.c_str());
    if (hr != S_OK)
        return hr;

    stateMachine().GotoState(pState->GetId());

    *pResult = true;

    // If we have reached here, then we want to break the chain of execution
    return S_FALSE;
}


//-----------------------------------------------------------------------------
// Template specialization of a creation function for CGotoState.  This
// will be used by the State Machine for creating instances of that class.
//-----------------------------------------------------------------------------
template <>
IAction* CCreateable<CGotoState, IAction>::NewInstance()
{
      IAction *pAction = new CGotoState;

    return pAction;
}


template <>
void CCreateable<CGotoState, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(GotoState)
DEFINE_ACTION_RELEASE_FXN(GotoState)
