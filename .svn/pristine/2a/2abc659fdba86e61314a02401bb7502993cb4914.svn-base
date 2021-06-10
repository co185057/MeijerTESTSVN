#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/RemoteDataCommand.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RemoteDataCommand.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/RemoteDataCommand.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     4/12/05 2:54p Dm185016
 * Changes for AllLaneStatusSummary
 * 
 * 2     2/28/05 4:40p Dm185016
 * lint
 * 
 * 1     2/14/05 7:03p Dm185016
 * Release Exclusive Access Event support
*/

#include "rcmapi.h"
#include "SMConstants.h"
#include "Createable.h"
#include "KeyValueHandler.h"
#include "RCMActionBase.h"
#include "Pool.h"
#include "Message.h"

using namespace STATE_MACHINE_NAMESPACE;

namespace fastlane
{

class RCMANAGER_API CRemoteDataCommand
    : virtual public CRCMActionBase
    , public CCreateable<CRemoteDataCommand, IAction>
//    , public CPool<CRemoteDataCommand>
{

public:
	CRemoteDataCommand();
	virtual ~CRemoteDataCommand();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

    virtual HRESULT ParseAuxData(const _TCHAR *pszRemoteData);
    virtual const _TCHAR * GetOperation();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(RDCParms)
        DECLARE_KV_CSTRING(AuxData)
        DECLARE_KV_CSTRING(Operation)
        DECLARE_KV_CSTRING(LookupTableName)
    DECLARE_KV_HANDLER_CLASS_END

private:

    // hide copy and assignment
	CRemoteDataCommand(const CRemoteDataCommand&);
	CRemoteDataCommand& operator = (const CRemoteDataCommand &);

    RDCParms m_parms;
};

}
