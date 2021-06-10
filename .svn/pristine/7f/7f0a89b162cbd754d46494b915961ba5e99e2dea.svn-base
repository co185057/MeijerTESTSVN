#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/EventCommand.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)EventCommand.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/EventCommand.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     3/14/05 8:27p Dm185016
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
#include "KeyMessageHandler.h"
#include "KeyValueHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CEventCommand
    : public CRCMActionBase
    , public CCreateable<CEventCommand, IAction>
//    , public CPool<CEventCommand>
{

public:

    CEventCommand();
	virtual ~CEventCommand();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(ECParms)
        DECLARE_KV_CSTRING(Type)
        DECLARE_KV_CSTRING(LookupTableName)
    DECLARE_KV_HANDLER_CLASS_END

    ECParms m_parms;

private:

    // hide copy and assignment
	CEventCommand(const CEventCommand&);
	CEventCommand& operator = (const CEventCommand &);

    StringType m_szType;
};
