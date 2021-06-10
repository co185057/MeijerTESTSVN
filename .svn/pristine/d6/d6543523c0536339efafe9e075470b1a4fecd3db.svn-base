#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/WinNT/XTimerObject.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)TimerObject.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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


#include "utils.h"
#include "xerror.h"
#include "xthread.h"
#include "semaphore.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace utils
{

class UTILS_API CTimerObject
    : public CThreadMemberBase<CTimerObject>
{
public:

    friend class CThreadMemberBase<CTimerObject>;

    ///
    /// 
    ///
    CTimerObject(long id
               , LONG nTimerDuration
               , WAITORTIMERCALLBACK pCallback
               , PVOID pParms
               , bool bPeriodic
               , HTRACE hTrace);

    virtual ~CTimerObject();
    
    void SetPeriod(LONG nTimerDuration);

    virtual void Stop();

    inline UINT_PTR GetID() const { return m_ulTimerIdentifier; }

    inline bool IsTimingInProgress() const { return m_bTiming; }

protected:

    virtual unsigned Run ( );

    virtual LARGE_INTEGER GetDueTime();

    virtual LONG GetPeriod();

private:

    CTimerObject();

    utils::CSemaphore semaTerminate;

    HANDLE m_hTimer;

    HRESULT m_nRC;

    UINT_PTR m_ulTimerIdentifier;

    LARGE_INTEGER m_liDueTime;
    bool m_bPeriodic;
    LONG m_lDuration;
    WAITORTIMERCALLBACK m_pCallback;
    PVOID m_pParm;

    mutable bool m_bTiming;

};

typedef CTimerObject *PTIMEROBJECT;
typedef std::map<int, PTIMEROBJECT> CTimerMap;
typedef CTimerMap::iterator CTimerMapIterator;

}
#pragma warning( pop )
