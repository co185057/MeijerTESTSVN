#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchLaneContextCommand.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SwitchLaneContextCommand.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SwitchLaneContextCommand.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
* 
* 1     1/20/05 10:35a Dm185016
* Added handler for lane context switches.
*/

#include "rcmapi.h"
#include "SMConstants.h"
#include "Createable.h"
#include "KeyValueHandler.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "Pool.h"
#include "Message.h"

using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

namespace fastlane
{

class RCMANAGER_API CSwitchLaneContextCommand
    : virtual public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CSwitchLaneContextCommand, IAction>
//    , public CPool<CSwitchLaneContextCommand>
{

public:
	CSwitchLaneContextCommand();
	virtual ~CSwitchLaneContextCommand();
    
    virtual HRESULT Execute(bool *, IMessage &);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SLCCParms)
        DECLARE_KV_CSTRING(Context);
    DECLARE_KV_HANDLER_CLASS_END

private:

    // hide copy and assignment
	CSwitchLaneContextCommand(const CSwitchLaneContextCommand&);
	CSwitchLaneContextCommand& operator = (const CSwitchLaneContextCommand &);

    SLCCParms m_parms;
};

}
