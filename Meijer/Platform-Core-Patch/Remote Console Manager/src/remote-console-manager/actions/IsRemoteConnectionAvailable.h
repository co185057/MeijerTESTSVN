#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsRemoteConnectionAvailable.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)IsRemoteConnectionAvailable.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsRemoteConnectionAvailable.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     2/15/05 11:28a Dm185016
 * TAR 293322.
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CIsRemoteConnectionAvailable
    : public CRCMActionBase
    , public CCreateable<CIsRemoteConnectionAvailable, IAction>
{

public:

    CIsRemoteConnectionAvailable();
	virtual ~CIsRemoteConnectionAvailable();
    
    virtual HRESULT Execute(bool *, IMessage &);

private:

    // hide copy and assignment
	CIsRemoteConnectionAvailable(const CIsRemoteConnectionAvailable&);
	CIsRemoteConnectionAvailable& operator = (const CIsRemoteConnectionAvailable &);
};
