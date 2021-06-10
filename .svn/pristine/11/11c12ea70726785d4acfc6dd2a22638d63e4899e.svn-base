#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsButtonEnabled.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)IsButtonEnabled.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsButtonEnabled.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 4     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     5/01/05 9:00a Tp151000
 * Performmance
*/

/*lint -save -e1516 */

#include "SMConstants.h"
#include "Createable.h"
#include "PSXAction.h"
#include "KeyValueHandler.h"
#include "Evaluateable.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CIsButtonEnabled
    : public CPSXAction
    , public IEvaluateable
    , public CCreateable<CIsButtonEnabled, IAction>
{

public:

    CIsButtonEnabled();
	virtual ~CIsButtonEnabled();
    
    virtual HRESULT Execute(bool *, IMessage &);

    //-------------------------------------------------------------------------
    // Methods from IEvaluateable
    //-------------------------------------------------------------------------
    virtual long GetValue() const;

    virtual void SetValue(long);

protected:

    HRESULT ParseOperands();

private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(IBEParms)
        DECLARE_KV_CSTRING(ContextName)
        DECLARE_KV_CSTRING(ButtonName)
    DECLARE_KV_HANDLER_CLASS_END

    IBEParms m_parms;

private:

    // hide copy and assignment
	CIsButtonEnabled(const CIsButtonEnabled&);
	CIsButtonEnabled& operator = (const CIsButtonEnabled &);
};

/*lint -restore */