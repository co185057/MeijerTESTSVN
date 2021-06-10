#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/AcquireExclusiveAccess.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)AcquireExclusiveAccess.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/AcquireExclusiveAccess.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:18p Dm185016
 * Moved to new Repository.
* 
* 1     1/13/05 1:28p Dm185016
* TAR 290304
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CAcquireExclusiveAccess
    : public CRCMActionBase
    , public CCreateable<CAcquireExclusiveAccess, IAction>
//    , public CPool<CAcquireExclusiveAccess>
{

public:

    CAcquireExclusiveAccess();
	virtual ~CAcquireExclusiveAccess();
    
    virtual HRESULT Execute(bool *, IMessage &);

private:

    // hide copy and assignment
	CAcquireExclusiveAccess(const CAcquireExclusiveAccess&);
	CAcquireExclusiveAccess& operator = (const CAcquireExclusiveAccess &);
};
