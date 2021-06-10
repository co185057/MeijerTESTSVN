#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveDelayedIntervention.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SaveDelayedIntervention.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveDelayedIntervention.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
* 
* 4     1/12/05 5:51p Dm185016
* TAR 290169
* 
* 3     12/22/04 11:01a Dm185016
* TAR 288934 - Added priority key from SCOTAPP
* 
* 2     12/02/04 3:39p Dm185016
* 
* 1     10/29/04 8:53a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMKVActionBase.h"
#include "ActionBaseRAP.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace rap;
using namespace STATE_MACHINE_NAMESPACE;

class CSaveDelayedIntervention
    : public CRCMKVActionBase<CSaveDelayedIntervention>
    , public CActionBaseRAP
    , public CCreateable<CSaveDelayedIntervention, IAction>
//    , public CPool<CSaveDelayedIntervention>
{

public:

    CSaveDelayedIntervention();
	virtual ~CSaveDelayedIntervention();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_MAP();

    DECLARE_KV_HANDLER(OnMessageName);
    DECLARE_KV_HANDLER(OnType);
    DECLARE_KV_HANDLER(OnPriority);

private:

    // hide copy and assignment
	CSaveDelayedIntervention(const CSaveDelayedIntervention&);
	CSaveDelayedIntervention& operator = (const CSaveDelayedIntervention &);

    StringType m_sType;
    StringType m_sMsgName;
    StringType m_sPriority;

    DWORD m_dwPriority;
};
