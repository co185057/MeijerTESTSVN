#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/SendMsg.h 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)SendMsg.h    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/SendMsg.h $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     2/10/05 5:48p Dm185016
 * TAR 292915
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
* 
* 5     1/12/05 6:30p Dm185016
* Select support.
* 
* 4     10/29/04 9:27a Dm185016
* Removed reference to MessageObject.
* Added copy-plist function parameter.
* 
* 2     6/30/04 3:30p Dm185016
* Added parameter list support
* 
* 1     4/30/04 1:01p Dm185016
* Action to send a message given its name
*/

#include "SMConstants.h"
#include "Createable.h"
#include "ActionBase.h"
#include "KeyValueHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE
{

class IMessage;

class CSendMsg
    : public CActionBase
    , public CCreateable<CSendMsg, IAction>
    //, public CPool<CSendMsg>
{

public:
	CSendMsg();
	virtual ~CSendMsg();
    
    virtual HRESULT Execute(bool *, IMessage &);

    virtual bool IsInternal();

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SMParms)
        DECLARE_KV_CSTRING(MsgName);
        DECLARE_KV_CSTRING(Operation);
        DECLARE_KV_BOOL(Immediate);
        DECLARE_KV_BOOL(CopyPlist);
    DECLARE_KV_HANDLER_CLASS_END

private:

    // hide copy and assignment
	CSendMsg(const CSendMsg&);
	CSendMsg& operator = (const CSendMsg &);

    CParameterList m_vParms;

    StringType m_sParameterList;

    SMParms m_parms;
};

};
