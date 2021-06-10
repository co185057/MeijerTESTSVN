#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Win2000/TimerManager2000.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)TimerManager2000.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Win2000/TimerManager2000.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:33p Dm185016
 * Moved to new Repository.
* 
* 1     10/04/04 6:54p Dm185016
* 
* 1     9/07/04 3:06p Dm185016
* TAR 277300 - Added ability to switch between NT and 2000 type timers
* based on compile switch.
*/

#include <map>
#include "TimerManager.h"
#include "Utils.h"


#pragma warning( push )
#pragma warning( disable : 4251 )

typedef std::map<int, HANDLE> CHANDLEMap;
typedef CHANDLEMap::iterator CHANDLEMapIterator;

//lint --e{1735}
class UTILS_API CTimerManager2000
    : public CTimerManager
{

public:
    
    CTimerManager2000();
    virtual ~CTimerManager2000();

    virtual HRESULT Start(int *, DWORD, WAITORTIMERCALLBACK, PVOID, bool bPeriodic=false);

    virtual HRESULT Stop(int);

    virtual HRESULT GetHRESULT(ErrorTypes);

private:

    HANDLE m_hTimerQueue;

    long m_nTimeoutCount;

//  CTimeoutsMap m_mapCurrentTimeouts;
    CHANDLEMap m_mapCurrentTimeouts;
};
#pragma warning( pop )
