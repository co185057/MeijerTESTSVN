#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsCurrentTriliteColor.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsCurrentTriliteColor.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsCurrentTriliteColor.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 4     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     5/01/05 8:58a Tp151000
 * Performmance
*/

#include "SMConstants.h"
#include "Createable.h"
#include "KeyValueHandler.h"
#include "ActionBaseRAP.h"
#include "RCMActionBase.h"
#include "RemoteAPMgr.h"
#include "Evaluateable.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CIsCurrentTriliteColor
    : public CRCMActionBase
    , public CActionBaseRAP
    , public IEvaluateable
    , public CCreateable<CIsCurrentTriliteColor, IAction>
{

public:

    CIsCurrentTriliteColor();
	virtual ~CIsCurrentTriliteColor();
    
    virtual HRESULT Execute(bool *, IMessage &);

    //-------------------------------------------------------------------------
    // Methods from IEvaluateable
    //-------------------------------------------------------------------------
    virtual long GetValue() const;

    virtual void SetValue(long);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(ICTCParms)
        DECLARE_KV_INT(Green)
        DECLARE_KV_INT(Yellow)
        DECLARE_KV_INT(Red)
    DECLARE_KV_HANDLER_CLASS_END

    ICTCParms m_parms;

private:

    // hide copy and assignment
	CIsCurrentTriliteColor(const CIsCurrentTriliteColor&);
	CIsCurrentTriliteColor& operator = (const CIsCurrentTriliteColor &);
};
