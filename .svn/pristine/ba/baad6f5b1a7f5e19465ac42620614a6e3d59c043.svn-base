#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearRAPDataNeededIntervention.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ClearRAPDataNeededIntervention.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearRAPDataNeededIntervention.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     11/10/05 3:26p Tk124825
 * TAR 311902
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
* 
* 2     12/09/04 5:23p Dm185016
* TAR 287903.  Handler to clear rap-data-needed interventions.
* 
* 1     12/02/04 3:33p Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "RemoteAPMgr.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CClearRAPDataNeededIntervention
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CClearRAPDataNeededIntervention, IAction>
//    , public CPool<CClearRAPDataNeededIntervention>
{

public:

    CClearRAPDataNeededIntervention();
	virtual ~CClearRAPDataNeededIntervention();
    
    virtual HRESULT Execute(bool *, IMessage &);

    static bool accept(CIntervention *pIntervention);

protected:
    
    static CClearRAPDataNeededIntervention &crdni() 
    {
        _ASSERT(m_pCurrentInstance != NULL);
        return *m_pCurrentInstance; 
    }

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(CRDNIParms)
        DECLARE_KV_CSTRING(Type)
        DECLARE_KV_INT(Cookie) // TAR 311902
        DECLARE_KV_INT(View)   // TAR 311902
   DECLARE_KV_HANDLER_CLASS_END

private:

    // hide copy and assignment
	CClearRAPDataNeededIntervention(const CClearRAPDataNeededIntervention&);
	CClearRAPDataNeededIntervention& operator = (const CClearRAPDataNeededIntervention &);

    CRDNIParms m_parms;

    static CClearRAPDataNeededIntervention *m_pCurrentInstance;
};
