#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearCurrentIntervention.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ClearCurrentIntervention.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearCurrentIntervention.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 4     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     4/15/05 9:18a Dm185016
 * Merge from Patch B
 * 
 * 1     4/14/05 10:11p Dm185016
 * TAR 298021
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CClearCurrentIntervention
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CClearCurrentIntervention, IAction>
//    , public CPool<CClearCurrentIntervention>
{

public:

    CClearCurrentIntervention();
	virtual ~CClearCurrentIntervention();
    
    virtual HRESULT Execute(bool *, IMessage &);

private:

    // hide copy and assignment
	CClearCurrentIntervention(const CClearCurrentIntervention&);
	CClearCurrentIntervention& operator = (const CClearCurrentIntervention &);
};
