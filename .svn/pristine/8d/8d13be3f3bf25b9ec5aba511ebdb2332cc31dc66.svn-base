#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckAssistModeControl.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)CheckAssistModeControl.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckAssistModeControl.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     4/05/05 11:56a Dm185016
 * TAR 297356 - Made IEvaluateable for <if>
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
* 
* 1     11/16/04 11:23a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMKVActionBase.h"
#include "ActionBaseRAP.h"
#include "Evaluateable.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CCheckAssistModeControl
    : public CRCMKVActionBase<CCheckAssistModeControl>
    , public CActionBaseRAP
    , public IEvaluateable
    , public CCreateable<CCheckAssistModeControl, IAction>
//    , public CPool<CCheckAssistModeControl>
{

public:

    CCheckAssistModeControl();
	virtual ~CCheckAssistModeControl();
    
    virtual HRESULT Execute(bool *, IMessage &);

    //-------------------------------------------------------------------------
    // Methods from IEvaluateable
    //-------------------------------------------------------------------------
    virtual long GetValue() const;

    virtual void SetValue(long);

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_MAP();

    DECLARE_KV_HANDLER(OnValidValues);

private:

    // hide copy and assignment
	CCheckAssistModeControl(const CCheckAssistModeControl&);
	CCheckAssistModeControl& operator = (const CCheckAssistModeControl &);

    bool m_bRetValue;
};
