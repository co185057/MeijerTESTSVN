#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveStartTransactionIntervention.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SaveStartTransactionIntervention.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveStartTransactionIntervention.h $
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
* 1     1/13/05 1:28p Dm185016
* TAR 290304
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
using namespace rap;

class CSaveStartTransactionIntervention
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CSaveStartTransactionIntervention, IAction>
//    , public CPool<CSaveStartTransactionIntervention>
{

public:

    CSaveStartTransactionIntervention();
	virtual ~CSaveStartTransactionIntervention();
    
    virtual HRESULT Execute(bool *, IMessage &);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SSTIParms)
        DECLARE_KV_LONG(View);
        DECLARE_KV_CSTRING(Priority);
    DECLARE_KV_HANDLER_CLASS_END

private:

    // hide copy and assignment
	CSaveStartTransactionIntervention(const CSaveStartTransactionIntervention&);
	CSaveStartTransactionIntervention& operator = (const CSaveStartTransactionIntervention &);

    SSTIParms m_parms;

    DWORD m_dwPriority;
};
