#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/GenerateLocalButtonPress.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)GenerateLocalButtonPress.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/GenerateLocalButtonPress.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
* 
* 2     12/16/04 9:56a Dm185016
* Rewritten to use a KV list for the parameters.  Support for Suspend
* Transaction.
* 
* 1     11/16/04 11:23a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "PSXAction.h"
#include "ActionBaseRAP.h"
#include "KeyValueHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CGenerateLocalButtonPress
    //: public CRCMKVActionBase<CTransactionBegin>
    : public CPSXAction
    , public CActionBaseRAP
    , public CCreateable<CGenerateLocalButtonPress, IAction>
//    , public CPool<CGenerateLocalButtonPress>
{

public:

    CGenerateLocalButtonPress();
	virtual ~CGenerateLocalButtonPress();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(GLBPParms)
        DECLARE_KV_CSTRING(ButtonName);
        DECLARE_KV_CSTRING(ContextName);
    DECLARE_KV_HANDLER_CLASS_END

private:

    // hide copy and assignment
	CGenerateLocalButtonPress(const CGenerateLocalButtonPress&);
	CGenerateLocalButtonPress& operator = (const CGenerateLocalButtonPress &);

    StringType m_sButtonName;
    StringType m_sContextName;

    GLBPParms m_parms;
};
