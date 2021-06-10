#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/EnableInputEvents.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsInputEventEnabled.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/EnableInputEvents.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     2/03/05 5:42p Tp151000
 * TAR 292361
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
* 
* 1     11/16/04 11:23a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "KeyValueHandler.h"
#include "Pool.h"
#include "PSXAction.h"


namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CEnableInputEvents
    //: public CKeyValueHandler<CEnterStoreMode>
    //, public CPSXAction
    : public CPSXAction
    , public CActionBaseRAP
    , public CCreateable<CEnableInputEvents, IAction>
//    , public CPool<CEnableInputEvents>
{

public:

    CEnableInputEvents();
	virtual ~CEnableInputEvents();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(EIEParms)
        DECLARE_KV_BOOL(Enable);
    DECLARE_KV_HANDLER_CLASS_END

private:

    // hide copy and assignment
	CEnableInputEvents(const CEnableInputEvents&);
	CEnableInputEvents& operator = (const CEnableInputEvents &);

    EIEParms m_parms;
};
