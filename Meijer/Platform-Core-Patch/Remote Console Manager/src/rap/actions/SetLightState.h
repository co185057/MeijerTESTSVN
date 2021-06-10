#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SetLightState.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SetLightState.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SetLightState.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     5/01/05 8:57a Tp151000
 * Performmance
 * 
 * 1     3/11/05 2:45p Dm185016
 * 
 * 2     2/28/05 4:40p Dm185016
 * lint
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
* 
* 5     1/12/05 6:09p Dm185016
* Flash support (disabled)
* 
* 4     12/22/04 11:15a Dm185016
* Added parameter to set the lane button text to the proper color.
* 
* 3     12/07/04 4:24p Dm185016
* Change to RAP action
* 
* 2     10/14/04 1:42p Dm185016
* Added actual implementation
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMKVActionBase.h"
#include "ActionBaseRAP.h"
#include "RemoteAPMgr.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CSetLightState
    : public CRCMKVActionBase<CSetLightState>
    , public CActionBaseRAP
    , public CCreateable<CSetLightState, IAction>
//    , public CPool<CSetLightState>
{

public:

    CSetLightState();
	virtual ~CSetLightState();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_MAP();

    DECLARE_KV_HANDLER(OnGreen);
    DECLARE_KV_HANDLER(OnYellow);
    DECLARE_KV_HANDLER(OnRed);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SLSParms)
        DECLARE_KV_CSTRING(LaneButtonTextControlName)
        DECLARE_KV_CSTRING(LaneButtonControlName)
    DECLARE_KV_HANDLER_CLASS_END

    SLSParms m_parms;

private:

    IMessage &GetMessage();

    // hide copy and assignment
	CSetLightState(const CSetLightState&);
	CSetLightState& operator = (const CSetLightState &);

    bool IsExistingColor(rap::ETriliteColors newColor);

    bool m_bGreen;
    bool m_bYellow;
    bool m_bRed;

    bool m_bFlash;

    IMessage *m_pMessage;

    HRESULT m_hr;
};
