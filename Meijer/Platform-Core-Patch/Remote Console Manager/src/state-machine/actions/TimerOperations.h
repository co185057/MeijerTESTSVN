#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/TimerOperations.h 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)TimerOperations.h    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/TimerOperations.h $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     2/10/05 5:48p Dm185016
 * TAR 292915
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
* 
* 3     6/11/04 11:00a Dm185016
* TAR 270904 - Added new wait-timer action.
* 
* 2     2/10/04 11:46a Dm185016
* Moved common support to base
* 
* 1     1/16/04 9:16a Dm185016
* New support for Timers and Timeouts
*/

#include "SMConstants.h"
#include "Createable.h"
#include "ActionBase.h"
#include "Pool.h"
#include "Timer.h"
#include "Property.h"

namespace STATE_MACHINE_NAMESPACE 
{

class CAbstractStateMachine;
class IMessage;
class CTimerImpl;

typedef struct
{
    ITimer *pTimer;
    CAbstractStateMachine *pSM;
} CCallbackParameter;

class CTimerOperations
    : public CActionBase
    , public CCreateable<CTimerOperations, IAction>
    //, public CPool<CTimerOperations>
{

public:

    enum Operations
    {
        NO_OP = 0,
        STOP = 1,
        START,
        WAIT,
    };

    CTimerOperations();
    CTimerOperations(Operations);
	virtual ~CTimerOperations();
    
    virtual const _TCHAR *GetComment(void);

    virtual HRESULT Execute(bool *, IMessage &);

    bool IsInternal();

private:
    
    const _TCHAR *GetTimerName(void) ;

    HRESULT ParseOperands(int);

    HRESULT ParseOperation();

    HRESULT StartTimer();
    HRESULT StopTimer();
    HRESULT WaitTimer();

    CTimerImpl &timer();

    Operations m_nOperation;

    // hide copy and assignment
	CTimerOperations(const CTimerOperations&);
	CTimerOperations& operator = (const CTimerOperations &);

    STATE_MACHINE_NAMESPACE::ITimer *m_pTimer;
};

};