#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/WinNT/TimerManagerNT.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)TimerManagerNT.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/WinNT/TimerManagerNT.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     5/31/05 11:38a Dm185016
 * is_timer_timing action
 * 
 * 2     2/28/05 3:35p Dm185016
 * lint
 * 
 * 1     1/26/05 2:33p Dm185016
 * Moved to new Repository.
* 
* 1     10/04/04 6:55p Dm185016
* 
* 1     9/07/04 3:06p Dm185016
* TAR 277300 - Added ability to switch between NT and 2000 type timers
* based on compile switch.
*/
#include <map>
#include "Utils.h"
#include "XTimerObject.h"
#include "TimerManager.h"


#pragma warning( push )
#pragma warning( disable : 4251 )

using namespace utils;

class UTILS_API CTimerManagerNT
    : public CTimerManager
{

public:
    
    CTimerManagerNT(HTRACE hTrace);
    virtual ~CTimerManagerNT();

    virtual HRESULT Start(int *, DWORD, WAITORTIMERCALLBACK, PVOID, bool bPeriodic=false);

    virtual HRESULT Stop(int);

    virtual HRESULT IsTiming(int);

    virtual HRESULT GetHRESULT(ErrorTypes);

private:

    CTimerManagerNT();

    long m_nTimeoutCount;

    CTimerMap m_mapCurrentTimeouts;
};
#pragma warning( pop )
