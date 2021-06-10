#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendAllLaneStatusSummary.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SendAllLaneStatusSummary.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendAllLaneStatusSummary.h $
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

#include "SMConstants.h"
#include "Createable.h"
#include "SendUnsolicitedData.h"
//#include "KeyValueHandler.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CSendAllLaneStatusSummary
    : public CSendUnsolicitedData
    , public CCreateable<CSendAllLaneStatusSummary, IAction>
{

public:

    CSendAllLaneStatusSummary();
	virtual ~CSendAllLaneStatusSummary();

protected:

    virtual HRESULT ParseOperands();

    virtual HRESULT ParseMessage(IMessage &);

    virtual const _TCHAR *GetUnsolicitedData();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SALSSParms)
        DECLARE_KV_CSTRING(AuxData)
    DECLARE_KV_HANDLER_CLASS_END

    SALSSParms m_parms;

private:

    // hide copy and assignment
	CSendAllLaneStatusSummary(const CSendAllLaneStatusSummary&);
	CSendAllLaneStatusSummary& operator = (const CSendAllLaneStatusSummary &);
};
