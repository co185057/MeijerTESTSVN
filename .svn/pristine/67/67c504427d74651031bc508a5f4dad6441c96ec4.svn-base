#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendAllInterventionEvents.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SendAllInterventionEvents.h    $Revision: 3 $ $Date $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendAllInterventionEvents.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     2/17/05 5:55p Dm185016
 * Alert buttons functionality
*/

#include "SMConstants.h"
#include "SendInterventionEventBase.h"
#include "Createable.h"
#include "KeyValueHandler.h"
#include "ActionBaseRAP.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CSendAllInterventionEvents
    : public CSendInterventionEventBase
    , public CActionBaseRAP
    , public CCreateable<CSendAllInterventionEvents, IAction>
//    , public CPool<CSendAllInterventionEvents>
{

public:

    CSendAllInterventionEvents();
	virtual ~CSendAllInterventionEvents();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    const _TCHAR *GetInterventionType();
    const _TCHAR *GetOperation();

    bool IsOperationRequired() const;

private:

    // hide copy and assignment
	CSendAllInterventionEvents(const CSendAllInterventionEvents&);
	CSendAllInterventionEvents& operator = (const CSendAllInterventionEvents &);

    const _TCHAR *m_pszInterventionType;
};
