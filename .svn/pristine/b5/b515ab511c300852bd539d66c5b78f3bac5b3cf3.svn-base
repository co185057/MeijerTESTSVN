#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsCurrentRemoteConnection.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)IsCurrentRemoteConnection.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsCurrentRemoteConnection.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     3/28/05 5:04p Dm185016
 * TAR 296771 - Made IEvaluateable for use in <if
 * expression="IsCurrentRemoteConnection()">
 * 
 * 1     3/07/05 6:58p Dm185016
*/

/*lint -save -e1516 */

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "KeyValueHandler.h"
#include "Evaluateable.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CIsCurrentRemoteConnection
    : public CRCMActionBase
    , public IEvaluateable
    , public CCreateable<CIsCurrentRemoteConnection, IAction>
{

public:

    CIsCurrentRemoteConnection();
	virtual ~CIsCurrentRemoteConnection();
    
    virtual HRESULT Execute(bool *, IMessage &);

    //-------------------------------------------------------------------------
    // Methods from IEvaluateable
    //-------------------------------------------------------------------------
    virtual long GetValue() const;

    virtual void SetValue(long);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(ICRCParms)
        DECLARE_KV_CSTRING(RemoteConnectionName)
    DECLARE_KV_HANDLER_CLASS_END

    ICRCParms m_parms;

private:

    // hide copy and assignment
	CIsCurrentRemoteConnection(const CIsCurrentRemoteConnection&);
	CIsCurrentRemoteConnection& operator = (const CIsCurrentRemoteConnection &);
};

/*lint -restore */