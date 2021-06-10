#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveImmediateIntervention.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SaveImmediateIntervention.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveImmediateIntervention.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     3/03/05 3:13p Dm185016
 * TAR 291700
 * 
 * 2     2/28/05 4:15p Dm185016
 * lint
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
* 
* 1     10/29/04 8:53a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "KeyValueHandler.h"
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

class CSaveImmediateIntervention
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CSaveImmediateIntervention, IAction>
//    , public CPool<CSaveImmediateIntervention>
{

public:

    CSaveImmediateIntervention();
	virtual ~CSaveImmediateIntervention();
    
    virtual HRESULT Execute(bool *, IMessage &);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SIIParms)
        DECLARE_KV_CSTRING(Type)
    DECLARE_KV_HANDLER_CLASS_END

private:

    // hide copy and assignment
	CSaveImmediateIntervention(const CSaveImmediateIntervention&);
	CSaveImmediateIntervention& operator = (const CSaveImmediateIntervention &);
};
