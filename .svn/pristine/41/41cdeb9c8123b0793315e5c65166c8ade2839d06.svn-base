#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ConnectCommandRAP.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ConnectCommandRAP.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ConnectCommandRAP.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     2/25/05 5:16p Tk124825
 * Initial version
*/

#include "SMConstants.h"
#include "Createable.h"
#include "ConnectCommandBase.h"
#include "ActionBaseRAP.h"
#include "KeyMessageHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CConnectCommandRAP
    : public CConnectCommandBase
    , public CActionBaseRAP
    , public CCreateable<CConnectCommandRAP, IAction>
//    , public CPool<CConnectCommandRAP>
{

public:

    CConnectCommandRAP();
	virtual ~CConnectCommandRAP();

protected:

    virtual StringType GetOperands();

private:

    // hide copy and assignment
	CConnectCommandRAP(const CConnectCommandRAP&);
	CConnectCommandRAP& operator = (const CConnectCommandRAP &);
};
