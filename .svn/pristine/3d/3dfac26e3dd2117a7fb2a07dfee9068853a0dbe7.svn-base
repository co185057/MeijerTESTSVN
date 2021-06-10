#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsInterventionOutstanding.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsInterventionOutstanding.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsInterventionOutstanding.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     5/04/05 6:42p Dm185016
 * TAR 299908
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
* 
* 1     12/14/04 11:13a Dm185016
* TAR 288478  Added actions to clear button text
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "Evaluateable.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CIsInterventionOutstanding
    : public CRCMActionBase
    , public CActionBaseRAP
    , public IEvaluateable
    , public CCreateable<CIsInterventionOutstanding, IAction>
//    , public CPool<CIsInterventionOutstanding>
{

public:

    CIsInterventionOutstanding();
	virtual ~CIsInterventionOutstanding();
    
    virtual HRESULT Execute(bool *, IMessage &);

    inline long GetValue() const { return 0; }

    inline void SetValue(long) {}

private:

    // hide copy and assignment
	CIsInterventionOutstanding(const CIsInterventionOutstanding&);
	CIsInterventionOutstanding& operator = (const CIsInterventionOutstanding &);
};
