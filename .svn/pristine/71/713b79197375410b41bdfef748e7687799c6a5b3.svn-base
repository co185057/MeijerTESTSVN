#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayNextDelayedIntervention.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisplayNextDelayedIntervention.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayNextDelayedIntervention.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
* 
* 2     1/12/05 5:50p Dm185016
* TAR 289897
* 
* 1     12/22/04 10:35a Dm185016
* TAR 288934 - Action to display the next intervention at the head of the
* delayed intervention Q
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


class CDisplayNextDelayedIntervention
    : public CDisplayInterventionBase
    , public CCreateable<CDisplayNextDelayedIntervention, IAction>
//    , public CPool<CDisplayNextDelayedIntervention>
{

public:

    CDisplayNextDelayedIntervention();
	virtual ~CDisplayNextDelayedIntervention();

protected:

    virtual CIntervention *GetIntervention();

    virtual bool OverrideDisplay();
    virtual bool IsAddDescriptionToReceipt();

private:

    // hide copy and assignment
	CDisplayNextDelayedIntervention(const CDisplayNextDelayedIntervention&);
	CDisplayNextDelayedIntervention& operator = (const CDisplayNextDelayedIntervention &);
};
