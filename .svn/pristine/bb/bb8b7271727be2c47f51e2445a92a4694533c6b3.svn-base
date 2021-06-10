#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveAsDelayedIntervention.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SaveAsDelayedIntervention.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveAsDelayedIntervention.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     6/24/05 1:39p Dm185016
 * Added capability for customer created rap-data-needed contexts.
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
* 
* 4     12/22/04 11:01a Dm185016
* TAR 288934 - Added priority key from SCOTAPP
* 
* 3     11/30/04 4:21p Dm185016
* Added handling for cancel generic transactions.
* 
* 2     11/22/04 3:59p Dm185016
* Removed extraneous code
* 
* 1     11/18/04 1:37p Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "KeyValueHandler.h"
#include "ActionBaseRAP.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CSaveAsDelayedIntervention
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CSaveAsDelayedIntervention, IAction>
//    , public CPool<CSaveAsDelayedIntervention>
{

public:

    CSaveAsDelayedIntervention();
	virtual ~CSaveAsDelayedIntervention();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    virtual IMessage & HandleCancelTransaction(IMessage &message);

protected:

    virtual HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SADIParms)
        DECLARE_KV_INT(View)
        DECLARE_KV_CSTRING(LookupTableName)
        DECLARE_KV_CSTRING(Priority)
        DECLARE_KV_CSTRING(ViewText)
        DECLARE_KV_CSTRING(MessageName)
        DECLARE_KV_CSTRING(Section)
    DECLARE_KV_HANDLER_CLASS_END

    SADIParms m_parms;

    DWORD GetPriority();
    
    HRESULT GetUserMessage(IMessage **ppMsg,  IMessage &originalMessage);

private:

    // hide copy and assignment
	CSaveAsDelayedIntervention(const CSaveAsDelayedIntervention&);
	CSaveAsDelayedIntervention& operator = (const CSaveAsDelayedIntervention &);
};
