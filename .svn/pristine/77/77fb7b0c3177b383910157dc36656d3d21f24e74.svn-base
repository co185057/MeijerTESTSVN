#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/RemoteDataCommandRAP.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)RemoteDataCommandRAP.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/RemoteDataCommandRAP.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 4     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     4/12/05 2:53p Dm185016
 * Changes for AllLaneStatusSummary
*/

#include "rcmapi.h"
#include "SMConstants.h"
#include "Createable.h"
#include "KeyValueHandler.h"
#include "RemoteDataCommand.h"
#include "ActionBaseRAP.h"
#include "Pool.h"
#include "Message.h"

using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

namespace fastlane
{

class RCMANAGER_API CRemoteDataCommandRAP
    : public CRemoteDataCommand
    , public CActionBaseRAP
    , public CCreateable<CRemoteDataCommandRAP, IAction>
//    , public CPool<CRemoteDataCommandRAP>
{

public:
	CRemoteDataCommandRAP();
	virtual ~CRemoteDataCommandRAP();

protected:

    virtual HRESULT ParseAuxData(const _TCHAR *pszRemoteData);
    virtual const _TCHAR * GetOperation();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(RDCRapParms)
        DECLARE_KV_CSTRING(Command)
    DECLARE_KV_HANDLER_CLASS_END

    RDCRapParms m_parms;

private:

    // hide copy and assignment
	CRemoteDataCommandRAP(const CRemoteDataCommandRAP&);
	CRemoteDataCommandRAP& operator = (const CRemoteDataCommandRAP &);
};

}
