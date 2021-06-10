#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckRAPDataNeededEvent.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)CheckRAPDataNeededEvent.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckRAPDataNeededEvent.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 5     6/24/05 1:38p Dm185016
 * Added capability for customer created rap-data-needed contexts.
 * 
 * 3     4/15/05 9:19a Dm185016
 * Merge from Patch B
 * 
 * 2     4/14/05 10:06p Dm185016
 * TAR 298021
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
* 
* 2     11/19/04 1:35p Dm185016
* Removed extraneous code.
* 
* 1     11/18/04 1:37p Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "KeyValueHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;


class CCheckRAPDataNeededEvent
    : public CRCMActionBase
    , public CCreateable<CCheckRAPDataNeededEvent, IAction>
//    , public CPool<CCheckRAPDataNeededEvent>
{

public:

    CCheckRAPDataNeededEvent();
	virtual ~CCheckRAPDataNeededEvent();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    virtual HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(CRDNEParms)
        DECLARE_KV_CSTRING(LookupTableName)
        DECLARE_KV_INT(View)
        DECLARE_KV_CSTRING(Priority)
    DECLARE_KV_HANDLER_CLASS_END

    CRDNEParms m_parms;
    
    DWORD GetPriority();
    
    HRESULT CheckUserRAPDataNeeded();

private:

    // hide copy and assignment
	CCheckRAPDataNeededEvent(const CCheckRAPDataNeededEvent&);
	CCheckRAPDataNeededEvent& operator = (const CCheckRAPDataNeededEvent &);
};
