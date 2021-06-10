#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/PushbackImmediateIntervention.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)PushbackImmediateIntervention.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/PushbackImmediateIntervention.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 4     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     5/02/05 2:58p Tp151000
 * TAR 299442
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMKVActionBase.h"
#include "ActionBaseRAP.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace rap;
using namespace STATE_MACHINE_NAMESPACE;

class CPushbackImmediateIntervention
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CPushbackImmediateIntervention, IAction>
//    , public CPool<CPushbackImmediateIntervention>
{

public:

    CPushbackImmediateIntervention();
	virtual ~CPushbackImmediateIntervention();
    
    virtual HRESULT Execute(bool *, IMessage &);

private:

    // hide copy and assignment
	CPushbackImmediateIntervention(const CPushbackImmediateIntervention&);
	CPushbackImmediateIntervention& operator = (const CPushbackImmediateIntervention &);
};
