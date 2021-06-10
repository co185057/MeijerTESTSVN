#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsNotHandheld.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)IsNotHandheld.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsNotHandheld.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     3/02/05 4:51p Dm185016
 * TAR 294890.
 * 
 * 1     2/18/05 11:26a Dm185016
 * TAR 293795.
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

class CIsNotHandheld
    : public CRCMActionBase
    , public CCreateable<CIsNotHandheld, IAction>
//    , public CPool<CIsNotHandheld>
{

public:

    CIsNotHandheld();
	virtual ~CIsNotHandheld();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(INHParms)
        DECLARE_KV_BOOL(UseMessage)
        DECLARE_KV_CSTRING(RemoteConnectionName)
    DECLARE_KV_HANDLER_CLASS_END

    INHParms m_parms;

private:

    // hide copy and assignment
	CIsNotHandheld(const CIsNotHandheld&);
	CIsNotHandheld& operator = (const CIsNotHandheld &);
};
