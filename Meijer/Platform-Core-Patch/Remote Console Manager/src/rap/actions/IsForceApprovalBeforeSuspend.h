#pragma once
// $Header:
/*
 * @(#)IsForceApprovalBeforeSuspend.h 
 *
 * Copyright 2007 by NCR Corporation,
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "Evaluateable.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CIsForceApprovalBeforeSuspend
    : public CRCMActionBase
    , public CActionBaseRAP
    , public IEvaluateable
    , public CCreateable<CIsForceApprovalBeforeSuspend, IAction>
{

public:

    CIsForceApprovalBeforeSuspend();
	virtual ~CIsForceApprovalBeforeSuspend();
    
    virtual HRESULT Execute(bool *, IMessage &);

    virtual long GetValue() const;

    virtual void SetValue(long lNewValue);

private:

    // hide copy and assignment
	CIsForceApprovalBeforeSuspend(const CIsForceApprovalBeforeSuspend&);
	CIsForceApprovalBeforeSuspend& operator = (const CIsForceApprovalBeforeSuspend &);
};
