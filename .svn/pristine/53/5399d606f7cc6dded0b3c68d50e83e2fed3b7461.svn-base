#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendSuspendRequest.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SendSuspendRequest.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendSuspendRequest.h $
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
* 1     11/30/04 4:18p Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMKVActionBase.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CSendSuspendRequest
    : public CRCMKVActionBase<CSendSuspendRequest>
    , public CCreateable<CSendSuspendRequest, IAction>
//    , public CPool<CSendSuspendRequest>
{

public:

    CSendSuspendRequest();
	virtual ~CSendSuspendRequest();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_MAP();

    DECLARE_KV_HANDLER(OnReason);

private:

    // hide copy and assignment
	CSendSuspendRequest(const CSendSuspendRequest&);
	CSendSuspendRequest& operator = (const CSendSuspendRequest &);

    StringType m_sReason;
};
