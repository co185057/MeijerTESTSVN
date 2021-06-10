#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ScrollReceipt.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)ScrollReceipt.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ScrollReceipt.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:40p Dm185016
 * lint
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
* 
* 1     12/09/04 1:49p Dm185016
* TAR 287907
*/

/*lint -save -e1516 */

#include "SMConstants.h"
#include "Createable.h"
#include "PSXAction.h"
#include "KeyValueHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CScrollReceipt
    : public CPSXAction
    , public CCreateable<CScrollReceipt, IAction>
//    , public CPool<CScrollReceipt>
{

public:

    CScrollReceipt();
	virtual ~CScrollReceipt();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SIParms)
        DECLARE_KV_CSTRING(ReceiptControlName)
        DECLARE_KV_CSTRING(ScrollType)
    DECLARE_KV_HANDLER_CLASS_END

private:

    // hide copy and assignment
	CScrollReceipt(const CScrollReceipt&);
	CScrollReceipt& operator = (const CScrollReceipt &);

    SIParms m_parms;
};

/*lint -restore */