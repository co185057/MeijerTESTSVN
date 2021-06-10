#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearAllInterventions.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ClearAllInterventions.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearAllInterventions.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 3     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     5/31/05 11:33a Dm185016
 * TAR 301203
 * 
 * 1     5/19/05 5:19p Dm185016
 * TAR 300190
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RemoteAPMgr.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;
using namespace fastlane;

class CClearAllInterventions
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CClearAllInterventions, IAction>
//    , public CPool<CClearAllInterventions>
{

public:

    CClearAllInterventions();
	virtual ~CClearAllInterventions();
    
    virtual HRESULT Execute(bool *, IMessage &);

    static bool accept(IAction *, const CIntervention *pIntervention);
    //static bool accept(CClearAllInterventions &, CIntervention *pIntervention);

private:

    // hide copy and assignment
	CClearAllInterventions(const CClearAllInterventions&);
	CClearAllInterventions& operator = (const CClearAllInterventions &);
};
