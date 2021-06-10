#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendInterventionEventBase.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SendInterventionEventBase.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendInterventionEventBase.h $
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

#include "BaseObject.h"
#include "RCMActionBase.h"

class CSendInterventionEventBase
    : public CRCMActionBase
    , public virtual CBaseObject
{

public:

    CSendInterventionEventBase() {};
    virtual ~CSendInterventionEventBase() {};

protected:

    virtual HRESULT SendEvent();

    virtual const _TCHAR *GetInterventionType()=0;
    virtual const _TCHAR *GetOperation()=0;

    virtual bool IsOperationRequired() const=0;

private:

    // hide copy and assignment
	CSendInterventionEventBase(const CSendInterventionEventBase&);
	CSendInterventionEventBase& operator = (const CSendInterventionEventBase &);
};
