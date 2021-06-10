#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/MatchLaneContext.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)MatchLaneContext.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/MatchLaneContext.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 3     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     5/31/05 11:33a Dm185016
 * TAR 301203
*/

#include "rcmapi.h"
#include "SMConstants.h"
#include "Createable.h"
#include "Evaluateable.h"
#include "KeyValueHandler.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "Pool.h"
#include "Message.h"

using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

namespace fastlane
{

class RCMANAGER_API CMatchLaneContext
    : virtual public CRCMActionBase
    , public CActionBaseRAP
    , public IEvaluateable
    , public CCreateable<CMatchLaneContext, IAction>
//    , public CPool<CMatchLaneContext>
{

public:
	CMatchLaneContext();
	virtual ~CMatchLaneContext();
    
    virtual HRESULT Execute(bool *, IMessage &);

    //-------------------------------------------------------------------------
    // Methods from IEvaluateable
    //-------------------------------------------------------------------------
    virtual long GetValue() const { return 0; }

    virtual void SetValue(long){}

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SLCCParms)
        DECLARE_KV_CSTRING(Context);
    DECLARE_KV_HANDLER_CLASS_END

private:

    // hide copy and assignment
	CMatchLaneContext(const CMatchLaneContext&);
	CMatchLaneContext& operator = (const CMatchLaneContext &);

    SLCCParms m_parms;
};

}
