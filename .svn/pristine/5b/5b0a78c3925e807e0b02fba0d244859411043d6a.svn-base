#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/DisconnectCommand.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)DisconnectCommand.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/DisconnectCommand.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     2/28/05 4:35p Dm185016
 * lint
 * 
 * 2     2/18/05 4:35p Dm185016
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
* 
* 1     10/29/04 9:04a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "ReleaseExclusiveAccessBase.h"
#include "KeyMessageHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CDisconnectCommand
    : public CReleaseExclusiveAccessBase
    , public CCreateable<CDisconnectCommand, IAction>
//    , public CPool<CDisconnectCommand>
{

public:

    CDisconnectCommand();
	virtual ~CDisconnectCommand();
    
    virtual HRESULT Execute(bool *, IMessage &);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(DParms)
        DECLARE_KV_CSTRING(RemoteConnectionName)
    DECLARE_KV_HANDLER_CLASS_END

    DParms m_parms;

private:

    // hide copy and assignment
	CDisconnectCommand(const CDisconnectCommand&);
	CDisconnectCommand& operator = (const CDisconnectCommand &);
};
