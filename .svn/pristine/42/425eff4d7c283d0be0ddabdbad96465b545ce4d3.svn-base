// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/TimerManager.cpp 1     6/20/08 1:13p Sa250050 $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/TimerManager.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     2/28/05 3:34p Dm185016
 * lint
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
// 
// 5     10/04/04 6:55p Dm185016
// Trace object is no longer shared.
// 
// 4     9/07/04 3:05p Dm185016
// TAR 277300 - Added ability to switch between NT and 2000 type timers
// based on compile switch.
// 
// 3     6/02/04 4:22p Dm185016
// Added new invalid interval error
// 
// 2     1/23/04 3:37p Dm185016
// DLL Support
// 
// 1     1/16/04 9:32a Dm185016
// New facility for managing Timers via the Timer Queue
*/

#include "StdAfx.h"
#include "TimerManager.h"

#if defined(_USE_2000_TIMERS_)
#include "Win2000\TimerManager2000.h"
#else
#include "WinNT\TimerManagerNT.h"
#endif


CTimerManager::CTimerManager()
{
}


CTimerManager::CTimerManager(HTRACE hTrace)
    : CBaseObject(hTrace)
{
}


CTimerManager::~CTimerManager()
{
}


CTimerManager *CTimerManager::NewInstance(HTRACE hTrace)
{
#if defined(_USE_2000_TIMERS_)
    return new CTimerManager2000(hTrace);
#else
    return new CTimerManagerNT(hTrace);
#endif
}


