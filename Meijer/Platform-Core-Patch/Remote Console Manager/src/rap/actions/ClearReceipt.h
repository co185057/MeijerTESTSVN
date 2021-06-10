#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearReceipt.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ClearReceipt.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearReceipt.h $
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
* 2     1/05/05 1:55p Tk124825
* TAR 287909 - Added code to parse total text, total amount, item text &
* Item  value. 
* 
* 1     12/16/04 9:53a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "PSXAction.h"
#include "ActionBaseRAP.h"
#include "Pool.h"
#include "KeyValueHandler.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CClearReceipt
    //: public CRCMKVActionBase<CClearReceipt>
    : public CPSXAction
    , public CActionBaseRAP
    , public CCreateable<CClearReceipt, IAction>
//    , public CPool<CClearReceipt>
{

public:

    CClearReceipt();
	virtual ~CClearReceipt();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

private:
    DECLARE_KV_HANDLER_CLASS_BEGIN(CRParms)
        DECLARE_KV_CSTRING(ItemCountStaticTextControlName);
        DECLARE_KV_CSTRING(TotalAmountStaticTextControlName);
        DECLARE_KV_CSTRING(ItemCountTextControlName);
        DECLARE_KV_CSTRING(TotalAmountTextControlName);
        DECLARE_KV_BOOL(Visible);
    DECLARE_KV_HANDLER_CLASS_END
    // hide copy and assignment
	CClearReceipt(const CClearReceipt&);
	CClearReceipt& operator = (const CClearReceipt &);

    CRParms m_parms;

};
