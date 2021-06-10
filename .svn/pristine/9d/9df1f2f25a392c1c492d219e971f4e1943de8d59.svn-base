#pragma once
/*
 * @(#)SelectButton.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SelectButton.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     4/05/05 12:00p Dm185016
 * TAR 297356 - Now allows translation of button name to another.
 * 
 * 2     2/28/05 4:40p Dm185016
 * lint
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
* 
* 3     11/16/04 11:42a Dm185016
* Added ability to ignore remote keystrokes for assist mode, which is
* controlled by SCOTAPP.
* 
* 2     10/29/04 9:14a Dm185016
* Filled in fxn.
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
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

class CSelectButton
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CSelectButton, IAction>
//    , public CPool<CSelectButton>
{

public:

	CSelectButton();
	virtual ~CSelectButton();
    
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
	CSelectButton(const CSelectButton&);
	CSelectButton& operator = (const CSelectButton &);

    long m_lLParam;
    long m_lWParam;
    long m_lEventID;
    StringType m_sControlName;
    StringType m_sContextName;
    StringType m_sEventName;
    StringType m_sRemoteConnectionName;

};
