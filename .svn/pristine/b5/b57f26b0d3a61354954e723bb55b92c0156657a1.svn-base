#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearApprovedIntervention.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ClearApprovedIntervention.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearApprovedIntervention.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     6/23/06 8:33p Lt185007
 * tar migration 300190
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
* 
* 1     12/09/04 1:45p Dm185016
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

class CClearApprovedIntervention
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CClearApprovedIntervention, IAction>
//    , public CPool<CClearApprovedIntervention>
{

public:

    CClearApprovedIntervention();
	virtual ~CClearApprovedIntervention();
    
    virtual HRESULT Execute(bool *, IMessage &);

    static bool accept(IAction *, const CIntervention *pIntervention);
    //static bool accept(CClearApprovedIntervention &, CIntervention *pIntervention);

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(CAIParms)
        DECLARE_KV_CSTRING(Type)
        DECLARE_KV_INT(Cookie)
    DECLARE_KV_HANDLER_CLASS_END

    CAIParms m_parms;

private:

    // hide copy and assignment
	CClearApprovedIntervention(const CClearApprovedIntervention&);
	CClearApprovedIntervention& operator = (const CClearApprovedIntervention &);

    void SignalApproval();
    StringType m_ApprovalType;
};
