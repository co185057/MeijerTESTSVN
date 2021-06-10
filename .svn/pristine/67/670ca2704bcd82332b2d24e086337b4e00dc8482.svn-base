#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SaveRemoteConnection.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)SaveRemoteConnection.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SaveRemoteConnection.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     3/29/05 11:45a Dm185016
 * Problem:  HHRAP attempted to connect.  Connection timed out after 5
 * seconds but PSX kept connection alive anyway.  When HHRAP pressed
 * button after, RCM did not ignore as it should.  Changed to return
 * false, if the remote connection object does not exist for this
 * connection.
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
* 
* 1     12/22/04 11:14a Dm185016
* HH support.  Saves the remote connection in RCManager.
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "KeyValueHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CSaveRemoteConnection
    : public CRCMActionBase
    , public CCreateable<CSaveRemoteConnection, IAction>
//    , public CPool<CSaveRemoteConnection>
{

public:

    CSaveRemoteConnection();
	virtual ~CSaveRemoteConnection();
    
    virtual HRESULT Execute(bool *, IMessage &);

private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SRCParms)
        DECLARE_KV_CSTRING(RemoteConnectionName)
    DECLARE_KV_HANDLER_CLASS_END

    SRCParms m_parms;

private:

    // hide copy and assignment
	CSaveRemoteConnection(const CSaveRemoteConnection&);
	CSaveRemoteConnection& operator = (const CSaveRemoteConnection &);
};
