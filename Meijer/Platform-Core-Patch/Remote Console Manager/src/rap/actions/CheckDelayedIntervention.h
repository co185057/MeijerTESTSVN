#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckDelayedIntervention.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)CheckDelayedIntervention.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckDelayedIntervention.h $
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
* 4     12/22/04 10:31a Dm185016
* TAR 288934 - Split into this and SwitchContextDelayedIntervention.
* 
* 3     11/22/04 3:44p Dm185016
* Removed extraneous code
* 
* 2     11/16/04 11:27a Dm185016
* Now allows multiple state switches
* 
* 1     10/29/04 8:53a Dm185016
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

class CCheckDelayedIntervention
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CCheckDelayedIntervention, IAction>
//    , public CPool<CCheckDelayedIntervention>
{

public:

    CCheckDelayedIntervention();
	virtual ~CCheckDelayedIntervention();
    
    virtual HRESULT Execute(bool *, IMessage &);

private:

    // hide copy and assignment
	CCheckDelayedIntervention(const CCheckDelayedIntervention&);
	CCheckDelayedIntervention& operator = (const CCheckDelayedIntervention &);
};
