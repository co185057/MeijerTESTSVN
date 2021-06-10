#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/TimerManager.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)TimerManager.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/TimerManager.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 4     5/31/05 11:38a Dm185016
 * is_timer_timing action
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 5     10/04/04 6:55p Dm185016
* Trace object is no longer shared.
* 
* 4     9/07/04 3:05p Dm185016
* TAR 277300 - Added ability to switch between NT and 2000 type timers
* based on compile switch.
* 
* 3     6/02/04 4:22p Dm185016
* Added new invalid interval error
* 
* 2     1/23/04 3:36p Dm185016
* DLL support
* 
* 1     1/16/04 9:32a Dm185016
* New facility for managing Timers via the Timer Queue
*/

/*
#include "Timeout.h"

typedef struct
{
    ITimeout *pTimeout;
} CCallbackParameter;
*/
#include <map>
#include "Utils.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

//lint --e{1735}
class UTILS_API CTimerManager
    : public CBaseObject
{

public:
    enum ErrorTypes
    {
        START_ERROR_TIMEOUT_ALREADY_IN_PROGRESS = 1,
        START_ERROR_UNABLE_TO_START_TIMER,
        START_ERROR_ZERO_INVALID_INTERVAL,
        STOP_ERROR_TIMEOUT_NOT_IN_PROGRESS,
        STOP_ERROR_UNABLE_TO_STOP_TIMER
    };
    
    CTimerManager(HTRACE hTrace);
    virtual ~CTimerManager();

    virtual HRESULT Start(int *, DWORD, WAITORTIMERCALLBACK, PVOID, bool bPeriodic=false)=0;

    virtual HRESULT Stop(int)=0;

    virtual HRESULT IsTiming(int)=0;

    virtual HRESULT GetHRESULT(ErrorTypes)=0;

    static CTimerManager *NewInstance(HTRACE hTrace);

protected:

    CTimerManager();

private:

    CTimerManager(const CTimerManager &);
    CTimerManager &operator =(const CTimerManager &);   // Hide the assignment operator
};
#pragma warning( pop )
