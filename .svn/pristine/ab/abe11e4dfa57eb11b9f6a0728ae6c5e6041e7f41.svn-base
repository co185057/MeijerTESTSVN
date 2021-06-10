#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/DisableButton.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)DisableButton.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/DisableButton.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:35p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
* 
* 2     1/20/05 10:39a Dm185016
* TAR 291218 - Need to be able to specify ALL_CONTEXTS for buttons
* 
* 1     10/29/04 9:04a Dm185016
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

class CDisableButton
    //: public CRCMKVActionBase<CTransactionBegin>
    : public CPSXAction
    , public CCreateable<CDisableButton, IAction>
//    , public CPool<CDisableButton>
{

public:

    CDisableButton();
	virtual ~CDisableButton();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();
    HRESULT NextOperands(CParameterList::iterator &iter, CParameterList::const_iterator &end);

private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(DParms)
        DECLARE_KV_CSTRING(ContextName)
        DECLARE_KV_CSTRING(ButtonName)
    DECLARE_KV_HANDLER_CLASS_END

private:

    // hide copy and assignment
	CDisableButton(const CDisableButton&);
	CDisableButton& operator = (const CDisableButton &);

    DParms m_parms;
};

/*lint -restore */