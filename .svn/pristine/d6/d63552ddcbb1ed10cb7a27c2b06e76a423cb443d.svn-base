#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsInterventionTypeOutstanding.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsInterventionTypeOutstanding.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsInterventionTypeOutstanding.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 8     12/08/06 1:53p Tn185011
 * Fix TAR 339602: 1st suspend from RAP/LaneStatus after FL reboot
 * requires 3 "Suspend" presses
 * 
 * 6     12/05/06 1:47p Jj100011
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/07/05 6:58p Dm185016
 * TAR 293999
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "KeyValueHandler.h"
#include "Pool.h"
#include "Evaluateable.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CIsInterventionTypeOutstanding
    : public CRCMActionBase
    , public CActionBaseRAP
	, public IEvaluateable
    , public CCreateable<CIsInterventionTypeOutstanding, IAction>
//    , public CPool<CIsInterventionTypeOutstanding>
{

public:

    CIsInterventionTypeOutstanding();
	virtual ~CIsInterventionTypeOutstanding();
    
    virtual HRESULT Execute(bool *, IMessage &);
	//-------------------------------------------------------------------------
    // Methods from IEvaluateable
    //-------------------------------------------------------------------------
	virtual long GetValue() const {return 0;}

	virtual void SetValue(long){}

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(IITOParms)
        DECLARE_KV_CSTRING(Type)
    DECLARE_KV_HANDLER_CLASS_END

    IITOParms m_parms;

private:

    // hide copy and assignment
	CIsInterventionTypeOutstanding(const CIsInterventionTypeOutstanding&);
	CIsInterventionTypeOutstanding& operator = (const CIsInterventionTypeOutstanding &);
};
