#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsAssistModeEnabled.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsAssistModeEnabled.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsAssistModeEnabled.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     4/15/05 9:19a Dm185016
 * Merge from Patch B
 * 
 * 2     4/14/05 10:09p Dm185016
 * Fixed problem with action not being suitable for <if>
 * 
 * 1     4/07/05 1:44p Dm185016
 * TAR 297347
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

class CIsAssistModeEnabled
    : public CRCMActionBase
    , public CActionBaseRAP
    , public IEvaluateable
    , public CCreateable<CIsAssistModeEnabled, IAction>
//    , public CPool<CIsAssistModeEnabled>
{

public:

    CIsAssistModeEnabled();
	virtual ~CIsAssistModeEnabled();
    
    virtual HRESULT Execute(bool *, IMessage &);

    virtual long GetValue() const;

    virtual void SetValue(long lNewValue);

private:

    // hide copy and assignment
	CIsAssistModeEnabled(const CIsAssistModeEnabled&);
	CIsAssistModeEnabled& operator = (const CIsAssistModeEnabled &);
};
