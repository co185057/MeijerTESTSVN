#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CanApproveCommand.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)CanApproveCommand.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CanApproveCommand.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/11/05 1:45p Dm185016
 * Moved to rap from rcm
 * 
 * 2     3/11/05 1:43p Dm185016
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
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
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CCanApproveCommand
    : public CRCMKVActionBase<CCanApproveCommand>
    , public CActionBaseRAP
    , public CCreateable<CCanApproveCommand, IAction>
//    , public CPool<CCanApproveCommand>
{

public:

    CCanApproveCommand();
	virtual ~CCanApproveCommand();
    
    virtual HRESULT Execute(bool *, IMessage &);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_MAP();

    DECLARE_KV_HANDLER(OnType);

private:

    IMessage &GetMessage();

    // hide copy and assignment
	CCanApproveCommand(const CCanApproveCommand&);
	CCanApproveCommand& operator = (const CCanApproveCommand &);

    StringType m_szType;

    IMessage *m_pMessage;

    HRESULT m_hr;
};
