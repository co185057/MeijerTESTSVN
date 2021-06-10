#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/CleanState.h 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)CleanState.h    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/CleanState.h $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     5/31/05 11:39a Dm185016
 * TAR 301203
*/

#include "SMConstants.h"
#include "Createable.h"
#include "ActionBase.h"
#include "Pool.h"
#include "Property.h"

namespace STATE_MACHINE_NAMESPACE 
{

class CCleanState
    : public CActionBase
    , public CCreateable<CCleanState, IAction>
    //, public CPool<CCleanState>
{

public:

    CCleanState();
    virtual ~CCleanState();

    virtual HRESULT Execute(bool *, IMessage &);

    bool IsInternal();

private:

    // hide copy and assignment
    CCleanState(const CCleanState&);
    CCleanState& operator = (const CCleanState &);
};

};