#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayDelayedIntervention.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisplayDelayedIntervention.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayDelayedIntervention.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     3/07/05 6:39p Dm185016
 * TAR 293999.
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
* 
* 4     1/12/05 5:50p Dm185016
* TAR 289897
* 
* 3     12/22/04 10:33a Dm185016
* TAR 288934 - Factored out common code into DisplayInterventionBase.
* 
* 2     12/07/04 4:16p Dm185016
* Removed method definition now in macro
* 
* 1     11/30/04 4:18p Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "DisplayInterventionBase.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;


class CDisplayDelayedIntervention
    : public CDisplayInterventionBase
    , public CCreateable<CDisplayDelayedIntervention, IAction>
{

public:

    CDisplayDelayedIntervention();
	virtual ~CDisplayDelayedIntervention();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    virtual CIntervention *GetIntervention();

    virtual bool OverrideDisplay();
    virtual bool IsAddDescriptionToReceipt();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(DDIParms)
        DECLARE_KV_CSTRING(Type);
    DECLARE_KV_HANDLER_CLASS_END

    DDIParms m_parms;

private:

    // hide copy and assignment
	CDisplayDelayedIntervention(const CDisplayDelayedIntervention&);
	CDisplayDelayedIntervention& operator = (const CDisplayDelayedIntervention &);

    IMessage *m_pMsg;
};
