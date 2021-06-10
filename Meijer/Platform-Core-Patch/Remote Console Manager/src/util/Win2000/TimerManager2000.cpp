// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Win2000/TimerManager2000.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)TimerManager2000.cpp    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Win2000/TimerManager2000.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     3/24/05 7:28p Dm185016
 * UNICODE
 * 
 * 2     2/28/05 3:34p Dm185016
 * lint
 * 
 * 1     1/26/05 2:33p Dm185016
 * Moved to new Repository.
// 
// 1     10/04/04 6:54p Dm185016
// 
// 1     9/07/04 3:06p Dm185016
// TAR 277300 - Added ability to switch between NT and 2000 type timers
// based on compile switch.
*/
//lint --e{774}

#include "StdAfx.h"
#include "TimerManager2000.h"
//#include "Timeout.h"
#include "xerror.h"


CTimerManager2000::CTimerManager2000()
    : m_hTimerQueue(NULL)
    , m_nTimeoutCount(0)
{
    IMODTRACE_DEVELOP();

    m_hTimerQueue = ::CreateTimerQueue();
    if (!m_hTimerQueue)
    {
    	ISystemErrorInfo error(_T("CreateTimerQueue"));
        ITRACE_ERROR(error);
    }
}


CTimerManager2000::~CTimerManager2000()
{
    IMODTRACE_DEVELOP();

    try
    {
        if (!::DeleteTimerQueue(m_hTimerQueue))
        {
    	    ISystemErrorInfo error(_T("DeleteTimerQueue"));
            ITRACE_ERROR(error);
        }
    }
    catch(...)
        { ITRACE_DOT_ERROR(); }

    m_hTimerQueue = NULL;
}


// Note:  This is not thread safe!
HRESULT CTimerManager2000::Start(int *pnCookie
                             , DWORD nTimerDuration
                             , WAITORTIMERCALLBACK pCallback
                             , PVOID pParm
                             , bool bPeriodic)
{
    HRESULT hr = S_OK;
    HANDLE hTimer;

    _ASSERTE(0!=nTimerDuration);

    if (0 == nTimerDuration)
    {
        ITRACE_ERROR(_T("Timer duration of zero not allowed"));
        hr = GetHRESULT(START_ERROR_ZERO_INVALID_INTERVAL);
    }

    else if (CreateTimerQueueTimer(
            &hTimer
            , m_hTimerQueue
            , pCallback
            , pParm
            , nTimerDuration
            , (bPeriodic ? nTimerDuration : 0)
            , 0))
    {
        m_nTimeoutCount++;
        m_mapCurrentTimeouts.insert(std::make_pair(m_nTimeoutCount, hTimer));

        if (pnCookie != NULL)
            *pnCookie = m_nTimeoutCount;
    }
    else
    {
    	ISystemErrorInfo error(_T("CreateTimerQueueTimer"));
        ITRACE_ERROR(error);
        hr = GetHRESULT(START_ERROR_UNABLE_TO_START_TIMER);
    }

    return hr;
}


HRESULT CTimerManager2000::Stop(int nCookie)
{
    HRESULT hr = S_OK;

    CHANDLEMapIterator iter = m_mapCurrentTimeouts.find(nCookie);
    if (iter == m_mapCurrentTimeouts.end())
        return GetHRESULT(STOP_ERROR_TIMEOUT_NOT_IN_PROGRESS);

    HANDLE hTimer = iter->second;

    m_mapCurrentTimeouts.erase(nCookie);

    if (!DeleteTimerQueueTimer(
            m_hTimerQueue
            , hTimer
            , NULL ) )
    {
    	ISystemErrorInfo error(_T("DeleteTimerQueueTimer"));
        ITRACE_ERROR(error);
        hr = GetHRESULT(STOP_ERROR_UNABLE_TO_STOP_TIMER);
    }

    return hr;
}


HRESULT CTimerManager2000::GetHRESULT(ErrorTypes)
{
    return S_OK;
}

