#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchContextRAPDataNeededIntervention.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SwitchContextRAPDataNeededIntervention.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchContextRAPDataNeededIntervention.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     6/24/05 1:39p Dm185016
 * Added capability for customer created rap-data-needed contexts.
 * 
 * 1     4/15/05 9:18a Dm185016
 * Merge from Patch B
 * 
 * 1     4/14/05 10:11p Dm185016
 * TAR 298021
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

class CSwitchContextRAPDataNeededIntervention
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CGotoActionBase
    , public CCreateable<CSwitchContextRAPDataNeededIntervention, IAction>
//    , public CPool<CSwitchContextRAPDataNeededIntervention>
{

public:

    CSwitchContextRAPDataNeededIntervention();
	virtual ~CSwitchContextRAPDataNeededIntervention();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    virtual HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SCRDNIParms)
        DECLARE_KV_CSTRING(LookupTableName)
        DECLARE_KV_INT(View)
        DECLARE_KV_CSTRING(Priority)
        DECLARE_KV_CSTRING(MessageName)
        DECLARE_KV_CSTRING(StateName)
    DECLARE_KV_HANDLER_CLASS_END

    SCRDNIParms m_parms;

private:

    // hide copy and assignment
	CSwitchContextRAPDataNeededIntervention(const CSwitchContextRAPDataNeededIntervention&);
	CSwitchContextRAPDataNeededIntervention& operator = (const CSwitchContextRAPDataNeededIntervention &);
};
