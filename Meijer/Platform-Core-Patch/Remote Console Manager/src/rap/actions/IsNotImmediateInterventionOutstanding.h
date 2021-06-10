#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsNotImmediateInterventionOutstanding.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsNotImmediateInterventionOutstanding.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsNotImmediateInterventionOutstanding.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     5/03/07 1:20p Jj100011
 * Tar 346732
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     2/04/05 9:56a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "Pool.h"
#include "Evaluateable.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CIsNotImmediateInterventionOutstanding
    : public CRCMActionBase
    , public CActionBaseRAP
	, public IEvaluateable
    , public CCreateable<CIsNotImmediateInterventionOutstanding, IAction>
//    , public CPool<CIsNotImmediateInterventionOutstanding>
{

public:

    CIsNotImmediateInterventionOutstanding();
	virtual ~CIsNotImmediateInterventionOutstanding();
	//-------------------------------------------------------------------------
    // Methods from IEvaluateable
    //-------------------------------------------------------------------------
	virtual long GetValue() const {return 0;}

	virtual void SetValue(long){}
    
    virtual HRESULT Execute(bool *, IMessage &);

private:

    // hide copy and assignment
	CIsNotImmediateInterventionOutstanding(const CIsNotImmediateInterventionOutstanding&);
	CIsNotImmediateInterventionOutstanding& operator = (const CIsNotImmediateInterventionOutstanding &);
};
