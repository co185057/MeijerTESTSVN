// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/TimerOperations.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)TimerOperations.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/TimerOperations.cpp $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 4     3/22/05 2:50p Dm185016
 * UNICODE
 * 
 * 3     2/28/05 3:48p Dm185016
 * lint
 * 
 * 2     2/10/05 5:48p Dm185016
 * TAR 292915
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
// 
// 13    1/18/05 3:56p Dm185016
// UNICODE support
// 
// 12    10/04/04 6:44p Dm185016
// Trace object is no longer shared.
// 
// 11    9/07/04 3:01p Dm185016
// TAR 277300 - Now ignores call if not from timeout.
// 
// 10    6/11/04 11:00a Dm185016
// TAR 270904 - Added new wait-timer action.
// 
// 9     6/02/04 4:22p Dm185016
// Fixed memory leak.
// 
// 8     4/16/04 4:18p Dm185016
// Allow timeout value from variable
// 
// 7     4/01/04 5:58p Dm185016
// If the timer is not active, then treated same as if timer stopped.
// 
// 6     3/19/04 3:08p Dm185016
// Removed trace
// 
// 5     3/01/04 10:15a Dm185016
// Added Allocation/Release functions
// 
// 4     2/24/04 3:41p Dm185016
// Added retry of stop operation
// 
// 3     2/10/04 11:46a Dm185016
// Moved common support to base
// 
// 2     1/27/04 2:03p Dm185016
// DLL Support
// 
// 1     1/16/04 9:16a Dm185016
// New support for Timers and Timeouts
*/
#include "StdAfx.h"
#include "TimerOperations.h"
#include "SMError.h"
#include "SMTimerManager.h"
#include "TimerImpl.h"
#include "Variable.h"
#include "InternalOps.h"

using namespace STATE_MACHINE_NAMESPACE;


//-----------------------------------------------------------------------------
// Forward declaration of the callback routine for handling timeouts.
//-----------------------------------------------------------------------------
static VOID CALLBACK TimerCallback(
    PVOID lpParameter,  
    BOOLEAN bTimedOut);


//-----------------------------------------------------------------------------
// No-arg constructor
//-----------------------------------------------------------------------------
CTimerOperations::CTimerOperations()
    : m_nOperation(NO_OP)
    , m_pTimer(NULL)
{
    SetName(_T("CTimerOperations"));
}


CTimerOperations::CTimerOperations(Operations op)
    : m_nOperation(op)
    , m_pTimer(NULL)
{
}


CTimerOperations::~CTimerOperations()
{
}


bool CTimerOperations::IsInternal() 
{
    return true; 
}


CTimerImpl &CTimerOperations::timer() 
{ 
    _ASSERTE(m_pTimer!=NULL);

    return *(CTimerImpl*)m_pTimer; 
}


const _TCHAR *CTimerOperations::GetComment(void) 
{
    if (!Comment.IsValid())
    {
        StringType sComment;
        switch (m_nOperation)
        {
            case WAIT :
                sComment = _T("Waiting on timer.  Name:  ");
                sComment += GetTimerName(); 
                break;
            case START :
                sComment = _T("Starting timer.  Name:  ");
                sComment += GetTimerName(); 
                break;
            case STOP :
                sComment = _T("Stopping timer.  Name:  ");
                sComment += GetTimerName(); 
                break;
        }

        Comment.Value(sComment);
    }

    return Comment.Value().c_str();
}


const _TCHAR *CTimerOperations::GetTimerName(void) 
{
    return (m_pTimer==NULL ? _T("Unknown") : timer().Name.Value().c_str());
}


//-----------------------------------------------------------------------------
// Examines the operands from the XML, which is expected to have the form:
//
//  <action>
//      <action-name>start-timer</action-name>
//      <parameter>
//          <value>timer1</value>
//      </parameter>
//      <parameter>
//          <value>5000</value>
//      </parameter>
//
//      ...
//
//  </action>
// The first <parameter> tag specifies which timer to operate on.
// The second <parameter> tag specifies the timeout value.
//-----------------------------------------------------------------------------
HRESULT CTimerOperations::ParseOperands(int nOpCount)
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
    hr = stateMachine().GetTimer(&m_pTimer, sTimerName.c_str());
    if (hr != S_OK)
        return hr;

    if (nOpCount == 1)
        return hr;

    iter++;
    if (iter == parms.end())
    {
        ITRACE_ERROR(_T("Missing timeout value"));
        return SECURITY_MISSING_TIMEOUT_VALUE;
    }
    StringType sTimeoutValue = *iter;
    int nTimeout = _tstoi(sTimeoutValue.c_str());
    ITRACE_DEVELOP(_T("timeout value=") + IString(sTimeoutValue.c_str()));

    if (nTimeout == 0)
	{
		IVariable *pVar;
		if (S_OK == stateMachine().GetVariable(&pVar, sTimeoutValue.c_str()))
			nTimeout = pVar->GetValue();
	}

    if (nTimeout == 0)
        return SECURITY_INVALID_TIMEOUT_VALUE;

    ((CTimerImpl *)m_pTimer)->SetTimeoutDuration(nTimeout);

    iter++;
    if (iter != parms.end())
    {
        StringType sTimeoutType = *iter;
        bool bPeriodic = (_tcscmp(sTimeoutType.c_str(), _T("periodic")) == 0);
        ((CTimerImpl *)m_pTimer)->SetPeriodic(bPeriodic);
    }

    return hr;
}


