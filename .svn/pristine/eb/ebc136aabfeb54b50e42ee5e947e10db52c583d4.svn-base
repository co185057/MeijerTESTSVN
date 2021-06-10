#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/Append0.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)Append0.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/Append0.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     3/16/05 11:01a Dm185016
 * TAR 295691 - Transaction variable name now obtained from the XML
 * 
 * 2     2/28/05 4:06p Dm185016
 * lint
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
* 
* 1     11/16/04 11:23a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "ActionBaseRAP.h"
#include "PSXAction.h"
#include "KeyValueHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CAppend0
    : public CActionBaseRAP
    , public CPSXAction
    , public CCreateable<CAppend0, IAction>
//    , public CPool<CAppend0>
{

public:

    CAppend0();
	virtual ~CAppend0();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(A0Parms)
        DECLARE_KV_CSTRING(TransactionVariableName);
    DECLARE_KV_HANDLER_CLASS_END

    A0Parms m_parms;

private:

    // hide copy and assignment
	CAppend0(const CAppend0&);
	CAppend0& operator = (const CAppend0 &);
};
