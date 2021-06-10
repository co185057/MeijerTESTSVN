#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchContextImmediateIntervention.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SwitchContextImmediateIntervention.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchContextImmediateIntervention.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/03/05 3:14p Dm185016
 * TAR 291700
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "GotoActionBase.h"
#include "KeyValueHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CSwitchContextImmediateIntervention
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CGotoActionBase
    , public CCreateable<CSwitchContextImmediateIntervention, IAction>
//    , public CPool<CSwitchContextImmediateIntervention>
{

public:

    CSwitchContextImmediateIntervention();
	virtual ~CSwitchContextImmediateIntervention();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    virtual HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SCIIParms)
        DECLARE_KV_CSTRING(Type)
    DECLARE_KV_HANDLER_CLASS_END

private:

    // hide copy and assignment
	CSwitchContextImmediateIntervention(const CSwitchContextImmediateIntervention&);
	CSwitchContextImmediateIntervention& operator = (const CSwitchContextImmediateIntervention &);
};
