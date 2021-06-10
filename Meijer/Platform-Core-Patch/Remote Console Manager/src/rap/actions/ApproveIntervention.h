#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ApproveIntervention.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ApproveIntervention.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ApproveIntervention.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     3/17/05 10:31a Dm185016
 * Fixed yellow light after Approve&Collect pressed.
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
* 
* 3     11/22/04 3:43p Dm185016
* Removed extraneous code
* 
* 2     11/16/04 11:28a Dm185016
* Rewritten
* 
* 1     11/02/04 3:05p Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMKVActionBase.h"
#include "ActionBaseRAP.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CApproveIntervention
    : public CRCMKVActionBase<CApproveIntervention>
    , public CActionBaseRAP
    , public CCreateable<CApproveIntervention, IAction>
//    , public CPool<CApproveIntervention>
{

public:

    CApproveIntervention();
	virtual ~CApproveIntervention();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();
    
    const _TCHAR *GetApprovalType(IMessage &message);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_MAP();

    DECLARE_KV_HANDLER(OnView);
    DECLARE_KV_HANDLER(OnType);
    DECLARE_KV_HANDLER(OnCommand);
    DECLARE_KV_HANDLER(OnApprovalCode);
    DECLARE_KV_HANDLER(OnCategory);

private:

    // hide copy and assignment
	CApproveIntervention(const CApproveIntervention&);
	CApproveIntervention& operator = (const CApproveIntervention &);

    HRESULT m_hr;

    int m_nView;

    StringType m_sApprovalCategory;
    StringType m_sApprovalCode;
    StringType m_sViewName;
    StringType m_sCommand;
    StringType m_sCategory;
};
