#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ConnectCommandBase.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ConnectCommandBase.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ConnectCommandBase.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     2/25/05 5:41p Tk124825
 * Initial Version
*/

#include "SMConstants.h"
#include "RCMActionBase.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CConnectCommandBase
    : public CRCMActionBase
{

public:

    CConnectCommandBase();
	virtual ~CConnectCommandBase();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    virtual StringType GetOperands()=0;

private:

    // hide copy and assignment
	CConnectCommandBase(const CConnectCommandBase&);
	CConnectCommandBase& operator = (const CConnectCommandBase &);
};
