#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsNotConfirmSuspend.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsNotConfirmSuspend.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsNotConfirmSuspend.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     1/11/07 9:18a Jj100011
 * 339602
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     2/08/05 11:03a Tp151000
 * 290952
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
* 
* 1     11/16/04 11:23a Dm185016
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

class CIsNotConfirmSuspend
    : public CRCMActionBase
    , public CActionBaseRAP
	, public IEvaluateable
    , public CCreateable<CIsNotConfirmSuspend, IAction>
//    , public CPool<CIsNotConfirmSuspend>
{

public:

    CIsNotConfirmSuspend();
	virtual ~CIsNotConfirmSuspend();
    
    virtual HRESULT Execute(bool *, IMessage &);
	//-------------------------------------------------------------------------
    // Methods from IEvaluateable
    //-------------------------------------------------------------------------
	virtual long GetValue() const {return 0;}

	virtual void SetValue(long){}

private:

    // hide copy and assignment
	CIsNotConfirmSuspend(const CIsNotConfirmSuspend&);
	CIsNotConfirmSuspend& operator = (const CIsNotConfirmSuspend &);
};