//-----------------------------------------------------------------------------
// Examines the name saved by the SM and determines which operation to perform
//-----------------------------------------------------------------------------
HRESULT CTimerOperations::ParseOperation()
{ 
    HRESULT hr = S_OK;
    StringType name = GetName();

    if (_tcscmp(name.c_str(), START_TIMER) == 0)
        m_nOperation = START;
    else if (_tcscmp(name.c_str(), STOP_TIMER) == 0)
        m_nOperation = STOP;
    else if (_tcscmp(name.c_str(), WAIT_TIMER) == 0)
        m_nOperation = WAIT;
    else
        m_nOperation = NO_OP;

    return hr;
}

    
//-----------------------------------------------------------------------------
// Called by the State Machine to perform timer related operations such as 
// Start and Stop.
//-----------------------------------------------------------------------------
HRESULT CTimerOperations::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = S_OK;

    hr = ParseOperation();
    if (hr != S_OK)
        return hr;

    switch (m_nOperation)
    {
        case START :
            hr = StartTimer();
            break;
        case STOP :
            hr = StopTimer();
            break;
        case WAIT :
            hr = WaitTimer();
            break;
        default:
            hr = SECURITY_UNDEFINED_OPERATION;
            break;
    }

    *pResult = (hr == S_OK);

    return hr;
}


//-----------------------------------------------------------------------------
// This method is called to start create a timer and start it.
//
// This is not thread safe!
//-----------------------------------------------------------------------------
HRESULT CTimerOperations::StartTimer()
{
    HRESULT hr = true;

    hr = ParseOperands(3);
    if (hr != S_OK)
        return hr;

    int nCookie;
    CCallbackParameter *pParm = new CCallbackParameter;
    pParm->pTimer   = m_pTimer;
    pParm->pSM      = &stateMachine();
    hr = stateMachine().GetTimerMgr().Start(&nCookie
                                            , timer().GetTimeoutDuration()
                                            , TimerCallback
                                            , pParm
                                            , m_pTimer->IsPeriodic());
    timer().SetTimerHandle(nCookie);

    return hr;
}


//-----------------------------------------------------------------------------
// This method is called to wait on a timer.
//
// This is not thread safe!
//-----------------------------------------------------------------------------
HRESULT CTimerOperations::WaitTimer()
{
    IMODTRACE_DEVELOP();

    HRESULT hr = true;

    hr = ParseOperands(1);
    if (hr != S_OK)
        return hr;

    stateMachine().GetTimerMgr().Wait();

    return hr;
}


//-----------------------------------------------------------------------------
// This function is called to terminate a timer that is currently in pregress.
//-----------------------------------------------------------------------------
HRESULT CTimerOperations::StopTimer()
{
    HRESULT hr = true;

    hr = ParseOperands(1);
    if (hr != S_OK)
        return hr;

    // We need to give up control, so that timeout thread can return before the
    // stop is attempted.
    ::Sleep(0);

    int hTimerHandle = timer().GetTimerHandle();

    hr = stateMachine().GetTimerMgr().Stop(hTimerHandle);
    for (int i=0 
         ; (i<4)
         ; i++)
    {
        switch (hr)
        {
            // Intentionally fall thru here after resetting the hr to S_OK
            case SECURITY_TIMEOUT_NOT_IN_PROGRESS :
                hr = S_OK;
            case S_OK :
                i = 4;
                break;
            default :
                ITRACE_DEVELOP(_T("Retrying operation.  hr=") + IString(hr).d2x());
                ::Sleep(500);
                hr = stateMachine().GetTimerMgr().Stop(hTimerHandle);
                break;
        }
    }

    return hr;
}


//-----------------------------------------------------------------------------
// Template specialization of a creation function for CTimerOperations.  This
// will be used by the State Machine for creating instances of that class.
//-----------------------------------------------------------------------------
template <>
IAction* CCreateable<CTimerOperations, IAction>::NewInstance()
{
      IAction *pAction = new CTimerOperations;

    return pAction;
}


template <>
void CCreateable<CTimerOperations, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(TimerOperations)
DEFINE_ACTION_RELEASE_FXN(TimerOperations)


//-----------------------------------------------------------------------------
// This function provides the Callback mechanism for the timers when they
// expire, or are terminated.
// 
// @param lpParameter This parameter is used to contain a pointer to an
//                    instance of the CCallbackParameter struct, which contains
//                    values required in order to properly notify the State
//                    Machine of the occurence of a timeout.
// @param bTimedOut T if the timer completed or false if it was interrupted.
//-----------------------------------------------------------------------------
VOID CALLBACK TimerCallback(
    PVOID lpParameter,  
    BOOLEAN bTimedOut)
{
    // Grab the address of the parameter list struct.
    CCallbackParameter *pParm = (CCallbackParameter *)lpParameter;

    if (bTimedOut)
    {
        // Inform the state machine that the timeout has occurred.
        STATE_MACHINE_NAMESPACE::CAbstractStateMachine &sm = *pParm->pSM;
        sm.OnTimerTimeout(pParm->pTimer);

        sm.GetTimerMgr().Post();
    }

    delete pParm;
}
