// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/WinNT/TimerManagerNT.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)TimerManagerNT.cpp    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/WinNT/TimerManagerNT.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     5/31/05 11:38a Dm185016
 * is_timer_timing action
 * 
 * 2     2/22/05 1:23p Dm185016
 * Clean up warnings
 * 
 * 1     1/26/05 2:33p Dm185016
 * Moved to new Repository.
// 
// 1     10/04/04 6:55p Dm185016
// 
// 1     9/07/04 3:06p Dm185016
// TAR 277300 - Added ability to switch between NT and 2000 type timers
// based on compile switch.
*/

#include "StdAfx.h"
#include "TimerManagerNT.h"
#include "xerror.h"


CTimerManagerNT::CTimerManagerNT(HTRACE hTrace)
    : CTimerManager(hTrace)
    , m_nTimeoutCount(0)
{
}


CTimerManagerNT::~CTimerManagerNT()
{
    IMODTRACE_DEVELOP();

    try
    {
        PTIMEROBJECT pTimer;
        CTimerMap::iterator iter = m_mapCurrentTimeouts.begin();
        for (;iter != m_mapCurrentTimeouts.end(); iter++)
        {
            pTimer = iter->second;

            delete pTimer;
        }
        m_mapCurrentTimeouts.clear();
    }
    catch (...)
        { ITRACE_DOT_ERROR(); }
}


// Note:  This is not thread safe!
HRESULT CTimerManagerNT::Start(int *pnCookie
                             , DWORD nTimerDuration
                             , WAITORTIMERCALLBACK pCallback
                             , PVOID pParm
                             , bool bPeriodic)
{
    HRESULT hr = S_OK;

    PTIMEROBJECT pTimer = new CTimerObject(++m_nTimeoutCount
                                         , nTimerDuration
                                         , pCallback
                                         , pParm
                                         , bPeriodic
                                         , GetTraceHandle());

    try
    {
        pTimer->Start();

        ::Sleep(0);

        int nID = (int)pTimer->GetID();

        *pnCookie = nID;

        m_mapCurrentTimeouts.insert(std::make_pair(nID, pTimer));
    }
    catch (...)
    {
        ITRACE_ERROR(_T("Failed to start timer"));
        hr = GetHRESULT(START_ERROR_UNABLE_TO_START_TIMER);
    }

    return hr;
}


HRESULT CTimerManagerNT::Stop(int nCookie)
{
    HRESULT hr = S_OK;

    CTimerMapIterator iter = m_mapCurrentTimeouts.find(nCookie);
    if (iter == m_mapCurrentTimeouts.end())
        return GetHRESULT(STOP_ERROR_TIMEOUT_NOT_IN_PROGRESS);

    PTIMEROBJECT pTimer = iter->second;

    m_mapCurrentTimeouts.erase(nCookie);

    pTimer->Stop();

    delete pTimer;

    return hr;
}


HRESULT CTimerManagerNT::IsTiming(int nCookie)
{
    CTimerMapIterator iter = m_mapCurrentTimeouts.find(nCookie);
    if (iter == m_mapCurrentTimeouts.end())
        return GetHRESULT(STOP_ERROR_TIMEOUT_NOT_IN_PROGRESS);

    PTIMEROBJECT pTimer = iter->second;

    return (pTimer->IsTimingInProgress() ? S_OK : S_FALSE);
}


HRESULT CTimerManagerNT::GetHRESULT(ErrorTypes /* errorType */ )
{
    return S_OK;
}

