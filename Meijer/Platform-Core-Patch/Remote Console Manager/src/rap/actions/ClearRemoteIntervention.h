#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearRemoteIntervention.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ClearRemoteIntervention.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
 *
 * Copyright 2006 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearRemoteIntervention.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 3     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/24/06 10:38a Dm185016
 * TAR 315980
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "KeyValueHandler.h"
#include "Action.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

namespace rap 
{
    typedef IMessage CIntervention;
}
using namespace rap;

class CClearRemoteIntervention
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CClearRemoteIntervention, IAction>
//    , public CPool<CClearRemoteIntervention>
{

public:

    CClearRemoteIntervention();
	virtual ~CClearRemoteIntervention();
    
    virtual HRESULT Execute(bool *, IMessage &);

    static bool accept(IAction *, const CIntervention *);

protected:

    HRESULT ParseInterventionData(const CIntervention *);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(CCCIParms)
        DECLARE_KV_INT(Cookie) // TAR 311902
   DECLARE_KV_HANDLER_CLASS_END

    CCCIParms m_parms;
    CCCIParms m_intervention;

private:

    // hide copy and assignment
	CClearRemoteIntervention(const CClearRemoteIntervention&);
	CClearRemoteIntervention& operator = (const CClearRemoteIntervention &);
};
