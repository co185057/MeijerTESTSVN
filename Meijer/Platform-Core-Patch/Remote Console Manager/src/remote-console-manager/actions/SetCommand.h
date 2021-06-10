#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SetCommand.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)SetCommand.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SetCommand.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
* 
* 2     10/29/04 9:14a Dm185016
* Filled in fxn.
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMKVActionBase.h"
#include "ActionBaseRAP.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace rap;
using namespace STATE_MACHINE_NAMESPACE;

class CSetCommand
    : public CRCMKVActionBase<CSetCommand>
    , public CActionBaseRAP
    , public CCreateable<CSetCommand, IAction>
//    , public CPool<CSetCommand>
{

public:

    CSetCommand();
	virtual ~CSetCommand();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

	HRESULT Set();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_MAP();

    DECLARE_KV_HANDLER(OnId);
    DECLARE_KV_HANDLER(OnValue);

private:

    IMessage &GetMessage();

    // hide copy and assignment
	CSetCommand(const CSetCommand&);
	CSetCommand& operator = (const CSetCommand &);

    StringType m_szId;
    StringType m_szValue;

    IMessage *m_pMessage;

    HRESULT m_hr;
};
