#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SetLaneName.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SetLaneName.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SetLaneName.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
* 
* 2     1/07/05 10:15a Dm185016
* TAR 288479 - Lane name on button was not updating until transaction
* start.
* 
* 1     11/18/04 1:37p Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "KeyValueHandler.h"
#include "PSXAction.h"
#include "ActionBaseRAP.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CSetLaneName
    : public CPSXAction
    , public CKeyValueHandler<CSetLaneName>
    , public CActionBaseRAP
    , public CCreateable<CSetLaneName, IAction>
//    , public CPool<CSetLaneName>
{

public:

    CSetLaneName();
	virtual ~CSetLaneName();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:
    
    HRESULT ParseOperands();
    
    HRESULT ParseStringKeys(const _TCHAR *pszKeys) { return ParseKeys(pszKeys, GetTraceHandle()); }

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_MAP();

    DECLARE_KV_HANDLER(OnLaneName);
    DECLARE_KV_HANDLER(OnButtonName);

private:

    // hide copy and assignment
	CSetLaneName(const CSetLaneName&);
	CSetLaneName& operator = (const CSetLaneName &);

    StringType m_sLaneName;
    StringType m_sButtonName;
};
