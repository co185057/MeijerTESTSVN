#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisableAMOnHandheld.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisableAMOnHandheld.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisableAMOnHandheld.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/14/05 8:26p Dm185016
 * TAR 294587
*/

#include "SMConstants.h"
#include "Createable.h"
#include "PSXAction.h"
#include "KeyValueHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CDisableAMOnHandheld
    : public CPSXAction
    , public CCreateable<CDisableAMOnHandheld, IAction>
//    , public CPool<CDisableAMOnHandheld>
{

public:

    CDisableAMOnHandheld();
	virtual ~CDisableAMOnHandheld();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(DAMOHParms)
        DECLARE_KV_CSTRING(ContextName)
        DECLARE_KV_CSTRING(ButtonName)
    DECLARE_KV_HANDLER_CLASS_END

    DAMOHParms m_parms;

private:

    // hide copy and assignment
	CDisableAMOnHandheld(const CDisableAMOnHandheld&);
	CDisableAMOnHandheld& operator = (const CDisableAMOnHandheld &);
};
