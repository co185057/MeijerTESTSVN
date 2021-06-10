#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsRemoteConnectionInputEnabled.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)IsRemoteConnectionInputEnabled.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsRemoteConnectionInputEnabled.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/30/05 5:06p Dm185016
 * TAR 297001
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

class CIsRemoteConnectionInputEnabled
    : public CRCMActionBase
    , public CCreateable<CIsRemoteConnectionInputEnabled, IAction>
//    , public CPool<CIsRemoteConnectionInputEnabled>
{

public:

    CIsRemoteConnectionInputEnabled();
	virtual ~CIsRemoteConnectionInputEnabled();
    
    virtual HRESULT Execute(bool *, IMessage &);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(IRCIEParms)
        DECLARE_KV_CSTRING(RemoteConnectionName)
    DECLARE_KV_HANDLER_CLASS_END

    IRCIEParms m_parms;

private:

    // hide copy and assignment
	CIsRemoteConnectionInputEnabled(const CIsRemoteConnectionInputEnabled&);
	CIsRemoteConnectionInputEnabled& operator = (const CIsRemoteConnectionInputEnabled &);
};
