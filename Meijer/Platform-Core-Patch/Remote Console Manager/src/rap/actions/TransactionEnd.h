#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/TransactionEnd.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)TransactionEnd.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/TransactionEnd.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/11/05 2:46p Dm185016
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
* 
* 3     12/02/04 3:44p Dm185016
* Now saving status of transaction in RemoteAPMgr.
* 
* 2     10/29/04 9:16a Dm185016
* Filled in fxn.
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
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

class CTransactionEnd
    //: public CRCMKVActionBase<CTransactionBegin>
    : public CPSXAction
    , public CActionBaseRAP
    , public CCreateable<CTransactionEnd, IAction>
//    , public CPool<CTransactionEnd>
{

public:

    CTransactionEnd();
	virtual ~CTransactionEnd();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

private:

    // hide copy and assignment
	CTransactionEnd(const CTransactionEnd&);
	CTransactionEnd& operator = (const CTransactionEnd &);
};
