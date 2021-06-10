#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/MakeInterventionCurrent.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)MakeInterventionCurrent.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/MakeInterventionCurrent.h $
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

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "KeyValueHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CMakeInterventionCurrent
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CMakeInterventionCurrent, IAction>
//    , public CPool<CMakeInterventionCurrent>
{

public:

    CMakeInterventionCurrent();
	virtual ~CMakeInterventionCurrent();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(MICParms)
        DECLARE_KV_CSTRING(Type)
    DECLARE_KV_HANDLER_CLASS_END

    MICParms m_parms;

private:

    // hide copy and assignment
	CMakeInterventionCurrent(const CMakeInterventionCurrent&);
	CMakeInterventionCurrent& operator = (const CMakeInterventionCurrent &);
};
