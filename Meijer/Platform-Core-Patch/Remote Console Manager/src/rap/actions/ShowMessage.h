#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ShowMessage.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)ShowMessage.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ShowMessage.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/10/05 5:27p Dm185016
 * Changed to use kv <parameters>
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
* 
* 1     11/16/04 11:23a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "PSXAction.h"
#include "ActionBaseRAP.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CShowMessage
    //: public CRCMKVActionBase<CShowMessage>
    : public CActionBaseRAP
    , public CPSXAction
    , public CCreateable<CShowMessage, IAction>
//    , public CPool<CShowMessage>
{

public:

    CShowMessage();
	virtual ~CShowMessage();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SMParms)
        DECLARE_KV_CSTRING(TransactionVariableName);
        DECLARE_KV_CSTRING(StringId);
    DECLARE_KV_HANDLER_CLASS_END

    SMParms m_parms;

private:

    // hide copy and assignment
	CShowMessage(const CShowMessage&);
	CShowMessage& operator = (const CShowMessage &);
};
