#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/PopState.h 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)PopState.h    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/PopState.h $
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
* 2     5/21/04 9:16a Dm185016
* Removed unneeded declaration
* 
* 1     3/31/04 10:14a Dm185016
* New for push/pop state mechanism
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

class CPopState
    : public CActionBase
    , public CCreateable<CPopState, IAction>
    //, public CPool<CPopState>
{

public:

    CPopState();
    virtual ~CPopState();

    virtual HRESULT Execute(bool *, IMessage &);

    bool IsInternal();

private:

    // hide copy and assignment
    CPopState(const CPopState&);
    CPopState& operator = (const CPopState &);

    bool m_bUseCurrentState;

    StringType sStateName;

};

};