#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/GotoState.h 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)GotoState.h    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/GotoState.h $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 4     4/12/05 2:49p Dm185016
 * 
 * 3     4/12/05 11:44a Dm185016
 * TAR 296771
 * 
 * 2     2/10/05 5:48p Dm185016
 * TAR 292915
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
* 
* 1     10/29/04 9:27a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "ActionBase.h"
#include "KeyValueHandler.h"
#include "Pool.h"
#include "Property.h"

namespace STATE_MACHINE_NAMESPACE 
{

class CGotoState
    : public CActionBase
    , public CCreateable<CGotoState, IAction>
{

public:

    CGotoState();
    virtual ~CGotoState();

    virtual HRESULT Execute(bool *, IMessage &);

    bool IsInternal();

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(GTSParms)
        DECLARE_KV_CSTRING(StateName)
        DECLARE_KV_CSTRING(StateId)
    DECLARE_KV_HANDLER_CLASS_END

    GTSParms m_parms;

private:

    // hide copy and assignment
    CGotoState(const CGotoState&);
    CGotoState& operator = (const CGotoState &);
};

};
