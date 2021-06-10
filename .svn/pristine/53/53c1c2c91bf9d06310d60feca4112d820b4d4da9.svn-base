#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/PushState.h 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)PushState.h    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/PushState.h $
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
* 2     3/25/04 5:13p Dm185016
* Added ability to push the current state
* 
* 1     3/25/04 3:19p Dm185016
* New action to push the state
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
#include "Property.h"

namespace STATE_MACHINE_NAMESPACE 
{

class CPushState
    : public CActionBase
    , public CCreateable<CPushState, IAction>
    //, public CPool<CPushState>
{

public:

    CPushState();
    virtual ~CPushState();

    virtual HRESULT Execute(bool *, IMessage &);

    bool IsInternal();

private:

    HRESULT ParseOperands();

    // hide copy and assignment
    CPushState(const CPushState&);
    CPushState& operator = (const CPushState &);

    bool m_bUseCurrentState;

    StringType sStateName;

};

};