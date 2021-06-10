#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearLaneStatusWindows.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ClearLaneStatusWindows.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearLaneStatusWindows.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     1/13/06 2:28p Dm185016
 * TAR 301329
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
* 
* 1     12/14/04 11:13a Dm185016
* TAR 288478  Added actions to clear button text
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMKVActionBase.h"
#include "ActionBaseRAP.h"
#include "DisplayControlParser.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;


class CClearLaneStatusWindows
    : public CActionBaseRAP
    , public CDisplayControlParser
    , public CCreateable<CClearLaneStatusWindows, IAction>
//    , public CPool<CClearLaneStatusWindows>
{

public:

    CClearLaneStatusWindows();
	virtual ~CClearLaneStatusWindows();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

private:

    // hide copy and assignment
	CClearLaneStatusWindows(const CClearLaneStatusWindows&);
	CClearLaneStatusWindows& operator = (const CClearLaneStatusWindows &);
};
