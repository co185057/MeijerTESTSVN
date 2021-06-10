#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsHandheldAssistModeAllowed.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsHandheldAssistModeAllowed.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsHandheldAssistModeAllowed.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     4/05/05 11:53a Dm185016
 * TAR 297356
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

class CIsHandheldAssistModeAllowed
    : public CRCMActionBase
    , public CActionBaseRAP
    , public IEvaluateable
    , public CCreateable<CIsHandheldAssistModeAllowed, IAction>
//    , public CPool<CIsHandheldAssistModeAllowed>
{

public:

    CIsHandheldAssistModeAllowed();
	virtual ~CIsHandheldAssistModeAllowed();
    
    virtual HRESULT Execute(bool *, IMessage &);

    //-------------------------------------------------------------------------
    // Methods from IEvaluateable
    //-------------------------------------------------------------------------
    virtual long GetValue() const;

    virtual void SetValue(long);

private:

    // hide copy and assignment
	CIsHandheldAssistModeAllowed(const CIsHandheldAssistModeAllowed&);
	CIsHandheldAssistModeAllowed& operator = (const CIsHandheldAssistModeAllowed &);
};
