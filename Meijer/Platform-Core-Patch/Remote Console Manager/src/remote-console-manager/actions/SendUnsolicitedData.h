#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SendUnsolicitedData.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)SendUnsolicitedData.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SendUnsolicitedData.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     4/12/05 2:56p Dm185016
 * Changes for AllLaneStatusSummary
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
* 
* 1     11/16/04 11:42a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMKVActionBase.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CSendUnsolicitedData
    : public CRCMKVActionBase<CSendUnsolicitedData>
    , public CCreateable<CSendUnsolicitedData, IAction>
//    , public CPool<CSendUnsolicitedData>
{

public:

    CSendUnsolicitedData();
	virtual ~CSendUnsolicitedData();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    virtual HRESULT ParseOperands();

    virtual HRESULT ParseMessage(IMessage &);

    virtual const _TCHAR *GetUnsolicitedData();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_MAP();

    DECLARE_KV_HANDLER(OnUnsolicitedData);

private:

    // hide copy and assignment
	CSendUnsolicitedData(const CSendUnsolicitedData&);
	CSendUnsolicitedData& operator = (const CSendUnsolicitedData &);

    StringType m_sData;
};
