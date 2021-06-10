#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/IsTimerTiming.h 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)IsTimerTiming.h    $Revision: 1 $ $Date: 6/20/08 1:11p $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/IsTimerTiming.h $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     5/31/05 11:39a Dm185016
 * is_timer_timing action
*/

#include "SMConstants.h"
#include "Createable.h"
#include "ActionBase.h"
#include "Evaluateable.h"
#include "Pool.h"
#include "Timer.h"
#include "Property.h"

namespace STATE_MACHINE_NAMESPACE 
{

class CAbstractStateMachine;
class IMessage;
class CTimerImpl;

class CIsTimerTiming
    : public CActionBase
    , public IEvaluateable
    , public CCreateable<CIsTimerTiming, IAction>
    //, public CPool<CIsTimerTiming>
{

public:

    enum Operations
    {
        NO_OP = 0,
        STOP = 1,
        START,
        WAIT,
        TIMING,
    };

    CIsTimerTiming();
    CIsTimerTiming(Operations);
	virtual ~CIsTimerTiming();
    
    virtual const _TCHAR *GetComment(void);

    virtual HRESULT Execute(bool *, IMessage &);

    bool IsInternal();

    long GetValue() const { return 0; }

    void SetValue(long) {}

private:
    
    const _TCHAR *GetTimerName(void) ;

    HRESULT ParseOperands();

    CTimerImpl &timer();

    Operations m_nOperation;

    // hide copy and assignment
	CIsTimerTiming(const CIsTimerTiming&);
	CIsTimerTiming& operator = (const CIsTimerTiming &);

    STATE_MACHINE_NAMESPACE::ITimer *m_pTimer;
};

};