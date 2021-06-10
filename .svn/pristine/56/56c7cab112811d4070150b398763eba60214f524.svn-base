#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMTimerManager.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)SMTimerManager.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMTimerManager.h $
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
* 
* 6     10/04/04 6:42p Dm185016
* Trace object is no longer shared.
* 
* 5     9/07/04 3:00p Dm185016
* TAR 277300 - Added ability to switch between NT and 2000 type timers
* based on compile switch.
* 
* 4     6/11/04 10:59a Dm185016
* TAR 270904 - Added Wait/Post mechanism for timers.
* 
* 3     6/02/04 4:20p Dm185016
* 
* 2     1/27/04 1:58p Dm185016
* DLL Support
* 
* 1     1/16/04 9:04a Dm185016
* State Machine specific timer support
*/

//lint --e{1535}

#include "singleton.h"
#include "TimerManager.h"
#include "statem.h"

namespace utils
{
    class CSemaphore;
}


class STATEM_API CSMTimerManager
    : public CTimerManager
{

public:

    CSMTimerManager(HTRACE hTrace);

    virtual ~CSMTimerManager();

    virtual HRESULT Start(int *, DWORD, WAITORTIMERCALLBACK, PVOID, bool bPeriodic=false);

    virtual HRESULT Stop(int);

    virtual HRESULT IsTiming(int);

    virtual HRESULT GetHRESULT(ErrorTypes);

    void Wait(long timeout=(long)INFINITE);

    void Post();

protected:

    utils::CSemaphore &semaTimerWait();

    CTimerManager &timerMgr();

private:

    CSMTimerManager();
    CSMTimerManager(const CSMTimerManager &);               // Hidden Copy construcor
    CSMTimerManager &operator =(const CSMTimerManager &);   // Hidden assignment operator

    utils::CSemaphore *m_pSemaTimerWait;

    CTimerManager *m_pTimerMgr;
};
