#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendInterventionEvent.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SendInterventionEvent.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendInterventionEvent.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 4     5/01/05 8:58a Tp151000
 * Performmance
 * 
 * 2     2/23/05 8:37p Dm185016
 * Values for Red and yellow could be 4.  Converted to use LONG instead of
 * BOOL.
 * 
 * 1     2/17/05 5:55p Dm185016
 * Alert buttons functionality
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
* 
* 5     1/12/05 6:09p Dm185016
* Flash support (disabled)
* 
* 4     12/22/04 11:15a Dm185016
* Added parameter to set the lane button text to the proper color.
* 
* 3     12/07/04 4:24p Dm185016
* Change to RAP action
* 
* 2     10/14/04 1:42p Dm185016
* Added actual implementation
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
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

class CSendInterventionEvent
    : public CSendInterventionEventBase
    , public CActionBaseRAP
    , public CCreateable<CSendInterventionEvent, IAction>
//    , public CPool<CSendInterventionEvent>
{

public:

    CSendInterventionEvent();
	virtual ~CSendInterventionEvent();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

    const _TCHAR *GetInterventionType();
    const _TCHAR *GetOperation();

    bool IsOperationRequired() const;

    bool IsPreviousInterventionType() const;

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SIEParms)
        DECLARE_KV_CSTRING(Operation);
        DECLARE_KV_LONG(Green);
        DECLARE_KV_LONG(Red);
        DECLARE_KV_LONG(Yellow);
    DECLARE_KV_HANDLER_CLASS_END

    SIEParms m_parms;

private:

    // hide copy and assignment
	CSendInterventionEvent(const CSendInterventionEvent&);
	CSendInterventionEvent& operator = (const CSendInterventionEvent &);
};
