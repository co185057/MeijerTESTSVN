#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsExclusiveAccessGranted.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsExclusiveAccessGranted.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsExclusiveAccessGranted.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     4/21/05 7:21p Dm185016
 * Performance for HH
 * 
 * 1     3/07/05 6:58p Dm185016
 * TAR 294609.
 * TAR 294610.
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "Evaluateable.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CIsExclusiveAccessGranted
    : public CRCMActionBase
    , public CCreateable<CIsExclusiveAccessGranted, IAction>
    , public IEvaluateable
//    , public CPool<CIsExclusiveAccessGranted>
{

public:

    CIsExclusiveAccessGranted();
	virtual ~CIsExclusiveAccessGranted();
    
    virtual HRESULT Execute(bool *, IMessage &);

    //-------------------------------------------------------------------------
    // Methods from IEvaluateable
    //-------------------------------------------------------------------------
    virtual long GetValue() const;

    virtual void SetValue(long);

private:

    // hide copy and assignment
	CIsExclusiveAccessGranted(const CIsExclusiveAccessGranted&);
	CIsExclusiveAccessGranted& operator = (const CIsExclusiveAccessGranted &);
};
