#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsHandheld.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)IsHandheld.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/IsHandheld.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     5/04/05 6:43p Dm185016
 * TAR 299908
 * 
 * 2     3/02/05 5:26p Dm185016
 * TAR 294890.
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
* 
* 1     12/22/04 11:13a Dm185016
* HH support.  action returns true if current remote connection is
* handheld.
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "KeyValueHandler.h"
#include "Evaluateable.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CIsHandheld
    : public CRCMActionBase
    , public IEvaluateable
    , public CCreateable<CIsHandheld, IAction>
//    , public CPool<CIsHandheld>
{

public:

    CIsHandheld();
	virtual ~CIsHandheld();
    
    virtual HRESULT Execute(bool *, IMessage &);

    inline long GetValue() const { return 0; }

    inline void SetValue(long) {}

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(IHParms)
        DECLARE_KV_BOOL(UseMessage)
        DECLARE_KV_CSTRING(RemoteConnectionName)
    DECLARE_KV_HANDLER_CLASS_END

    IHParms m_parms;

private:

    // hide copy and assignment
	CIsHandheld(const CIsHandheld&);
	CIsHandheld& operator = (const CIsHandheld &);
};
