#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ControlCommand.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ControlCommand.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ControlCommand.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     3/08/05 10:26a Dm185016
 * 
 * 2     2/28/05 4:35p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "KeyValueHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CControlCommand
    : public CRCMActionBase
    , public CCreateable<CControlCommand, IAction>
//    , public CPool<CControlCommand>
{

public:

    CControlCommand();
	virtual ~CControlCommand();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:
    
    virtual HRESULT ParseOperands();

    HRESULT SetError(IMessage &, HRESULT);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(CCParms)
        DECLARE_KV_CSTRING(Operation)
        DECLARE_KV_CSTRING(LookupTableName)
        DECLARE_KV_CSTRING(VariableName)
    DECLARE_KV_HANDLER_CLASS_END

    CCParms m_parms;

private:

    // hide copy and assignment
	CControlCommand(const CControlCommand&);
	CControlCommand& operator = (const CControlCommand &);
};
