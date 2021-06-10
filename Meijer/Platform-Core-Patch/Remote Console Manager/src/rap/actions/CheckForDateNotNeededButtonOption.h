#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckForDateNotNeededButtonOption.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)CheckForDateNotNeededButtonOption.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckForDateNotNeededButtonOption.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/28/05 2:20p Tp151000
 * TAR 296896
 * 
 * 1     3/17/05 2:30p Tp151000
 * Add Media Low event
 * 
 * 1     3/11/05 2:41p Dm185016
 * 
 * 2     2/28/05 4:35p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
* 
* 4     12/31/04 9:19a Dm185016
* TAR 288930 - Fixed problem logging enter/exit store mode.  Name of
* transaction variable now read from XML.
* 
* 3     11/16/04 11:38a Dm185016
* Rewritten
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
*/

/*lint -save -e1516 */

#include "SMConstants.h"
#include "Createable.h"
#include "PSXAction.h"
#include "RemoteAPMgr.h"
#include "ActionBaseRAP.h"
#include "RCMActionBase.h"
#include "KeyValueHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CCheckForDateNotNeededButtonOption
    : public CPSXAction
    , public CCreateable<CCheckForDateNotNeededButtonOption, IAction>
//    , public CPool<CCheckForDateNotNeededButtonOption>
{

public:

    CCheckForDateNotNeededButtonOption();
	virtual ~CCheckForDateNotNeededButtonOption();
    
    virtual HRESULT Execute(bool *, IMessage &);

private:

    HRESULT ParseOperands();
// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(CFDNNBOParms)
        DECLARE_KV_BOOL(DOBNotNeeded);
        DECLARE_KV_CSTRING(ButtonName);
    DECLARE_KV_HANDLER_CLASS_END

    CFDNNBOParms m_parms;

private:

    // hide copy and assignment
	CCheckForDateNotNeededButtonOption(const CCheckForDateNotNeededButtonOption&);
	CCheckForDateNotNeededButtonOption& operator = (const CCheckForDateNotNeededButtonOption &);


};

/*lint -restore */