// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/WinNT/XTimerObject.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)TimerObject.cpp    $Revision: 1 $ $Date: 6/20/08 1:13p $
 *
 * Copyright 2004, NCR Corporation.
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/WinNT/XTimerObject.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 8     5/17/06 2:19p Tp151000
 * 324401
 * 
 * 7     5/31/05 11:38a Dm185016
 * is_timer_timing action
 * 
 * 4     3/24/05 7:28p Dm185016
 * UNICODE
 * 
 * 3     2/28/05 3:35p Dm185016
 * lint
 * 
 * 2     2/22/05 1:23p Dm185016
 * Clean up warnings
 * 
 * 1     1/26/05 2:33p Dm185016
 * Moved to new Repository.
// 
// 2     1/18/05 4:03p Dm185016
// UNICODE support
// 
// 1     10/04/04 6:55p Dm185016
// 
// 1     9/07/04 3:06p Dm185016
// TAR 277300 - Added ability to switch between NT and 2000 type timers
// based on compile switch.
*/
/*lint -save -e666 */

#include "stdafx.h"
#include "XTimerObject.h"
//#include "verify.h"

using namespace utils;


/**
* Default no-argument constructor
*/
CTimerObject::CTimerObject(long id
                         , LONG nTimerDuration
                         , WAITORTIMERCALLBACK pCallback
                         , PVOID pParm
                         , bool bPeriodic
                         , HTRACE hTrace)
    : semaTerminate(hTrace)
    , m_hTimer(NULL)
    , m_ulTimerIdentifier((UINT_PTR)id)
    , m_bPeriodic(bPeriodic)
    , m_pCallback(pCallback)
    , m_pParm(pParm)
    , m_bTiming(false)
{
    CTOR_BEGIN(hTrace)

    IMODTRACE_DEVELOP();

    SetPeriod((LONG)nTimerDuration);

    m_hTimer = ::CreateWaitableTimer(NULL, TRUE, NULL);
    if (m_hTimer == NULL)
    {
        ITHROWSYSTEMERROR(GetLastError(), _T("CreateWaitableTimer")
                        , IException::unrecoverable);
    }

    CTOR_END()
}


CTimerObject::~CTimerObject()
{
    //lint --e{1506}
    IMODTRACE_DEVELOP();

    Stop();     //lint !e1506

    if (!::CloseHandle(m_hTimer))
    {
        ISystemErrorInfo sysError((HRESULT)GetLastError(), _T("CloseHandle"));
        ITRACE_DEVELOP((const _TCHAR *)sysError.text());
    }

    m_pParm = NULL;
}


LARGE_INTEGER CTimerObject::GetDueTime()
{
    IMODTRACE_DEVELOP();

    return m_liDueTime;
}


void CTimerObject::SetPeriod(LONG nTimerDuration)
{
    m_lDuration = nTimerDuration;

    m_liDueTime.QuadPart = Int32x32To64(nTimerDuration, -10000L);
}


LONG CTimerObject::GetPeriod()
{
    IMODTRACE_DEVELOP();

    if (m_bPeriodic)
        return m_lDuration;
    else
        return 0;
}


void CTimerObject::Stop()
{
    semaTerminate.Post();

    CThreadBase::Stop();

    m_bTiming = false;
}


unsigned CTimerObject::Run( )
{
    IMODTRACE_DEVELOP();

    if (m_hTimer == NULL)
        return (unsigned)E_FAIL;

    m_bTiming = true;

    LARGE_INTEGER liDueTime = GetDueTime();
    if (!::SetWaitableTimer(m_hTimer, &liDueTime, GetPeriod(), NULL, NULL, 0))
    {
        ISystemErrorInfo sysError((HRESULT)GetLastError(), _T("SetWaitableTimer"));
        ITRACE_DEVELOP((const _TCHAR *)sysError.text());

        m_nRC = E_FAIL;
    }

    HANDLE handles[2];
    handles[0] = m_hTimer;
    handles[1] = (HANDLE)semaTerminate;

    // Wait for either the termination semaphore or the timer to be signalled.
    ITRACE_DEVELOP(_T("Waiting ..."));
    DWORD dwWhich = ::WaitForMultipleObjects(2, handles, FALSE, INFINITE);
    ITRACE_DEVELOP(_T("Wait ended.  dwWhich=") + IString(dwWhich));
    switch (dwWhich)
    {
        // If the wait failed in any manner, then it will come here
        case WAIT_FAILED :
        {
            ISystemErrorInfo sysError((HRESULT)GetLastError(), _T("WaitForMultipleObjects"));
            ITRACE_DEVELOP((const _TCHAR *)sysError.text());

            m_nRC = E_FAIL;
            (*m_pCallback)(m_pParm, FALSE);
            break;
        }

        // The first handle was signalled so call the timer callback
        case WAIT_OBJECT_0 : 
            ITRACE_DEVELOP(_T("Calling the callback routine"));
            (*m_pCallback)(m_pParm, TRUE);
            break;

        // Any other value means that the thread needs to terminate
        default:
            (*m_pCallback)(m_pParm, FALSE);
            break;
    }

    return (unsigned)m_nRC;
}


/*lint -restore */