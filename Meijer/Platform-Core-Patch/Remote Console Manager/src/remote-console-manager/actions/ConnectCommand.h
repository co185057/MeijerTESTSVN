#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ConnectCommand.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ConnectCommand.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ConnectCommand.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:31p Dm185016
 * Alert button support.
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
* 
* 4     12/22/04 11:10a Dm185016
* HH support.  Factored out remote connection data into RemoteConnection
* object.
* 
* 3     12/14/04 11:25a Dm185016
* HH support.
* 
* 2     11/02/04 3:09p Dm185016
* 
* 1     10/29/04 9:04a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "ConnectCommandBase.h"
#include "KeyMessageHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CConnectCommand
    : public CConnectCommandBase
    , public CCreateable<CConnectCommand, IAction>
//    , public CPool<CConnectCommand>
{

public:

    CConnectCommand();
	virtual ~CConnectCommand();

protected:

    virtual StringType GetOperands();

private:

    // hide copy and assignment
	CConnectCommand(const CConnectCommand&);
	CConnectCommand& operator = (const CConnectCommand &);
};
