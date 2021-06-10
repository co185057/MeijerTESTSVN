#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SetReleaseExclusiveAccessRequired.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SetReleaseExclusiveAccessRequired.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SetReleaseExclusiveAccessRequired.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/07/05 6:58p Dm185016
 * TAR 294609.
 * TAR 294610.
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
using namespace fastlane;

class CSetReleaseExclusiveAccessRequired
    : public CRCMActionBase
    , public CCreateable<CSetReleaseExclusiveAccessRequired, IAction>
//    , public CPool<CSetReleaseExclusiveAccessRequired>
{

public:

    CSetReleaseExclusiveAccessRequired();
	virtual ~CSetReleaseExclusiveAccessRequired();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SREAParms)
        DECLARE_KV_BOOL(ReleaseNeeded);
    DECLARE_KV_HANDLER_CLASS_END

    SREAParms m_parms;

private:

    // hide copy and assignment
	CSetReleaseExclusiveAccessRequired(const CSetReleaseExclusiveAccessRequired&);
	CSetReleaseExclusiveAccessRequired& operator = (const CSetReleaseExclusiveAccessRequired &);
};
