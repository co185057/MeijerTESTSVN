#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/actions/SignonCommand.h 4     5/04/09 10:43a Sm133076 $
/*
 * @(#)SignonCommand.h    $Revision: 4 $ $Date: 5/04/09 10:43a $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/actions/SignonCommand.h $
 * 
 * 4     5/04/09 10:43a Sm133076
 * 381428 Meijer lab 4.32.6 FL Reports: Daily Summary Report by Cashier
 * are reporting incorrect operator ID
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/11/05 2:48p Dm185016
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
* 
* 4     1/13/05 6:49p Dm185016
* Timer Server Support
* 
* 3     11/16/04 11:44a Dm185016
* Added saving operator values
* 
* 2     11/02/04 3:17p Dm185016
* More updates
* 
* 1     10/29/04 9:04a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMKVActionBase.h"
#include "ActionBaseRAP.h"
#include "KeyMessageHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CSignonCommand
    : public CRCMKVActionBase<CSignonCommand>
    , public CActionBaseRAP
    , public CCreateable<CSignonCommand, IAction>
//    , public CPool<CSignonCommand>
{

public:

    CSignonCommand();
	virtual ~CSignonCommand();
    
    virtual HRESULT Execute(bool *, IMessage &);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_MAP();

    DECLARE_KV_HANDLER(OnOperatorID);
    DECLARE_KV_HANDLER(OnOperatorPwd);
    DECLARE_KV_HANDLER(OnValid);
    DECLARE_KV_HANDLER(OnRemoteConnectionName);
    DECLARE_KV_HANDLER(OnTimeRequest);

	DECLARE_KV_HANDLER(OnAuxDataRequest);
	DECLARE_KV_HANDLER(OnApprovalCode);

private:

    // hide copy and assignment
	CSignonCommand(const CSignonCommand&);
	CSignonCommand& operator = (const CSignonCommand &);

    StringType m_szRemoteConnectionName;
    StringType m_szOpId;
    StringType m_szOpPwd;
    BOOL m_nValid;
    bool m_bTimeRequest;

	StringType m_szAuxData;
	int m_nApprovalCode;
};
