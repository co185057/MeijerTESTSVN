#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ConnectCommandComplete.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ConnectCommandComplete.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ConnectCommandComplete.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/15/05 3:44p Dm185016
 * TAR 295676 - AcceptConnection is now handled by a separate thread to
 * prevent hanging the state machine thread.
*/

#include "SMConstants.h"
#include "RCMActionBase.h"
#include "Createable.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CConnectCommandComplete
    : public CRCMActionBase
    , public CCreateable<CConnectCommandComplete, IAction>
{

public:

    CConnectCommandComplete();
	virtual ~CConnectCommandComplete();
    
    virtual HRESULT Execute(bool *, IMessage &);

private:

    // hide copy and assignment
	CConnectCommandComplete(const CConnectCommandComplete&);
	CConnectCommandComplete& operator = (const CConnectCommandComplete &);
};
