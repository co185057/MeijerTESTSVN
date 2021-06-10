#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/NullAction.h 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)NullAction.h    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/NullAction.h $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
* 
* 4     2/10/04 11:46a Dm185016
* Moved common support to base
* 
* 3     1/27/04 2:03p Dm185016
* DLL Support
* 
* 2     1/16/04 9:15a Dm185016
* Namespace support
*/

#include "SMConstants.h"
#include "Createable.h"
#include "ActionBase.h"

namespace STATE_MACHINE_NAMESPACE
{

class IMessage;

class CNullAction
    : public CActionBase
    , public CCreateable<CNullAction, IAction>
{

public:
	CNullAction();
	virtual ~CNullAction();
    
    virtual HRESULT Execute(bool *, IMessage &);

    virtual int GetId(void) const { return NULL_ACTION; }

    virtual bool IsInternal();

private:

    // hide copy and assignment
	CNullAction(const CNullAction&);
	CNullAction& operator = (const CNullAction &);
};

};
