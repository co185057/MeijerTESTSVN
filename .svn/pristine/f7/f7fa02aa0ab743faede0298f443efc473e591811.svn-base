#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ReleaseExclusiveAccess.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)ReleaseExclusiveAccess.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ReleaseExclusiveAccess.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/18/05 4:27p Dm185016
 * TAR 293807
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
* 
* 1     1/13/05 1:28p Dm185016
* TAR 290304
*/

#include "SMConstants.h"
#include "Createable.h"
#include "ReleaseExclusiveAccessBase.h"
#include "KeyValueHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CReleaseExclusiveAccess
    : public CReleaseExclusiveAccessBase
    , public CCreateable<CReleaseExclusiveAccess, IAction>
//    , public CPool<CReleaseExclusiveAccess>
{

public:

    CReleaseExclusiveAccess();
	virtual ~CReleaseExclusiveAccess();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(REAParms)
        DECLARE_KV_BOOL(ReleaseNeeded);
    DECLARE_KV_HANDLER_CLASS_END

    REAParms m_parms;

private:

    // hide copy and assignment
	CReleaseExclusiveAccess(const CReleaseExclusiveAccess&);
	CReleaseExclusiveAccess& operator = (const CReleaseExclusiveAccess &);
};
