// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/IsTimerTiming.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)IsTimerTiming.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/IsTimerTiming.cpp $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     5/31/05 11:39a Dm185016
 * is_timer_timing action
*/
#include "StdAfx.h"
#include "IsTimerTiming.h"
#include "SMError.h"
#include "SMTimerManager.h"
#include "TimerImpl.h"
#include "InternalOps.h"

using namespace STATE_MACHINE_NAMESPACE;

//-----------------------------------------------------------------------------
// No-arg constructor
//-----------------------------------------------------------------------------
CIsTimerTiming::CIsTimerTiming()
    : m_pTimer(NULL)
{
    SetName(_T("CIsTimerTiming"));
}


CIsTimerTiming::~CIsTimerTiming()
{
}


bool CIsTimerTiming::IsInternal() 
{
    return true; 
}


CTimerImpl &CIsTimerTiming::timer() 
{ 
    _ASSERTE(m_pTimer!=NULL);

    return *(CTimerImpl*)m_pTimer; 
}


const _TCHAR *CIsTimerTiming::GetComment(void) 
{
    if (!Comment.IsValid())
    {
        StringType sComment = _T("Checking timer.  Name:  ");
        sComment += GetTimerName(); 

        Comment.Value(sComment);
    }

    return Comment.Value().c_str();
}


const _TCHAR *CIsTimerTiming::GetTimerName(void) 
{
    return (m_pTimer==NULL ? _T("Unknown") : timer().Name.Value().c_str());
}


//-----------------------------------------------------------------------------
// Examines the operands from the XML, which is expected to have the form:
//
//  <action>
//      <action-name>is_timer_timing</action-name>
//      <parameter>
//          <value>timer1</value>
//      </parameter>
//
//      ...
//
//  </action>
// The first <parameter> tag specifies which timer to operate on.
// The second <parameter> tag specifies the timeout value.
//-----------------------------------------------------------------------------
HRESULT CIsTimerTiming::ParseOperands()
{ 
    HRESULT hr = S_OK;
    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter == parms.end())
    {
        ITRACE_ERROR(_T("Missing timer name"));
        return SECURITY_MISSING_TIMER_NAME;
    }
    StringType sTimerName = *iter;
    ITRACE_DEVELOP(_T("timer name=") + IString(sTimerName.c_str()));
    return stateMachine().GetTimer(&m_pTimer, sTimerName.c_str());
}

    
//-----------------------------------------------------------------------------
// Called by the State Machine to perform timer related operations such as 
// Start and Stop.
//-----------------------------------------------------------------------------
HRESULT CIsTimerTiming::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = S_OK;

    hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    int hTimerHandle = timer().GetTimerHandle();

    hr = stateMachine().GetTimerMgr().IsTiming(hTimerHandle);

    *pResult = (hr == S_OK);

    return hr;
}


//-----------------------------------------------------------------------------
// Template specialization of a creation function for CIsTimerTiming.  This
// will be used by the State Machine for creating instances of that class.
//-----------------------------------------------------------------------------
template <>
IAction* CCreateable<CIsTimerTiming, IAction>::NewInstance()
{
      IAction *pAction = new CIsTimerTiming;

    return pAction;
}


template <>
void CCreateable<CIsTimerTiming, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsTimerTiming)
DEFINE_ACTION_RELEASE_FXN(IsTimerTiming)
