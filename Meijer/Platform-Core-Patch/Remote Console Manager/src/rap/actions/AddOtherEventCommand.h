#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/AddOtherEventCommand.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)AddOtherEventCommand.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/AddOtherEventCommand.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/11/05 2:39p Dm185016
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
* 
* 1     10/29/04 9:04a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMKVActionBase.h"
#include "KeyMessageHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CAddOtherEventCommand
    : public CRCMKVActionBase<CAddOtherEventCommand>
    , public CKeyMessageHandler<CAddOtherEventCommand>
    , public CCreateable<CAddOtherEventCommand, IAction>
//    , public CPool<CAddOtherEventCommand>
{

public:

    friend class CKeyMessageHandler<CAddOtherEventCommand>;

    CAddOtherEventCommand();
	virtual ~CAddOtherEventCommand();
    
    virtual HRESULT Execute(bool *, IMessage &);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_MAP();

    DECLARE_KV_HANDLER(OnDescription);

// This section contains the mapping of key value to message name
private:

    DECLARE_KEY_MSG_MAP();

private:

    IMessage &GetMessage();

    // hide copy and assignment
	CAddOtherEventCommand(const CAddOtherEventCommand&);
	CAddOtherEventCommand& operator = (const CAddOtherEventCommand &);

    IMessage *m_pMessage;

    HRESULT m_hr;
};
