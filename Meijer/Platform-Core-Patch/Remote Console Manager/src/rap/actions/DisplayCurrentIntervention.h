#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayCurrentIntervention.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisplayCurrentIntervention.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
 *
 * Copyright 2006 by NCR Corporation,
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */

#include "SMConstants.h"
#include "Createable.h"
#include "RCMKVActionBase.h"
#include "DisplayInterventionBase.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;


class CDisplayCurrentIntervention
    : public CDisplayInterventionBase
    , public CCreateable<CDisplayCurrentIntervention, IAction>
//    , public CPool<CDisplayCurrentIntervention>
{

public:

    CDisplayCurrentIntervention();
	virtual ~CDisplayCurrentIntervention();

protected:

    virtual CIntervention *GetIntervention();

    virtual bool OverrideDisplay();
    virtual bool IsAddDescriptionToReceipt();

private:

    // hide copy and assignment
	CDisplayCurrentIntervention(const CDisplayCurrentIntervention&);
	CDisplayCurrentIntervention& operator = (const CDisplayCurrentIntervention &);
};
