// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SelectKeyDown.h 3     10/21/08 4:09p Sm133076 $
#pragma once
/*
 * @(#)SelectKeyDown.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SelectKeyDown.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 4     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     4/18/05 7:19p Dm185016
 * Keyboard support
*/
#include "Createable.h"
#include "RCMActionBase.h"
#include "KeyValueHandler.h"
#include "ActionBaseRAP.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CSelectKeyDown
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CSelectKeyDown, IAction>
//    , public CPool<CSelectKeyDown>
{

public:

	CSelectKeyDown();
	virtual ~CSelectKeyDown();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

    virtual StringType TranslateButtonName(const _TCHAR *);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SBParms)
        DECLARE_KV_CSTRING(LookupTableName)
        DECLARE_KV_LONG(LParam);
        DECLARE_KV_LONG(WParam);
        DECLARE_KV_LONG(EventID);
        DECLARE_KV_CSTRING(ControlName);
        DECLARE_KV_CSTRING(ContextName);
        DECLARE_KV_CSTRING(EventName);
        DECLARE_KV_CSTRING(RemoteConnectionName);
    DECLARE_KV_HANDLER_CLASS_END

    SBParms m_parms;

private:

    // hide copy and assignment
	CSelectKeyDown(const CSelectKeyDown&);
	CSelectKeyDown& operator = (const CSelectKeyDown &);
};
