#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayImmediateIntervention.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisplayImmediateIntervention.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayImmediateIntervention.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     1/13/06 2:30p Dm185016
 * TAR 301329
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
* 
* 2     11/30/04 4:20p Dm185016
* Updated to set the lane status button text and lane status windows
* text.
* 
* 1     10/29/04 8:53a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "KeyValueHandler.h"
#include "ActionBaseRAP.h"
#include "DisplayControlParser.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;


class CDisplayImmediateIntervention
    : public CActionBaseRAP
    , public CDisplayControlParser
    , public CCreateable<CDisplayImmediateIntervention, IAction>
//    , public CPool<CDisplayImmediateIntervention>
{

public:

    CDisplayImmediateIntervention();
	virtual ~CDisplayImmediateIntervention();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    const _TCHAR *GetDescription() const;

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(DIIParms)
        DECLARE_KV_CSTRING(Description)
        DECLARE_KV_CSTRING(ViewText)
        DECLARE_KV_CSTRING(View)
    DECLARE_KV_HANDLER_CLASS_END

    DIIParms m_parms;           

private:

    // hide copy and assignment
	CDisplayImmediateIntervention(const CDisplayImmediateIntervention&);
	CDisplayImmediateIntervention& operator = (const CDisplayImmediateIntervention &);
};
