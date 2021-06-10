// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMTimerManager.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)TimerManager.cpp    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMTimerManager.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     5/31/05 11:37a Dm185016
 * is_timer_timing action
 * 
 * 2     2/28/05 3:54p Dm185016
 * lint
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
// 
// 6     10/04/04 6:42p Dm185016
// Trace object is no longer shared.
// 
// 5     9/07/04 3:00p Dm185016
// TAR 277300 - Added ability to switch between NT and 2000 type timers
// based on compile switch.
// 
// 4     6/11/04 10:59a Dm185016
// TAR 270904 - Added Wait/Post mechanism for timers.
// 
// 3     6/02/04 4:19p Dm185016
// Added new error code for invalid timer value.
// 
// 2     1/27/04 1:58p Dm185016
// Removed tracing
// 
// 1     1/16/04 9:04a Dm185016
// State Machine specific timer support
*/

#include "StdAfx.h"
#include "SMTimerManager.h"
#include "SMError.h"
#include "Semaphore.h"

using namespace utils;


CSMTimerManager::CSMTimerManager(HTRACE hTrace)
    : CTimerManager(hTrace)
    , m_pSemaTimerWait(NULL)
    , m_pTimerMgr(NULL)
{
}


CTimerManager &CSMTimerManager::timerMgr()
{
    if (m_pTimerMgr == NULL)
        m_pTimerMgr = CTimerManager::NewInstance(GetTraceHandle());

    return *m_pTimerMgr;
}


CSMTimerManager::~CSMTimerManager()
{
    delete m_pTimerMgr;
    delete m_pSemaTimerWait;
}


utils::CSemaphore &CSMTimerManager::semaTimerWait() 
{ 
    if (m_pSemaTimerWait == NULL)
        m_pSemaTimerWait = new utils::CSemaphore(GetTraceHandle());

    return *m_pSemaTimerWait;
}


// Note:  This is not thread safe!
HRESULT CSMTimerManager::Start(int *pnCookie
                             , DWORD nTimerDuration
                             , WAITORTIMERCALLBACK pCallback
                             , PVOID pParm
                             , bool bPeriodic)
{
    HRESULT hr = timerMgr().Start(pnCookie
                             , nTimerDuration
                             , pCallback
                             , pParm
                             , bPeriodic);

    ITRACE_DEVELOP(_T("Started timer:  ") + IString(*pnCookie) 
                 + _T(".  Duration:  ") + IString(nTimerDuration) 
                 + _T(".  hr:  ") + IString(hr));

    return hr;
}


HRESULT CSMTimerManager::Stop(int nCookie)
{
    HRESULT hr = timerMgr().Stop(nCookie);

    ITRACE_DEVELOP(_T("Stopped timer:  ") + IString(nCookie));

    return hr;
}


HRESULT CSMTimerManager::IsTiming(int nCookie)
{
    HRESULT hr = timerMgr().IsTiming(nCookie);

    return hr;
}


HRESULT CSMTimerManager::GetHRESULT(CSMTimerManager::ErrorTypes errorType)
{
    HRESULT hr = S_OK;
    switch (errorType)
    {
        case START_ERROR_TIMEOUT_ALREADY_IN_PROGRESS :
            hr = SECURITY_TIMEOUT_ALREADY_IN_PROGRESS;
            break;
        case START_ERROR_UNABLE_TO_START_TIMER :
            hr = SECURITY_UNABLE_TO_START_TIMER;
            break;
        case STOP_ERROR_TIMEOUT_NOT_IN_PROGRESS :
            hr = SECURITY_TIMEOUT_NOT_IN_PROGRESS;
            break;
        case STOP_ERROR_UNABLE_TO_STOP_TIMER :
            hr = SECURITY_UNABLE_TO_STOP_TIMER;
            break;
        case START_ERROR_ZERO_INVALID_INTERVAL :
            hr = SECURITY_INVALID_TIMER_INTERVAL_ZERO;
            break;
    }

    return hr;
}


void CSMTimerManager::Post()
{
    semaTimerWait().Post();
}


void CSMTimerManager::Wait(long /* timeout */ )
{
    ULONG ulReset;

    semaTimerWait().Wait();
    semaTimerWait().Reset(ulReset);
}
