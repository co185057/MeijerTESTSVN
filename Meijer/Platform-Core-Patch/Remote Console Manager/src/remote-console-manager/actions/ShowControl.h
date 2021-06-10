#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ShowControl.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)ShowControl.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ShowControl.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     2/28/05 4:26p Dm185016
 * lint
 * 
 * 2     2/21/05 8:08p Dm185016
 * TAR 293968
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
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

class CShowControl
    //: public CRCMKVActionBase<CTransactionBegin>
    : public CPSXAction
    , public CCreateable<CShowControl, IAction>
//    , public CPool<CShowControl>
{

public:

    CShowControl();
	virtual ~CShowControl();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SCParms)
        DECLARE_KV_CSTRING(ContextName)
        DECLARE_KV_CSTRING(ControlName)
    DECLARE_KV_HANDLER_CLASS_END

    SCParms m_parms;

private:

    // hide copy and assignment
	CShowControl(const CShowControl&);
	CShowControl& operator = (const CShowControl &);
};

/*lint -restore */